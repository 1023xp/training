#!/usr/bin/perl
#
# (c) John Berthels 2005 <jjberthels@gmail.com>. See COPYING for license.
#
use strict;
use warnings;
use Test::More tests => 159;

use constant PAGE_SIZE => 4096;

use_ok('Exmap');

my $UTILDIR = "../utils";
$ENV{LD_LIBRARY_PATH} .= ":$UTILDIR";
my $SA_EXE = "$UTILDIR/sharedarray";
my $SA_LIB = "$UTILDIR/libsharedarray.so";
my $MI_EXE = "$UTILDIR/mapit";
my $MI_DAT = "./mapit.dat";

my %SA_SYMBOLS = (
		  donttouch => {
				resident_fraction => 0,
				private => 1,
			       },
		  readme => {
			     resident_fraction => 1,
			     private => 0,
			    },
		  writeme => {
			      resident_fraction => 1,
			      private => 1,
			     },
		  readhalf => {
			       resident_fraction => 0.5,
			       private => 0,
			      },
		  writehalf => {
				resident_fraction => 0.5,
				private => 1,
			       },
		 );

foreach my $sym (keys %SA_SYMBOLS) {
    $SA_SYMBOLS{"uninit_$sym"} = $SA_SYMBOLS{$sym};
}

my $NUM_INSTANCES = 10;

# These need to match sharedarray.[ch] (ok, could read them from there)
my $NUM_ARRAYS = 10;
my $ARRAY_SIZE = 100 * PAGE_SIZE;

ok(-x $SA_EXE, "can find $SA_EXE executable");
ok(-f $SA_LIB, "can find $SA_LIB library");

my @fhs = start_instances($SA_EXE, $NUM_INSTANCES);
push @fhs, start_instances($MI_EXE, $NUM_INSTANCES);

# ------------------------------------------------------------
# Load the data
my $exmap = Exmap->new;
ok($exmap, "can create instance");
is($exmap->num_procs, 0, "zero processes at start");
ok($exmap->load, "can load");
my $num_procs = $exmap->num_procs;
ok($num_procs > 0, "some processes after load");

my @procs = grep { $_->exe_name && $_->exe_name =~ /$SA_EXE$/ } $exmap->procs;
is(scalar @procs, $NUM_INSTANCES, "can find all our procs");

my $proc = $procs[0];
my $sizes = $proc->sizes;
ok($sizes, "can get sizes");
ok($sizes->{vm} > $NUM_ARRAYS * $ARRAY_SIZE, "image is big enough");
my $ps_size = get_pid_size_from_ps($proc->pid);
is($sizes->{vm}, $ps_size, "exmap info matches ps output");

ok($sizes->{resident} > 0, "nonzero resident size");
ok($sizes->{eff_resident} > 0, "nonzero effective size");
ok($sizes->{eff_resident} < $sizes->{resident},
   "effective is smaller than resident");

my @files = $exmap->files;
ok(@files > 0, "can find some resident file");

# This is slightly evil. SA_LIB is a path beginning ../
# which will happily match as a regexp...
@files = grep { $_->name =~ /$SA_LIB$/ } @files;
is(scalar @files, 1, "the file is only listed once");
my $file = shift @files;

ok($file->is_elf, "file recognised as an elf file");

# All our procs should be mapping our shared lib

my @procs_for_file = $file->procs;
is(scalar @procs_for_file, $NUM_INSTANCES, "right number of procs for file");
foreach my $proc (@procs_for_file) {
    ok($proc->exe_name =~ /$SA_EXE$/, "each proc mapping lib is one of ours");
}

foreach my $proc (@procs) {
    $sizes = $proc->sizes($file);
    my $arrays_size = $NUM_ARRAYS * $ARRAY_SIZE;
    my $delta = abs($sizes->{vm} - $arrays_size) / $arrays_size;
    # Not exact, because there will be some function code etc.
    ok($delta < 0.01, "Shared lib has correct size in each proc [$arrays_size != $sizes->{vm}]");
}

my $text = $file->elf->section_by_name(".text");
ok($text, "can find text section");
ok($text->size > 0, "text section has nonzero size");
my @sizes = $procs[0]->elf_range_sizes($file,
				       $text->mem_range);
is(scalar @sizes, 1, "one size returned for one range");
is(2 * $text->size, $sizes[0]->{resident},
   "lib text is resident and mapped twice (!)");

my $bss = $file->elf->section_by_name(".bss");
ok($bss, "can find bss section");

my $data = $file->elf->section_by_name(".data");
ok($data, "can find data section");

is($data->size, $bss->size, "data and bss sizes the same");
@sizes = $procs[0]->elf_range_sizes($file, $data->mem_range);
is(scalar @sizes, 1, "one size returned for one range");
my @bss_sizes = $procs[0]->elf_range_sizes($file, $bss->mem_range);
is(scalar @bss_sizes, 1, "one size returned for one bss range");
my $delta = $sizes[0]->{resident} - $bss_sizes[0]->{resident};
ok($delta <= PAGE_SIZE, "data and bss resident are within a page");


# Find how many arrays-worth will be resident in .bss
my @bss_syms = grep { $_ =~ /uninit_/ } keys %SA_SYMBOLS;
my $resident_number_of_arrays = 0;
foreach my $sym (@bss_syms) {
    $resident_number_of_arrays += $SA_SYMBOLS{$sym}->{resident_fraction};
}
my $arrays_size = $ARRAY_SIZE * $resident_number_of_arrays;

foreach my $proc (@procs) {
    @sizes = $proc->elf_range_sizes($file, $bss->mem_range);
    is(scalar @sizes, 1, "one size returned for one range");
    my $resident_size_for_file_in_proc = $sizes[0]->{resident};
    is($resident_size_for_file_in_proc, $arrays_size,
       "correct resident size in bss")
}


foreach my $sym_name (keys %SA_SYMBOLS) {
    my $sym = $file->elf->symbol_by_name($sym_name);
    ok($sym, "can find symbol $sym_name");
    is($sym->size, $ARRAY_SIZE, "symbol $sym_name has correct size");
    @sizes = $procs[0]->elf_range_sizes($file, $sym->range);
    is(scalar @sizes, 1, "one size returned for one range");
    my $resident_size = $sizes[0]->{resident};
    is($resident_size,
       $SA_SYMBOLS{$sym_name}->{resident_fraction} * $ARRAY_SIZE,
       "symbol $sym_name has correct resident size");

    # Uninitialised space which is only read appears to be shared.
    # This is cool from a memusage point of view, but it means that it is
    # shared among nearly all running procs to varying degrees. So we can't
    # really account for it.
    next if ($sym_name =~ /^uninit_read/);

    @sizes = $procs[0]->elf_range_sizes($file, $sym->range);
    is(scalar @sizes, 1, "one size returned for one range");
    my $eff_resident_size = $sizes[0]->{eff_resident};
    my $expected_esize
	= $SA_SYMBOLS{$sym_name}->{resident_fraction} * $ARRAY_SIZE;
    if (!$SA_SYMBOLS{$sym_name}->{private}) {
	# Shared...
	$expected_esize /= $NUM_INSTANCES;
    }

    # Floating pt calc, so avoid == test
    my $delta = abs($eff_resident_size - $expected_esize);
    ok($delta < 0.001,
       "$sym_name has correct esize [$eff_resident_size - $expected_esize]");
}


# Test non-elf maps
@procs = grep { $_->exe_name && $_->exe_name =~ /$MI_EXE$/ } $exmap->procs;
is(scalar @procs, $NUM_INSTANCES, "can find all our mapit procs");

@files = $exmap->files;
ok(@files > 0, "can find some resident files");
@files = grep { $_->name =~ /$MI_DAT$/ } @files;
is(scalar @files, 1, "$MI_DAT file is only listed once");
$file = shift @files;

ok(!$file->is_elf, "$MI_DAT isn't an elf file");
my $mi_data_size = -s $MI_DAT;
ok($mi_data_size > 0, "$MI_DAT isn't empty");
foreach my $proc (@procs) {
    $sizes = $proc->sizes($file);
    is($sizes->{vm}, $mi_data_size, "correct data file size");
    is($sizes->{resident}, $mi_data_size, "whole data file is resident");
    is($sizes->{eff_resident}, $mi_data_size / $NUM_INSTANCES,
       "data file is shared between instances");
    
    
}
stop_instances(@fhs);

exit 0;

# ------------------------------------------------------------

sub stop_instances
{
    my @fhs = shift;
    foreach my $fh (@fhs) {
	print $fh "\n";
	close $fh or die("problem stopping instance : $!");
    }
    return 1;
}

sub start_instances
{
    my $exe = shift;
    my $num_instances = shift;

    my @fhs;

    while ($num_instances-- > 0) {
	my $fh;
	open ($fh, "|$exe")
	    or die("Can't start [$exe] : $!");
	push @fhs, $fh;
    }
    return @fhs;
}

sub get_pid_size_from_ps
{
    my $pid = shift;
    my @lines = `ps -e -o pid,vsz`;
    chomp @lines;
    @lines = grep { /\s*$pid\s/ } @lines;
    return undef unless scalar @lines == 1;
    my ($pspid, $size) = $lines[0] =~ /\s*(\d+)\s+(\d+)$/;
    return $size * 1024;
}
