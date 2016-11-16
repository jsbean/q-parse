//
//  Path.hpp
//  qparse
//
//  Created by Florent Jacquemard on 05/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#ifndef Path_hpp
#define Path_hpp

#include <stdio.h>
#include <assert.h>
#include <vector>
#include <map>

#include "segment.hpp"


#endif /* Path_hpp */


// a path defines an interval of positions in [0..RES]
// where RES is the resolution associated to the given schema
class Path
{
public:
    Path():_begin(0),_len(0){}
    
    // Path(b, l) begin an interval starting at b (samples) and of length l.
    Path(size_t, size_t);
    
    
    // Path(p, n, i) construct a new interval defined as the ith subsegment
    // of a division of interval p into n segments
    // the interval length must be divisible by n
    // UNUSED? TODO REM.
    Path(const Path&, size_t, size_t);
    

    // return the left bound of the interval defined by this Path (in samples)
    size_t begin() const;

    // return the right bound of the interval defined by this Path (in samples)
    // WARNING: the right bound is the last position of the interval +1
    // i.e. it does not belong to the interval
    size_t end() const;

    // return the length of the interval defined by this Path (in samples)
    size_t length() const;

    // the position given belongs to the interval defined by this Path
    bool member(size_t);

    // the position given belongs to the interval defined by this Path
    // and is closer to the left bound than to the right bound
    bool aligned(size_t);
    
    
protected:
    
    // position of left bound of interval (in samples)
    size_t _begin;
    
    // length of interval (in samples)
    size_t _len;
    
    // _dur : inverse of duration defined by path
};




// a Alignment defines the alignement on a Path (interval)
// of the points of an input segment
class Alignment: public Path
{
    
public:
    
    Alignment();
    
    // Alignment(s) = alignement of the input segment s to the interval [0..r]
    // (initial (toplevel) path).
    Alignment(const Segment&);
    
    ~Alignment();
    
    // input segment
    const Segment& input() const { return _seg; }
    
    // date in stored Segment
    //int date(size_t) const;
    
    // number of points of segment in the fist half of this interval
    size_t l_size() const { return _seg_llen; }
    
    // index in segment of the first element of segment inside the fist half of this interval
    // out_of_range (= size of segment) id lsize() == 0
    size_t l_first() const { return _seg_lbeg; }

    // number of points of segment in the second half of this interval
    size_t r_size() const { return _seg_rlen; }
    
    // index in segment of the first element of segment inside the second half of this interval
    // out_of_range (= size of segment) id rsize() == 0
    size_t r_first() const { return _seg_rbeg; }
    
    bool habited() const { return ((_seg_llen + _seg_rlen) > 0); }
    
    // sub(a, i) returns a alignement for the i-1th subpath (interval)
    // of the division of this alignement's interval in n equal parts.
    // a must be > 1
    // i must be smaller than a
    // the interval length must be divisible by a
    Alignment* sub(size_t, size_t);
    
private:

    // resolution
    size_t _res;  // SUPPR (deja dans segment)
    
    Segment _seg;
    
    // number of segment points in the first half of this path
    size_t _seg_llen;

    // index of first segment point in the first half of this path
    // or out_of_bound (segment size) is there are none
    size_t _seg_lbeg;
    
    // number of segment points in the second half of this path
    size_t _seg_rlen;

    // index of first segment point in the second half of this path
    // or out_of_bound (segment size) is there are none
    size_t _seg_rbeg;
    
    // every entry in this map associate to
    // an arity n a partition a1,...,an of the root Alignment
    // TODO change to    vector<vector<AlignmentTree*>> _children;
    map<size_t, vector<Alignment*>> _children;
   
    // align(b) set the above values, starting from index b
    // and return the next index in segment to be processed
    // or the size of segment if end of segment is reached.
    size_t align(size_t);
    
    // Alignment(s, b, l) = alignement of the interval [b..b+l] subset [0,..,r]
    // to the input segment s.
    // WARNING: must be aligned afterwards.
    // use only internaly construction of recursive paths.
    Alignment(const Segment&, size_t, size_t);
    
};


