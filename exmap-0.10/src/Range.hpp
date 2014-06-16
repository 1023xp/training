/*
 * (c) John Berthels 2005 <jjberthels@gmail.com>. See COPYING for license.
 */
#ifndef _RANGE_H
#define _RANGE_H

#include <string>
#include <list>

#include <boost/shared_ptr.hpp>

class Range;
typedef boost::shared_ptr<Range> RangePtr;
/// Handle manipulation of half-open (start <= x < end) ranges
class Range
{
public:
    Range(unsigned long start, unsigned long end);
    /// The range start
    unsigned long start() const;
    /// The range end
    unsigned long end() const;
    /// The range length (note that (x, x) has a size of zero)
    unsigned long size() const;
    
    /// Return a newly allocated Range which is the intersection
    /// (null if intersection is empty)
    RangePtr intersect(const Range &r) const;
    /// Same, but takes a RangePtr
    RangePtr intersect(const RangePtr &r) const;
    /// True if start and end are equal
    bool operator==(const Range &r) const;
    bool operator!=(const Range &r) const { return !(*this == r); }
    /// Compare by start only
    bool operator<(const Range &other) const;
    
    /// Return new range shifted up by v
    RangePtr add(unsigned long v) const;
    /// Return new range shifted down by v
    RangePtr subtract(unsigned long v) const;
    /// True if the range contains v (remember ranges don't contain the end)
    bool contains(unsigned long v) const;
    /// True if the range contains both start and end of r
    bool contains(const Range &r) const;
    /// Same, but takes a RangePtr
    bool contains(const RangePtr &r) const;
    /// True if the intersection with r is non-empty
    bool overlaps(const Range &r) const;
    /// Same, but takes a RangePtr
    bool overlaps(const RangePtr &r) const;
    /// Returns a 
    RangePtr truncate_below(unsigned long v) const;
    RangePtr truncate_above(unsigned long v) const;
    RangePtr merge(const Range &r) const;
    std::string to_string() const;
    void print(std::ostream &os) const;

    std::list<RangePtr> invert_list(const std::list<RangePtr> &l);

    std::list<RangePtr> restrict(const std::list<RangePtr> &l);
    
    static std::list<RangePtr> merge_list(const std::list<RangePtr> &l);

    /// True if any of the ranges overlap
    static bool any_overlap(const std::list<RangePtr> &l);
    
private:
    unsigned long _start;
    unsigned long _end;
};

extern std::ostream &operator<<(std::ostream &os, const Range &r);
extern std::ostream &operator<<(std::ostream &os, const RangePtr &r);

// This probably deserves some explanation. boost::shared_ptr provides
// a template for operator< which is based on the number of reference
// counts of the shared pointers. This isn't really useful. The
// following template specialisation (naughtily placed into the boost
// namespace) overrides this with a method which proxies the
// comparison to the pointed-to object. This means that (amongst other
// things) you can .sort() a list of boost::shared_ptrs by their
// underlying class.
namespace boost {
    
template<> inline
bool operator< <Range, Range>(boost::shared_ptr<Range> const &a,
			      boost::shared_ptr<Range> const &b)
{
    return *a < *b;
}

};


#endif
