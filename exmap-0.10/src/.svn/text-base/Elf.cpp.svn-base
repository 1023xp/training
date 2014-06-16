/*
 * (c) John Berthels 2005 <jjberthels@gmail.com>. See COPYING for license.
 */
#include "Elf.hpp"

#include <sstream>
#include <unistd.h> // getpagesize()
#include <string.h>

using namespace std;
using namespace jutil;
using namespace Elf;


static bool load_table(std::istream &is,
	const std::string &table_name,
	unsigned long offset,
	short num_chunks,
	short chunksize,
	std::list<std::string> &entries);

int Elf::page_size()
{
    static int _page_size = 0;

    if (_page_size == 0) {
	_page_size = getpagesize();
    }
    return _page_size;
}

Address Elf::page_align_down(const Address &addr)
{
    return addr & ~(page_size()-1);
}

Address Elf::page_align_up(const Address &addr)
{
    return page_align_down(addr + page_size() - 1);
}

// ------------------------------------------------------------

File::File()
    : _started_lazy_load_sections(false)
{ }

unsigned long File::elf_file_type()
{
    return _filestruct->type();
}

bool File::load(const string &fname, bool warn_if_non_elf)
{
    unload();
    _fname = fname;

    if (!is_regular_file(fname)) {
	return false;
    }
    
    if (!open_file()) {
	warn << "File::load - can't open file: " << fname << "\n";
	return false;
    }

    if (!load_file_header()) {
	if (warn_if_non_elf) {
	    warn << "File::load - failed to load header: " << fname << "\n";
	}
	return false;
    }

    if (!load_segments()) {
	warn << "File::load - failed to load segment info: " << fname << "\n";
	return false;
    }
    return true;
}
     
void File::unload(void)
{
    _started_lazy_load_sections = false;
    _ifs.clear();
    _ifs.close();
    _fname.clear();
    _segments.clear();
    _sections.clear();
    _symbol_table_section.reset();
    _filestruct.reset();
}


list<SymbolPtr> File::find_symbols_in_mem_range(const RangePtr &mrange)
{
    if (!lazy_load_sections() || !_symbol_table_section) {
	list<SymbolPtr> empty;
	return empty;
    }
    return _symbol_table_section->find_symbols_in_mem_range(mrange);
}

list<SymbolPtr> File::defined_symbols()
{
    list<SymbolPtr> syms;
    list<SymbolPtr> result;
    list<SymbolPtr>::iterator it;
    
    syms = all_symbols();
    for (it = syms.begin(); it != syms.end(); ++it) {
	if ((*it)->is_defined()) {
	    result.push_back(*it);
	}
    }
    return result;
}

list<SymbolPtr> File::all_symbols()
{
    if (!lazy_load_sections()
	|| !_symbol_table_section) {
	list<SymbolPtr> empty;
	return empty;
    }
    return _symbol_table_section->symbols();
}

SymbolPtr File::symbol(const string &symname)
{
    list<SymbolPtr> syms = all_symbols();
    list<SymbolPtr>::iterator it;
    for (it = syms.begin(); it != syms.end(); ++it) {
	if ((*it)->name() == symname) {
	    return *it;
	}
    }
    return SymbolPtr((Symbol *) 0);
}

list<SymbolPtr> File::symbols_in_section(const SectionPtr &section)
{
    list<SymbolPtr> result;
    if (!lazy_load_sections() || !section) {
	list<SymbolPtr> empty;
	return empty;
    }

    return find_symbols_in_mem_range(section->mem_range());
}

string File::filename()
{
    return _fname;
}

int File::num_sections()
{
    return sections().size();
}

int File::num_segments()
{
    return segments().size();
}

list<SegmentPtr> File::segments()
{
    return _segments;
}

list<SegmentPtr> File::loadable_segments()
{
    list<SegmentPtr> result;
    list<SegmentPtr>::iterator it;
    for (it = _segments.begin(); it != _segments.end(); ++it) {
	if ((*it)->is_load()) {
	    result.push_back(*it);
	}
    }
    return result;
}

SectionPtr File::section(int i)
{
    list<SectionPtr>::iterator it;

    if (lazy_load_sections()) {
	for (it = _sections.begin(); it != _sections.end(); ++it) {
	    if (i-- == 0) {
		return *it;
	    }
	}
    }
    
    return SectionPtr((Section *) 0);
}

SectionPtr File::section(const string &name)
{
    list<SectionPtr>::iterator it;
    
    if (lazy_load_sections()) {
	for (it = _sections.begin(); it != _sections.end(); ++it) {
	    if ((*it)->name() == name) {
		return *it;
	    }
	}
    }
    
    return SectionPtr((Section *) 0);
}

list<SectionPtr> File::mappable_sections()
{
    list<SectionPtr> result;
    list<SectionPtr>::iterator it;

    if (!lazy_load_sections()) {
	list<SectionPtr> empty;
	return empty;
    }

    for (it = _sections.begin(); it != _sections.end(); ++it) {
	if ((*it)->addr() != 0) {
	    result.push_back(*it);
	}
    }
    return result;
}

list<SectionPtr> File::sections()
{
    if (!lazy_load_sections()) {
	list<SectionPtr> empty;
	return empty;
    }
    return _sections;
}

bool File::is_executable()
{
    return elf_file_type() == ET_EXEC;
}

bool File::is_shared_object()
{
    return elf_file_type() == ET_DYN;
}



bool File::lazy_load_sections()
{
    // Stop possible recursion
    if (_started_lazy_load_sections) {
	return true;
    }
    _started_lazy_load_sections = true;
    if (!load_sections()) { return false; }
    return correlate_string_sections();
}

bool File::open_file()
{
    uid_t new_euid = 0, orig_euid = 0;

    // If we are root, become the file owner. Otherwise, we might not
    // be able to open the file (e.g. a file on an NFS mount with root
    // squash).
    orig_euid = geteuid();
    if (orig_euid == 0
	&& file_owner(_fname, new_euid)
	&& new_euid != 0) {
	seteuid(new_euid);
    }
    
    if (_ifs.is_open()) {
	_ifs.close();
    }
    _ifs.clear();
    _ifs.open(_fname.c_str());
    if (!_ifs.is_open()) {
	warn << "Can't open file: " << _fname << "\n";
    }

    if (new_euid != 0) {
	seteuid(orig_euid); // ok, it will always be 0...
    }
    
    return _ifs.is_open();
}

bool File::correlate_string_sections()
{
    list<SectionPtr>::iterator it;
    SectionPtr string_table;
    int string_table_index = _filestruct->shstrndx();
    if (string_table_index >= num_sections()) {
	warn << "correlate_string_sections - invalid string index\n";
	return false;
    }
    string_table = section(string_table_index);
    if (!string_table->is_string_table()) {
	warn << "correlate_string_sections - invalid string section\n";
	return false;
    }

    for (it = _sections.begin(); it != _sections.end(); ++it) {
	(*it)->set_name(_ifs, string_table);
	SectionPtr &sect = *it;
	if (sect->is_symbol_table()) {
	    if (!sect->is_dynsym_table() || !_symbol_table_section) {
		_symbol_table_section = sect;
	    }
	    SectionPtr string_table = section(sect->link());
	    sect->load_symbols(_ifs, string_table);
	}
    }
    return true;
}

bool File::load_file_header()
{
    char ident_buf[EI_NIDENT];

    memset(ident_buf, '\0', sizeof(ident_buf));
    binary_read(_ifs, ident_buf);
    _ifs.clear();
    _ifs.seekg(0, ios::beg);

    if (memcmp(ident_buf, "\x7f\x45\x4c\x46", 4) != 0) {
	/* Not an ELF file */
	return false;
    }

    char e_class = ident_buf[EI_CLASS];
    switch (e_class) {
	case ELFCLASS32:
	    _filestruct.reset(new FileStruct<Elf32_Ehdr>(_ifs));
	    break;
	case ELFCLASS64:
	    _filestruct.reset(new FileStruct<Elf64_Ehdr>(_ifs));
	    break;
	default:
	    warn << "Unrecognised ELF class: " << e_class << "\n";
	    return false;
	    break;
    }

    return _ifs.good();
}

bool File::load_sections()
{
    list<string> entries;
    list<string>::iterator it;
    
    if (!load_table(_ifs,
		    "section header table",
		    _filestruct->shoff(),
		    _filestruct->shnum(),
		    _filestruct->shentsize(),
		    entries)
	|| entries.empty()) {
	return false;
    }

    for (it = entries.begin(); it != entries.end(); ++it) {
	SectionPtr s(new Section);
	if (s->init(*it)) {
	    _sections.push_back(s);
	}
	else {
	    warn << "Failed to init section\n";
	}
    }
    return !_sections.empty();
}

bool File::load_segments()
{
    list<string> entries;
    list<string>::iterator it;
    
    if (!load_table(_ifs,
		    "segment header table",
		    _filestruct->phoff(),
		    _filestruct->phnum(),
		    _filestruct->phentsize(),
		    entries)
	|| entries.empty()) {
	return false;
    }

    for (it = entries.begin(); it != entries.end(); ++it) {
	SegmentPtr s(new Segment);
	if (s->init(*it)) {
	    _segments.push_back(s);
	}
	else {
	    warn << "Failed to init segment\n";
	}
    }
    return !_segments.empty();
}

// ------------------------------------------------------------

/*
typedef struct
{
  Elf32_Word	p_type;			// Segment type
  Elf32_Off	p_offset;		// Segment file offset
  Elf32_Addr	p_vaddr;		// Segment virtual address
  Elf32_Addr	p_paddr;		// Segment physical address
  Elf32_Word	p_filesz;		// Segment size in file
  Elf32_Word	p_memsz;		// Segment size in memory
  Elf32_Word	p_flags;		// Segment flags
  Elf32_Word	p_align;		// Segment alignment
} Elf32_Phdr;
*/

bool Segment::init(const string &buffer)
{
    switch (buffer.size()) {
	case sizeof(Elf32_Phdr):
	    _segstruct.reset(new SegmentStruct<Elf32_Phdr>(buffer));
	    break;
	case sizeof(Elf64_Phdr):
	    _segstruct.reset(new SegmentStruct<Elf64_Phdr>(buffer));
	    break;
	default:
	    warn << "Invalid segment size: " << buffer.size();
	    return false;
	    break;
    }

    _mem_range.reset(new Range(_segstruct->vaddr(),
			       _segstruct->vaddr() + _segstruct->memsz()));
    _file_range.reset(new Range(_segstruct->offset(),
				_segstruct->offset() + _segstruct->filesz()));
    return true;
}


Address Segment::offset()
{
    return _segstruct->offset();
}

Address Segment::align()
{
    return _segstruct->align();
}

RangePtr Segment::mem_range()
{
    return _mem_range;
}

RangePtr Segment::file_range()
{
    return _file_range;
}

bool Segment::is_load()
{
    return _segstruct->type() == PT_LOAD;
}

bool Segment::is_readable()
{
    return flag_is_set(PF_R);
}

bool Segment::is_writable()
{
    return flag_is_set(PF_W);
}

bool Segment::is_executable()
{
    return flag_is_set(PF_X);
}

bool Segment::flag_is_set(int flag)
{
    return _segstruct->flags() & flag;
}

// ------------------------------------------------------------

/*
typedef struct
{
  Elf32_Word	sh_name;		// Section name (string tbl index)
  Elf32_Word	sh_type;		// Section type
  Elf32_Word	sh_flags;		// Section flags
  Elf32_Addr	sh_addr;		// Section virtual addr at execution
  Elf32_Off	sh_offset;		// Section file offset
  Elf32_Word	sh_size;		// Section size in bytes
  Elf32_Word	sh_link;		// Link to another section
  Elf32_Word	sh_info;		// Additional section information
  Elf32_Word	sh_addralign;		// Section alignment
  Elf32_Word	sh_entsize;		// Entry size if section holds table
} Elf32_Shdr;
*/

bool Section::init(const string &buffer)
{
    switch (buffer.size()) {
	case sizeof(Elf32_Shdr):
	    _sectstruct.reset(new SectionStruct<Elf32_Shdr>(buffer));
	    break;
	case sizeof(Elf64_Shdr):
	    _sectstruct.reset(new SectionStruct<Elf64_Shdr>(buffer));
	    break;
	default:
	    warn << "Invalid section size: " << buffer.size();
	    return false;
	    break;
    }

    _file_range.reset(new Range(_sectstruct->offset(),
				_sectstruct->offset() + _sectstruct->size()));
    _mem_range.reset(new Range(_sectstruct->addr(),
			       _sectstruct->addr() + _sectstruct->size()));

    return true;
}

string Section::name()
{
    return _name;
}

bool Section::set_name(istream &is, const SectionPtr &string_table)
{
    int name_index = _sectstruct->name();
    _name = string_table->find_string(is, name_index);
    return is.good();
}

RangePtr Section::file_range()
{
    return _file_range;
}

RangePtr Section::mem_range()
{
    return _mem_range;
}

bool Section::is_null()
{
    return _sectstruct->type() == SHT_NULL;
}

bool Section::is_string_table()
{
    return _sectstruct->type() == SHT_STRTAB;
}

bool Section::is_symbol_table()
{
    return _sectstruct->type() == SHT_SYMTAB || is_dynsym_table();
}

bool Section::is_dynsym_table()
{
    return _sectstruct->type() == SHT_DYNSYM;
}

bool Section::is_nobits()
{
    return _sectstruct->type() == SHT_NOBITS;
}

std::string Section::find_string(istream &is, int index)
{
    if (!is_string_table() || index < 0) {
	return false;
    }

    int offset = _sectstruct->offset() + index;
    is.seekg(offset, ios_base::beg);
    // Symbols are null-terminated.
    const int MAX_SYMBOL_SIZE = 1024;
    char *buf = (char *)alloca(MAX_SYMBOL_SIZE);
    is.getline(buf, MAX_SYMBOL_SIZE, '\0');
    string result(buf);
    if (!is) {
	result.clear();
    }
    return result;
}

std::list<SymbolPtr> Section::symbols()
{
    return _symbols;
}

std::list<SymbolPtr> Section::find_symbols_in_mem_range(const RangePtr &mrange)
{
    list<SymbolPtr> result;
    list<SymbolPtr>::iterator it;

    for (it = _symbols.begin(); it != _symbols.end(); ++it) {
	if ((*it)->is_defined() && mrange->overlaps(*(*it)->range())) {
	    result.push_back(*it);
	}
    }
    return result;
}

unsigned long Section::addr()
{
    return _sectstruct->addr();
}

unsigned long Section::link()
{
    return _sectstruct->link();
}

unsigned long Section::size()
{
    return _sectstruct->size();
}

bool Section::load_symbols(std::istream &is,
			   const SectionPtr &string_table)
{
    if (!is_symbol_table()) {
	warn << "Can't load symbols from non-symbol-table section\n";
	return false;
    }
    unsigned int symentry_size = _sectstruct->entsize();
    if (symentry_size <= 0) {
	warn << "Invalid symbol entry table size " << symentry_size << "\n";
	return false;
    }

    int total_size = _sectstruct->size();
    if (total_size % symentry_size != 0) {
	warn << "Section::load_symbols - size mismatch " << symentry_size
	     << ", " << total_size << "\n";
	return false;
    }

    list<string> entries;
    if (!load_table(is,
		"symbol table",
		_sectstruct->offset(),
		(total_size / symentry_size),
		symentry_size,
		entries)
	|| entries.empty()) {
	return false;
    }
    
    _symbols.clear();
    list<string>::iterator it;
    for(it = entries.begin(); it != entries.end(); ++it) {
	SymbolPtr symbol(new Symbol);
	if (symbol->init(*it)) {
	    symbol->set_name(is, string_table);
	    _symbols.push_back(symbol);
	}
	else {
	    warn << "Failed to init symbol from buffer\n";
	}
    }

    return true;
}
			   

// ------------------------------------------------------------

bool Symbol::init(const string &buffer)
{
    switch (buffer.size()) {
	case sizeof(Elf32_Sym):
	    _symstruct.reset(new SymbolStruct<Elf32_Sym>(buffer));
	    break;
	case sizeof(Elf64_Sym):
	    _symstruct.reset(new SymbolStruct<Elf64_Sym>(buffer));
	    break;
	default:
	    warn << "Invalid symbol size: " << buffer.size() << "\n";
	    return false;
	    break;
    }

    _range.reset(new Range(_symstruct->value(),
			   _symstruct->value() + _symstruct->size()));

    return true;
}

bool Symbol::set_name(istream &is, const SectionPtr &string_table)
{
    int name_index = _symstruct->name();
    _name = string_table->find_string(is, name_index);
    return is.good();
}

string Symbol::name()
{
    return _name;
}

int Symbol::size()
{
    return _symstruct->size();
}

RangePtr Symbol::range()
{
    return _range;
}

bool Symbol::is_defined()
{
    return _symstruct->value() != 0 && !_name.empty();
}

int Symbol::type()
{
    return _symstruct->info() & 0xf;
}

bool Symbol::is_func()
{
    return type() == STT_FUNC;
}

bool Symbol::is_data()
{
    return type() == STT_OBJECT;
}

bool Symbol::is_file()
{
    return type() == STT_FILE;
}

bool Symbol::is_section()
{
    return type() == STT_SECTION;
}

// ------------------------------------------------------------

bool load_table(istream &is,
	const std::string &table_name,
	unsigned long offset,
	short num_chunks,
	short chunksize,
	list<string> &entries)
{
    entries.clear();
    
    if (offset == 0) {
	warn << "No " << table_name << " present\n";
	return false;
    }
    if (num_chunks < 1) {
	warn << "Invalid number of chunks " << num_chunks
	     << " in " << table_name << "\n";
	return false;
    }

    is.seekg(offset, ios_base::beg);
    if (!is.good()) {
	warn << "Can't seek to offset: " << offset << "\n";
	return false;
    }

    unsigned long read_size = num_chunks * chunksize;
    char *buffer = (char *) alloca(read_size);
    is.read(buffer, read_size);
    if (!is.good()) {
	warn << "Can't read " << read_size << " bytes\n";
	return false;
    }

    for (unsigned int i = 0; i < read_size; i += chunksize) {
	string s(buffer + i, chunksize);
	entries.push_back(s);
    }

    return true;
}

