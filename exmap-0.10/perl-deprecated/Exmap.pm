#
# (c) John Berthels 2005 <jjberthels@gmail.com>. See COPYING for license.
#
use Elf;
use strict;
use warnings;

my $DEBUG_ON = $ENV{EXMAP_DEBUG};

sub debug
{
    print STDERR join(":", @_), "\n" if $DEBUG_ON;
}

# ------------------------------------------------------------
package Exmap::Obj;

my $OBJ_LIFETIME_DEBUG = 0;

sub _init { return shift; }

sub new
{
    my $c = shift;
    $c = ref $c if ref $c;
    my $s = {};
    bless $s, $c;
    print "C: $s\n" if $OBJ_LIFETIME_DEBUG;
    return $s->_init(@_);
}

sub DESTROY
{
    my $s = shift;
    print "D: $s\n" if $OBJ_LIFETIME_DEBUG;
}

    
# ------------------------------------------------------------
# Map page cookie -> usage count
package Exmap::PagePool;
use base qw/Exmap::Obj/;

# This used to be an object with accessors to add pages and read
# counts.  But they showed up as very hot in profiling, and accessing
# via a straight hash is still fairly clean.

sub clear
{
    my $s = shift;
    %$s = ();
    #    print "C:\n";
}

# ------------------------------------------------------------
package Exmap::FilePool;
use base qw/Exmap::Obj/;

sub clear
{
    my $s = shift;
    %$s = ();
}

sub name_to_file
{
    my $s = shift;
    my $fname = shift;
    return $s->{$fname};
}

sub get_or_make_file
{
    my $s = shift;
    my $fname = shift;

    my $file = $s->name_to_file($fname);
    return $file if $file;
    $s->{$fname} = Exmap::File->new($fname);
    return $s->{$fname};
}

sub files
{
    my $s = shift;
    return values %$s;
}

# ------------------------------------------------------------
package Exmap;
use base qw/Exmap::Obj/;

sub _init
{
    my $s = shift;
    $s->{_procs} = [];
    $s->{_pid_to_proc} = {};
    $s->{_page_pool} = Exmap::PagePool->new;
    $s->{_file_pool} = Exmap::FilePool->new;
    return $s;
}

sub procs { return @{$_[0]->{_procs}}; }
sub page_pool { return $_[0]->{_page_pool}; }
sub file_pool { return $_[0]->{_file_pool}; }
sub files { return $_[0]->file_pool->files; }

sub pids
{
    my $s = shift;
    return keys %{$s->{_pid_to_proc}};
}

sub pid_to_proc
{
    my $s = shift;
    my $pid = shift;
    return $s->{_pid_to_proc}->{$pid};
}

sub num_procs
{
    my $s = shift;
    return scalar($s->procs);
}

sub _all_pids
{
    my $s = shift;
    my @pids = map { s!^/proc/!!; $_; } glob "/proc/[0-9]*";
    return sort { $a <=> $b } @pids;
}

sub load
{
    my $s = shift;
    my $progress = shift;
    my $test_info = shift;

    $s->_load_procs($test_info)
	or return undef;

    $progress->number_of_ticks(scalar $s->procs)
	if $progress;

    $s->_calculate_file_mappings($progress)
	or return undef;

    $progress->finished if $progress;
    
    return 1;
}

sub _load_procs
{
    my $s = shift;
    my $test_info = shift;
    
    my $pp = $s->page_pool;
    $pp->clear;
    
    # Don't monitor ourselves, our VMAs etc will change too much as we run
    my @pids = grep { $_ != $$ } $s->_all_pids();
    @pids = @{$test_info->{pids}} if $test_info;

    my @procs;
    foreach my $pid (@pids) {

	my $proc_prefix = "/proc";
	$proc_prefix = $test_info->{proc} if $test_info;
	my $exmap_data = $test_info->{exmap_files}->{$pid};
	
	my $proc = Exmap::Process->new($pid, $proc_prefix);
	unless ($proc->load($pp, $exmap_data)) {
	    warn("Can't load info for pid $pid");
	    next;
	}
	push @procs, $proc if $proc->has_mm;
    }
    
    $s->{_procs} = \@procs;
    $s->{_pid_to_proc} = { map { $_->pid => $_ } @procs };
    
    return scalar @procs;
}

sub _calculate_file_mappings
{
    my $s = shift;
    my $progress = shift;
    
    foreach my $proc ($s->procs) {
	warn("Failed to process maps for pid ", $proc->pid)
	    unless $proc->_calc_vma_maps($s->file_pool);
	$progress->tick($proc->pid . ": " . $proc->cmdline)
	    if $progress;
    }
    return scalar $s->files;
}


# ------------------------------------------------------------
# Abstract base class for callers of Exmap::load to get progress updates
package Exmap::Progress;
use base qw/Exmap::Obj/;

# Called when the load initialises. You can override this if you wish.
sub number_of_ticks
{
    my $s = shift;
    $s->{_num_ticks} = shift;
    return 1;
}

# Called whenever we tick. You'll want to override this.
sub tick
{
    my $s = shift;
    my $text = shift;
    return 1;
}

# Called after the last tick. You'll probably want to override this.
sub finished
{
    my $s = shift;
    my $text = shift;
    return 1;
}

# ------------------------------------------------------------
package Exmap::Process;
use base qw/Exmap::Obj/;

use constant EXMAP_FILE => "/proc/exmap";

sub _init
{
    my $s = shift;
    $s->{_pid} = shift;
    $s->{_proc_prefix} = shift || "/proc";
    $s->{_exe_name} = readlink "$s->{_proc_prefix}/$s->{_pid}/exe";
    my @cmdline = split /[ \0]/, `cat $s->{_proc_prefix}/$s->{_pid}/cmdline`;
    if (@cmdline > 1) {
	# Hack so we can see [kdeinit]
	if ($cmdline[1] =~ /^\[/ ) {
	    @cmdline = @cmdline[0..1];
	}
	else {
	    @cmdline = $cmdline[0];
	}
    }
    $s->{_cmdline} = join(" ", @cmdline);
    $s->{_files} = {};
    return $s;
}

sub load
{
    my $s = shift;
    my $page_pool = shift;
    my $test_exmap_file = shift; # Or undef for the real exmap

    unless ($s->_load_vmas($page_pool)) {
	warn "Can't load vmas for " . $s->pid;
	return undef;
    }
    return 1 unless $s->has_mm;
    unless ($s->_load_page_info($test_exmap_file)) {
	warn "Can't load page info for " . $s->pid;
	return undef;
    }
    return 1;
}

sub pid { return $_[0]->{_pid}; }
sub exe_name { return $_[0]->{_exe_name}; }
sub cmdline { return $_[0]->{_cmdline}; }
sub _vmas { return @{$_[0]->{_vmas}}; }
sub has_mm
{
    my $s = shift;
    return exists $s->{_vmas} && scalar $s->_vmas > 0;
}
sub maps { return @{$_[0]->{_maps}}; }
sub files { return values %{$_[0]->{_files}}; }

sub _find_vma_by_addr
{
    my $s = shift;
    my $addr = shift;

    return $s->{_start_to_vma}->{$addr};
}

sub add_file
{
    my $s = shift;
    my $file = shift;

    # Store as a hash for easy uniqueness. Hash keys are stringified,
    # so we need to store the obj reference as a value
    $s->{_files}->{$file} = $file;
}

sub _has_file
{
    my $s = shift;
    my $file = shift;

    my @matches = grep { $_ eq $file } $s->files;
    warn("File ", $file->name, " present in process ", $s->pid,
	 " more than once")
	if (scalar @matches > 1);
    return scalar @matches == 1;
}

sub _restrict_maps_to_file
{
    my $s = shift;
    my $file = shift;
    my @maps = @_;

    unless ($file) {
	warn("No file to specified");
	return ();
    }
    unless ($s->_has_file($file)) {
	warn("PID ", $s->pid, " doesn't have file ", $file->name);
	return ();
    }

    my %count;
    my @file_maps = $file->maps;
    foreach my $map (@maps, @file_maps) {
	$count{$map}++;
    }
    # Only keep those in both arrays.
    @maps = grep { $count{$_} > 1 } @maps;

    return @maps;
}

sub _refine_maps_to_elf_range
{
    my $s = shift;
    my $elf_range = shift;
    my @maps = @_;

    return () unless $elf_range->size > 0;

    my @refinements;
    
    foreach my $map (@maps) {
	if ($map->elf_range
	    && $map->elf_range->overlaps($elf_range)) {
	    my $subrange = $elf_range->intersect($map->elf_range);
	    my $mem_range = $map->elf_to_mem_range($subrange);
	    push @refinements, { map => $map,
				 range => $mem_range };
	}
    }

    unless (@refinements) {
	my $warnstr = $s->pid . ": no map refinements for elf range "
	    . $elf_range->to_string. ": "
		. join(", ", map { $_->elf_range
				       ? $_->elf_range->to_string
				   : "undef" } @maps);
	warn($warnstr);
    }
    
    return @refinements;
}


# This takes on optional 'file' parameter, which may also be followed
# by an optional 'elf_range' parameter. These are both used to restrict the
# maps to be summed over.
sub sizes
{
    my $s = shift;
    my $file = shift;

    my @maps = $s->maps;
    warn ("No maps in process", $s->pid) unless @maps;

    if ($file) {
	@maps = $s->_restrict_maps_to_file($file, @maps);
	warn ("No maps for file " . $file->name . " in process ", $s->pid)
	    unless @maps;
    }

    my $sizes = Exmap::Sizes->new;
    foreach my $m (@maps) {
	my $subsizes = $m->sizes_for_mem_range;
	$sizes->add($subsizes);
    }

    return $sizes;
}

sub elf_range_sizes
{
    my $s = shift;
    my $file = shift;
    my @elf_ranges = @_;

    my @maps = $s->maps;
    warn ("No maps in process", $s->pid) unless @maps;
    @maps = $s->_restrict_maps_to_file($file, @maps);
    warn ("No maps for file " . $file->name . " in process ", $s->pid)
	unless @maps;

    my @sizes;
    foreach my $elf_range (@elf_ranges) {
	# A list of { map => $map, range => $mem_range }. Undef range
	# implies full map.
	my @refinements = $s->_refine_maps_to_elf_range($elf_range, @maps);
	my $sizes = Exmap::Sizes->new;
	foreach my $r (@refinements) {
	    my $subsizes = $r->{map}->sizes_for_mem_range($r->{range});
	    $sizes->add($subsizes);
	}
	push @sizes, $sizes;
    }

    return @sizes;
}

sub _load_vmas
{
    my $s = shift;
    my $page_pool = shift;

    my $mapfile = "$s->{_proc_prefix}/" . $s->pid . "/maps";
    
    unless (open (M, "< $mapfile")) {
	warn("Can't open mapfile $mapfile: $!");
	return undef;
    }
    my @map_lines = <M>;
    close M;

    # Kernel threads have no maps. Thats OK.
    return 1 if (@map_lines == 0);

    my @vmas;
    foreach my $line (@map_lines) {
	$line =~ s/\r?\n$//;
	my $vma = Exmap::Vma->new($page_pool);
	unless ($vma->parse_line($line)) {
	    warn("Can't create VMA for line $line");
	    next;
	}
	# Don't add the [vdso] map, it doesn't exist as a vma
	# in the kernel.
	push @vmas, $vma unless $vma->is_vdso;
    }
    # Store as hash for fast addr lookup
    $s->{_start_to_vma} = {
		   map { $_->{info}->{start}, $_ } @vmas
		  };
    # Keep the ordered list
    $s->{_vmas} = \@vmas;
    
    return $s;
}

sub _load_page_info
{
    my $s = shift;
    my $test_exmap_file = shift;

    my $exmap_file = $test_exmap_file ? $test_exmap_file : EXMAP_FILE;
    # Ask exmap about our pid
    if ($test_exmap_file) {
	unless(open (E, "< $exmap_file")) {
	    warn("can't open test exmap file $test_exmap_file");
	    return undef;
	}
    }
    else {
	unless (open(E, "+> $exmap_file")) {
	    warn("can't open $exmap_file for writing : $!");
	    return undef;
	}
	print E $s->pid, "\n";
    }

    my $current_vma;
    my $page_cookie;
    my ($pfn, $swap_entry, $line);
    while ($line = <E>) {
	# Lines are either:
	# Start a new VMA:
	# VMA 0xdeadbeef <npages>
	# or
	# Page info
	# <pfn> <swap_entry>

	if ($line =~ /^VMA/) {
	    # New VMA
	    my ($vma_hex_start, $npages) = $line =~ /^VMA\s+0x(\S+)\s+(\d+)$/;
	    my $vma_start = hex $vma_hex_start;
	    my $vma = $s->_find_vma_by_addr($vma_start);
	    unless ($vma) {
		# TODO - try reload completely here?
		warn("PID ", $s->pid, " can't find VMA $vma_hex_start");
		return undef;
	    }
	    $current_vma = $vma;
	}
	else {
	    $page_cookie = Exmap::Page::line_to_cookie($line);
	    $current_vma->add_page($page_cookie);
	}
    }
    close E;

    return 1;
}


sub _calc_vma_maps
{
    my $s = shift;
    my $filepool = shift;

    my @maps; # Accumulate all the per-proc maps in here
    my @vmas = $s->_vmas;

    my $previous_vma;
    my $previous_file;
    foreach my $vma (@vmas) {
	my $file = $filepool->get_or_make_file($vma->{info}->{file});
	$file->add_proc($s);
	my @vma_maps = $vma->calc_maps($file,
				       $previous_vma,
				       $previous_file,
				       $s->pid);

	warn sprintf("%d: Can't calc maps for vma 0x%08x : %s",
		     $s->pid, $vma->{info}->{start}, $file->name)
	    unless @vma_maps;
	
	push @maps, @vma_maps;
	$s->add_file($file);
	$previous_vma = $vma;
	$previous_file = $file;
    }

    my @ranges = map { $_->mem_range } @maps;
    my $last_range;
    foreach my $range (@ranges) {
	if ($last_range) {
	    if ($range->overlaps($last_range)) {
		warn sprintf("%d: Invalid map list %s, %s",
			     $s->pid,
			     $last_range->to_string,
			     $range->to_string);
		return undef;
	    }
	}
	$last_range = $range;
    }

    $s->{_maps} = \@maps;

    return scalar @maps;
}


# ------------------------------------------------------------
package Exmap::File;
use base qw/Exmap::Obj/;

sub _init
{
    my $s = shift;
    $s->{_name} = shift;
    $s->{_maps} = [];
    $s->{_procs} = [];
    if (-f $s->name) {
	$s->{_elf} = Elf->new($s->name,
			      1); # Suppress warning if not elf
    }
    return $s;
}

sub name { return $_[0]->{_name}; }
sub procs { return @{$_[0]->{_procs}}; }
sub elf { return $_[0]->{_elf}; }
sub is_elf { return $_[0]->elf; } # Mmmm. Sugary.
sub maps { return @{$_[0]->{_maps}}; }

sub sizes
{
    my $s = shift;

    my $sizes = Exmap::Sizes->new;

    foreach my $map ($s->maps) {
	my $subsizes = $map->sizes_for_mem_range;
	$sizes->add($subsizes);
    }
    return $sizes;
}

sub add_map
{
    my $s = shift;
    my $map = shift;
    
    push @{$s->{_maps}}, $map;
    return 1;
}

sub add_proc
{
    my $s = shift;
    my $proc = shift;

    my %existing = map { $_ => 1 } $s->procs;
    push @{$s->{_procs}}, $proc unless $existing{$proc};
    
    return 1;
}


# ------------------------------------------------------------
package Exmap::Vma;
use base qw/Exmap::Obj/;

use constant ANON_NAME => "[anon]";
use constant VDSO_NAME => "[vdso]";

sub _init
{
    my $s = shift;
    $s->{_page_pool} = shift;
    $s->{_pages} = [];
    return $s;
}

sub page_pool { return $_[0]->{_page_pool}; }

sub parse_line
{
    my $s = shift;
    my $line = shift;
    
    my %info;

    @info{qw(hex_start hex_end perms hex_offset)}
	= ($line =~ /^([0-9a-f]+)-([0-9a-f]+)\s+(\S+)\s+([0-9a-f]+)/);

    if (not defined $info{hex_start}) {
	warn("Can't parse line [$line]");
	return undef;
    }
    $info{file} = substr($line, 49) if length $line >= 49;
    
    $info{file} ||= ANON_NAME;

    $s->{info} = \%info;

    $s->_calculate_info or return undef;

    return 1;
}

sub _calculate_info
{
    my $s = shift;

    my $info = $s->{info};

    # On 64-bit platforms, addresses are > 32-bit (duh) but the 'int'
    # type is still 32-bit. So perl warns here.
    { # New scope for local warning var
	local $^W = 0;
	$info->{start} = hex $info->{hex_start};
	$info->{offset} = hex $info->{hex_offset};
	$info->{end} = hex $info->{hex_end};
    }
    $info->{vm_size} = $info->{end} - $info->{start};

    return 1
}

sub add_page
{
    my $s = shift;
    my $page_cookie = shift;

    # Record all pages, in order
    push @{$s->{_pages}}, $page_cookie;

    $s->{_page_pool}->{$page_cookie}++;
}

sub is_vdso { return $_[0]->{info}->{file} eq VDSO_NAME; }

sub is_file_backed
{
    my $s = shift;
    # Names like [anon], [heap], and [vdso] don't count as file backed
    return  !($s->{info}->{file} =~ /^\[.*\]$/);
}

sub _addr_to_pgnum
{
    my $s = shift;
    my $addr = shift;

    if ($addr >= $s->{info}->{end}) {
	warn("$addr is beyond vma end " . $s->{info}->{end});
	return undef;
    }
    
    my $pgnum = Elf::page_align_down($addr);
    $pgnum -= $s->{info}->{start};
    if ($pgnum < 0) {
	warn("$addr is less than vma start " . $s->{info}->{start});
	return undef;
    }
#    $pgnum /= Elf::PAGE_SIZE;
    $pgnum >>= Elf::PAGE_SIZE_SHIFT;

    return $pgnum;
}

sub get_pages_for_range
{
    my $s = shift;
    my $range = shift;

    return undef unless $range->size > 0;

    my $vma_fname = $s->{info}->{file};
    
    my $start_pgnum = $s->_addr_to_pgnum($range->start);
    return undef unless defined $start_pgnum;
    my $end_pgnum = $s->_addr_to_pgnum($range->end - 1);
    return undef unless defined $end_pgnum;

    if ($start_pgnum == $end_pgnum) {
	my $page_cookie = $s->{_pages}->[$start_pgnum];
	unless (defined $page_cookie) {
	    warn("Can't find page for pgnum $start_pgnum in $vma_fname");
	    return undef;
	}
	return [{
		 page => $page_cookie,
		 bytes => $range->size,
		}];
    }


    my @info;
    my $page_cookie;
    
    $page_cookie = $s->{_pages}->[$start_pgnum];
    unless (defined $page_cookie) {
	warn("Can't find page for pgnum $start_pgnum in $vma_fname");
	return undef;
    }
    push @info, { page => $page_cookie,
		  bytes => Elf::PAGE_SIZE
		  - ($range->start - Elf::page_align_down($range->start))};
    
    $page_cookie = $s->{_pages}->[$end_pgnum];
    unless (defined $page_cookie) {
	warn("Can't find page for pgnum $end_pgnum in $vma_fname");
	return undef;
    }
    push @info, { page => $page_cookie,
		  bytes =>
		  $range->end - Elf::page_align_down($range->end - 1) };

    for (my $pgnum = $start_pgnum+1; $pgnum <= $end_pgnum-1; ++$pgnum) {
	$page_cookie = $s->{_pages}->[$pgnum];
	unless (defined $page_cookie) {
	    warn("Can't find page for pgnum $pgnum in $vma_fname");
	    return undef;
	}
	push @info, { page => $page_cookie,
		      bytes => Elf::PAGE_SIZE };
    }

    return \@info;
}

sub range
{
    my $s = shift;
    # TODO - do we call this often enough to cache this value?
    return Range->new($s->{info}->{start}, $s->{info}->{end});
}

# Come up with a list of elf maps which exactly cover this vma.  We
# can refer back to the previous vma & file to clarify the various
# cases.
#
# Cases: (1) This is an elf backed vma.  Then for each loadable
# segment, we calculate the elf address->vma address offset and work
# out which how much of the segment overlaps our address space. We
# turn those into maps and any holes into anon maps.
#
# (2a) This is not a file backed vma
# AND previous is file backed
# AND previous file is elf
# AND this vma is contiguous with the previous
# In which case we take the previous vma's segments and offsets
# and see if we have an overlap. We assert that the overlap will
# be at the beginning of this vma.
# Any remainder gets added turned into an anon map
#
# (2b) This is not a file backed vma and some or all of the
# previous conditions do not hold. In which case we turn the entire vma
# into an anon map.
#
# We roll 2a and 2b together, by doing (2b) from a calculated
# start address.
sub calc_maps
{
    my $s = shift;
    my $file = shift;
    my $previous_vma = shift;
    my $previous_file = shift;
    my $pid = shift; # For debugging

    my @maps;
    my @segs;

    ::debug(sprintf "%d: calc_maps", $pid);
    if ($file->is_elf) {
	# Case 1
	@segs = $file->elf->loadable_segments if $file->is_elf;

	foreach my $seg (@segs) {
	    my $seg_to_mem = $s->_get_seg_to_mem($seg);
	    my $seg_mem_range = $seg->mem_range->add($seg_to_mem);
	    my $subrange = $seg_mem_range->intersect($s->range);
	    next unless $subrange && $subrange->size > 0;
	    my $elf_subrange = $subrange->subtract($seg_to_mem);
	    my $map = Exmap::Map->new($s, $subrange, $elf_subrange);

	    ::debug(sprintf "%d: added elf map %s", $pid, $map->to_string);
	    push @maps, $map;
	    $file->add_map($map);
	}

	# Add holes
	my @mem_ranges = map { $_->mem_range } @maps;
	my $hole_ranges = $s->range->invert_list(@mem_ranges);
	my @hole_maps = map {
	    my $hmap = Exmap::Map->new($s, $_);
	    ::debug(sprintf "%d: added hole map %s", $pid, $hmap->to_string);
	    $file->add_map($hmap);
	    $hmap;
	} @$hole_ranges if $hole_ranges;
	
	push @maps, @hole_maps;
    }
    else {
	# Cases 2a and 2b
	my $non_elf_map;
	if ($previous_vma
	    && $previous_file
	    && $previous_vma->is_file_backed
	    && $previous_file->is_elf
	    && $s->range->start == $previous_vma->range->end) {

	    # Case 2a - the elf segment(s) could be continuing
	    # from the previous vma

	    my @prev_segs = $previous_file->elf->loadable_segments;
	    my $non_elf_start = $s->{info}->{start};
	    
	    # TODO - this is cut-and-paste from previous (with some
	    # differences). Make this a vma method.
	    foreach my $seg (@prev_segs) {

		my $seg_to_mem = $previous_vma->_get_seg_to_mem($seg);
		my $seg_mem_range = $seg->mem_range->add($seg_to_mem);
		my $subrange = $seg_mem_range->intersect($s->range);
		next unless $subrange && $subrange->size > 0;

		my $elf_subrange = $subrange->subtract($seg_to_mem);
		my $map = Exmap::Map->new($s, $subrange, $elf_subrange);
		$non_elf_start = $subrange->end;
		
		::debug(sprintf "%d: added elf cont map %s",
			$pid, $map->to_string);
		push @maps, $map;
		$previous_file->add_map($map);
	    }
	    
	    if ($non_elf_start < $s->{info}->{end}) {
		$non_elf_map = Exmap::Map->new($s,
					       Range->new($non_elf_start,
							  $s->{info}->{end},
							  undef));
		::debug(sprintf "%d: added non-elf map %s",
			$pid, $non_elf_map->to_string);
	    }
	}
	else {
	    # We just cover this vma with a non-elf range
	    $non_elf_map = Exmap::Map->new($s, $s->range, undef);
	    ::debug(sprintf "%d: added non-elf map %s",
		    $pid, $non_elf_map->to_string);
	}
	if ($non_elf_map) {
	    push @maps, $non_elf_map;
	    $file->add_map($non_elf_map);
	}
    }

    # Ensure they're in the right order.
    @maps = sort { $a->mem_range->start <=> $b->mem_range->start } @maps;

    # Sanity-check the maps
    my @ranges = map { $_->mem_range } @maps;
    if ($ranges[0]->start != $s->range->start) {
	warn sprintf("%d: first map not at start 0x%08x: %s",
	    $pid, $s->range->start, $ranges[0]->to_string);
	return ();
    }
    if ($ranges[-1]->end != $s->range->end) {
	warn sprintf("%d: last map not at end 0x%08x: %s",
	    $pid, $s->range->end, $ranges[-1]->to_string);
	return ();
    }

    my $last_range;
    foreach my $range (@ranges) {
	unless ($range->size > 0) {
	    warn sprintf("%d: zero length map %s", $pid, $range->to_string);
	    return ();
	}
	if ($last_range) {
	    unless ($last_range->end == $range->start) {
		warn sprintf("%d: Invalid map list %s, %s",
			     $pid,
			     $last_range->to_string,
			     $range->to_string);
		return ();
	    }
	}
	$last_range = $range;
    }

    return @maps;
}


sub _get_seg_to_mem
{
    my $s = shift;
    my $seg = shift;

    # Offset isn't valid unless we have a backing file
    return undef unless $s->is_file_backed;
    
    my $segmem_base = $seg->mem_range->start - $seg->offset;
    my $vmamem_base = $s->{info}->{start} - $s->{info}->{offset};
    return $vmamem_base - $segmem_base;
}


# ------------------------------------------------------------
package Exmap::Map;
use base qw/Exmap::Obj/;

sub _init
{
    my $s = shift;
    $s->{_vma} = shift;
    $s->{_mem_range} = shift;
    $s->{_elf_range} = shift;	# Optional - undef if not file backed

    if ($s->elf_range && $s->mem_range->size != $s->elf_range->size) {
	warn("Mem range != Elf mem range");
	return undef;
    }
    return $s;
}

sub mem_range { return $_[0]->{_mem_range}; }
sub elf_range { return $_[0]->{_elf_range}; }
sub _vma { return $_[0]->{_vma}; }

sub elf_to_mem_offset
{
    my $s = shift;
    return $s->mem_range->start - $s->elf_range->start;
}

sub elf_to_mem_range
{
    my $s = shift;
    my $elf_range = shift;

    unless ($s->elf_range->contains_range($elf_range)) {
	warn("Range " . $elf_range->to_string . " not contained within "
	     . $s->elf_range->to_string);
	return undef;
    }

    return $elf_range->add($s->elf_to_mem_offset);
}


sub sizes_for_mem_range
{
    my $s = shift;
    my $mrange = shift; # Optional -covers whole map if unspecified.

    my $subrange = $mrange
	? $s->mem_range->intersect($mrange)
	    : $s->mem_range;

    my $sizes = Exmap::Sizes->new;
    return $sizes unless $subrange->size > 0;

    my $infolist = $s->_vma->get_pages_for_range($subrange);
    my $pagepool = $s->_vma->page_pool;
    
    $sizes->{vm} = $subrange->size;

    my $page_cookie;
    my $count;
    foreach my $info (@$infolist) {
	$page_cookie = $info->{page};
	$count = $pagepool->{$page_cookie};
	die ("Zero count in pagepool for $page_cookie") unless $count > 0;

	if (Exmap::Page::is_mapped($page_cookie)) {
	    $sizes->{eff_mapped} += $info->{bytes} / $count;
	    $sizes->{mapped} += $info->{bytes};
	    $sizes->{sole_mapped} += $info->{bytes}
		if ($count == 1);

	    if (Exmap::Page::is_resident($page_cookie)) {
		$sizes->{eff_resident} += $info->{bytes} / $count;
		$sizes->{resident} += $info->{bytes};
		if (Exmap::Page::is_writable($page_cookie)) {
		    $sizes->{writable} += $info->{bytes};
		}
	    }
	}
    }

    return $sizes;
}

sub to_string
{
    my $s = shift;
    return "MAP: MEM "
	. $s->mem_range->to_string
	    . " ELF "
		. ($s->elf_range ? $s->elf_range->to_string : "undef")
		    . " FILE " . $s->_vma->{info}->{file};
    
}

# ------------------------------------------------------------
package Exmap::Page;

# We encode the info in the page cookie directly, to reduce storage.
# We could use a blessed scalar ref as an object, but that would be
# two scalars per page cookie, so we use static methods instead.

sub line_to_cookie
{
    my $line = shift;
    my ($resident, $writable, $cookie) = split(/\s+/, $line);

    $cookie = hex $cookie;
    $cookie <<= 2; # Make room for flag bits
    $cookie += $resident ? 2 : 0;
    $cookie += $writable ? 1 : 0;
    return $cookie;
}

sub is_resident { return $_[0] & 2; }
sub is_writable { return $_[0] & 1; }
sub is_mapped { return $_[0] != 0 }
sub is_swapped
{
    my $cookie = shift;
    return is_mapped($cookie) && !is_resident($cookie);
}

# ------------------------------------------------------------
package Exmap::Sizes;

sub new
{
    my $c = shift;
    my $s = {};
    bless $s, $c;
    foreach my $key ($s->keys) {
	$s->{$key} = 0;
    }
    $s->{_scale_factor} = 1;
    $s->{_scale_name} = "";
    return $s;
}

my %SIZE_NAMES = (
		  eff_mapped => "Eff. Mapped",
		  eff_resident => "Eff. Resident",
		  mapped => "Mapped",
		  resident =>  "Resident",
		  sole_mapped => "Sole Mapped",
		  vm => "VM",
		  writable => "Writable",
		 );

my @KEYS = sort keys %SIZE_NAMES;
sub keys
{
    # Bit of a cheat, we want the eff_ ones first and the alphasort
    # to the front. :-/
    return @KEYS;
}

# All the svals, in the same order returned by ->keys
sub multi_svals
{
    my $s = shift;
    return map { $s->sval($_) } @_;
}

sub scale_kbytes
{
    my $s = shift;
    $s->_scale_factor(1024);
    $s->_scale_name("K");
}

sub scale_mbytes
{
    my $s = shift;
    $s->_scale_factor(1024*1024);
    $s->_scale_name("Mbytes");
}

sub _scale_factor
{
    my $s = shift;
    my $scale_factor = shift;
    if ($scale_factor) {
	$s->{_scale_factor} = $scale_factor;
    }
    return $s->{_scale_factor};
}

sub _scale_name
{
    my $s = shift;
    my $val = shift;
    if ($val) {
	$s->{_scale_name} = $val
    }
    return $s->{_scale_name};
}

# Return the value divided by the current scaling factor, to 2 dec places
sub sval
{
    my $s = shift;
    my $key = shift;
    my $val = $s->{$key};
    unless (defined $val) {
	warn "Undefined val for key [$key]";
	return undef;
    }
    $val /= $s->_scale_factor;
    $val *= 100;
    $val = int $val;
    $val /= 100;
    return $val;
}

sub key_name
{
    my $s = shift;
    my $key = shift;
    return $SIZE_NAMES{$key} . " " . $s->_scale_name;
}

sub add
{
    my $s = shift;
    my $r = shift;

    foreach my $key ($r->keys) {
	my $val = $r->{$key};
	die("undefined size for $key")
	    unless defined $val;
	$s->{$key} += $val;
    }
    return 1;
}

1;
