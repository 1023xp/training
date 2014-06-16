/*
 * (c) John Berthels 2005 <jjberthels@gmail.com>. See COPYING for license.
 */
#include "Range.hpp"
#include <jutil.hpp>
#include <Trun.hpp>

#include <sstream>
#include <list>

class RangeTest : public Test
{
public:
    bool run();
};

using namespace std;
using namespace jutil;

bool RangeTest::run()
{
    plan(133);

    Range r1(3, 3);
    is(r1.start(), 3UL, "check start");
    is(r1.end(), 3UL, "check start");
    is(r1.size(), 0UL, "check zero length size");
    notok(r1.contains(3), "check doesn't contain start pt (since zero length)");
    notok(r1.contains(0), "check doesn't contain below");
    notok(r1.contains(4), "check doesn't contain below");
    notok(r1.overlaps(r1), "check zero range doesn't self-overlap");

    Range r2(2, 6);
    is(r2.start(), 2UL, "check start");
    is(r2.end(), 6UL, "check start");
    is(r2.size(), 4UL, "check length size");
    ok(r2.contains(2), "check contains start pt");
    notok(r2.contains(6), "check doesn't contain end pt");
    ok(r2.contains(4), "check contains within");

    ok(r2.overlaps(r2) , "check self-overlaps");
    ok(r2.overlaps(Range(2, 2)), "overlap1a");
    notok(r2.overlaps(Range(0, 0)), "overlap1");
    notok(r2.overlaps(Range(6, 6)), "overlap2");
    notok(r2.overlaps(Range(7, 8)), "overlap3");
    notok(r2.overlaps(Range(6, 8)), "overlap4");

    notok(r2.overlaps(Range(1, 2)), "overlap5");
    ok(r2.overlaps(Range(1, 3)), "overlap6");
    ok(r2.overlaps(Range(2, 3)), "overlap7");
    ok(r2.overlaps(Range(3, 4)), "overlap8");
    ok(r2.overlaps(Range(3, 6)), "overlap9");
    ok(r2.overlaps(Range(3, 7)), "overlap10");

    ok(r2.overlaps(Range(0, 10)), "overlap11");

    notok(r2.contains(Range(0, 1)), "contains range 1");
    notok(r2.contains(Range(6, 7)), "contains range 2");
    ok(r2.contains(r2), "contains range 3");
    ok(r2.contains(Range(3, 5)), "contains range 3");
    ok(r2 == Range(2, 6), "Range isn't changed by contains");


    ok(r2 == Range(2, 6), "range equality1");
    notok(r2 == Range(1, 6), "range equality2");
    notok(r2 == Range(2, 7), "range equality3");

    notok(r2.intersect(Range(0, 1)), "intersect1");
    notok(r2.intersect(Range(0, 2)), "intersect2");
    ok(*(r2.intersect(Range(2, 2))) == Range(2, 2), "intersect3");
    notok(r2.intersect(Range(6, 6)), "intersect4");
    notok(r2.intersect(Range(6, 7)), "intersect5");
    notok(r2.intersect(Range(6, 8)), "intersect6");
    
    ok(*(r2.intersect(Range(1, 3))) == Range(2, 3), "intersect7");
    ok(*(r2.intersect(Range(1, 9))) == Range(2, 6), "intersect7");
    ok(*(r2.intersect(Range(2, 9))) == Range(2, 6), "intersect7");
    ok(*(r2.intersect(Range(3, 9))) == Range(3, 6), "intersect7");
    ok(*(r2.intersect(Range(3, 4))) == Range(3, 4), "intersect7");
    ok(*(r2.intersect(Range(3, 6))) == Range(3, 6), "intersect7");

    ok(*(Range(0, 10).intersect(Range(3, 6))) == Range(3, 6),
       "intersect7");

    ok(*(Range(3, 6).intersect(Range(0, 10))) == Range(3, 6),
       "intersect7");

    r2.add(3);
    ok(r2 == Range(2,6), "add doesn't modify range");

    is(r2.add(3)->start(), 5UL, "add to range moves start");
    is(r2.add(3)->end(), 9UL, "add to range moves end");
    is(r2.add(3)->size(), 4UL, "add to range leaves size alone");
    r2.subtract(2);
    ok(r2 == Range(2,6), "subtract doesn't modify range");

    is(r2.subtract(2)->start(), 0UL, "subtract range moves start");
    is(r2.subtract(2)->end(), 4UL, "subtract range moves end");
    is(r2.subtract(2)->size(), 4UL, "subtract range leaves size alone");

    r2.truncate_below(4);
    ok(r2 == Range(2, 6), "truncate_below doesn't change range");

    ok(*(r2.truncate_below(1)) == Range(2, 6),
       "truncate_below below the range");
    ok(*(r2.truncate_below(2)) == Range(2, 6),
       "truncate_below at the start");
    ok(*(r2.truncate_below(4)) == Range(4, 6),
       "truncate_below in the middle");
    ok(*(r2.truncate_below(8)) == Range(8, 8),
       "truncate_below above the end");
    RangePtr r3 = Range(3, 7).truncate_below(7);
    ok(*r3 == Range(7, 7), "truncate_below at the end");

    r2.truncate_above(4);
    ok(r2 == Range(2, 6), "truncate_above doesn't change range");
    ok(*(r2.truncate_above(1)) == Range(1, 1),
       "truncate_above below the range");
    ok(*(r2.truncate_above(2)) == Range(2, 2),
       "truncate_above at the start");
    ok(*(r2.truncate_above(4)) == Range(2, 4),
       "truncate_above in the middle");
    ok(*(r2.truncate_above(8)) == Range(2, 6),
       "truncate_above above the end");

    Range t = r2;
    ok(t.size() > 0, "Can create a copy");
    is(t.start(), r2.start(), "have same start");
    is(t.end(), r2.end(), "have same end");
    is(t.size(), r2.size(), "have same size");

    std::stringstream sstr;
    sstr << r2;
    ok(!sstr.str().empty(), "can write a range to an ostream");
    sstr.str("");
    sstr << r3;
    ok(!sstr.str().empty(), "can write a rangeptr to an ostream");

    RangePtr lr0 = RangePtr(new Range(1, 2));
    RangePtr lr1 = RangePtr(new Range(3, 5));
    RangePtr lr2 = RangePtr(new Range(5, 7));
    
    notok(lr0->merge(*lr1), "can't merge non-adjacent");

    
    ok(lr1->merge(*lr2), "Can merge adjacent");
    ok(*(lr1->merge(*lr2)) == Range(3, 7), "Can merge adjacent 2");
    ok(*(lr2->merge(*lr1)) == Range(3, 7), "Can merge adjacent 3");
    
    RangePtr merge_result = Range(1, 2).merge(Range(1, 2));
    ok(merge_result, "self merge succeeds");
    ok(Range(1, 2) == *merge_result, "self merge returns self");
    
    merge_result = Range(1, 3).merge(Range(2, 4));
    ok(merge_result, "overlap merge succeeds");
    ok(Range(1, 4) == *merge_result, "overlap merge correct");
  
    merge_result = Range(2, 4).merge(Range(1, 3));
    ok(merge_result, "reverse overlap merge succeeds");
    ok(Range(1, 4) == *merge_result, "reverse overlap merge correct");

    list<RangePtr> l;
    l.push_back(lr0);
    l.push_back(lr1);
    l.push_back(lr2);

    // ------------------------------------------------------------
    // merge_list
    
    list<RangePtr> mr = Range::merge_list(l);
    is((int) mr.size(), 2, "merged list has 2 entries");
    ok(*(mr.front()) == *(l.front()), "first elt unchanged");
    ok(*(mr.back()) == Range(3, 7), "second elf merged");

    list<RangePtr> empty_list;
    mr = Range::merge_list(empty_list);
    ok(mr.empty(), "merging empty list leaves empty");

    l.clear();
    l.push_back(RangePtr(new Range(1, 3)));
    mr = Range::merge_list(l);
    is((int) mr.size(), 1, "merging single elt list leaves one elt");
    ok(*(mr.front()) == Range(1, 3),
       "merging single elt list leaves unchanged");

    l.clear();
    l.push_back(RangePtr(new Range(1, 3)));
    l.push_back(RangePtr(new Range(5, 10)));
    mr = Range::merge_list(l);
    is((int) mr.size(), 2, "merging unmergable list doesn't change no of elts");
    ok(*(mr.front()) == Range(1, 3), "unmergeable 1");
    ok(*(mr.back()) == Range(5, 10), "unmergeable 1");

    l.clear();
    l.push_back(RangePtr(new Range(1, 2)));
    l.push_back(RangePtr(new Range(2, 3)));
    l.push_back(RangePtr(new Range(3, 4)));
    l.push_back(RangePtr(new Range(4, 5)));
    mr = Range::merge_list(l);
    is((int) mr.size(), 1, "merging 4 into 1 count");
    ok(*(mr.front()) == Range(1, 5), "merge 4 into 1");

    l.clear();
    l.push_back(RangePtr(new Range(1, 2)));
    l.push_back(RangePtr(new Range(1, 2)));
    mr = Range::merge_list(l);
    is((int) mr.size(), 1, "merging 2 into 1 count");
    ok(*(mr.front()) == Range(1, 2), "merge 2 into 1");

    l.clear();
    l.push_back(RangePtr(new Range(2, 4)));
    l.push_back(RangePtr(new Range(1, 3)));
    mr = Range::merge_list(l);
    is((int) mr.size(), 1, "merging 2 into 1 count b");
    ok((*mr.front()) == Range(1, 4), "merge 2 into 1 b");

    // ------------------------------------------------------------
    // invert_list

    l.clear();
    l.push_back(RangePtr(new Range(1, 2)));
    l.push_back(RangePtr(new Range(3, 5)));
    l.push_back(RangePtr(new Range(5, 7)));

    list<RangePtr> rl = Range(0, 10).invert_list(l);
    is((int) rl.size(), 3, "Right number of inverted elements");
    ok(*(rl.front()) == Range(0, 1), "first inverted ok");
    rl.pop_front();
    ok(*(rl.front()) == Range(2, 3), "second inverted ok");
    rl.pop_front();
    ok(*(rl.front()) == Range(7, 10), "thirdinverted ok");

    rl = Range(1, 10).invert_list(l);
    is((int) rl.size(), 2, "Right number of inverted elements");
    ok(*(rl.front()) == Range(2, 3), "first inverted ok");
    rl.pop_front();
    ok(*(rl.front()) == Range(7, 10), "second inverted ok");

    rl = Range(1, 7).invert_list(l);
    is((int) rl.size(), 1, "Right number of inverted elements");
    ok(*(rl.front()) == Range(2, 3), "first inverted ok");

    l.clear();
    l.push_back(RangePtr(new Range(1, 2)));
    rl = Range(0, 3).invert_list(l);
    is((int) rl.size(), 2, "Right number of inverted elements");
    ok(*(rl.front()) == Range(0, 1), "inverted ok");
    ok(*(rl.back()) == Range(2, 3), "inverted ok");


    rl = Range(1, 2).invert_list(l);
    is((int) rl.size(), 0, "Right number of inverted elements");

    rl = Range(1, 3).invert_list(l);
    is((int) rl.size(), 1, "Right number of inverted elements");
    ok(*(rl.front()) == Range(2, 3), "inverted ok");

    l.clear();
    l.push_back(RangePtr(new Range(1, 2)));
    l.push_back(RangePtr(new Range(3, 5)));
    l.push_back(RangePtr(new Range(5, 7)));
    l.push_back(RangePtr(new Range(10, 15)));
    l.push_back(RangePtr(new Range(20, 30)));
    rl = Range(6, 8).invert_list(l);
    is((int) rl.size(), 1, "restricted invert 1");
    is(*rl.front(), Range(7, 8), "restricted invert 2");

    // ------------------------------------------------------------
    // any_overlap

    l.clear();
    l.push_back(RangePtr(new Range(1, 2)));
    l.push_back(RangePtr(new Range(3, 5)));
    l.push_back(RangePtr(new Range(5, 7)));

    ok(!Range::any_overlap(l), "overlap1");

    l.push_back(RangePtr(new Range(2, 3)));
    ok(!Range::any_overlap(l), "overlap2");

    l.push_back(RangePtr(new Range(6, 7)));
    ok(Range::any_overlap(l), "overlap3");

    l.pop_back();
    l.push_back(RangePtr(new Range(6, 8)));
    ok(Range::any_overlap(l), "overlap4");

    // Restrict a list to a containing range
    l.clear();
    l.push_back(RangePtr(new Range(2, 3)));
    l.push_back(RangePtr(new Range(4, 6)));
    l.push_back(RangePtr(new Range(6, 8)));
    list<RangePtr> rest_list = Range(0, 1).restrict(l);
    ok(rest_list.empty(), "restrict 1");
    rest_list = Range(1, 2).restrict(l);
    ok(rest_list.empty(), "restrict 1a");

    rest_list = Range(1, 3).restrict(l);
    is((int) rest_list.size(), 1, "restrict 2");
    is(*rest_list.front(), Range(2, 3), "restrict 3");

    rest_list = Range(0, 10).restrict(l);
    is(rest_list, Range::merge_list(l), "restrict 4");
    
    rest_list = Range(6, 10).restrict(l);
    is((int) rest_list.size(), 1, "restrict 5");
    is(*rest_list.front(), Range(6, 8), "restrict 6");

    rest_list = Range(5, 10).restrict(l);
    is((int) rest_list.size(), 1, "restrict 7");
    is(*rest_list.front(), Range(5, 8), "restrict 8");

    rest_list = Range(1, 5).restrict(l);
    is((int) rest_list.size(), 2, "restrict 9");
    is(*rest_list.front(), Range(2, 3), "restrict 10");
    is(*rest_list.back(), Range(4, 5), "restrict 11");

    l.clear();
    l.push_back(RangePtr(new Range(6, 8)));
    l.push_back(RangePtr(new Range(2, 3)));
    l.push_back(RangePtr(new Range(4, 6)));
    l.sort();
    is(*l.front(), Range(2, 3), "sorted pointer list 1");
    is(*l.back(), Range(6, 8), "sorted pointer list 2");

    return true;
}


RUN_TEST_CLASS(RangeTest);
