#!/usr/bin/perl
#
# (c) John Berthels 2005 <jjberthels@gmail.com>. See COPYING for license.
#
use strict;
use warnings;
use Test::More tests => 71;
use_ok('Elf');

my %TESTDAT = (
	       `which ls` => {
		   type => 'EXEC',
		   exe => 1,
		   num_symbols => 0,
	       },
	       '/usr/lib/libz.so' => {
		   type => 'DYN',
		   exe => '',
		   num_symbols => 0,
	       },
	       );

my @BUILDFILES = ({
    source_name => 'hello-world.c',
    built_name => 'hello-world',
    build_cmd => 'gcc -o hello-world hello-world.c',
    testdat => {
	type => 'EXEC',
	exe => 1,
    },
    text => <<'END_HW',
    #include <stdio.h>
    int main(void)
    {
	return 0;
    }
END_HW
});
    

my $e = Elf->new;
ok(!defined($e), "must supply filename");
$e = Elf->new('/doesnotexist', 1); # Suppress warning to stderr
ok(!defined($e), "must supply file which exists");
my $not_elf = '/etc/motd';
ok(-f $not_elf, "non-elf file exists");
$e = Elf->new($not_elf, 1); # Suppress warning to stderr
ok(!defined($e), "must supply file which is ELF");

create_build_files();

foreach my $f (keys %TESTDAT) {
    my $td = $TESTDAT{$f};
    chomp $f;
    $e = Elf->new($f);
    ok($e, "can instantiate on $f");
    is($f, $e->file, "correct file name");
    is($td->{type}, $e->e_type, "has right file type");
    is($td->{exe}, $e->is_executable, "file $f is " .
       ($td->{exe} ? '' : 'not ')
       . "an exe");

    my @readelf_lines = `readelf -S $f`;
    @readelf_lines = grep { /^\s*\[ *\d+\]/ } @readelf_lines;
    $td->{num_sections} = scalar @readelf_lines;
    
    is( $td->{num_sections}, $e->num_sections,
       "number of sections match readelf");

    ok($e->section_by_name('.text'), "can find text section");
    ok($e->section_by_name('.data'), "can find data section");
    ok($e->section_by_name('.bss'), "can find bss section");

    my $num_segments = $e->num_segments;
    ok($num_segments, "can get segments (program header)");

    my @lines = `readelf -l $f`;
    die("Can't run readelf") unless @lines;
    my ($line) = grep { /There are \d+ program headers/ } @lines;
    my ($readelf_numsegs) = $line =~ /There are (\d+) program headers/;
    $readelf_numsegs = $1;
    is($readelf_numsegs, $num_segments, "number of segments matches readelf");

    my @segments = $e->segments;
    is(scalar(@segments), $num_segments, "sanity check number of segments");
    @segments = $e->loadable_segments;
    is(2, scalar(@segments), "two areas for each elf");

    ok($segments[0]->is_readable, "first is readable");
    ok(!$segments[0]->is_writable, "first is not writable");
    ok($segments[0]->is_executable, "first is executable");

    ok($segments[1]->is_readable, "second is readable");
    ok($segments[1]->is_writable, "second is writable");
    ok(!$segments[1]->is_executable, "second is not executable");

    # Can't run a test-per-section, because the number of sections
    # varies with the compilation environment (and that throws off the
    # plan). So check they are all the same and then do one test.
    my $all_sections_match_ok = 1;
    foreach my $s ($e->sections) {
	my $readelf_name = shift @readelf_lines;
	$readelf_name =~ s/\r?\n$//;
	$readelf_name =~ s/^\s*\[ *\d+\]\s//;
	$readelf_name =~ s/\s.*$//;
	$all_sections_match_ok = 0 unless $s->name eq $readelf_name;
#	is($s->name, $readelf_name,
#	   "section names and order match readelf $readelf_name");
    }
    ok($all_sections_match_ok, "section names and order match readelf");

    my @symbols = $e->defined_symbols;
    is($td->{num_symbols}, scalar(@symbols), "correct number of symbols");

    if ($td->{type} eq 'EXEC' && $td->{num_symbols} > 0) {
	my $mainsym = $e->symbol_by_name('main');
	ok($mainsym, "can find 'main' symbol in exe");
	is($mainsym->name, 'main', "symbol has correct name");
	ok($mainsym->size > 0, "main has non-zero size");
	ok($mainsym->is_func, "main symbol is a function");

	my @symbols = grep {
	    $_->name eq 'main'
	} $e->symbols_in_section($e->section_by_name('.text'));
	is(scalar @symbols, 1, "can find 1 main symbol in .text");
	@symbols = grep {
	    $_->name eq 'main'
	} $e->symbols_in_section($e->section_by_name('.data'));
	is(scalar @symbols, 0, "can find no main symbol in .data");
    }
}

remove_build_files();

exit 0;


sub create_build_files
{
    foreach my $bf (@BUILDFILES) {
	my $sname = $bf->{source_name};
	my $name = $bf->{built_name};
	unless(open(F, "> $sname")) {
	    warn("Can't open $sname");
	    next;
	}
	print F $bf->{text};
	close F;
	unless (system($bf->{build_cmd}) == 0) {
	    warn("Can't build $sname");
	    unlink $sname;
	    next;
	}
	unless (-f $name) {
	    warn("Build didn't create $name");
	    unlink $sname;
	    next;
	}
	$TESTDAT{$name} = $bf->{testdat};
	$TESTDAT{$name}->{num_symbols} = nm_count_symbols($name);
    }
    
}

sub remove_build_files
{
    foreach my $bf (@BUILDFILES) {
	unlink $bf->{source_name};
	unlink $bf->{built_name};
    }
}

sub nm_count_symbols
{
    my $exe = shift;
    my @lines = `nm $exe`;
    @lines = grep { !/ (w|U) / } @lines;
    return scalar(@lines);
}
