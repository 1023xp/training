/*
 * (c) John Berthels 2005 <jjberthels@gmail.com>. See COPYING for license.
 */
#include <gtkmm.h>
#include <gtkmm/window.h>

#include "Exmap.hpp"
#include "jutil.hpp"

#include <boost/shared_ptr.hpp>

#include <sstream>
#include <iostream>
#include <list>


namespace Gexmap
{
    typedef boost::shared_ptr<Gtk::Widget> WidgetPtr;
    
    /// Abstract base class for all the list. This will automatically
    /// add in the columns needed for 'Sizes' after those added by the
    /// subclass, as well as setting up conveniences such as
    /// scrollbars, sortable columns etc.
    class SizeListView : public Gtk::Frame
    {
    public:
	SizeListView(const std::string &frame_name);
	virtual ~SizeListView();

	Gtk::TreeView &listwin();
	void update_view();

    protected:
	/// Call this from your ctor, after adding your columns to
	/// _coluns.
	void init_store_and_view();
	/// Call this from your ctor, after appending your columns to
	/// the treeview.
	void init_columns();
	/// When creating a new Row, call this to add in the values
	/// for the Sizes.
	void add_row_sizes(Gtk::TreeModel::Row &row,
			   Exmap::SizesPtr &sizes);

	/// Show the list view (hiding the label), and clear the store
	void show_and_clear_list();

	/// Show the label, with this text (hiding the list)
	void show_label(const std::string &txt);

	/// True if we are showing the list, false if the label
	bool showing_list;

	/// Adding a lot of data to a list can be costly, so bracket
	/// it with calls to these funcs.
	/// sorting with these funcs
	void start_mass_insert();
	void finished_mass_insert();
	
	Glib::RefPtr<Gtk::ListStore> _store;
	// We display either the treeview or the errlabel
	Gtk::TreeView _treeview;
	Gtk::Label _errlabel;
	
	// Hack: reparent the unused child widget to here
	Gtk::Frame _hidden;

	/// In the derived class, add columns definitions to this
	/// record in your ctor.
	Gtk::TreeModel::ColumnRecord _columns;
    private:
	Gtk::TreeModelColumn<float> _size_columns[Exmap::Sizes::NUM_SIZES];
	void _make_all_sortable_and_resizeable();
	static std::string SIZES_PRINTF_FORMAT;
	Gtk::ScrolledWindow _scrolled_window;
	int _saved_sort_id;
	Gtk::SortType _saved_sort_order;
    };
    typedef boost::shared_ptr<SizeListView> SizeListViewPtr;

    /// Abstract base class for showing processes in a list
    class ProcList : public SizeListView
    {
    public:
	ProcList(const std::string &frame_name);
	pid_t currently_selected();
    protected:
	void add_row(pid_t pid,
		     const std::string &cmdline,
		     Exmap::SizesPtr &sizes);
	Gtk::TreeModelColumn<pid_t> _pid;
	Gtk::TreeModelColumn<Glib::ustring> _cmdline;
    };
	
    /// Concrete subclass for showing a top-level list of processes
    class AllProcList : public ProcList
    {
    public:
	AllProcList();
	void set_data(const std::list<Exmap::ProcessPtr> &procs);
    private:
	Exmap::SizesPtr calc_totals(const std::list<Exmap::ProcessPtr> &procs);
    };
    
    /// Concrete subclass for showing the list of process which map a file
    class PerFileProcList : public ProcList
    {
    public:
	PerFileProcList();
	void set_data(const Exmap::FilePtr &file);
    };


    
    class AllFileList : public SizeListView
    {
    public:
	AllFileList();
	void set_data(const std::list<Exmap::FilePtr> &files);
	std::string currently_selected();
    private:
	Gtk::TreeModelColumn<Glib::ustring> _filename;
	Gtk::TreeModelColumn<int> _nprocs;
    };
    
    /// Concrete subclass for showing all files within a process
    class PerProcFileList : public SizeListView
    {
    public:
	PerProcFileList();
	void set_data(const Exmap::ProcessPtr &proc);
	std::string currently_selected();
    private:
	Gtk::TreeModelColumn<Glib::ustring> _filename;
    };
    

    /// Concrete subclass for showing the elf sections for a specific
    /// process and file.
    class ElfSectionList : public SizeListView
    {
    public:
	ElfSectionList();
	Gtk::TreeModelColumn<Glib::ustring> _name;
	Gtk::TreeModelColumn<off_t> _file_offset;
	void set_data(const Exmap::ProcessPtr &proc,
		      const Exmap::FilePtr &file,
		      // if true, ignore proc and show totals across all
		      bool show_all_procs = false);
	std::string currently_selected();
    };
    
    /// Concrete subclass for showing the elf symbols for a specific
    /// process, file and elf section
    class ElfSymbolList : public SizeListView
    {
    public:
	ElfSymbolList();
	Gtk::TreeModelColumn<Glib::ustring> _name;
	void set_data(const Exmap::ProcessPtr &proc,
		      const Exmap::FilePtr &file,
		      const Elf::SectionPtr &section);
    };

    /// Abstract base class for the file and proc tabs
    class ExmapTab : public Gtk::VPaned
    {
    public:
	ExmapTab();
	virtual void set_data(Exmap::SnapshotPtr &snapshot) = 0;
    protected:
	Gtk::VPaned _top_half;
	Gtk::VPaned _bottom_half;
	// Internal data
	Exmap::SnapshotPtr _snapshot;

	// Widgets
	ElfSectionList _sectionlist;
	ElfSymbolList _symlist;
    };


    /// The 'Files' Tab
    class FileTab : public ExmapTab
    {
    public:
	FileTab();
	void set_data(Exmap::SnapshotPtr &snapshot);
    private:
	// Callbacks
	void filelist_changed_cb();
	void proclist_changed_cb();
	void sectionlist_changed_cb();
	void all_procs_toggled_cb();

	void recalc_sectionlist();

	// Widgets
	AllFileList _allfilelist;
	Gtk::CheckButton _all_procs_checkbutton;
	Gtk::VBox _vbox;
	PerFileProcList _proclist;
    };

    /// The 'Processes' Tab
    class ProcTab : public ExmapTab
    {
    public:
	ProcTab();
	void set_data(Exmap::SnapshotPtr &snapshot);
    private:
	// Callbacks
	void proclist_changed_cb();
	void filelist_changed_cb();
	void sectionlist_changed_cb();
	
	// Widgets
	AllProcList _allproclist;
	PerProcFileList _filelist;
    };

    /// Status and button bar
    class BottomBar : public Gtk::HBox
    {
    public:
	BottomBar();
	void set_status(Exmap::SnapshotPtr &snapshot);
    private:
	Gtk::Label _plabel;
	Gtk::Label _flabel;
	Gtk::Button _quit_button;
	Gtk::Button _about_button;
	Gtk::Dialog _about_dialog;
	Gtk::Label _about_label;
	// Callbacks
	void quit_button_clicked_cb();
	void about_button_clicked_cb();
	void about_dialog_response_cb(int response);
    };
    
    /// The main, toplevel window
    class TopWin : public Gtk::Window
    {
    public:
	TopWin(Exmap::SnapshotPtr &snapshot);
	void load();
	static const int WIDTH = 800;
	static const int HEIGHT = 600;
    private:
	// Internal data
	Exmap::SnapshotPtr _snapshot;
	// Widgets
	ProcTab _proctab;
	FileTab _filetab;
	Gtk::Notebook _notebook;
	Gtk::VBox _vbox;
	BottomBar _bottom_bar;
    };
    
};

using namespace std;
using namespace jutil;
using namespace Exmap;
using namespace Gexmap;

// ------------------------------------------------------------

TopWin::TopWin(SnapshotPtr &snapshot)
    : _snapshot(snapshot)
{
    add(_vbox);
    _notebook.append_page(_proctab, "Processes");
    _notebook.append_page(_filetab, "Files");
    _vbox.add(_notebook);
    _vbox.pack_end(_bottom_bar, false, false);

    set_default_size(WIDTH, HEIGHT);

    show_all();
}

void TopWin::load()
{
    _snapshot->load();
    _proctab.set_data(_snapshot);
    _filetab.set_data(_snapshot);
    _bottom_bar.set_status(_snapshot);
}


// ------------------------------------------------------------

BottomBar::BottomBar()
    // Modal dialog
    : _about_dialog("About Exmap", true)
{
    _quit_button.set_label("_Quit");
    _quit_button.set_use_underline(true);
    _quit_button.signal_clicked()
	.connect(sigc::mem_fun(*this, &BottomBar::quit_button_clicked_cb));

    _about_button.set_label("_About");
    _about_button.set_use_underline(true);
    _about_button.signal_clicked()
	.connect(sigc::mem_fun(*this, &BottomBar::about_button_clicked_cb));

    stringstream sstr;
    sstr << "Exmap\n\n";
    sstr << "a shared memory analysis utility\n\n";
    sstr << "written by John Berthels\n\n";
    sstr << "http://www.berthels.co.uk/exmap\n";
    _about_label.set_text(sstr.str());
    _about_label.set_justify(Gtk::JUSTIFY_CENTER);

    _about_dialog.get_vbox()->add(_about_label);
    _about_dialog.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
    _about_dialog.signal_response()
	.connect(sigc::mem_fun(*this, &BottomBar::about_dialog_response_cb));

    add(_plabel);
    add(_flabel);
    pack_end(_quit_button, false, false);
    pack_end(_about_button, false, false);
}

void BottomBar::quit_button_clicked_cb()
{
    Gtk::Main::quit();
}

void BottomBar::about_button_clicked_cb()
{
    _about_dialog.show_all();
}

void BottomBar::about_dialog_response_cb(int response)
{
    // Don't care what the response is...
    (void) response;
    _about_dialog.hide();
}

void BottomBar::set_status(SnapshotPtr &snapshot)
{
    stringstream sstr;

    sstr << "Number of procs: " << snapshot->procs().size();
    _plabel.set_text(sstr.str());
    sstr.str("");
    sstr << "Number of files: " << snapshot->files().size();
    _flabel.set_text(sstr.str());
}

// ------------------------------------------------------------

SizeListView::SizeListView(const std::string &frame_name)
    : Gtk::Frame(frame_name),
      showing_list(true),
      _saved_sort_id(Gtk::TreeSortable::DEFAULT_UNSORTED_COLUMN_ID),
      _saved_sort_order(Gtk::SORT_DESCENDING)
{
    _hidden.add(_errlabel);
    _scrolled_window.add(_treeview);
    _scrolled_window.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    add(_scrolled_window);

    _errlabel.show();
    _scrolled_window.show_all();
}

void SizeListView::finished_mass_insert()
{
    if (_saved_sort_id == Gtk::TreeSortable::DEFAULT_UNSORTED_COLUMN_ID) { return; }
    _store->set_sort_column_id(_saved_sort_id, _saved_sort_order);
    _saved_sort_id = Gtk::TreeSortable::DEFAULT_UNSORTED_COLUMN_ID;
}

void SizeListView::start_mass_insert()
{
    _store->get_sort_column_id(_saved_sort_id, _saved_sort_order);
    _store->set_sort_column_id(Gtk::TreeSortable::DEFAULT_UNSORTED_COLUMN_ID,
			       Gtk::SORT_DESCENDING);
}

Gtk::TreeView &SizeListView::listwin()
{
    return _treeview;
}

SizeListView::~SizeListView()
{ }

void SizeListView::init_columns()
{
    Glib::RefPtr<Gtk::TreeModel> model = _treeview.get_model();

    int sort_colid = -1;
    for (int i = 0; i < Exmap::Sizes::NUM_SIZES; ++i) {
	int colid = _treeview.append_column_numeric(Exmap::Sizes::size_name(i),
						    _size_columns[i],
						    SIZES_PRINTF_FORMAT);
	
	// Remember the number of columns first time round the loop
	if (sort_colid < 0) {
	    sort_colid = colid - 1;
	}
    }
    
    _make_all_sortable_and_resizeable();
    _store->set_sort_column_id(sort_colid, Gtk::SORT_DESCENDING);
}

void SizeListView::_make_all_sortable_and_resizeable()
{
    int ncols = _columns.size();
    Gtk::TreeViewColumn *col;
    
    for (int i = 0; i < ncols; ++i) {
	col = _treeview.get_column(i);
	col->set_sort_column(i);
	col->set_resizable(true);
    }
    return;
}

void SizeListView::init_store_and_view()
{
    for (int i = 0; i < Exmap::Sizes::NUM_SIZES; ++i) {
	_columns.add(_size_columns[i]);
    }
    _store = Gtk::ListStore::create(_columns);
    _treeview.set_model(_store);
}

void SizeListView::add_row_sizes(Gtk::TreeModel::Row &row,
				 Exmap::SizesPtr &sizes)
{
    for (int i = 0; i < Exmap::Sizes::NUM_SIZES; ++i) {
	row[_size_columns[i]] = sizes->sval(i);
    }
}

// We scale to kbytes, so support 4G => 4000000-ish max => 7 leading
// digits, plus 2 digits after decimal pt plus the pt = 10 chars in
// total.
string SizeListView::SIZES_PRINTF_FORMAT("%.2f");

void SizeListView::show_label(const string &txt)
{
    _errlabel.set_text(txt);
    if (!showing_list) { return; }

    Gtk::Frame tmp;

    _errlabel.reparent(tmp);
    _scrolled_window.reparent(_hidden);
    _errlabel.reparent(*this);
    showing_list = false;
}

void SizeListView::show_and_clear_list()
{
    _store->clear();
    if (showing_list) { return; }

    Gtk::Frame tmp;

    _scrolled_window.reparent(tmp);
    _errlabel.reparent(_hidden);
    _scrolled_window.reparent(*this);
    showing_list = true;
}

// ------------------------------------------------------------

ProcList::ProcList(const std::string &frame_name)
    : SizeListView(frame_name)
{
    _columns.add(_pid);
    _columns.add(_cmdline);
    init_store_and_view();
    _treeview.append_column("PID", _pid);
    _treeview.append_column("Cmdline", _cmdline);
    init_columns();
}
    
pid_t ProcList::currently_selected()
{
    Glib::RefPtr<Gtk::TreeSelection> sel = listwin().get_selection();
    Glib::RefPtr<Gtk::TreeModel> model = listwin().get_model();
    pid_t pid = 0; // invalid

    if (sel->count_selected_rows() == 1) {
	Gtk::TreeModel::iterator iter = sel->get_selected(model);
	if (iter) {
	    Gtk::TreeModel::Row row = *iter;
	    pid = row.get_value(_pid);
	}
    }
    return pid;
}

void ProcList::add_row(pid_t pid, const string &cmdline, SizesPtr &sizes)
{

    Gtk::TreeModel::Row row = *(_store->append());
    row[_pid] = pid;
    row[_cmdline] = cmdline;
    add_row_sizes(row, sizes);
}

// ------------------------------------------------------------
    
AllProcList::AllProcList()
    : ProcList("All Processes")
{
}

void AllProcList::set_data(const list<ProcessPtr> &procs)
{
    list<Exmap::ProcessPtr>::const_iterator it;

    if (procs.empty()) {
	show_label("No processes found");
	return;
    }

    show_and_clear_list();

    start_mass_insert();
    for (it = procs.begin(); it != procs.end(); ++it) {
	SizesPtr sizes = (*it)->sizes();
	add_row((*it)->pid(),
		(*it)->cmdline(),
		sizes);
    }

    SizesPtr totals = calc_totals(procs);
    add_row(0, "TOTALS", totals);
    finished_mass_insert();
}

SizesPtr AllProcList::calc_totals(const list<ProcessPtr> &procs)
{
    list<ProcessPtr>::const_iterator it;
    SizesPtr totals (new Sizes);
    for (it = procs.begin(); it != procs.end(); ++it) {
	SizesPtr sizes = (*it)->sizes();
	totals->add(sizes);
    }
    return totals;
}

// ------------------------------------------------------------
    
PerFileProcList::PerFileProcList()
    : ProcList("Sizes for file in processes")
{
    show_label("No file selected");
}

void PerFileProcList::set_data(const Exmap::FilePtr &file)
{
    list<ProcessPtr> procs;

    if (file) {
	procs = file->procs();
    }
    
    if (procs.empty()) {
	string txt = "No processes for file " + file->name();
	show_label(txt);
	return;
    }

    list<Exmap::ProcessPtr>::iterator it;

    show_and_clear_list();

    start_mass_insert();
    for (it = procs.begin(); it != procs.end(); ++it) {
	SizesPtr sizes = (*it)->sizes(file);
	add_row((*it)->pid(),
		(*it)->cmdline(),
		sizes);
    }
    finished_mass_insert();
}
	
// ------------------------------------------------------------

PerProcFileList::PerProcFileList()
    : SizeListView("Sizes for files in selected process")
{
    _columns.add(_filename);
    init_store_and_view();
    _treeview.append_column("Filename", _filename);
    init_columns();
    show_label("No process selected");
}

void PerProcFileList::set_data(const Exmap::ProcessPtr &proc)
{
    list<Exmap::FilePtr> files;
    list<Exmap::FilePtr>::const_iterator it;

    if (proc) {
	files = proc->files();
    }

    if (files.empty()) {
	show_label("No files for process");
	return;
    }
    show_and_clear_list();
    
    start_mass_insert();
    for (it = files.begin(); it != files.end(); ++it) {
	Gtk::TreeModel::Row row = *(_store->append());
	row[_filename] = (*it)->name();
	Exmap::SizesPtr sizes = proc->sizes(*it);
	add_row_sizes(row, sizes);
    }
    finished_mass_insert();
}

string PerProcFileList::currently_selected()
{
    Glib::RefPtr<Gtk::TreeSelection> sel = listwin().get_selection();
    Glib::RefPtr<Gtk::TreeModel> model = listwin().get_model();
    string fname;

    if (sel->count_selected_rows() == 1) {
	Gtk::TreeModel::iterator iter = sel->get_selected(model);
	if (iter) {
	    Gtk::TreeModel::Row row = *iter;
	    fname = row.get_value(_filename);
	}
    }
    return fname;
}

// ------------------------------------------------------------

AllFileList::AllFileList()
    : SizeListView("All Files")
{
    _columns.add(_filename);
    _columns.add(_nprocs);
    init_store_and_view();
    _treeview.append_column("Filename", _filename);
    _treeview.append_column("Number of procs", _nprocs);
    init_columns();
}

void AllFileList::set_data(const list<FilePtr> &files)
{
    list<Exmap::FilePtr>::const_iterator it;

    if (files.empty()) {
	show_label("No files found");
	return;
    }

    show_and_clear_list();
    
    start_mass_insert();
    for (it = files.begin(); it != files.end(); ++it) {
	Gtk::TreeModel::Row row = *(_store->append());
	row[_filename] = (*it)->name();
	row[_nprocs] = (*it)->procs().size();
	Exmap::SizesPtr sizes = (*it)->sizes();
	add_row_sizes(row, sizes);
    }
    finished_mass_insert();
}

string AllFileList::currently_selected()
{
    Glib::RefPtr<Gtk::TreeSelection> sel = listwin().get_selection();
    Glib::RefPtr<Gtk::TreeModel> model = listwin().get_model();
    string fname;

    if (sel->count_selected_rows() == 1) {
	Gtk::TreeModel::iterator iter = sel->get_selected(model);
	if (iter) {
	    Gtk::TreeModel::Row row = *iter;
	    fname = row.get_value(_filename);
	}
    }
    return fname;
}


// ------------------------------------------------------------

ElfSectionList::ElfSectionList()
    : SizeListView("ELF Sections for selected file and process")
{
    _columns.add(_name);
    _columns.add(_file_offset);
    init_store_and_view();
    _treeview.append_column("Section name", _name);
    _treeview.append_column("File offset", _file_offset);
    init_columns();
    show_label("Process and file not selected");
}

void ElfSectionList::set_data(const Exmap::ProcessPtr &proc,
			      const Exmap::FilePtr &file,
			      bool show_all_procs)
{
    list<Elf::SectionPtr> sections;
    list<Elf::SectionPtr>::const_iterator it;
    Exmap::SizesPtr sizes;
    Elf::FilePtr elf;
    string err_label;

    if (!proc && !show_all_procs) {
	if (file) {
	    err_label = "No process selected";
	}
	else {
	    err_label = "No process and file selected";
	}
    }

    if (!file) {
	err_label = "No file selected";
    }
    else {
	elf = file->elf();
	if (!elf) {
	    err_label = file->name() + " is not an elf file";
	}
    }

    if (!err_label.empty()) {
	show_label(err_label);
	return;
    }

    show_and_clear_list();

    sections = elf->mappable_sections();
    start_mass_insert();
    for (it = sections.begin(); it != sections.end(); ++it) {
	Gtk::TreeModel::Row row = *(_store->append());
	row[_name] = (*it)->name();
	row[_file_offset] = (*it)->file_range()->start();
	if (show_all_procs) {
	    sizes = file->sizes((*it)->mem_range());
	}
	else {
	    sizes = proc->sizes(file, (*it)->mem_range());
	}
	add_row_sizes(row, sizes);
    }
    finished_mass_insert();
}

string ElfSectionList::currently_selected()
{
    Glib::RefPtr<Gtk::TreeSelection> sel = listwin().get_selection();
    Glib::RefPtr<Gtk::TreeModel> model = listwin().get_model();
    string section_name;

    if (sel->count_selected_rows() == 1) {
	Gtk::TreeModel::iterator iter = sel->get_selected(model);
	if (iter) {
	    Gtk::TreeModel::Row row = *iter;
	    section_name = row.get_value(_name);
	}
    }
    return section_name;
}



// ------------------------------------------------------------


ElfSymbolList::ElfSymbolList()
    : SizeListView("ELF symbols within selected section")
{
    _columns.add(_name);
    init_store_and_view();
    _treeview.append_column("Symbol name", _name);
    init_columns();
    show_label("Process, file and section not selected");
}

void ElfSymbolList::set_data(const Exmap::ProcessPtr &proc,
			     const Exmap::FilePtr &file,
			     const Elf::SectionPtr &section)
{
    list<Elf::SymbolPtr> symbols;
    list<Elf::SymbolPtr>::const_iterator it;
    Elf::FilePtr elf;
    string err_label;

    if (!proc) {
	err_label = "No process selected";
    }
    else {
	if (!file) {
	    err_label = "No file selected";
	}
	else {
	    elf = file->elf();
	    if (!elf) {
		err_label = file->name() + " is not an elf file";
	    }
	    else {
		if (!section) {
		    err_label = "No section selected";
		}
	    }
	}
    }

    if (!err_label.empty()) {
	show_label(err_label);
	return;
    }

    symbols = elf->symbols_in_section(section);
    if (symbols.empty()) {
	string txt = "No symbols found in section "
	    + section->name() + " in file " + file->name();
	show_label(txt);
	return;
    }

    show_and_clear_list();

    Exmap::SizesPtr sizes;
    start_mass_insert();
    for (it = symbols.begin(); it != symbols.end(); ++it) {
	Gtk::TreeModel::Row row = *(_store->append());
	row[_name] = (*it)->name();
	if (proc) {
	    sizes = proc->sizes(file, (*it)->range());
	}
	else {
	    // Null proc means across all processes
	    sizes = file->sizes((*it)->range());
	}
	add_row_sizes(row, sizes);
    }
    finished_mass_insert();
}
 

// ------------------------------------------------------------

ExmapTab::ExmapTab()
{
    add1(_top_half);
    add2(_bottom_half);

    _bottom_half.add1(_sectionlist);
    _bottom_half.add2(_symlist);

    int height = TopWin::HEIGHT;
    set_position(2 * height / 3);
    _top_half.set_position(height / 3);
    _bottom_half.set_position(height / 6);
}

// ------------------------------------------------------------


ProcTab::ProcTab()
{
    _top_half.add1(_allproclist);
    _top_half.add2(_filelist);

    add1(_top_half);
    add2(_bottom_half);

    _allproclist.listwin().get_selection()->signal_changed()
	.connect(sigc::mem_fun(*this, &ProcTab::proclist_changed_cb));
    _filelist.listwin().get_selection()->signal_changed()
    	.connect(sigc::mem_fun(*this, &ProcTab::filelist_changed_cb));
    _sectionlist.listwin().get_selection()->signal_changed()
    	.connect(sigc::mem_fun(*this, &ProcTab::sectionlist_changed_cb));
}

void ProcTab::set_data(SnapshotPtr &snapshot)
{
    _snapshot = snapshot;
    _allproclist.set_data(_snapshot->procs());
}

void ProcTab::proclist_changed_cb()
{
    pid_t pid = _allproclist.currently_selected();
    if (pid != 0) {
	Exmap::ProcessPtr proc = _snapshot->proc(pid);
	_filelist.set_data(proc);
    }
}

void ProcTab::filelist_changed_cb()
{
    pid_t pid = _allproclist.currently_selected();
    string fname = _filelist.currently_selected();
    Exmap::ProcessPtr proc;
    Exmap::FilePtr file;

    if (pid != 0 && !fname.empty()) {
	proc = _snapshot->proc(pid);
	file = _snapshot->file(fname);
    }
    _sectionlist.set_data(proc, file);
    Elf::SectionPtr null_section;
    _symlist.set_data(proc, file, null_section);
}
	
void ProcTab::sectionlist_changed_cb()
{
    pid_t pid = _allproclist.currently_selected();
    string fname = _filelist.currently_selected();
    string section_name = _sectionlist.currently_selected();

    Exmap::ProcessPtr proc;
    Exmap::FilePtr file;
    Elf::SectionPtr section;

    if (pid != 0) {
	proc = _snapshot->proc(pid);
    }
    if (!fname.empty()) {
	file = _snapshot->file(fname);
	Elf::FilePtr elf = file->elf();
	if (elf) { 
	    section = elf->section(section_name);
	}
    }
    _symlist.set_data(proc, file, section);
}
	
// ------------------------------------------------------------


FileTab::FileTab()
	: _all_procs_checkbutton("Show sizes across _all processes", true)
{
    _top_half.add1(_allfilelist);
    _vbox.pack_start(_all_procs_checkbutton, false, false);
    _vbox.add(_proclist);
    _top_half.add2(_vbox);

    _allfilelist.listwin().get_selection()->signal_changed()
    	.connect(sigc::mem_fun(*this, &FileTab::filelist_changed_cb));
    _proclist.listwin().get_selection()->signal_changed()
	.connect(sigc::mem_fun(*this, &FileTab::proclist_changed_cb));
    _sectionlist.listwin().get_selection()->signal_changed()
    	.connect(sigc::mem_fun(*this, &FileTab::sectionlist_changed_cb));
    _all_procs_checkbutton.signal_toggled()
	.connect(sigc::mem_fun(*this, &FileTab::all_procs_toggled_cb));
    // Can't select all procs until you have selected a file
    _all_procs_checkbutton.set_sensitive(false);
}

void FileTab::set_data(SnapshotPtr &snapshot)
{
    _snapshot = snapshot;
    _allfilelist.set_data(_snapshot->files());
}

void FileTab::filelist_changed_cb()
{
    string fname = _allfilelist.currently_selected();
    if (!fname.empty()) {
	Exmap::FilePtr file = _snapshot->file(fname);
	_all_procs_checkbutton.set_sensitive(true);
	_proclist.set_data(file);
    }
    else {
	_all_procs_checkbutton.set_sensitive(false);
    }
}

void FileTab::proclist_changed_cb()
{
    recalc_sectionlist();
}

void FileTab::recalc_sectionlist()
{
    string fname = _allfilelist.currently_selected();
    pid_t pid = _proclist.currently_selected();
    Exmap::ProcessPtr proc;
    Exmap::FilePtr file;
    bool show_all_procs = _all_procs_checkbutton.get_active();

    if (pid != 0) {
	proc = _snapshot->proc(pid);
    }
    if(!fname.empty()) {
	file = _snapshot->file(fname);
    }

    _sectionlist.set_data(proc, file, show_all_procs);
    Elf::SectionPtr null_section;
    _symlist.set_data(proc, file, null_section);
}

void FileTab::sectionlist_changed_cb()
{
    string fname = _allfilelist.currently_selected();
    pid_t pid = _proclist.currently_selected();
    string section_name = _sectionlist.currently_selected();

    Exmap::ProcessPtr proc;
    Exmap::FilePtr file;
    Elf::SectionPtr section;
    
    if (pid != 0) {
	proc = _snapshot->proc(pid);
    }
    if (!fname.empty()) {
	file = _snapshot->file(fname);
	Elf::FilePtr elf = file->elf();
	if (elf) { 
	    section = elf->section(section_name);
	}
    }

    _symlist.set_data(proc, file, section);
}
    
void FileTab::all_procs_toggled_cb()
{
    bool checkbox_ticked = _all_procs_checkbutton.get_active();
    // Setting sensitive like this should generate the proclist changed signal
    // and hence the Right Thing
    _proclist.set_sensitive(!checkbox_ticked);
    recalc_sectionlist();
}

// ------------------------------------------------------------

int main(int argc, char *argv[])
{
    Gtk::Main kit(argc, argv);

    // If you change the scale you may want to change SIZES_PRINTF_FORMAT
    Sizes::scale_kbytes();
    SysInfoPtr sysinfo(new LinuxSysInfo);
    SnapshotPtr snapshot(new Snapshot(sysinfo));
    TopWin topwin(snapshot);

    topwin.load();

    if (argc == 1) {
	Gtk::Main::run(topwin);
    }

    return 0;
}

