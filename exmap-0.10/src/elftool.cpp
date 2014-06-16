#include <iostream>
#include <string>
#include "Elf.hpp"
#include "jutil.hpp"

static bool show_elf_file(const std::string &fname);
static void show_segs(const std::list<Elf::SegmentPtr> &segs,
	std::ostream &os);

using namespace std;
using namespace jutil;
using namespace Elf;

int main(int argc, char **argv)
{
    if (argc < 2) {
	warn << "Insufficent number of arguements\n";
	return -1;
    }
    string fname(argv[1]);

    if (!show_elf_file(fname)) {
	warn << "Can't display elf file: " << fname << "\n";
	return -1;
    }

    return 0;
}

static bool show_elf_file(const std::string &fname)
{
    Elf::File elf_file;

    if (!elf_file.load(fname)) {
	warn << "Can't load file: " << fname << "\n";
	return false;
    }

    show_segs(elf_file.loadable_segments(), cout);
    return true;
}

static void show_segs(const std::list<SegmentPtr> &segs, ostream &os)
{
    list<SegmentPtr>::const_iterator it;

    for (it = segs.begin(); it != segs.end(); ++it) {
	const SegmentPtr &seg = *it;
	os << "SEG: MEM " << seg->mem_range()->to_string()
	    << " ELF " << seg->file_range()->to_string()
	    << " OFFSET " << seg->offset()
	    << "\n";
    }
}
