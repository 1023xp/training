/*
 * (c) John Berthels 2005 <jjberthels@gmail.com>. See COPYING for license.
 */
#include "jutil.hpp"
#include "Exmap.hpp"
#include "Elf.hpp"

#include <sstream>
#include <set>

#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>

using namespace Exmap;
using namespace std;
using namespace jutil;
using Elf::Address;

ostream &operator<<(ostream &os, const Exmap::Process &proc)
{
    proc.print(os);
    return os;
}

ostream &operator<<(ostream &os, const Exmap::Map &map)
{
    map.print(os);
    return os;
}


// ------------------------------------------------------------

Snapshot::Snapshot(SysInfoPtr &sys_info)
    : _page_pool(new PagePool),
      _file_pool(new FilePool),
      _sys_info(sys_info)
{
}

const list<ProcessPtr> Snapshot::procs()
{
    return map_values(_procs);
}


const list<pid_t> Snapshot::pids()
{
    return map_keys(_procs);
}	
	
const ProcessPtr Snapshot::proc(pid_t pid)
{
    ProcessPtr ret;
    map<pid_t, ProcessPtr>::const_iterator it;
    it = _procs.find(pid);
    if (it != _procs.end()) {
	ret = it->second;
    }
    return ret;
}

const list<FilePtr> Snapshot::files()
{
    return _file_pool->files();
}


const FilePtr Snapshot::file(const std::string &fname)
{
    return _file_pool->name_to_file(fname);
}


bool Snapshot::load()
{
    list<pid_t> pids = _sys_info->accessible_pids();

    if (!_sys_info->sanity_check()) {
	warn << "Can't get system info\n";
	exit(-1);
    }

    if (pids.empty()) {
	warn << "Snapshot::load - can't get pid list\n";
	return false;
    }

    if (!load_procs(pids)) {
	warn << "Snapshot::load - failed to load: processe\n";
	return false;
    }
    
    if (!calculate_file_mappings()) {
    	warn << "Snapshot::load - failed to load: calculate file mappings\n";
    	return false;
    }
    return true;
}

bool Snapshot::load_procs(const list<pid_t> &pids)
{
    list<pid_t>::const_iterator it;
    pid_t mypid = getpid();

    _procs.clear();
    _page_pool->clear();
    
    for (it = pids.begin(); it != pids.end(); ++it) {
	if (*it == mypid) {
	    // Don't monitor ourselves
	    continue;
	}

	ProcessPtr proc(new Process(_page_pool, *it));
	proc->selfptr(proc);
	if (!proc->load(_sys_info)) {
	    warn << "Snapshot::load_procs - can't load pid " << *it << "\n";
	    continue;
	}

	if (proc->has_mm()) {
	    _procs[*it] = proc;
	}
    }
    
    return !_procs.empty();
}

bool Snapshot::calculate_file_mappings()
{
    list<ProcessPtr>::iterator it;
    list<ProcessPtr> processes = procs();

    for (it = processes.begin(); it != processes.end(); ++it) {
	if (!(*it)->calculate_maps(_file_pool)) {
	    warn << "Failed to process maps for pid " << (*it)->pid() << "\n";
	    return false;
	}
    }
    return !_file_pool->files().empty();
}


// ------------------------------------------------------------

void PagePool::clear()
{
    _counts.clear();
}

// ------------------------------------------------------------

void FilePool::clear()
{
    _files.clear();
}

FilePtr FilePool::name_to_file(const string &name)
{
    return _files[name];
}

FilePtr FilePool::get_or_make_file(const string &name)
{
    map<string, FilePtr>::iterator it;

    it = _files.find(name);
    if (it == _files.end()) {
	FilePtr f(new File(name));
	_files[name] = f;
	it = _files.find(name);
    }
    return it->second;
}

list<FilePtr> FilePool::files()
{
    return map_values(_files);
}

// ------------------------------------------------------------

Process::Process(const PagePoolPtr &page_pool,
		 pid_t pid)
    : _pid(pid),
      _page_pool(page_pool)
{ }

const PagePoolPtr &Process::page_pool()
{
    return _page_pool;
}

boost::shared_ptr<Process> Process::selfptr()
{
    return _selfptr.lock();
}

void Process::selfptr(boost::shared_ptr<Process> &p)
{
    _selfptr = p;
}

bool Process::load(SysInfoPtr &sys_info)
{
    _cmdline = sys_info->read_cmdline(_pid);
    if (_cmdline.empty()) {
	_cmdline = "[nocmdline]";
    }
    string::size_type space = _cmdline.find(' ');
    if (space != string::npos) {
        _cmdline.erase(space);
    }

    if (!sys_info->read_vmas(_page_pool, _pid, _vmas)) {
	warn << "Process::load - can't load vmas: " << _pid << "\n";
	return false;
    }

    // Can't load pages if we don't have any...
    if (!has_mm()) { return true; }
    
    if (!load_page_info(sys_info)) {
	warn << "Process::load - can't load page info: " << _pid << "\n";
	return false;
    }

    remove_vdso_if_nopages();

    return true;
}

void Process::remove_vdso_if_nopages()
{
    stringstream pref;
    pref << pid() << " remove_vdso_if_nopages: ";
    list<VmaPtr>::iterator it;
    
    for (it = _vmas.begin(); it != _vmas.end(); ++it) {
	if ((*it)->is_vdso() && (*it)->num_pages() == 0) {
	    dbg << pref.str() << "removing\n";
	    _vmas.erase(it);
	    return;
	}
    }
}

string Process::cmdline()
{
    return _cmdline;
}

list<FilePtr> Process::files()
{
    list<FilePtr> files(_files.begin(), _files.end());
    return files;
}

list<MapPtr> Process::maps()
{
    return _maps;
}


pid_t Process::pid()
{
    return _pid;
}

SizesPtr Process::sizes()
{
    return Map::sum_sizes(_page_pool, _maps);
}

SizesPtr Process::sizes(const FilePtr &file)
{
    list<MapPtr> maps;
    maps = restrict_maps_to_file(file);
    return Map::sum_sizes(_page_pool, maps);
}

SizesPtr Process::sizes(const FilePtr &file,
			const RangePtr &elf_range)
{
    list<MapPtr> maps;
    list<MapPtr>::iterator it;
    maps = restrict_maps_to_file(file);
    SizesPtr sizes(new Sizes);

    for (it = maps.begin(); it != maps.end(); ++it) {
	RangePtr map_elf_range = (*it)->elf_range();
	if (map_elf_range) {
	    RangePtr subrange = elf_range->intersect(*map_elf_range);
	    if (subrange) {
		RangePtr mem_range = (*it)->elf_to_mem_range(subrange);
		SizesPtr subsizes = (*it)->sizes_for_mem_range(_page_pool,
							       mem_range);
		if (subsizes) {
		    sizes->add(subsizes);
		}
	    }
	}
    }
    return sizes;
}

list<MapPtr> Process::restrict_maps_to_file(const FilePtr &file)
{
    list<MapPtr> file_maps = file->maps();
    list<MapPtr> proc_maps = _maps;
    list<MapPtr> result = Map::intersect_lists(file_maps, proc_maps);
    if (result.size() == 0) {
	warn << _pid << ": empty restriction to file " << file->name() << "\n";
    }
    return result;
}



void Process::add_file(const FilePtr &file)
{
    _files.insert(file);
}

bool Process::calculate_maps(FilePoolPtr &file_pool)
{
    MapCalculator mc(_vmas, file_pool, selfptr());

    return mc.calc_maps(_maps) && !_maps.empty();
}



bool Process::load_page_info(SysInfoPtr &sys_info)
{
    map<Address, list<Page> > page_info;
    stringstream pref;
    pref << pid() << " load_page_info: ";

    if (!sys_info->read_page_info(_pid, page_info)) {
	warn << pref.str() << "can't read page info for " << _pid;
	return false;
    }

    map<Address, list<Page> >::iterator pi_it;
    for (pi_it = page_info.begin(); pi_it != page_info.end(); ++pi_it) {
	Address start_address = pi_it->first;
	VmaPtr vma;
	if (!find_vma_by_addr(start_address, vma)) {
	    // This can happen, a process can alloc whilst we are
	    // running
	    warn << pref.str() << "can't find vma at "
		 << hex << start_address << dec << ": " << _pid << "\n";
	    continue;
	}
	
	if (pi_it->second.size() == 0) {
	    warn << pref.str() << "VMA with no pages " << start_address << "\n";
	}
	vma->add_pages(pi_it->second);
	_page_pool->inc_pages_count(pi_it->second);
    }

    return true;
}


bool Process::has_mm()
{
    return _vmas.size() > 0;
}

bool Process::find_vma_by_addr(Address start,
				VmaPtr &vma)
{
    list<VmaPtr>::const_iterator it;

    for (it = _vmas.begin(); it != _vmas.end(); ++it) {
	if ((*it)->start() == start) {
	    vma = *it;
	    return true;
	}
    }
    return false;
}


void Process::print(ostream &os) const
{
    os << "PID: " << _pid << "\n"
	<< "CMD: " << _cmdline << "\n";
    std::list<MapPtr>::const_iterator map_it;
    for (map_it = _maps.begin(); map_it != _maps.end(); ++map_it) {
	os << **map_it << "\n";
    }
    os << "\n";
}



// ------------------------------------------------------------

Vma::Vma(Elf::Address start,
	 Elf::Address end,
	 off_t offset,
	 const std::string &fname)
    : _offset(offset),
      _fname(fname)
{
    _range = RangePtr(new Range(start, end));
}

Elf::Address Vma::start() { return _range->start(); }

Elf::Address Vma::end() { return _range->end(); }

RangePtr Vma::range() { return _range; }

std::string Vma::fname() { return _fname; }

off_t Vma::offset() { return _offset; }

Elf::Address Vma::vm_size() { return _range->size(); }

int Vma::num_pages()
{
    return _pages.size();
}

boost::shared_ptr<Vma> Vma::selfptr()
{
    return _selfptr.lock();
}

void Vma::selfptr(boost::shared_ptr<Vma> &p)
{
    _selfptr = p;
}

    
void Vma::add_pages(const list<Page> &pages)
{
    list<Page>::const_iterator it;
    for (it = pages.begin(); it != pages.end(); ++it) {
	_pages.push_back(*it);
    }
}

bool Vma::is_vdso()
{
    return fname() == "[vdso]";
}

bool Vma::is_file_backed()
{
    string::size_type pos = _fname.length();
    if (pos == 0) {
	warn << "Zero length file name in vma\n";
	return false;
    }
    // Names like [vdso], [anon], [stack] etc
    return !(_fname[0] == '[' && _fname[pos-1] == ']');
}


bool Vma::addr_to_pgnum(Address addr, unsigned int &pgnum)
{
    if (addr > end()) {
	warn << hex << addr << " is beyond vma end " << end() << dec << "\n";
	return false;
    }
    Address a = Elf::page_align_down(addr);
    if (a < start()) {
	warn << hex << addr << " is before vma start "
	    << start() << dec << "\n";
	return false;
    }
    a -= start();
    pgnum = a / Elf::page_size();
    return true;
}

bool Vma::get_pages_for_range(const RangePtr &mrange,
			      std::list<PartialPageInfo> &ppinfo)
{
    ppinfo.clear();
    
    if (mrange->size() <= 0) {
	warn << "Vma::get_pages_for_range - invalid range\n";
	return false;
    }
    if (!range()->contains(mrange)) {
	warn << "Vma::get_pages_for_range - range "
	    << mrange->to_string() << " outside vma " << to_string() << "\n";
	return false;
    }

    unsigned int start_pgnum, end_pgnum;
    if (!addr_to_pgnum(mrange->start(), start_pgnum)) {
	warn << "Vma::get_pages_for_range - can't get start pgnum\n";
	return false;
    }
    if (!addr_to_pgnum(mrange->end() - 1, end_pgnum)) {
	warn << "Vma::get_pages_for_range - can't get end pgnum\n";
	return false;
    }
    if (start_pgnum > end_pgnum) {
	warn << "start_pgnum greater than end: "
	     << start_pgnum << ", " << end_pgnum << "\n";
	return false;
    }
    if (start_pgnum >= _pages.size()) {
	warn << "Vma::get_pages_for_range - start pgnum out of range: "
	     << start_pgnum << ", " << _pages.size() << " " << _fname << "\n";
	return false;
    }
    if (end_pgnum >= _pages.size()) {
	warn << "Vma::get_pages_for_range - end pgnum out of range: "
	     << end_pgnum << ", " << _pages.size() << "\n";
	return false;
    }

    if (start_pgnum == end_pgnum) {
	ppinfo.push_back(PartialPageInfo(_pages[start_pgnum], mrange->size()));
	return true;
    }

    Address bytes = Elf::page_size()
	- (mrange->start() - Elf::page_align_down(mrange->start()));
    ppinfo.push_back(PartialPageInfo(_pages[start_pgnum], bytes));

    bytes = mrange->end() - Elf::page_align_down(mrange->end() - 1);
    if (bytes > 0) {
	ppinfo.push_back(PartialPageInfo(_pages[end_pgnum], bytes));
    }

    bytes = Elf::page_size();
    for (unsigned int pgnum = start_pgnum + 1; pgnum < end_pgnum; ++pgnum) {
	ppinfo.push_back(PartialPageInfo(_pages[pgnum], bytes));
    }

    return true;
}
	

string Vma::to_string() const
{
    stringstream sstr;
    sstr << hex;
    sstr << _range << ": " << _offset << " " << _fname;
    return sstr.str();
}



// ------------------------------------------------------------

    Page::Page(PageCookie cookie, bool resident, bool writable)
: _cookie(cookie), _resident(resident), _writable(writable)
{ }

PageCookie Page::cookie() const
{
    return _cookie;
}

bool Page::is_mapped() const
{
    return _cookie != 0;
}

bool Page::is_resident() const
{
    return _resident;
}

bool Page::is_writable() const
{
    return _writable;
}

void Page::print(ostream &os) const
{
    os << "(" << _cookie << ":" << _resident << ":" << _writable << ")";
}

// ------------------------------------------------------------

Sizes::Sizes()
{
    for (int i = 0; i < NUM_SIZES; ++i) {
	_values[i] = 0.0;
    }
}

double Sizes::val(int which)
{
    return _values[which];
}

double Sizes::sval(int which)
{
    return _values[which] / (double)_scale_factor;
}

void Sizes::increase(enum Measure which, double amount)
{
    _values[which] += amount;
}

void Sizes::add(const SizesPtr &other)
{
    for (int i = 0; i < NUM_SIZES; ++i) {
	_values[i] += other->_values[i];
    }
}

string Sizes::size_name(int which)
{
    string name = names[which];
    if (!_scale_name.empty()) {
	name += " (" + _scale_name + ")";
    }
    return name;
}

void Sizes::scale_units()
{
    _scale_factor = 1;
    _scale_name = "";
}

void Sizes::scale_kbytes()
{
    _scale_factor = 1024;
    _scale_name = "K";
}

void Sizes::scale_mbytes()
{
    _scale_factor = 1024 * 1024;
    _scale_name = "M";
}

const string Sizes::names[] = {
    "Effective Resident",
    "Effective Mapped",
    "Writable",
    "VM",
    "Sole Mapped",
    "Mapped",
    "Resident",
    "Num Sizes"
};

int Sizes::_scale_factor = 1;
string Sizes::_scale_name;

// ------------------------------------------------------------

File::File(const string &fname)
    : _fname(fname)
{
    if (file_exists(fname)) {
	_elf.reset(new Elf::File);
	if (!_elf->load(_fname, false)) {
	    _elf.reset((Elf::File *) 0);
	}
    }
}

string File::name()
{
    return _fname;
}

list<ProcessPtr> File::procs()
{
    list<ProcessPtr> result(_procs.begin(), _procs.end());
    return result;
}

Elf::FilePtr File::elf()
{
    return _elf;
}

bool File::is_elf()
{
    return _elf != 0;
}

list<MapPtr> File::maps()
{
    return _maps;
}

SizesPtr File::sizes()
{
    if (_procs.empty()) {
	warn << "File::sizes - no processes for file " << name() << "\n";
	SizesPtr null_sizes;
	return null_sizes;
    }
    // std::set doesn't have .front
    ProcessPtr proc = *(_procs.begin());
    // This goes over all procs (because of the _maps), the proc is
    // we're only using the proc to get to the pagepool.
    return Map::sum_sizes(proc->page_pool(), _maps);
}

SizesPtr File::sizes(const RangePtr &elf_range)
{
    SizesPtr null_sizes; // failure return
    stringstream pref;
    pref << "File::sizes " << name() << ": ";

    if (_procs.empty()) {
	warn << pref.str() << "no processes for file\n";
	return null_sizes;
    }

    SizesPtr totals(new Sizes);
    SizesPtr sizes;

    set<ProcessPtr>::iterator proc_it;
    list<MapPtr>::iterator map_it;
    list<MapPtr> maps_for_proc;
    RangePtr subrange, map_elf_range;
    ProcessPtr firstproc = *(_procs.begin()); // set has no ->front()
    PagePoolPtr page_pool = firstproc->page_pool();

    // We need to loop through the procs, because the mapping from
    // ELF virtual address to actual address can be different in each
    for (proc_it = _procs.begin(); proc_it != _procs.end(); ++proc_it) {
	maps_for_proc = Map::intersect_lists((*proc_it)->maps(),
					     _maps);
	if (maps_for_proc.empty()) {
	    warn << pref.str() << "no maps for process "
		<< (*proc_it)->pid() << "\n";
	    return null_sizes;
	}

	for (map_it = maps_for_proc.begin();
	     map_it != maps_for_proc.end();
	     ++map_it) {
	    map_elf_range = (*map_it)->elf_range();
	    if (!map_elf_range) continue;
	    subrange = map_elf_range->intersect(elf_range);
	    if (!subrange) continue;
	    RangePtr mem_range = (*map_it)->elf_to_mem_range(subrange);
	    sizes = (*map_it)->sizes_for_mem_range(page_pool,
						   mem_range);
	    if (sizes) {
		totals->add(sizes);
	    }
	}
    }

    return totals;
}

void File::add_maps(const list<MapPtr> &maps)
{
    _maps.insert(_maps.end(), maps.begin(), maps.end());
}

void File::add_map(const MapPtr &map)
{
    _maps.push_back(map);
}

void File::add_proc(const ProcessPtr &proc)
{
    _procs.insert(proc);
}

// ------------------------------------------------------------

Map::Map(const VmaPtr &vma,
	 const RangePtr &mem_range,
	 const RangePtr &elf_range)
    : _vma(vma), _mem_range(mem_range), _elf_range(elf_range)
{
    if (mem_range->size() <= 0) {
	warn << "Map: zero sized mem range " << to_string() << "\n";
    }
    if (elf_range && elf_range->size() != mem_range->size()) {
	warn << "Map: mismatched elf and mem ranges " << to_string() << "\n";
    }
}

RangePtr Map::mem_range() const
{
    return _mem_range;
}

RangePtr Map::elf_range() const
{
    return _elf_range;
}

Address Map::elf_to_mem_offset()
{
    return _mem_range->start() - _elf_range->start();
}


RangePtr Map::elf_to_mem_range(const RangePtr &elf_range)
{
    if (!_elf_range->contains(elf_range)) {
	warn << "Range " << elf_range->to_string()
	     << " not contained within " << _elf_range->to_string() << "\n";
	return RangePtr((Range *) 0);
    }

    return elf_range->add(elf_to_mem_offset());
}

string Map::to_string() const
{
    stringstream sstr;

    sstr << "MAP: MEM " << _mem_range->to_string()
	 << " ELF " << (_elf_range ? _elf_range->to_string() : "undef")
	 << " FILE " << _vma->fname();
    return sstr.str();
}

SizesPtr Map::sizes_for_mem_range(const PagePoolPtr &pp)
{
    return sizes_for_mem_range(pp, _mem_range);
}

SizesPtr Map::sizes_for_mem_range(const PagePoolPtr &pp,
				  const RangePtr &mrange)
{
    SizesPtr null_sizes;
    SizesPtr sizes(new Sizes);

    if (!_mem_range->contains(mrange)) {
	warn << "Non-overlapping range: " << *mrange
	     << " not within " << _mem_range << "\n";
	return null_sizes;
    }
    
    RangePtr subrange = _mem_range->intersect(*mrange);

    if (subrange->size() == 0) { return sizes; }
    
    std::list<Vma::PartialPageInfo> ppi_info;
    if (!_vma->get_pages_for_range(subrange, ppi_info)) {
	warn << "sizes_for_mem_range: Can't get pages for range "
	    << subrange->to_string() << "\n";
	return null_sizes;
    }

    std::list<Vma::PartialPageInfo>::iterator it;

    for (it = ppi_info.begin(); it != ppi_info.end(); ++it) {
	Page &page = (*it).page;
	int count = pp->count((*it).page);
	int bytes = (*it).bytes;

//	dbg << "page: " << hex << page.cookie() << dec
//	    << " count " << count << " bytes " << bytes << "\n";
	if (count <= 0) {
	    warn << "Invalid count for page\n";
	    continue;
	}

	sizes->increase(Sizes::VM, bytes);

	if (page.is_mapped()) {
	    sizes->increase(Sizes::MAPPED, bytes);
	    sizes->increase(Sizes::EFFECTIVE_MAPPED, (double) bytes / count);
	    if (count == 1) {
		sizes->increase(Sizes::SOLE_MAPPED, bytes);
	    }

	    if (page.is_resident()) {
		sizes->increase(Sizes::RESIDENT, bytes);
		sizes->increase(Sizes::EFFECTIVE_RESIDENT, (double) bytes / count);

		if (page.is_writable()) {
		    sizes->increase(Sizes::WRITABLE, bytes);
		}
	    }
	}
    }
    
    if (sizes->val(Sizes::VM) != subrange->size()) {
	warn << "Size mismatch: vm size " << sizes->val(Sizes::VM)
	     << " range " << subrange->to_string() << "\n";
	return null_sizes;
    }

    return sizes;
}

void Map::print(std::ostream &os) const
{
    os << to_string();
}

SizesPtr Map::sum_sizes(const PagePoolPtr &pp,
			const list<MapPtr> &maps)
{
    SizesPtr sizes(new Sizes);
    list<MapPtr>::const_iterator map_it;
    for (map_it = maps.begin(); map_it != maps.end(); ++map_it) {
	SizesPtr subsizes = (*map_it)->sizes_for_mem_range(pp);
	sizes->add(subsizes);
    }
    return sizes;
}

list<MapPtr> Map::intersect_lists(const list<MapPtr> &a_arg,
				  const list<MapPtr> &b_arg)
{
    list<MapPtr> a(a_arg), b(b_arg), result;

    a.sort();
    b.sort();
    
    while (!a.empty() && !b.empty()) {
	if (a.front() == b.front()) {
	    result.push_back(b.front());
	    b.pop_front();
	    a.pop_front();
	}
	else if (a.front() < b.front()) {
	    a.pop_front();
	}
	else {
	    b.pop_front();
	}
    }

    return result;
}

static bool mapptr_is_less_than(const MapPtr &lhs, const MapPtr &rhs)
{
    return lhs->mem_range()->start() < rhs->mem_range()->start();
}

list<MapPtr> Map::sort(const list<MapPtr> &maplist)
{
    list<MapPtr> result(maplist);
    result.sort(mapptr_is_less_than);
    return result;
}

// ------------------------------------------------------------

SysInfo::~SysInfo()
{ }

LinuxSysInfo::~LinuxSysInfo()
{ }

const std::string LinuxSysInfo::EXMAP_FILE("/proc/exmap");

list<pid_t> LinuxSysInfo::accessible_pids()
{
    list<pid_t> pids;
    pid_t pid;
    list<string> fnames;
    list<string>::iterator it;
    
    if (!read_directory("/proc", fnames)) {
	warn << "accessible_pids - can't read /proc\n";
	return pids; // empty return
    }

    for (it = fnames.begin(); it != fnames.end(); ++it) {
        if (isdigit((*it)[0])) {
	    pid = atoi(it->c_str());
	    string mapfile = proc_map_file(pid);
	    if (file_readable(mapfile)) {
	      pids.push_back(pid);
	    }
	}
    }

    return pids;
}

bool LinuxSysInfo::sanity_check()
{
    if (!file_exists(EXMAP_FILE)) {
	warn << "Can't find file " << EXMAP_FILE
	     << ": please check kernel module is loaded\n";
	return false;
    }

    return true;
}

bool LinuxSysInfo::read_page_info(pid_t pid,
				  map<Address, list<Page> > &page_info)
{
    list<string> lines;
    page_info.clear();

    stringstream sstr;
    sstr << pid << "\n";
    if (!overwrite_textfile(EXMAP_FILE, sstr.str())) {
	warn << "read_page_info - can't write to exmap: "
	     << pid <<"\n";
	return false;
    }

    if (!read_textfile(EXMAP_FILE, lines)) {
	warn << "read_page_info - can't read exmap: " << pid << "\n";
	return false;
    }

    list<Page> empty_pagelist;
    list<Page> *current_pagelist = NULL;
    
    list<string>::const_iterator it;
    for (it = lines.begin(); it != lines.end(); ++it) {
        // Lines are either:
        // Start a new VMA:
        // VMA <vma base as %p> <npages>
        // or
        // Page info
        // <pfn (as %p)> <writable:bool> <swap_entry>
	if (it->length() < 3) {
	    warn << "read_page_info - short line: " << *it << "\n";
	    continue;
	}

	if (it->substr(0, 3) == "VMA") {
	    Address start_addr;
	    string token;
	    sstr.str(it->substr(4)); // "VMA deadbeef"
	    sstr >> hex >> start_addr;

	    // Put a (copy of) the empty pagelist in for this address
	    page_info[start_addr] = empty_pagelist;

	    // And remember where to put the pages
	    current_pagelist = &(page_info[start_addr]);
	}
	else {
	    bool writable, resident;
	    PageCookie cookie;
	    if (!parse_page_line(*it, resident, writable, cookie)) {
		warn << "read_page_info - bad page line:" << pid << "\n";
		continue;
	    }
	    if (current_pagelist == NULL) {
		warn << "read_page_info: page info line before VMA\n";
		continue;
	    }
	    Page p(cookie, resident, writable);
	    current_pagelist->push_back(p);
	}
    }

    return true;
}

bool LinuxSysInfo::parse_page_line(const string &line,
	bool &resident,
	bool &writable,
	PageCookie &cookie)
{
    stringstream sstr(line);

    int i, i_resident;

    sstr >> i_resident;
    resident = i_resident;
    sstr >> i;
    writable = i;

    sstr >> hex >> cookie;
    cookie |= (i_resident << 31);

    return true;
}

std::string LinuxSysInfo::read_cmdline(pid_t pid)
{
    stringstream fname;
    list<string> lines;
    string cmdline;

    fname << "/proc/" << pid << "/cmdline";
    if (read_textfile(fname.str(), lines)
	&& lines.size() == 1) {
	cmdline = lines.front();
    }
    

    return cmdline;
}

bool LinuxSysInfo::read_vmas(const PagePoolPtr &pp,
			     pid_t pid,
			     list<VmaPtr> &vmas)
{
    vmas.clear();
    string mapfile = proc_map_file(pid);

    list<string> lines;

    if (!read_textfile(mapfile, lines)) {
	warn << "read_vmas - can't load maps for: " << pid << "\n";
	return false;
    }

    list<string>::iterator it;
    for(it = lines.begin(); it != lines.end(); ++it) {
	VmaPtr vma = parse_vma_line(*it);
	if (!vma) {
	    warn << "read_vmas - can't parse line\n";
	    continue;
	}
	vma->selfptr(vma);
	vmas.push_back(vma);
    }
    return true;
}



VmaPtr LinuxSysInfo::parse_vma_line(const string &line_arg)
{
    const string ANON_NAME("[anon]");
    string line(line_arg);

    string perms;
    Elf::Address start, end;
    off_t offset;
    string fname;
    string::size_type dashpos;
    VmaPtr vma;
    string token;
    

    // Break the range into two hex numbers
    dashpos = line.find('-');
    if (dashpos == string::npos) {
	warn << "Can't find dash in VMA line: " << line << "\n";
	return vma;
    }
    line[dashpos] = ' ';
    
    stringstream sstr(line);
    sstr >> hex >> start;
    sstr >> end;
    sstr >> perms;
    sstr >> offset;
    sstr >> token;
    sstr >> token;

    sstr >> fname;
    if (fname.empty()) {
	fname = ANON_NAME;
    }

    vma.reset(new Vma(start, end, offset, fname));

    dbg << "Parsed vma: " << hex << start << ", " << end
	<< ", " << offset << ": " << fname << "\n";

    return vma;
}

string LinuxSysInfo::proc_map_file(pid_t pid)
{
    stringstream sstr;
    sstr << "/proc/" << pid << "/maps";
    return sstr.str();
}



    
MapCalculator::MapCalculator(const list<VmaPtr> &vmas,
			     FilePoolPtr &file_pool,
			     const ProcessPtr &proc)
: _vmas(vmas), _file_pool(file_pool), _proc(proc)
{
    _maps.clear();
}

bool MapCalculator::calc_maps(list<MapPtr> &maps)
{
    walk_vma_files();

    list<string> fnames = map_keys(_fname_to_vmas);
    list<string>::iterator it;
    for (it = fnames.begin(); it != fnames.end(); ++it) {
	if (!calc_maps_for_file(*it)) {
	    warn << "calc_maps: failed to calc for " << *it << "\n";
	    return false;
	}
    }

    if (!add_holes()) {
	warn << "calc_maps: failed to add hole maps\n";
	return false;
    }

    maps = Map::sort(_maps);

    if (!sanity_check(maps)) {
	warn << "calc_maps: sanity check failed\n";
	return false;
    }


    return true;
}

bool MapCalculator::add_holes()
{
    stringstream pref;
    pref << _proc->pid() << " add_holes: ";
    list<MapPtr>::iterator map_it;
    list<RangePtr> map_ranges;

    // this wouldn't require a loop or a (named) helper function in a
    // decent language
    for (map_it = _maps.begin(); map_it != _maps.end(); ++map_it) {
	map_ranges.push_back((*map_it)->mem_range());
    }

    list<VmaPtr>::iterator vma_it;
    list<RangePtr>::iterator hole_it;
    RangePtr null_range;
    for (vma_it = _vmas.begin(); vma_it != _vmas.end(); ++vma_it) {
	VmaPtr &vma = *vma_it;
	dbg << pref.str() << "adding holes for vma range"
	    << vma->range() << "\n";
	list<RangePtr> vma_holes = vma->range()->invert_list(map_ranges);
	FilePtr file = _file_pool->get_or_make_file(vma->fname());
	for (hole_it = vma_holes.begin();
		hole_it != vma_holes.end();
		++hole_it) {
	    MapPtr map(new Map(vma, *hole_it, null_range));
	    _maps.push_back(map);
	    file->add_map(map);
	    dbg << pref.str() << "adding hole " << map->to_string() << "\n";
	}
    }

    return true;
}

bool MapCalculator::sanity_check(const list<MapPtr> &maps)
{
    stringstream pref;
    pref << _proc->pid() << " sanity_check: ";
    list<MapPtr>::const_iterator map_it = maps.begin();
    list<VmaPtr>::iterator vma_it = _vmas.begin();


    while (vma_it != _vmas.end()) {
	VmaPtr &vma = *vma_it;
	dbg << pref.str() << "VMA: " << vma->to_string() << "\n";

	if ((*map_it)->mem_range()->start() != vma->start()) {
	    warn << pref.str() << "map not at start of vma: "
		<< (*map_it)->to_string() << " " << vma->to_string() << "\n";
	    return false;
	}

	MapPtr lastmap;
	while ((*map_it)->mem_range()->end() < vma->end()) {
	    dbg << pref.str() << (*map_it)->to_string() << "\n";
	    if (!vma->range()->contains((*map_it)->mem_range())) {
		warn << pref.str() << "map not contained within vma: "
		    << (*map_it)->to_string() << " " << vma->to_string()
		    << "\n";
		return false;
	    }

	    ++map_it;
	    dbg << pref.str() << (*map_it)->to_string() << "\n";

	    if (map_it == maps.end()) {
		warn << pref.str() << "maps don't cover vma "
		    << vma->to_string() << "\n";
		return false;
	    }
	    if (lastmap && lastmap->mem_range()->end() != (*map_it)->mem_range()->start()) {
		warn << pref.str() << "maps are not contiguous "
		    << lastmap->to_string() << " "
		    << (*map_it)->to_string() << "\n";
		warn << dump_maps_to_string(maps);
		return false;
	    }
	    lastmap = *map_it;
	}

	if ((*map_it)->mem_range()->end() != vma->end()) {
	    warn << pref.str() << "map doesn't end at end of vma: "
		<< (*map_it)->to_string() << " " << vma->to_string() << "\n";
	    return false;
	}
	
	++vma_it;
	++map_it;
	if (vma_it != _vmas.end() && map_it == maps.end()) {
	    warn << pref.str() << "not enough maps for vmas "
		<< vma->to_string() << "\n";
	    return false;
	}
	if (vma_it != _vmas.end()) {
	    dbg << pref.str() << "VMA: " << vma->to_string() << "\n";
	}
	if (map_it != maps.end()) {
	    dbg << pref.str() << (*map_it)->to_string() << "\n";
	}
    }

    if (map_it != maps.end()) {
	warn << pref.str() << "too many maps for vmas "
	    << (*map_it)->to_string() << "\n";
	warn << dump_maps_to_string(maps);
	return false;
    }

    return true;
}

string MapCalculator::dump_maps_to_string(const std::list<MapPtr> &maps)
{
    stringstream sstr;
    list<MapPtr>::const_iterator map_it;

    sstr << "----------------------------\n";
    for (map_it = maps.begin(); map_it != maps.end(); ++map_it) {
	sstr << (*map_it)->to_string() << "\n";
    }
    sstr << "----------------------------\n";
    return sstr.str();
}

bool MapCalculator::calc_maps_for_file(const string &fname)
{
    stringstream pref;
    pref << _proc->pid() << " " << fname << " calc_maps_for_file: ";
    FilePtr file = _file_pool->get_or_make_file(fname);

    size_t num_maps_before = _maps.size();
    if (file->is_elf()) {
	dbg << pref.str() << "elf file\n";
	if(!calc_maps_for_elf_file(fname, file)) {
	    warn << pref.str() << "failed to calc elf file maps\n";
	    return false;
	}
    }
    else {
	dbg << pref.str() << "non elf file\n";
	if(!calc_maps_for_nonelf_file(fname, file)) {
	    warn << pref.str() << "failed to calc nonelf file maps\n";
	    return false;
	}
    }

    if (_maps.size() == num_maps_before) {
	warn << pref.str() << "no maps added for file\n";
	return false;
    }

    return true;
}

bool MapCalculator::calc_maps_for_nonelf_file(const string &fname,
	const FilePtr &file)
{
    stringstream pref;
    pref << _proc->pid() << " " << fname << " calc_maps_for_nonelf_file: ";
    list<VmaPtr> filevmas = _fname_to_vmas[fname];

    if (filevmas.empty()) {
	warn << pref.str() << "no vmas for nonelf file\n";
	return false;
    }

    list <VmaPtr>::const_iterator it;
    RangePtr null_range;
    for (it = filevmas.begin(); it != filevmas.end(); ++it) {
	const VmaPtr &vma = *it;
	// filevmas includes any trailing non file backed vmas. we don't
	// want them here
	if (vma->fname() != fname) {
	    continue;
	    // not a warning
	    dbg << pref.str() << "vma name mismatch "
		<< vma->to_string() << "\n";
//	    warn << pref.str() << "vma name mismatch "
//		<< vma->to_string() << "\n";
	}
	MapPtr map(new Map(vma, vma->range(), null_range));
	_maps.push_back(map);
	file->add_map(map);
	dbg << pref.str() << "adding nonelf map " << map->to_string() << "\n";
    }

    return true;
}

bool MapCalculator::calc_maps_for_elf_file(const string &fname,
	const FilePtr &file)
{
    stringstream pref;
    pref << _proc->pid() << " " << fname << " calc_maps_for_elf_file: ";
    list<Elf::SegmentPtr> segs = file->elf()->loadable_segments();
    if (segs.empty()) {
	warn << pref.str() << "no loadable segments\n";
	return false;
    }

    list<VmaPtr> filevmas = _fname_to_vmas[fname];
    if (filevmas.empty()) {
	warn << pref.str() << "no vmas for segment\n";
	return false;
    }

    list<Elf::SegmentPtr>::iterator it;
    for (it = segs.begin(); it != segs.end(); ++it) {
	// This method consumes any filevmas it has covered
	if (!calc_map_for_seg(file, *it, filevmas)) {
	    warn << pref.str() << "can't calc map for seg\n";
	    return false;
	}
    }

    return true;
}

bool MapCalculator::calc_map_for_seg(const FilePtr &file,
	const Elf::SegmentPtr &seg,
	list<VmaPtr> &filevmas)
{
    stringstream pref;
    string fname = file->name();
    pref << _proc->pid() << " " << fname << " calc_map_for_seg: ";

    dbg << pref.str() << "calc_map_for_seg "
	<< seg->file_range()->to_string() << "\n";

    if (filevmas.empty()) {
	warn << pref.str() << "empty vma list\n";
    }
    if(!filevmas.front()->is_file_backed()) {
	warn << pref.str() << "non-file backed first vma\n";
	return false;
    }
    
    list<VmaPtr>::const_iterator it;
    Address seg_to_mem = 0;
    for (it = filevmas.begin(); it != filevmas.end(); ++it) {
	const VmaPtr &vma = *it;

	// Save the seg-to-mem for the last file backed, for following
	// anon map
	if (vma->is_file_backed()) {
	    Address segmem_base = seg->mem_range()->start() - seg->offset();
	    Address vmamem_base = vma->start() - vma->offset();
	    seg_to_mem = vmamem_base - segmem_base;
	}

	RangePtr seg_mem_range = seg->mem_range()->add(seg_to_mem);
	RangePtr working_mrange = seg_mem_range->intersect(*(vma->range()));
	if (!working_mrange || working_mrange->size() <= 0) {
	    continue;
	}
	RangePtr elf_mem_range = working_mrange->subtract(seg_to_mem);
	MapPtr map(new Map(vma, working_mrange, elf_mem_range));
	_maps.push_back(map);
	file->add_map(map);
	dbg << pref.str() << "adding elf map " << map->to_string() << "\n";

	if (!vma->is_file_backed()) {
	    break;
	}
    }

    if (_maps.empty()) {
	warn << pref.str() << "empty maps after loop\n";
	return false;
    }

    filevmas.pop_front();
    dbg << pref.str() << "consuming vma\n";
    if (!filevmas.empty() && !filevmas.front()->is_file_backed()) {
	filevmas.pop_front();
	dbg << pref.str() << "consuming anon vma\n";
    }

    return true;
}


void MapCalculator::walk_vma_files()
{
    list<VmaPtr>::iterator it;

    string last_file_backed;
    for(it = _vmas.begin(); it != _vmas.end(); ++it) {
	VmaPtr &vma = *it;

	FilePtr file = _file_pool->get_or_make_file(vma->fname());
	file->add_proc(_proc);
	_proc->add_file(file);

	// associate non-file-backed vmas with the last file backed
	// one
	if (vma->is_file_backed()) {
	    last_file_backed = vma->fname();
	}
	if (!last_file_backed.empty()) {
	    _fname_to_vmas[last_file_backed].push_back(vma);
	}
    }
}

