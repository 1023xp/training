#!/usr/bin/perl
#
# (c) John Berthels 2005 <jjberthels@gmail.com>. See COPYING for license.
#
use strict;
use warnings;
use Test::More tests => 115;

BEGIN { use_ok qw(Range); }

my $r = Range->new(0, -1);
ok(!$r, "Can't have reversed range");
$r = Range->new(0, 0);
ok($r, "Can have empty range 1");

$r = Range->new(3, 3);
ok($r, "Can have empty range 2");
is($r->start, 3, "check start");
is($r->end, 3, "check end");
is($r->size, 0, "check zero length size");
ok(!$r->contains(3), "check doesn't contain start pt (since zero length)");
ok(!$r->contains(0), "check doesn't contain below");
ok(!$r->contains(4), "check doesn't contain above");
ok(!$r->overlaps($r), "zero-length range doesn't self-overlap");

$r = Range->new(2, 6);
is($r->start, 2, "check start");
is($r->end, 6, "check end");
is($r->size, 4, "check length size");
ok($r->contains(2), "check contains start pt");
ok(!$r->contains(6), "check doesn't contain end");
ok($r->contains(4), "check contains within");

ok($r->overlaps($r), "check self-overlaps");
ok($r->overlaps(Range->new(2, 2)), "overlap1a");
ok(!$r->overlaps(Range->new(0, 0)), "overlap1");
ok(!$r->overlaps(Range->new(6, 6)), "overlap2");
ok(!$r->overlaps(Range->new(7, 8)), "overlap3");
ok(!$r->overlaps(Range->new(6, 8)), "overlap4");

ok(!$r->overlaps(Range->new(1, 2)), "overlap5");
ok($r->overlaps(Range->new(1, 3)), "overlap6");
ok($r->overlaps(Range->new(2, 3)), "overlap7");
ok($r->overlaps(Range->new(3, 4)), "overlap8");
ok($r->overlaps(Range->new(3, 6)), "overlap9");
ok($r->overlaps(Range->new(3, 7)), "overlap10");

ok($r->overlaps(Range->new(0, 10)), "overlap11");

ok(!$r->contains_range(Range->new(0, 1)), "contains range 1");
ok(!$r->contains_range(Range->new(6, 7)), "contains range 2");
ok($r->contains_range($r), "contains range 3");
ok($r->contains_range(Range->new(3, 5)), "contains range 3");
ok($r->equals(Range->new(2, 6)), "Range isn't changed by contains_range");

ok($r->equals(Range->new(2, 6)), "range equality1");
ok(!$r->equals(Range->new(1, 6)), "range equality2");
ok(!$r->equals(Range->new(2, 7)), "range equality3");

ok(!defined $r->intersect(Range->new(0, 1)), "intersect1");
ok(!defined $r->intersect(Range->new(0, 2)), "intersect2");
ok($r->intersect(Range->new(2, 2))->equals(Range->new(2, 2)), "intersect3");
ok(!defined $r->intersect(Range->new(6, 6)), "intersect4");
ok(!defined $r->intersect(Range->new(6, 7)), "intersect5");
ok(!defined $r->intersect(Range->new(6, 8)), "intersect6");

ok($r->intersect(Range->new(1, 3))->equals(Range->new(2, 3)), "intersect7");
ok($r->intersect(Range->new(1, 9))->equals(Range->new(2, 6)), "intersect7");
ok($r->intersect(Range->new(2, 9))->equals(Range->new(2, 6)), "intersect7");
ok($r->intersect(Range->new(3, 9))->equals(Range->new(3, 6)), "intersect7");
ok($r->intersect(Range->new(3, 4))->equals(Range->new(3, 4)), "intersect7");
ok($r->intersect(Range->new(3, 6))->equals(Range->new(3, 6)), "intersect7");

ok(Range->new(0, 10)->intersect(Range->new(3, 6))->equals(Range->new(3, 6)),
   "intersect7");

ok(Range->new(3, 6)->intersect(Range->new(0, 10))->equals(Range->new(3, 6)),
   "intersect7");

$r->add(3);
ok($r->equals(Range->new(2,6)), "add doesn't modify range");

is($r->add(3)->start, 5, "add to range moves start");
is($r->add(3)->end, 9, "add to range moves end");
is($r->add(3)->size, 4, "add to range leaves size alone");
$r->subtract(2);
ok($r->equals(Range->new(2,6)), "subtract doesn't modify range");

is($r->subtract(2)->start, 0, "subtract range moves start");
is($r->subtract(2)->end, 4, "subtract range moves end");
is($r->subtract(2)->size, 4, "subtract range leaves size alone");


$r->truncate_below(4);
ok($r->equals(Range->new(2, 6)), "truncate_below doesn't change range");

ok($r->truncate_below(1)->equals(Range->new(2, 6)),
   "truncate_below below the range");
ok($r->truncate_below(2)->equals(Range->new(2, 6)),
   "truncate_below at the start");
ok($r->truncate_below(4)->equals(Range->new(4, 6)),
   "truncate_below in the middle");
ok($r->truncate_below(8)->equals(Range->new(8, 8)),
   "truncate_below above the end");
$r = Range->new(3, 7)->truncate_below(7);
ok($r->equals(Range->new(7, 7)), "truncate_below at the end");


my $t = $r->copy;
ok($t, "Can create a copy");
is($t->start, $r->start, "have same start");
is($t->end, $r->end, "have same end");
is($t->size, $r->size, "have same size");


my @r = (Range->new(1, 2),
	 Range->new(3, 5),
	 Range->new(5, 7));

ok(!$r[0]->merge($r[1]), "Can't merge non-adjacent");
ok($r[1]->merge($r[2]), "Can merge adjacent");
ok($r[1]->merge($r[2])->equals(Range->new(3, 7)), "Can merge adjacent 2");

$r = Range->new(1, 2)->merge(Range->new(1, 2));
ok($r, "self merge succeeds");
ok(Range->new(1, 2)->equals($r), "self merge returns self");

$r = Range->new(1, 3)->merge(Range->new(2, 4));
ok($r, "overlap merge succeeds");
ok(Range->new(1, 4)->equals($r), "overlap merge correct");

$r = Range->new(2, 4)->merge(Range->new(1, 3));
ok($r, "reverse overlap merge succeeds");
ok(Range->new(1, 4)->equals($r), "reverse overlap merge correct");


my @mr = Range::merge_list(@r);
is(scalar @mr, 2, "merged list has 2 entries");
ok($mr[0]->equals($r[0]), "first elt unchanged");
ok($mr[1]->equals(Range->new(3, 7)), "second elf merged");

@r = ();
@mr = Range::merge_list(@r);
is(scalar @mr, 0, "merging empty list leaves empty");

@r = (Range->new(1, 3));
@mr = Range::merge_list(@r);
is(scalar @mr, 1, "merging single elt list leaves one elt");
ok($mr[0]->equals(Range->new(1, 3)),
   "merging single elt list leaves unchanged");

@r = (Range->new(1, 3), Range->new(5, 10));
@mr = Range::merge_list(@r);
is(scalar @mr, 2, "merging unmergable list doesn't change no of elts");
ok($mr[0]->equals(Range->new(1, 3)), "unmergeable 1");
ok($mr[1]->equals(Range->new(5, 10)), "unmergeable 1");

@r = (Range->new(1, 2),
      Range->new(2, 3),
      Range->new(3, 4),
      Range->new(4, 5));
@mr = Range::merge_list(@r);
is(scalar @mr, 1, "merging 4 into 1 count");
ok($mr[0]->equals(Range->new(1, 5)), "merge 4 into 1");

@r = (Range->new(1, 2),
      Range->new(1, 2));
@mr = Range::merge_list(@r);
is(scalar @mr, 1, "merging 2 into 1 count");
ok($mr[0]->equals(Range->new(1, 2)), "merge 2 into 1");

@r = (Range->new(2, 4),
      Range->new(1, 3));
@mr = Range::merge_list(@r);
is(scalar @mr, 1, "merging 2 into 1 count b");
ok($mr[0]->equals(Range->new(1, 4)), "merge 2 into 1 b");


# Inverting

@r = (Range->new(1, 2),
      Range->new(3, 5),
      Range->new(5, 7));

my $rl = Range->new(0, 10)->invert_list(@r);
ok($rl, "Can invert list");
SKIP: {
    skip "undef from invert list", 4 unless $rl;
    is(scalar @$rl, 3, "Right number of inverted elements");
    ok($rl->[0]->equals(Range->new(0, 1)), "first inverted ok");
    ok($rl->[1]->equals(Range->new(2, 3)), "second inverted ok");
    ok($rl->[2]->equals(Range->new(7, 10)), "third inverted ok");
}

$rl = Range->new(1, 10)->invert_list(@r);
ok($rl, "Can invert list");
SKIP: {
    skip "undef from invert list", 3 unless $rl;
    is(scalar @$rl, 2, "Right number of inverted elements");
    ok($rl->[0]->equals(Range->new(2, 3)), "first inverted ok");
    ok($rl->[1]->equals(Range->new(7, 10)), "second inverted ok");
}

$rl = Range->new(1, 7)->invert_list(@r);
ok($rl, "Can invert list");
SKIP: {
    skip "undef from invert list", 2 unless $rl;
    is(scalar @$rl, 1, "Right number of inverted elements");
    ok($rl->[0]->equals(Range->new(2, 3)), "first inverted ok");
}

@r = (Range->new(1, 2));
$rl = Range->new(0, 3)->invert_list(@r);
ok($rl, "Can invert list");
SKIP : {
    skip "undef from invert list", 3 unless $rl;
    is(scalar @$rl, 2, "Right number of inverted elements");
    ok($rl->[0]->equals(Range->new(0, 1)), "inverted ok");
    ok($rl->[1]->equals(Range->new(2, 3)), "inverted ok");
}

$rl = Range->new(1, 2)->invert_list(@r);
ok($rl, "Can invert list");
SKIP : {
    skip "undef from invert list", 1 unless $rl;
    is(scalar @$rl, 0, "Right number of inverted elements");
}

$rl = Range->new(1, 3)->invert_list(@r);
ok($rl, "Can invert list");
SKIP : {
    skip "undef from invert list", 2 unless $rl;
    is(scalar @$rl, 1, "Right number of inverted elements");
    ok($rl->[0]->equals(Range->new(2, 3)), "inverted ok");
}
