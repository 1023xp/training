#!/usr/bin/perl -w
#
# (c) John Berthels 2005 <jjberthels@gmail.com>. See COPYING for license.
#
use strict;
use Exmap;
use Gtk2;
use Gtk2::SimpleList;
use Glib; # For KeyFile

# There must be a better way to arrange windows?
use constant WIDTH => 800;
use constant HEIGHT => 600;

my $CFG;
main(@ARGV);
exit 0;

=head1 NAME

exmap.pl - a perl/GTK GUI to the Exmap memory analysis functionality

=cut

sub main
{
    my $doquit = shift;

    $CFG = Config->new;
    $CFG->load; # It's OK if this fails - cfg file might not exist.
    
    my $exmap = Exmap->new;
    die("Can't initialise exmap data") unless $exmap;

    my $progress = Progress->new;
    $exmap->load($progress)
	or die("Can't load exmap process information");

    print "Calculating...\n";
    Gtk2->init;
    my $mw = Gtk2::Window->new("toplevel");

    # Why is this necessary?
    $mw->set_default_size(WIDTH, HEIGHT);
    $mw->signal_connect(destroy => sub { Gtk2->main_quit; });

    my $tabwin = Gtk2::Notebook->new;

    my $symlist = ElfSymbolList->new;

    my @tabs;
    my $proctab = ProcTab->new($exmap, $symlist);
    $tabwin->append_page($proctab->window, "Processes");
    push @tabs, $proctab;
    my $filetab = FileTab->new($exmap, $symlist);
    push @tabs, $filetab;
    $tabwin->append_page($filetab->window, "Files");

    $tabwin->signal_connect(switch_page => sub {
				my $nb = shift;
				my $page = shift;
				my $pagenum = shift;

				# Gotta love closures
				my $tab = $tabs[$pagenum];
				$tab->show_tab;
				});

    my $bottombar = make_bottombar($exmap);

    my $hpane = Gtk2::HPaned->new;
    $hpane->pack1($tabwin, 1, 1);
    $hpane->pack2($symlist->window, 1, 1);

    my $vbox = Gtk2::VBox->new;
    $vbox->add($hpane);
    $vbox->pack_end($bottombar, 0, 0, 0);

    $mw->add($vbox);
    $mw->show_all;

	
    print "Running\n";
    Gtk2->main unless $doquit;

    $CFG->check_save;
}

sub make_bottombar
{
    my $exmap = shift;
    my $bottombar = Gtk2::HBox->new;

    my @procs = $exmap->procs;
    my $totals = Exmap::Sizes->new;
    $totals->scale_mbytes;
    foreach my $proc (@procs) {
	my $sizes = $proc->sizes;
	$totals->add($sizes);
    }
    my $text = sprintf ("Number of Procs: %d Number of Files: %d\n",
			scalar @procs,
			scalar($exmap->files));

    $text .= join( "|", map {
	$totals->key_name($_) . " " . $totals->sval($_);
    } $totals->keys);

    $bottombar->pack_start(Gtk2::Label->new($text), 0, 0, 0);

    my $quit_button = Gtk2::Button->new("Quit");
    $quit_button->signal_connect(clicked => sub { Gtk2::main_quit; });
    $bottombar->pack_end($quit_button, 0, 0, 0);

    return $bottombar;
}

# ------------------------------------------------------------

=head2 Progress

Progress indicator. Currently writes to stdout.

=cut

package Progress;
use base qw/Exmap::Progress/;

sub number_of_ticks
{
    my $s = shift;
    $s->{_total_ticks} = shift;
    $s->{_this_tick} = 0;
    print "Number of procs: $s->{_total_ticks}\n";
    return 1;
}

sub tick
{
    my $s = shift;
    my $text = shift;
    my $digits = length $s->{_total_ticks};
    my $tick = sprintf("%0${digits}d", ++$s->{_this_tick});
    print "$tick/$s->{_total_ticks}: Loaded: $text\n";
    return 1;
}

sub finished
{
    my $s = shift;
    print "Finished loading\n";
    return 1;
}


# ------------------------------------------------------------

=head2 View

Abstract base class for all View elements. These are thin perl object
wrappers around Gtk Widgets, accessed via the C<window> method.

The view has these virtual methods:

=over

=item _init_windows

This method should set up the gtk widgets, and set C<window> to the
top-level widget.

=item set_data

This is passed object-specific data, which is intended to be of use
when updating the view. Expensive calculation is to be avoided.

=item update_view

This is called to paint the widgets. It is generally called
immediately after set_data, but may be delayed if a widget isn't in
view.

=back

Any args passed to ->new get passed to C<set_data>.

=cut

package View;

sub new
{
    my $c = shift;
    $c = ref $c if ref $c;
    my $s = {};
    bless $s, $c;
    $s->_init_windows;
    $s->set_data(@_);
    return $s;
}

sub window
{
    my $s = shift;
    my $win = shift;
    if ($win) {
	$s->{_window} = $win;
    }
    return $s->{_window};
}

sub _init_windows { die "_init_windows called in abstract base class"; }
sub update_view { die "_init_windows called in abstract base class"; }
sub set_data { die "set_data called in abstract base class"; }

# ------------------------------------------------------------

=head2 ListView

Abstract base class for all View elements consisting of a list of
items which have 'sizes'. Each row may start with zero or more 'first
columns' and is then followed by the 'sizes' for that row.

An update_view method is provided to display these, and provides the
functionality of selecting which size columns to display for a given
view, depending on the configuration.

The derived class lists its 'first columns' (by overriding the
C<_first_columns> method).


The initial list view sort column is set to the first size column.

It tweaks the underlying list model (ensures all columns sortable,
resizeable) and adds dynamic horizontal and vertical scrollbars.

If a derived class overrides C<_frame_name> with a method which
returns a string, the list object will be wrapped in a frame with that
label.

The top level widget is the scrolledlist, the underlying
Gtk::SimpleList is accessible via the C<list_window> method.


Derived classes must implement a C<set_data> method. This should not be
an expensive call - calculation should be deferred to the update_view
stage.

After C<set_data> has been called either _rows must be set to a list of Row
objects. These will be called from C<update_view> in order to provide
the row first_cols and sizes, allowing the size calculations to be
deferred until that time.

=cut

package ListView;
use base qw/View/;

sub _init_windows
{
    my $s = shift;

    my @cols = $s->_first_columns;
    my $start_sort_col = (scalar @cols) / 2;
    my $sizes = Exmap::Sizes->new;
    $sizes->scale_kbytes;
    push @cols,
	map { $sizes->key_name($_) => 'text' } $CFG->cols_for_listview($s);
	
    my $listwin = Gtk2::SimpleList->new(@cols);
    $s->list_window($listwin);
    $s->_make_all_sortable;

    my $model = $s->list_window->get_model;
    $model->set_sort_column_id($start_sort_col, 'descending');

    $s->_make_all_resizable;
    $s->_set_all_col_sortfunc;

    my $scr_list = Gtk2::ScrolledWindow->new;
    $scr_list->set_policy('automatic', 'automatic');
    $scr_list->add($listwin);

    $s->window($scr_list);

    my $frame_text = $s->_frame_name;
    if ($frame_text) {
	my $frame = Gtk2::Frame->new($frame_text);
	$frame->add($s->window);
	$s->window($frame);
    }
    return 1;
}

sub _frame_name
{
    return undef;
}

sub _first_columns { die "_first_columns called in listview" };

sub list_window
{
    my $s = shift;
    my $win = shift;
    if ($win) {
	$s->{_list_window} = $win;
    }
    return $s->{_list_window};
}

sub _make_all_sortable
{
    my $s = shift;
    return $s->_foreach_column( sub {
        my $s = shift;
	my $colid = shift;
	my $col = shift;
	$s->list_window->get_column($colid)->set_sort_column_id($colid);
    });
}

sub _make_all_resizable
{
    my $s = shift;
    return $s->_foreach_column( sub {
        my $s = shift;
	my $colid = shift;
	my $col = shift;
	$s->list_window->get_column($colid)->set_resizable(1);
    });
}


sub _set_all_col_sortfunc
{
    my $s = shift;

    # Do a numeric sort on all numeric strings, and string sort on others
    my $sort_func = sub {
	my $model = shift;
       my $a = shift;
       my $b = shift;
       my $colid = shift;
       $a = lc $model->get_value($a, $colid);
       $b = lc $model->get_value($b, $colid);
       
       return 0 if (!defined $a) && (!defined $b);
       return +1 if not defined $a;
       return -1 if not defined $b;
       
       # Allow various numeric seperators, to be more locale friendly
       my $number_re = qr/^[\s\d\.,_]+$/;
       if ($a =~ $number_re && $b =~ $number_re) {
           $a <=> $b;
       }
       else {
           $a cmp $b;
       }
   };
    
    return $s->_foreach_column( sub {
				    my $s = shift;
	my $colid = shift;
	my $col = shift;
	$s->list_window->get_model->set_sort_func($colid, $sort_func, $colid);
    });
}


sub _foreach_column
{
    my $s = shift;
    my $subref = shift;
    
    my $win = $s->list_window;
    my @cols = $win->get_columns;
    my $colid = 0;

    foreach my $col (@cols) {
	$subref->($s, $colid, $col);
	++$colid;
    }
    return;
}

sub update_view
{
    my $s = shift;
    my $lw = $s->list_window;


    # Do nothing unless we have an update
    return 1 unless $s->{_rows};

    # Assign data to the Gtk widget in one go, rather than push each
    # row into the tied array.
    my @rows;

    my @cols = $CFG->cols_for_listview($s);
    foreach my $row (@{$s->{_rows}}) {
	my @row = $row->first_cols;
	my $sizes = $row->sizes;
	if ($sizes) {
	    $sizes->scale_kbytes;
	    push @row, $sizes->multi_svals(@cols);
	}
	push @rows, [@row];
    }

    # Calling this appears to call Gtk2::ListStore::set, which has
    # performance problems.

    #    @{$lw->{data}} = @rows;
    @{$lw->{data}} = ();

    my $model = $lw->get_model;
    my $insert_at = 1 + scalar @rows;
    foreach my $row (@rows) {
	my $colnum = 0;
	my @values = map { ($colnum++, $_) } @$row;
	$model->insert_with_values($insert_at, @values);
    }

    # Flag that we have consumed these rows
    $s->{_rows} = undef;

    return 1;
}


# ------------------------------------------------------------

=head2 Row

This is a single listview Row. It can seperately return the initial
columns and sizes, to avoid the expense of calculating the sizes.

=cut

package Row;

sub new
{
    my $c = shift;
    $c = ref $c if ref $c;
    my $s = {};
    $s->{_first_cols} = shift;
    $s->{_size_closure} = shift;
    bless $s, $c;
    return $s;
}

sub first_cols { return @{$_[0]->{_first_cols}}; }

# Invoke the closure to find the sizes
sub sizes
{
    my $s = shift;
    my $closure = $s->{_size_closure};
    return $closure ? $closure->() : undef;
}


# ------------------------------------------------------------

=head2 ProcList

This is a ListView showing a list of processes.

=cut

package ProcList;
use base qw/ListView/;

sub _first_columns
{
    return (PID => 'int',
	    Cmdline => 'text');
}

sub set_data
{
    my $s = shift;
    my @rows = map {
	my $proc = $_;
	Row->new( [ $proc->pid,
		    $proc->cmdline ],
		  sub { return $proc->sizes; } );
    } @_;
    $s->{_rows} = \@rows;
    return 1;
}

# ------------------------------------------------------------

=head2 FileList

This is a ListView showing a list of files.

=cut

package FileList;
use base qw/ListView/;

sub _first_columns
{
    return ('File Name' => 'text',
	    'Num Procs' => 'int');
}

sub set_data
{
    my $s = shift;
    my @rows = map {
	my $file = $_;
	Row->new( [ $file->name,
		    scalar($file->procs)],
		  sub { return $file->sizes; } );
    } @_;
    $s->{_rows} = \@rows;
    return 1;
}

# ------------------------------------------------------------

=head2 FilesPerProcList

This is a ListView showing a list of files within a given process.

=cut

package FilesPerProcList;
use base qw/ListView/;

sub _frame_name
{
    return "Files mapped by process";
}

sub _first_columns
{
    return ('File Name' => 'text');
}


sub set_data
{
    my $s = shift;
    my $proc = shift;

    $s->{_rows} = [];
    if ($proc) {
	my @rows = map {
	    my $file = $_;
	    Row->new([$file->name],
		     sub { $proc->sizes($file) });
	} $proc->files;
	$s->{_rows} = [@rows];
    }
    else {
	$s->{_rows} = [Row->new( ["No process selected"] )];
    }
    return 1;
}

# ------------------------------------------------------------

=head2 ProcsPerFileList

This is a ListView showing a list of processes which map a given file.

=cut

package ProcsPerFileList;
use base qw/ListView/;


sub _frame_name
{
    return "Processes mapping file";
}

sub _first_columns
{
    return (PID => 'int',
	    Cmdline => 'text');
}


sub set_data
{
    my $s = shift;
    my $file = shift;

    $s->{_rows} = [];
    if ($file) {
	my @rows = map {
	    my $proc = $_;
	    Row->new([$proc->pid,
		      $proc->cmdline],
		     sub { $proc->sizes($file) });
	} $file->procs;
	$s->{_rows} = [@rows];
    }
    else {
	$s->{_rows} = [Row->new([0, "No file selected"])];
    }

    return 1;
}


# ------------------------------------------------------------

=head2 ElfSectionList

This is a ListView showing a list of ELF sections within an ELF file.

=cut

package ElfSectionList;
use base qw/ListView/;

sub _frame_name
{
    return "ELF Sections for selected file and process";
}

sub _first_columns
{
    return ('Name' => 'text',
	    'File Offset' => 'text');
}

sub file { return $_[0]->{_file}; }
sub proc { return $_[0]->{_proc}; }

sub _absent_row { return $_[0]->{_absent_text}; }

sub set_data
{
    my $s = shift;
    my $file = shift;
    my $proc = shift;

    # Save these for later querying by the section view
    $s->{_file} = $file;
    $s->{_proc} = $proc;


    $s->{_rows} = [];

    my $absent_text;
    $absent_text = "No file or proc selected"
	unless $file && $proc;
    $absent_text = $file->name ." is not an ELF file"
	if $file && !$file->is_elf;

    if ($absent_text) {
	$s->{_rows} = [Row->new([$absent_text])];
    }
    else {
	# Deferred calculation via closures.
	# First row does the calculation and shifts out the first value.
	# The later rows just shift out the row values.
	my @sections = $file->elf->mappable_sections;
	my @ranges = map { $_->mem_range } @sections;

	# The closures manipulate this array
	my @row_data;

	my $calculator = sub {
	    @row_data = $proc->elf_range_sizes($file, @ranges);
	    shift @row_data;
	};
	my $shifter = sub { shift @row_data; };
	
	my @closures = ($calculator, map { $shifter } @sections);
	pop @closures;
	
	my @rows = map {
	    my $section = $_;
	    Row->new([$section->name,
		      $section->is_nobits ? "absent" : $section->offset],
		     shift @closures);
	} @sections;
	$s->{_rows} = [@rows];
    }

    return 1;
}


# ------------------------------------------------------------

=head2 ElfSymbolList

This is a ListView showing a list of ELF symbols within an ELF section.

=cut

package ElfSymbolList;
use base qw/ListView/;
use constant PAGE_SIZE => 4096;

sub _frame_name
{
    return "ELF Symbols in selected section";
}

sub _first_columns
{
    return ('Symbol Name' => 'text');
}


sub set_data
{
    my $s = shift;
    my $file = shift;
    my $proc = shift;
    my $section_name = shift;

    $s->{_rows} = [];

    my $absent_text;

    # Last test will take precedence
    $absent_text = "No ELF section selected" unless $section_name;
    $absent_text = $file->name ." is not an ELF file"
	if $file && !$file->is_elf;
    $absent_text = "No file and proc selected" unless $file && $proc;

    my $section;
    my @symbols;
    unless ($absent_text) {
	$section = $file->elf->section_by_name($section_name);
	@symbols = $file->elf->symbols_in_section($section);
	
	$absent_text = "No symbols in $section_name in " . $file->name
	    unless @symbols;
    }

    if($absent_text) {
	$s->{_rows} = [Row->new([$absent_text])];
    }
    else {
	# Deferred calculation via closures. See above.
	my @ranges = map { $_->range; } @symbols;
	my @row_data;
	
	my $calculator = sub {
	    @row_data = $proc->elf_range_sizes($file, @ranges);
	    shift @row_data;
	};
	my $shifter = sub { shift @row_data; };
	
	my @closures = ($calculator, map { $shifter } @symbols);
	pop @closures;
	
	my @rows = map {
	    my $sym = $_;
	    Row->new([$sym->name],
		     shift @closures);
	} @symbols;
	$s->{_rows} = [@rows];
    }

    return 1;
}


sub register_section_list
{
    my $s = shift;
    my $sectionlist = shift;

    my $lw = $sectionlist->list_window;

    $lw->get_selection
	->signal_connect(changed => sub {
			     my $selection = shift;
			     my $model = $lw->get_model;
			     my $iter = $selection->get_selected;
			     return undef unless $iter;
			     my $section_name = $model->get_value($iter, 0);
			     $s->set_data($sectionlist->file,
					  $sectionlist->proc,
					  $section_name);
			     $s->update_view;
			 });
}



# ------------------------------------------------------------

=head2 Config

Wrapper around the Gtk2::KeyFile to provide a config file.

This is loaded from ~/.exmaprc if present and non-empty.

It is saved back to .exmaprc if it exists. Hence a way to get a config
file is to create an empty ~/.exmaprc and run exmap. On exit, it will
save back the default config.

=cut

package Config;

use constant GROUP_LIST_COLUMNS => "List Columns";
use constant GROUP_COLUMN_DEFS => "Column Definitions";
use constant BUILTIN => "built-in";

sub new
{
    my $c = shift;
    $c = ref $c if ref $c;
    my $s = bless {}, $c;
    $s->{_keyfile} = Glib::KeyFile->new;
    unless ($s->{_keyfile}) {
	warn("Can't create keyfile");
	return undef;
    }
    $s->{_keyfile}->set_list_separator(ord ",");
    $s->{_file} = $ENV{HOME} . "/.exmaprc";
    $s->{_save_file} = 0; # Only save a file if we loaded one.
    return $s;
}

sub load
{
    my $s = shift;
    my $flags = ['keep-comments', 'keep-translations'];

    my $worked;
    if (-f $s->{_file} && !-z $s->{_file}) {
	# Wrap in eval as load_from_file likes to call croak :-(
	eval {
	    $worked = $s->{_keyfile}->load_from_file($s->{_file}, $flags);
	};
	if (!$worked || $@) {
	    $@ ||= "";
	    print STDERR "Failed to load cfg file $s->{_file} : $@";
	}
    }
    else {
	$worked = 1;
	$s->_set_defaults;
    }
    $s->{_save_file} = -f $s->{_file};

    return $worked;
}

sub check_save
{
    my $s = shift;
    if ($s->{_save_file} && $s->{_keyfile} && $s->{_file}) {
	open(S, "> $s->{_file}")
	    or die("Can't open file $s->{_file} to save cfg : $!");
	print S $s->{_keyfile}->to_data;
	close S;
    }
    return 1;
}

sub _set_defaults
{
    my $s = shift;
    my @cols = Exmap::Sizes::keys;
    my @listviews = qw/ProcList
		       FileList
		       FilesPerProcList
		       ProcsPerFileList
		       ElfSectionList
		       ElfSymbolList/;
    my $kf = $s->{_keyfile};

    map { $kf->set_value(GROUP_COLUMN_DEFS, $_, BUILTIN) } @cols;

    map { $kf->set_value(GROUP_LIST_COLUMNS, $_, ""); } @listviews;

    return 1;
}

sub cols_for_listview
{
    my $s = shift;
    my $lv_name = shift;
    $lv_name =~ s/^:://;
    $lv_name =~ s/=.*$//;
    my @cols = $s->{_keyfile}->get_string_list(GROUP_LIST_COLUMNS,
					       $lv_name);
    @cols = $s->_trim_spaces(@cols);
    my @defined_cols = $s->defined_cols;

    my @good_cols;

    if (@cols) {
	foreach my $col (@cols) {
	    if (grep { $_ eq $col } @defined_cols) {
		push @good_cols, $col;
	    }
	    else {
		warn("Unrecognised column name $col for view $lv_name");
	    }
	}
    }
    else {
	@good_cols = @defined_cols
    }

    return @good_cols;
}

sub defined_cols
{
    my $s = shift;
    return $s->_trim_spaces($s->{_keyfile}->get_keys(GROUP_COLUMN_DEFS));
}

sub _trim_spaces
{
    my $s = shift;
    map { s/^\s+//; s/\s+$//; $_ } @_;
}


# ------------------------------------------------------------

=head2 ExmapTabPage

Abstract base class for the file/proc tabs. This contains the common
mainlist/sublist/elfsectionlist functionality.

=cut

package ExmapTabPage;
use base qw/View/;

sub _init_windows
{
    my $s = shift;
    $s->{_exmap} = shift;
    $s->{_symlist} = shift;

    my $vbox = Gtk2::VBox->new;
    $s->{_sectionlist} = ElfSectionList->new;

    $vbox->add($s->{_mainlist}->window);
    $vbox->add($s->{_sublist}->window);
    $vbox->add($s->{_sectionlist}->window);

    $s->window($vbox);

    my $mainlistwin = $s->{_mainlist}->list_window;
    my $sublistwin = $s->{_sublist}->list_window;

    # Update the sublist data when the mainwin selection changes
    $mainlistwin->get_selection
	->signal_connect(changed => sub {
			   my $selection = shift;
			   my $model = $mainlistwin->get_model;
			   my $iter = $selection->get_selected;
			   return undef unless $iter;
			   my $item = $model->get_value($iter, 0);
			   my $data = $s->_mainlist_item_to_data($item);
			   $s->{_sublist}->set_data($data);
			   $s->{_sublist}->update_view;
			   # Also clear section list
			   $s->{_sectionlist}->set_data();
			   $s->{_sectionlist}->update_view;
		       });

    # Update the section list when the sublist selection changes
    $sublistwin->get_selection
	->signal_connect(changed => sub {
			     my $selection = shift;
			     my $model = $sublistwin->get_model;
			     my $subiter = $selection->get_selected;
			     return undef unless $subiter;
			     my $subitem = $model->get_value($subiter, 0);
			     my $subdata = $s->_sublist_item_to_data($subitem);
			     my $mainiter = $mainlistwin->get_selection->get_selected;
			     my $mainitem = $mainlistwin->get_model->get_value($mainiter, 0);
			     my $maindata = $s->_mainlist_item_to_data($mainitem);
			     if ($s->_main_first_for_sectionlist) {
				 $s->{_sectionlist}->set_data($maindata,
							      $subdata);
			     }
			     else {
				 $s->{_sectionlist}->set_data($subdata,
							      $maindata);
			     }
			     $s->{_sectionlist}->update_view;
			 });
    
    return 1;
}

sub _pid_to_proc
{
    my $s = shift;
    my $pid = shift;
    return $s->{_exmap}->pid_to_proc($pid);
}

sub _fname_to_file
{
    my $s = shift;
    my $fname = shift;
    my $file = $s->{_exmap}->file_pool->name_to_file($fname);
    return $file;
}

sub show_tab
{
    my $s = shift;
    $s->{_mainlist}->update_view;
    $s->{_sublist}->update_view;
    $s->{_sectionlist}->update_view;
}

# ------------------------------------------------------------

=head2 ProcTab

Class to wrap the 'Processes' tab in the view. The mainlist is a
ProcList, the sublist is a 'FilesPerProcList'.

=cut

package ProcTab;
use base qw/ExmapTabPage/;

sub _init_windows
{
    my $s = shift;

    $s->{_mainlist} = ProcList->new;
    $s->{_sublist} = FilesPerProcList->new;

    return $s->SUPER::_init_windows;
}

sub _mainlist_item_to_data
{
    my $s = shift;
    return $s->_pid_to_proc(@_);
}

sub _sublist_item_to_data
{
    my $s = shift;
    return $s->_fname_to_file(@_);
}

sub set_data
{
    my $s = shift;
    $s->{_exmap} = shift;
    my $symlist = shift;

    $s->{_mainlist}->set_data($s->{_exmap}->procs);
    $symlist->register_section_list($s->{_sectionlist});

    return 1;
}

sub _main_first_for_sectionlist { return 0; }

# ------------------------------------------------------------

=head2 FileTab

Class to wrap the 'Files' tab in the view. The mainlist is a
FileList, the sublist is a 'ProcsPerFileList'.

=cut

package FileTab;
use base qw/ExmapTabPage/;

sub _init_windows
{
    my $s = shift;
    $s->{_mainlist} = FileList->new;
    $s->{_sublist} = ProcsPerFileList->new;
    return $s->SUPER::_init_windows;
}


sub _mainlist_item_to_data
{
    my $s = shift;
    return $s->_fname_to_file(@_);
}

sub _sublist_item_to_data
{
    my $s = shift;
    return $s->_pid_to_proc(@_);
}

sub set_data
{
    my $s = shift;
    $s->{_exmap} = shift;
    my $symlist = shift;

    $s->{_mainlist}->set_data($s->{_exmap}->files);
    $symlist->register_section_list($s->{_sectionlist});

    return 1;
}

sub _main_first_for_sectionlist { return 1; }


