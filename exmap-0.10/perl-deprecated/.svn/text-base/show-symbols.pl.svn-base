#!/usr/bin/perl
#
# (c) John Berthels 2005 <jjberthels@gmail.com>. See COPYING for license.
#
use warnings;
use strict;
use Elf;

my $file = shift;
die("No file specified") unless $file;
my $e = Elf->new($file);
foreach my $s ($e->defined_symbols) {
    print join(" ", sprintf("0x%08x", $s->value), $s->size, $s->name), "\n";
}
    
