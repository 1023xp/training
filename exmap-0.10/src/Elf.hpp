/*
 * (c) John Berthels 2005 <jjberthels@gmail.com>. See COPYING for license.
 */
#ifndef _ELF_CPP_H
#define _ELF_CPP_H

#include <list>
#include <string>
#include <fstream>

#include <elf.h>

#include <boost/shared_ptr.hpp>

#include "Range.hpp"
#include "jutil.hpp"

#include <limits.h>

namespace Elf
{
    int page_size();
    int map_alignment();

    typedef unsigned long Address;
    Address page_align_down(const Address &addr);
    Address page_align_up(const Address &addr);

    class SymbolStructBase
    {
	public:
	    virtual ~SymbolStructBase() { };
	    virtual unsigned long size() = 0;
	    virtual unsigned long value() = 0;
	    virtual unsigned long name() = 0;
	    virtual unsigned long info() = 0;
    };
    typedef boost::shared_ptr<SymbolStructBase> SymbolStructPtr;

    template <typename StructType>
	class SymbolStruct : public SymbolStructBase
    {
	public:
	    SymbolStruct(const std::string &buffer) {
		memcpy(&_data, buffer.data(), sizeof(_data));
	    }
	    unsigned long size() { return _data.st_size; }
	    unsigned long value() { return _data.st_value; }
	    unsigned long name() { return _data.st_name; }
	    unsigned long info() { return _data.st_info; }
	private:
	    StructType _data;
    };

    class Section;
    typedef boost::shared_ptr<Section> SectionPtr;
    /// Hold information on a single ELF symbol
    class Symbol
    {
    public:
	bool init(const std::string &data);
	int size();
	/// True if the symbol has a name and a non-zero value
	bool is_defined();
	bool is_func();
	bool is_data();
	bool is_file();
	bool is_section();
	std::string name();
	bool set_name(std::istream &is, const SectionPtr &string_table);
	RangePtr range();
    private:
	int type();
	SymbolStructPtr _symstruct;
	std::string _name;
	RangePtr _range;
    };
    typedef boost::shared_ptr<Symbol> SymbolPtr;
    
    class SectionStructBase
    {
	public:
	    virtual ~SectionStructBase() { };
	    virtual unsigned long size() = 0;
	    virtual unsigned long offset() = 0;
	    virtual unsigned long addr() = 0;
	    virtual unsigned long name() = 0;
	    virtual unsigned long type() = 0;
	    virtual unsigned long entsize() = 0;
	    virtual unsigned long link() = 0;
    };
    typedef boost::shared_ptr<SectionStructBase> SectionStructPtr;

    template <typename StructType>
	class SectionStruct : public SectionStructBase
    {
	public:
	    SectionStruct(const std::string &buffer) {
		memcpy(&_data, buffer.data(), sizeof(_data));
	    }
	    unsigned long size() { return _data.sh_size; }
	    unsigned long offset() { return _data.sh_offset; }
	    unsigned long addr() { return _data.sh_addr; }
	    unsigned long name() { return _data.sh_name; }
	    unsigned long type() { return _data.sh_type; }
	    unsigned long entsize() { return _data.sh_entsize; }
	    unsigned long link() { return _data.sh_link; }
	private:
	    StructType _data;
    };

    /// Hold information on a single ELF section (program header)
    class Section
    {
    public:
	bool init(const std::string &buffer);
	std::string name();
	bool set_name(std::istream &is, const SectionPtr &string_table);
	RangePtr file_range();
	RangePtr mem_range();
	/// Returns the sh_type value. See elf.h
	bool is_null();
	bool is_string_table();
	bool is_symbol_table();
	bool is_dynsym_table();
	bool is_nobits();
	std::list<SymbolPtr> symbols();
	std::list<SymbolPtr> find_symbols_in_mem_range(const RangePtr &mrange);
	bool load_symbols(std::istream &is,
			  const SectionPtr &string_table);
	std::string find_string(std::istream &is, int index);
	unsigned long addr();
	unsigned long link();
	unsigned long size();
    private:
	SectionStructPtr _sectstruct;
	RangePtr _mem_range;
	RangePtr _file_range;
	std::list<SymbolPtr> _symbols;
	std::string _name;
	unsigned long _name_index;
	unsigned long _link;
	unsigned long _addr;
	unsigned long _size;
	unsigned long _offset;
	char _type;
	unsigned long _entsize;
    };

    class SegmentStructBase
    {
	public:
	    virtual ~SegmentStructBase() { };
	    virtual unsigned long vaddr() = 0;
	    virtual unsigned long memsz() = 0;
	    virtual unsigned long offset() = 0;
	    virtual unsigned long filesz() = 0;
	    virtual unsigned long type() = 0;
	    virtual unsigned long flags() = 0;
	    virtual unsigned long align() = 0;
    };
    typedef boost::shared_ptr<SegmentStructBase> SegmentStructPtr;

    template <typename StructType>
	class SegmentStruct : public SegmentStructBase
    {
	public:
	    SegmentStruct(const std::string &buffer) {
		memcpy(&_data, buffer.data(), sizeof(_data));
	    }
	    unsigned long vaddr() { return _data.p_vaddr; }
	    unsigned long memsz() { return _data.p_memsz; }
	    unsigned long offset() { return _data.p_offset; }
	    unsigned long filesz() { return _data.p_filesz; }
	    unsigned long type() { return _data.p_type; }
	    unsigned long flags() { return _data.p_flags; }
	    unsigned long align() { return _data.p_align; }
	private:
	    StructType _data;
    };

    /// Hold information on a single ELF segment
    class Segment
    {
    public:
	bool init(const std::string &buffer);
	RangePtr mem_range();
	RangePtr file_range();
	Address offset();
	unsigned long align();
	bool is_load();
	bool is_readable();
	bool is_writable();
	bool is_executable();
    private:
	RangePtr _mem_range;
	RangePtr _file_range;
	bool flag_is_set(int flag);
	SegmentStructPtr _segstruct;
    };
    typedef boost::shared_ptr<Segment> SegmentPtr;
    
    class FileStructBase
    {
	public:
	    virtual ~FileStructBase() { };
	    virtual unsigned long type() = 0;
	    virtual unsigned long shoff() = 0;
	    virtual unsigned long shnum() = 0;
	    virtual unsigned long shentsize() = 0;
	    virtual unsigned long phoff() = 0;
	    virtual unsigned long phnum() = 0;
	    virtual unsigned long phentsize() = 0;
	    virtual unsigned long shstrndx() = 0;
    };
    typedef boost::shared_ptr<FileStructBase> FileStructPtr;

    template <typename StructType>
	class FileStruct : public FileStructBase
    {
	public:
	    FileStruct(std::istream &is) {
		jutil::binary_read(is, _data);
	    }
	    unsigned long type() { return _data.e_type; }
	    unsigned long shoff() { return _data.e_shoff; }
	    unsigned long shnum() { return _data.e_shnum; }
	    unsigned long shentsize() { return _data.e_shentsize; }
	    unsigned long phoff() { return _data.e_phoff; }
	    unsigned long phnum() { return _data.e_phnum; }
	    unsigned long phentsize() { return _data.e_phentsize; }
	    unsigned long shstrndx() { return _data.e_shstrndx; }
	private:
	    StructType _data;
    };

    /// Hold information on a single ELF file, 32- or 64-bit
    class File
    {
    public:
	File();
	/// Load information from the specified file. Returns false if not
	/// an elf file (or does not exist). Will also warn unless
	/// 'warn_if_non_elf' is false.
	bool load(const std::string &fname, bool warn_if_non_elf = true);
	/// Clean the object ready to be used again
	void unload();
	/// List of symbols in the given virtual memory range
	std::list<SymbolPtr> find_symbols_in_mem_range(const RangePtr &mrange);
	/// List of symbols for which is_defined is true
	std::list<SymbolPtr> defined_symbols();
	/// List of all symbols
	std::list<SymbolPtr> all_symbols();
	/// Find the symbol for a given name (null if we don't have it)
	SymbolPtr symbol(const std::string &symname);
	/// List of symbols in a given ELF section
	std::list<SymbolPtr> symbols_in_section(const SectionPtr &section);
	/// The underlying filename
	std::string filename();
	/// Number of ELF sections
	int num_sections();
	/// Number of ELF segments
	int num_segments();
	/// List of segments
	std::list<SegmentPtr> segments();
	/// List of segments which are of type PT_LOAD (i.e. those which
	/// are mapped into memory when the ELF file is loaded by the system)
	std::list<SegmentPtr> loadable_segments();
	/// Get segment by index number
	SectionPtr section(int i);
	/// Get segment by name
	SectionPtr section(const std::string &name);
	/// List of sections which appear in the elf memory image
	std::list<SectionPtr> mappable_sections();
	/// List of all sections
	std::list<SectionPtr> sections();
	/// The raw e_type field from the struct
	unsigned long elf_file_type();
	/// Syntactic sugar for elf_file_type() == ET_EXEC
	bool is_executable();
	/// Syntactic sugar for elf_file_type() == ET_DYN
	bool is_shared_object();
    private:
	bool lazy_load_sections();
	bool open_file();
	bool correlate_string_sections();
	bool load_file_header();
	bool load_sections();
	bool load_segments();

	bool _started_lazy_load_sections;
	std::ifstream _ifs;
	std::string _fname;
	std::list<Elf::SegmentPtr> _segments;
	std::list<Elf::SectionPtr> _sections;
	SectionPtr _symbol_table_section;
	FileStructPtr _filestruct;
    };
    typedef boost::shared_ptr<File> FilePtr;
};

#endif
