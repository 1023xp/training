/*
 * (c) John Berthels 2005 <jjberthels@gmail.com>. See COPYING for license.
 */
#ifndef _EXMAP_H
#define _EXMAP_H

#include <list>
#include <map>
#include <string>
#include <vector>
#include <set>

#include <boost/smart_ptr.hpp>

#include <sys/types.h>
#include "jutil.hpp"
#include "Elf.hpp"

namespace Exmap 
{
    typedef unsigned long PageCookie;

    // We exclusively pass around class instances via the reference-counted
    // boost smart ptr 'shared_ptr'. Forward declare all classes here, we'll
    // need most/all of them in method declarations.
 
    class Vma;
    typedef boost::shared_ptr<Vma> VmaPtr;
    class Map;
    typedef boost::shared_ptr<Map> MapPtr;
    class Sizes;
    typedef boost::shared_ptr<Sizes> SizesPtr;
    class Process;
    typedef boost::shared_ptr<Process> ProcessPtr;
    class File;
    typedef boost::shared_ptr<File> FilePtr;
    class PagePool;
    typedef boost::shared_ptr<PagePool> PagePoolPtr;
    class Page;
    typedef boost::shared_ptr<Page> PagePtr;
    class SysInfo;
    typedef boost::shared_ptr<SysInfo> SysInfoPtr;

    /// This is the interface to the system to query information
    /// about processes (pids, vmas, page info). It's abstract to
    /// allow plugging in mock objects for testing (and to help
    /// porting).
    class SysInfo
    {
    public:
	virtual ~SysInfo();

	/// Get a list of pids for which we can read info
	virtual std::list<pid_t> accessible_pids() = 0;

	/// Check that the system is in a good state (e.g. required
	/// kernel modules available, versions, etc).
	virtual bool sanity_check() = 0;

	/// Read the page info for a pid
	virtual bool read_page_info(pid_t pid,
				    std::map<Elf::Address,
				    std::list<Page> > &pi) = 0;

	/// Read cmdline for pid
	virtual std::string read_cmdline(pid_t pid) = 0;

	/// Read vma list for pid
	virtual bool read_vmas(const PagePoolPtr &pp,
			       pid_t pid,
			       std::list<VmaPtr> &vmas) = 0;

    private:
    };

    /// Concrete implementation of Sysinfo for a Linux system.
    class LinuxSysInfo : public SysInfo
    {
    public:
	virtual ~LinuxSysInfo();
	virtual std::list<pid_t> accessible_pids();
	virtual bool sanity_check();
	virtual bool read_page_info(pid_t pid,
			    std::map<Elf::Address, std::list<Page> > &pi);
	virtual std::string read_cmdline(pid_t pid);
	virtual bool read_vmas(const PagePoolPtr &pp,
			       pid_t pid,
			       std::list<VmaPtr> &vmas);
    protected:
	/// Parse a single /proc/xxx/maps line and instantiate a vma
	/// protected to allow use by mock testing objects.
	VmaPtr parse_vma_line(const std::string &line);
	/// Parse a single /proc/exmap page line
	bool parse_page_line(const std::string &line,
		bool &resident,
		bool &writable,
		PageCookie &cookie);
    private:

        std::string proc_map_file(pid_t pid);
	static const std::string EXMAP_FILE;
    };


    /// Holds the various measures we can make of a File, Process or
    /// ELF memory range. Sizes are measured as doubles, to avoid too much
    /// rounding down when calculating the effective values.
    /// A static unit/scale may also be set.
    class Sizes
    {
    public:
	Sizes();
	/// These are the size measures we support. Keep in sync with the
	/// Sizes::names[] array.
	/// They are ordered by 'usefulness'.
	enum Measure {
	    EFFECTIVE_RESIDENT = 0,
	    EFFECTIVE_MAPPED,
	    WRITABLE,
	    VM,
	    SOLE_MAPPED,
	    MAPPED,
	    RESIDENT,
	    NUM_SIZES,
	};
	
	/// Get the value, scaled into the current units
	double sval(int which);

	/// Get the value
	double val(int which);

	/// Add to a value
	void increase(enum Measure which, double amount);

	/// Human readable name for the size
	static std::string size_name(int which);

	/// Add the values from another set of sizes.
	void add(const SizesPtr &other);

	/// Set scale factor to 1
	static void scale_units();
	
	/// Set scale factor to kbytes
	static void scale_kbytes();
	
	/// Set scale factor to mbytes
	static void scale_mbytes();
	
    private:
	/// These are the size names
	const static std::string names[];

	/// The measure values
	double _values[NUM_SIZES];

	/// Scale starts off as 1
	static int _scale_factor;

	/// Scale name starts off as empty
	static std::string _scale_name;
    };

    /// Thin class to hold information about a single page
    class Page
    {
    public:
	Page(PageCookie cookie, bool resident, bool writable);
	bool is_mapped() const;
	bool is_resident() const;
	bool is_writable() const;
	PageCookie cookie() const;
	void print(std::ostream &os) const;
    private:
	PageCookie _cookie;
	int _resident : 1;
	int _writable : 1;
    };
    

    /// Hold information about one process vma (i.e. one line from
    /// /proc/xxx/maps).
    class Vma
    {
    public:
	Vma(Elf::Address start,
		Elf::Address end,
		off_t offset,
		const std::string &fname);

	PagePoolPtr &page_pool();
	
	/// Record that we own these pages
	void add_pages(const std::list<Page> &pages);

	/// The vma start address
	Elf::Address start();

	/// The vma end addres
	Elf::Address end();

	/// For convenience the range of the start/end
	RangePtr range();
	
	/// The name of the underlying file (possibly [anon] or other
	/// if there is no real file)
	std::string fname();

	/// The offset of the vma start within the file
	/// (not a useful value unless the Vma is file backed)
	off_t offset();

	/// The length of the VM area
	Elf::Address vm_size();

	std::string to_string() const;

	/// True if the vma is the special page - we generally wish to
	/// ignore.
	bool is_vdso();
	
	/// True if there is a file backing this VMA (checks the
	/// filename for specials like [anon], etc
	bool is_file_backed();

	std::list<MapPtr> calc_maps(const FilePtr &file,
				    const VmaPtr &previous_vma,
				    const FilePtr &previous_file,
				    pid_t pid);

	boost::shared_ptr<Vma> selfptr();
	void selfptr(boost::shared_ptr<Vma> &p);

	int num_pages();

	/// Struct to hold page + overlap info
	struct PartialPageInfo
	{
	    PartialPageInfo(const Page &p,
		    Elf::Address b) : page(p), bytes(b) { }
	    Page page;
	    Elf::Address bytes;
	};
	
	/// Get a list of pages which cover the range, together
	/// with the per-page size of the overlap with the range
	/// (i.e. always page-size except at the start and end)
	/// Does a lot of error checking, too.
	bool get_pages_for_range(const RangePtr &mrange,
				 std::list<PartialPageInfo> &info);
	
    private:
	/// Get the pgnum (index into the page vector) of the given
	/// address.
	bool addr_to_pgnum(Elf::Address addr, unsigned int &pgnum);
	
	RangePtr _range;
	off_t _offset;
	std::string _fname;
	std::vector<Page> _pages;
	boost::weak_ptr<Vma> _selfptr;
    };

    /// A map represents a range of memory (mem_range) within a
    /// vma. Unlike a VMA, it has a single 'meaning' throughout its range.
    ///
    /// Some maps maintain a 1-1 relationship between 'ELF memory addresses'
    /// and "VMA memory addresses". The ELF memory addresses refer to the
    /// virtual addresses used within a particular ELF file (the 'file' of
    /// the Vma referenced by the map).
    ///
    /// Note that a Map may actually be outside the Vma to which it refers. 
    /// This is due to a possibility that the '.bss' section of an ELF
    /// Segment may "spill over" into a following VMA.
    class Map
    {
    public:
	Map(const VmaPtr &vma,
	    const RangePtr &mem_range,
	    const RangePtr &elf_range);
	/// The VMA memory range of the map
	RangePtr mem_range() const;
	/// The 'ELF virtual memory' of the map (may be NULL)
	RangePtr elf_range() const;
	/// Convert an elf-mem-range to a VMA memory range
	RangePtr elf_to_mem_range(const RangePtr &elf_range);
	/// Return the sizes for the whole Map VMA mem range
	SizesPtr sizes_for_mem_range(const PagePoolPtr &pp);
	/// Return the sizes for a subrange of the vma mem range
	SizesPtr sizes_for_mem_range(const PagePoolPtr &pp,
				     const RangePtr &mrange);
	/// Represent the map in string form
	std::string to_string() const;
	/// Write the map to a ostream in string form
	void print(std::ostream &os) const;

	/// Add up all the sizes for a list of maps
	static SizesPtr sum_sizes(const PagePoolPtr &pp,
				  const std::list<MapPtr> &maps);

	/// Sort a list of MapPtr
	static std::list<MapPtr> sort(const std::list<MapPtr> &maplist);
	
	/// Return the intersection of the two lists (considered as
	/// sets).
	static std::list<MapPtr> intersect_lists(const std::list<MapPtr> &la,
						 const std::list<MapPtr> &lb);
    private:
	Elf::Address elf_to_mem_offset();
	const VmaPtr _vma;
	const RangePtr _mem_range;
	const RangePtr _elf_range;
    };
    
    /// Hold the information about one file.
    class File
    {
    public:
	File(const std::string &fname);
	std::string name();
	/// List of processes which map this file.
	std::list<ProcessPtr> procs();
	/// List of all maps which refer to this file (over many procs)
	std::list<MapPtr> maps();
	/// Return the file ELF object if it is an ELF file, null o/w
	Elf::FilePtr elf();
	/// True if the file is an ELF file.
	bool is_elf();
	/// Return the sizes for all maps over this file.
	SizesPtr sizes();
	/// Return the sizes for all maps in all processes over this elf range
	SizesPtr sizes(const RangePtr &elf_range);

	/// Register a map with this file
	void add_map(const MapPtr &map);
	/// Register a list of maps with this file
	void add_maps(const std::list<MapPtr> &maps);
	/// Register a proc with this file
	void add_proc(const ProcessPtr &proc);
    private:
	std::string _fname;
	std::list<MapPtr> _maps;
	std::set<ProcessPtr> _procs;
	Elf::FilePtr _elf;
    };

    /// Holds all the file objects, indexed by name
    class FilePool
    {
    public:
	void clear();
	FilePtr name_to_file(const std::string &name);
	FilePtr get_or_make_file(const std::string &name);
	std::list<FilePtr> files();
    private:
	std::map<std::string, FilePtr> _files;
    };
    typedef boost::shared_ptr<FilePool> FilePoolPtr;

    /// Hold information regarding each page in use
    class PagePool
    {
    public:
	/// Empty the pagepool
	void clear();
	/// Fetch the usage count of a page
	inline int count(const Page &page) {
	    return _counts[page.cookie()];
	};
	/// Increase the count of a page (to 1 if the page is previously
	/// unseen).
	inline void inc_page_count(const Page &page) {
	    _counts[page.cookie()] = _counts[page.cookie()]++;
	};
	/// Increase the count of a list of pages.
	inline void inc_pages_count(const std::list<Page> &pages) {
	    std::list<Page>::const_iterator it;
	    for (it = pages.begin(); it != pages.end(); ++it) {
		inc_page_count(*it);
	    }
	};

    private:
	std::map<PageCookie, int> _counts;
    };

    
    /// Hold the information about one process.
    class Process
    {
    public:
	Process(const PagePoolPtr &pp, pid_t pid);
	/// Load the pid-specific information from the sysinfo
	bool load(SysInfoPtr &sys_info);
	/// True if the process has its own memory region (some kernel threads
	/// have pids but no mem).
	bool has_mm();
	/// The pid
	pid_t pid();
	/// The process command line
	std::string cmdline();
	/// The list of files the process maps
	std::list<FilePtr> files();
	/// List of all maps which refer to this process (over all files)
	std::list<MapPtr> maps();
	/// The sizes over all the process maps
	SizesPtr sizes();
	/// The sizes over all the maps associated with a given file
	SizesPtr sizes(const FilePtr &file);
	/// The sizes over a given elf range associated with a given file
	SizesPtr sizes(const FilePtr &file,
		       const RangePtr &elf_range);
	/// Process the vma info into a collection of maps. Also associates
	/// the maps with the files and processes.
	bool calculate_maps(FilePoolPtr &file_pool);
	/// Todo - private ctors and write a factory method which removes the
	/// need for selfptr()
	boost::shared_ptr<Process> selfptr();
	/// Todo - private ctors and write a factory method which removes the
	/// need for selfptr()
	void selfptr(boost::shared_ptr<Process> &p);
	/// Associate a file with this process
	void add_file(const FilePtr &file);
	/// Retrieve the page_pool
	const PagePoolPtr &page_pool();
	/// Write some process info to the ostream
	void print(std::ostream &os) const;
    private:
	void remove_vdso_if_nopages();
	boost::weak_ptr<Process> _selfptr;
	bool load_page_info(SysInfoPtr &sys_info);
	bool find_vma_by_addr(Elf::Address start,
			       VmaPtr &current_vma);
	std::list<MapPtr> restrict_maps_to_file(const FilePtr &file);
	/// The ordered list of vmas read from /proc/xxx/maps.
	std::list<VmaPtr> _vmas;

	/// The pid of this process.
	pid_t _pid;

	/// Read from /proc/xxx/cmdline and processed
	std::string _cmdline;

	std::list<MapPtr> _maps;

	std::set<FilePtr> _files;

	const PagePoolPtr &_page_pool;
    };

    /// Hold info about one complete snapshot of process info
    class Snapshot
    {
    public:
	/// Ctor requires a source of info
	Snapshot(SysInfoPtr &sys_info);
	
	/// Get a list of all processes in the snapshot.
	const std::list<ProcessPtr> procs();

	/// Get a list of pids of all processes in the snapshot.
	const std::list<pid_t> pids();
	
	/// Return number of procs in the snapshot.
	inline int num_procs() { return _procs.size(); }
	
	/// Get a process with a particular pid.
	const ProcessPtr proc(pid_t pid);

	/// Get a list of all files in the snapshot.
	const std::list<FilePtr> files();
	
	/// Get a particular file
	const FilePtr file(const std::string &fname);
	
	/// Load the snapshot
	bool load();
    private:

	// ----------------------------------------
	// Methods
	// ----------------------------------------

	/// Load the pid list as procs.
	bool load_procs(const std::list<pid_t> &pids);

	/// Calculate the ELF file->VMA mappings
	bool calculate_file_mappings();

	// ----------------------------------------
	// Data
	// ----------------------------------------
	
	/// Map pid -> process. Also store all our processes
	std::map<pid_t, ProcessPtr> _procs;
	
	/// Global per-page information
	PagePoolPtr _page_pool;

	/// Map name -> file. Also store all our files.
	FilePoolPtr _file_pool;

	/// Source of our information about processes
	SysInfoPtr &_sys_info;
    };
    typedef boost::shared_ptr<Snapshot> SnapshotPtr;

    /// Method class to wrap the (complex) calculation of maps within vmas
    class MapCalculator
    {
	public:
	    /// Init the calculator. The calculator is per-proc, per vma-list
	    MapCalculator(const std::list<VmaPtr> &vmas,
		    FilePoolPtr &file_pool,
		    const ProcessPtr &proc);

	    /// Do the calculation.
	    bool calc_maps(std::list<MapPtr> &maps);
	private:

	    bool calc_maps_for_file(const std::string &fname);
	    bool calc_maps_for_elf_file(const std::string &fname,
		    const FilePtr &file);
	    bool calc_maps_for_nonelf_file(const std::string &fname,
		    const FilePtr &file);
	    bool calc_map_for_seg(const FilePtr &file,
		    const Elf::SegmentPtr &seg,
		    std::list<VmaPtr> &vmas);
	    bool add_holes();
	    bool sanity_check(const std::list<MapPtr> &maps);
	    void walk_vma_files();
	    std::string dump_maps_to_string(const std::list<MapPtr> &maps);
	    std::map<std::string, std::list<Exmap::VmaPtr> > _fname_to_vmas;

	    std::list<VmaPtr> _vmas;
	    FilePoolPtr _file_pool;
	    const ProcessPtr _proc;
	    std::list<MapPtr> _maps;
    };

};

std::ostream &operator<<(std::ostream &os, const Exmap::Process &proc);
std::ostream &operator<<(std::ostream &os, const Exmap::Map &map);

#endif
