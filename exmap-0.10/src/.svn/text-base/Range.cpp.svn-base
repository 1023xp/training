/*
 * (c) John Berthels 2005 <jjberthels@gmail.com>. See COPYING for license.
 */
#include <sstream>
#include "Range.hpp"

using namespace std;

Range::Range(unsigned long start, unsigned long end)
    : _start(start),
      _end(end)
{ }

unsigned long Range::start() const { return _start; }
unsigned long Range::end() const { return _end; }
unsigned long Range::size() const
{
    return _end - _start;
}
    
RangePtr Range::intersect(const Range &r) const
{
    if (!overlaps(r)) {
	return RangePtr((Range *) 0);
    }
    unsigned long start = (_start > r._start) ? _start : r._start;
    unsigned long end = (_end > r._end) ? r._end : _end;
    return RangePtr(new Range(start, end));
}

RangePtr Range::intersect(const RangePtr &r) const
{
    if (!r) { return RangePtr((Range *) 0); }
    return intersect(*r);
}

bool Range::operator==(const Range &r) const
{
    return _start == r._start && _end == r._end;
}

bool Range::operator<(const Range &other) const
{
    return _start < other._start;
}



void Range::print(std::ostream &os) const
{
    os << to_string();
}

RangePtr Range::add(unsigned long v) const
{
    return RangePtr(new Range(_start + v, _end + v));
}

RangePtr Range::subtract(unsigned long v) const
{
    return add(-v);
}

bool Range::contains(unsigned long v) const
{
    return _start <= v && v < _end;
}

bool Range::contains(const RangePtr &r) const
{
    if (!r) { return false; }
    return contains(*r);
}

bool Range::contains(const Range &r) const
{
    RangePtr is = intersect(r);
    return is && *is == r;
}

bool Range::overlaps(const Range &r) const
{
    return contains(r._start)
	|| r.contains(_start)
	|| (r._start <= _start && _end < r._end);
}

RangePtr Range::truncate_below(unsigned long v) const
{
    unsigned long start = _start < v ? v : _start;
    unsigned long end = _end < v ? v : _end;
    return RangePtr(new Range(start, end));
}

RangePtr Range::truncate_above(unsigned long v) const
{
    unsigned long start = _start > v ? v : _start;
    unsigned long end = _end > v ? v : _end;
    return RangePtr(new Range(start, end));
}

RangePtr Range::merge(const Range &r) const
{
    if (_end == r._start) {
	return RangePtr(new Range(_start, r._end));
    }
    if (_start == r._end) {
	return RangePtr(new Range(r._start, _end));
    }
    if (overlaps(r)) {
	unsigned long start = _start < r._start ? _start : r._start;
	unsigned long end = _end > r._end ? _end : r._end;
	return RangePtr(new Range(start, end));
    }
    else {
	return RangePtr((Range *) 0);
    }
}

string Range::to_string() const
{
    stringstream sstr;
    sstr << hex << "(" << _start << "," << _end << ")";
    return sstr.str();
}


list<RangePtr> Range::merge_list(const list<RangePtr> &arg)
{
    list<RangePtr> l(arg); // mutable copy of the list
    l.sort();
    list<RangePtr> result;

    RangePtr current;
    while (!l.empty()) {
	if (current) {
	    // Attempt to merge the front
	    RangePtr merged = current->merge(*(l.front()));
	    if (merged) {
		// worked
		current = merged;
		l.pop_front();
	    }
	    else {
		// didn't work
		result.push_back(current);
		current.reset((Range *) 0);
	    }
	}
	else {
	    current = l.front();
	    l.pop_front();
	}
    }

    if (current) {
	result.push_back(current);
    }
    return result;
}


list<RangePtr> Range::invert_list(const list<RangePtr> &arg)
{
    list<RangePtr> l = restrict(arg);
    list<RangePtr> result;

    unsigned long val = _start;
    while (!l.empty()) {
	unsigned long front_start = l.front()->_start;
	if (val < front_start) {
	    result.push_back(RangePtr(new Range(val, front_start)));
	}
	val = l.front()->_end;
	l.pop_front();
    }
    if (val < _end) {
	result.push_back(RangePtr(new Range(val, _end)));
    }
    return result;
}

list<RangePtr> Range::restrict(const list<RangePtr> &arg)
{
    list<RangePtr> result;
    list<RangePtr> l = merge_list(arg);
    list<RangePtr>::iterator it;
    for (it = l.begin(); it != l.end(); ++it) {
	RangePtr subr = (*it)->truncate_below(_start);
	if (subr && subr->size() > 0) {
	    subr = subr->truncate_above(_end);
	}
	if (subr && subr->size() > 0) {
	    result.push_back(subr);
	}
    }

    return result;
}

bool Range::any_overlap(const list<RangePtr> &arg)
{
    list<RangePtr> l = arg;

    if (l.size() < 2) { return false; }

    // Run a pairwise check for overlaps on a sorted copy of the list
    
    l.sort();
    RangePtr last = l.front();
    l.pop_front();
    
    while (!l.empty()) {
	if (l.front()->overlaps(*last)) {
	    return true;
	}
	last = l.front();
	l.pop_front();
    }

    return false;
}

ostream &operator<<(ostream &os, const Range &r)
{
    r.print(os);
    return os;
}
ostream &operator<<(ostream &os, const RangePtr &r)
{
    r->print(os);
    return os;
}
