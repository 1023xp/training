/*
 * (c) John Berthels 2005 <jjberthels@gmail.com>. See COPYING for license.
 */
#include <Trun.hpp>
#include "Elf.hpp"
#include "jutil.hpp"
#include "Pcre.hpp"

#include <list>
#include <string>
#include <stdlib.h> // For strtol

class ElfTest : public Test
{
public:
    bool setup();
    bool run();
    bool maintests();
    bool mix32_64();
    bool teardown();
private:
    struct testdat
    {
	unsigned long type;
	bool is_exe;
	std::string func_symbol;
    };
    struct section_info
    {
	std::string name;
	unsigned long size;
    };
    std::list<struct section_info> read_section_info(const std::string &fname);
    int read_number_of_symbols(const std::string &fname);
    std::map<std::string, struct testdat> _testdat;
};

using namespace std;
using namespace Pcre;

bool ElfTest::run()
{
    return maintests() && mix32_64();
}

bool ElfTest::maintests()
{
    plan(114);

    Elf::File e;

    notok(e.load(string("./doesnotexist")), "can't load missing file");
    const string nonelf("/etc/motd");
    ok(jutil::file_exists(nonelf), "nonelf file exists");
    notok(e.load(nonelf), "can't load existing non-elf file");
    const string exist_but_noprivs("/proc/1/auxv");
    ok(jutil::file_exists(exist_but_noprivs), "file exists");
    notok(e.load(exist_but_noprivs), "can't load existing non-privs file");
       
    map<string, struct testdat>::iterator it;
    for (it = _testdat.begin(); it != _testdat.end(); ++it) {
	struct testdat *td = &(it->second);
	string fname = it->first;
	list<string> section_lines, lines;
	
	ok(e.load(fname), "can load file " + fname);
	is(e.filename(), fname, "has correct name");
	is(e.elf_file_type(), td->type, "file " + fname + " correct elf type");
	is(e.is_executable(), td->is_exe, "file is_exe correct");

	list<struct section_info> section_info = read_section_info(fname);
	ok(section_info.size() > 0, "can readelf section info");

	is (e.num_sections(), (int) section_info.size(),
	    "number of sections match readelf");

	ok(e.section(".text"), "can find text section");
	ok(e.section(".data"), "can find data section");
	ok(e.section(".bss"), "can find bss section");

	int num_segments = e.num_segments();
	ok(num_segments > 0, "can find segments");

	ok(jutil::read_proc_output("readelf -l " + fname,
				   lines), "can readelf segments");
	Regexp re;
	re.compile("There are (\\d+) program headers");
	re.grep(lines);
	is((int) lines.size(), 1, "found right line in readelf output");
	list<string> captures;
	re.match_capture(lines.front(), captures);
	is((int) captures.size(), 1, "got my one capture");
	int readelf_numsegs = atoi(captures.front().c_str());
	is(num_segments, readelf_numsegs, "number of segments matches readelf");

	list<Elf::SegmentPtr> segs = e.segments();
	is((int) segs.size(), num_segments, "sanity check num of segs");
	segs = e.loadable_segments();
	is((int) segs.size(), 2, "two loadable segs for each elf");

	ok(segs.front()->is_readable(), "first is readable");
	notok(segs.front()->is_writable(), "first is not writable");
	ok(segs.front()->is_executable(), "first is executable");
	
	ok(segs.back()->is_readable(), "second is readable");
	ok(segs.back()->is_writable(), "second is writable");
	notok(segs.back()->is_executable(), "second is not executable");


	bool all_sections_match = true;
	list<Elf::SectionPtr> sections = e.sections();
	is((int) sections.size(), e.num_sections(),
	   "sanity check num sections");
	list<Elf::SectionPtr>::iterator it;
	int isection = 0;

	// Only perform one test at max in this loop...
	for (it = sections.begin(); it != sections.end(); ++it) {
	    if (section_info.empty()) {
		all_sections_match = false;
		break;
	    }

	    struct section_info si = section_info.front();
	    section_info.pop_front();

	    Elf::SectionPtr sect = e.section(isection++);

	    if (si.name != sect->name()) {
		// Throw the failed test
		is(sect->name(), si.name, "Section names match");
		all_sections_match = false;
		break;
	    }

	    if (si.size != sect->size()) {
		is(sect->size(), si.size,
		   "Section sizes match for " + sect->name());
		all_sections_match = false;
		break;
	    }
	}


	// If we didn't throw a -ve test above, throw a +ve here (to
	// keep to the plan).
	if (all_sections_match) {
	    ok(all_sections_match, "section names and order match readelf");
	}

	int readelf_num_symbols = read_number_of_symbols(fname);
	ok(readelf_num_symbols > 0, "could get number of symbols");
	list<Elf::SymbolPtr> syms;
	syms = e.all_symbols();
	is(readelf_num_symbols, (int) syms.size(), "correct number of symbols");

	if (td->type == ET_EXEC && readelf_num_symbols > 0) {

	    string fsym = td->func_symbol;
	    if (!fsym.empty()) {
		Elf::SymbolPtr funcsym = e.symbol(fsym);
		ok(funcsym, "can find " + fsym + " symbol in exe");
		is(funcsym->name(), fsym, "symbol has correct name");
		ok(funcsym->size() > 0, fsym + " has non-zero size");
		ok(funcsym->is_func(), fsym + " symbol is a function");

		syms = e.symbols_in_section(e.section(".text"));
		list<Elf::SymbolPtr>::iterator sym_it;
		for (sym_it = syms.begin(); sym_it != syms.end(); ++sym_it) {
		    if ((*sym_it)->name() == fsym) {
			break;
		    }
		}
		ok(sym_it != syms.end(), "found " + fsym + " in text section");

		syms = e.symbols_in_section(e.section(".data"));
		for (sym_it = syms.begin(); sym_it != syms.end(); ++sym_it) {
		    if ((*sym_it)->name() == fsym) {
			break;
		    }
		}
		ok(sym_it == syms.end(), "didn't find " + fsym + " in data section");
	    }
	}
    }

    int page_size = Elf::page_size();
    ok(page_size > 0, "check we have a sane page size");
    is((int) Elf::page_align_down(page_size), page_size, "pad 1");
    is((int) Elf::page_align_down(page_size-1), 0, "pad 2");
    is((int) Elf::page_align_down(page_size+1), page_size, "pad 3");
    is((int) Elf::page_align_down(2*page_size), 2*page_size, "pad 4");
    is((int) Elf::page_align_down(2*page_size-1), page_size, "pad 5");
    is((int) Elf::page_align_down(2*page_size+1), 2*page_size,
	    "pad 6");
    
    is((int) Elf::page_align_up(page_size), page_size, "pad 1");
    is((int) Elf::page_align_up(page_size-1), page_size, "pad 2");
    is((int) Elf::page_align_up(page_size+1), 2*page_size, "pad 3");
    is((int) Elf::page_align_up(2*page_size), 2*page_size, "pad 4");
    is((int) Elf::page_align_up(2*page_size-1), 2*page_size, "pad 5");
    is((int) Elf::page_align_up(2*page_size+1), 3*page_size, "pad 6");
    
    return true;
}

bool ElfTest::setup()
{
    struct testdat td;

    td.type = ET_EXEC;
    td.is_exe = true;
    td.func_symbol = "";
    _testdat["/bin/ls"] = td;
    
    td.type = ET_DYN;
    td.is_exe = false;
    td.func_symbol = "gzopen";
    _testdat["/usr/lib/libz.so"] = td;

    td.type = ET_EXEC;
    td.is_exe = true;
    td.func_symbol = "main";
    _testdat["./t_elf"] = td;
    return true;
}

bool ElfTest::teardown()
{
    return false;
}

bool ElfTest::mix32_64()
{
    return true;
}

RUN_TEST_CLASS(ElfTest);

list<struct ElfTest::section_info>
ElfTest::read_section_info(const string &fname)
{
    list<struct section_info> retval;
    list<string> readelf_lines;
    list<string> captures;
    bool ranok;
    bool readelf64 = true;
    string line, hexsize;
    string::size_type pos;

    ranok = jutil::read_proc_output("readelf -S " + fname, readelf_lines);
    ok(ranok, "can readelf the file");

    // Size on same line as name
    Regexp detect_readelf_32_re;
    detect_readelf_32_re.compile("Name *Type *Addr *Off *Size");

    Regexp section_start_re;
    section_start_re.compile("^\\s*\\[ *\\d+] (.+)");

    while (!readelf_lines.empty()) {
	line = readelf_lines.front();
	readelf_lines.pop_front();

	if (detect_readelf_32_re.matches(line)) {
	    readelf64 = false;
	    continue;
	}

	if (section_start_re.match_capture(line, captures)) {

	    // Pull the name from the line
	    struct section_info current_section;
	    current_section.name = captures.front();
	    pos = current_section.name.find(' ');
	    if (pos != string::npos) {
		current_section.name.erase(pos);
	    }
	    //
	    // Size is either on this line or next, depending on 32/64 readelf
	    if (readelf64) {
		line = readelf_lines.front();
		readelf_lines.pop_front();
		hexsize = line.substr(8, 16);
	    }
	    else {
		hexsize = line.substr(57, 6);
	    }

	    current_section.size = strtol(hexsize.c_str(), NULL, 16);
	    retval.push_back(current_section);
	}
    }

    return retval;
}

int ElfTest::read_number_of_symbols(const string &fname)
{
    list<string> captures, orig_lines;
    list<string> symtabs;

    ok(jutil::read_proc_output("readelf -s " + fname, orig_lines),
       "can readelf symbol info from " + fname);

    symtabs.push_back(".symtab");
    symtabs.push_back(".dynsym");
    list<string>::iterator it;

    for (it = symtabs.begin(); it != symtabs.end(); ++it) {
	Regexp re;
	string &symtab = *it;
	re.compile("^Symbol table '" + symtab + "' contains (\\d+) entries");
	list<string> lines(orig_lines);
	re.grep(lines);
	if (!lines.empty()) {
	    is((int) lines.size(), 1,
	       "readelf has exactly one " + symtab + " table");
	    re.match_capture(lines.front(), captures);
	    is((int) captures.size(), 1, "got my one capture");
	    // Take first match, i.e. prefer symtab to dynsym
	    return atoi(captures.front().c_str());
	}
    }

    return 0;
}
