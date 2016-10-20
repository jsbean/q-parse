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

#include "segment.hpp"


#endif /* Path_hpp */


// a path defines an interval of positions in [0..RES]
// where RES is the resolution associated to the given schema
class Path
{
public:
    Path():_begin(0),_len(0){}
    
    Path(unsigned int, unsigned int);
    

    // return the left bound of the interval defined by this Path (in samples)
    unsigned int begin() const;

    // return the right bound of the interval defined by this Path (in samples)
    // WARNING: the right bound is the last position of the interval +1
    // i.e. it does not belong to the interval
    unsigned int end() const;

    // return the length of the interval defined by this Path (in samples)
    unsigned int length() const;

    // the position given belongs to the interval defined by this Path
    bool member(unsigned int);

    // the position given belongs to the interval defined by this Path
    // and is closer to the left bound than to the right bound
    bool aligned(unsigned int);
    
    // sub(n, i) return a new path defined as the ith subsegment
    // of a division of this Path into n segments
    // the interval length must be divisible by n
    // UNUSED? TODO REM.
    Path* sub(unsigned int, unsigned int);
    
protected:
    
    // position of left bound of interval (in samples)
    unsigned int _begin;
    
    // length of interval (in samples)
    unsigned int _len;
    
    // _dur : inverse of duration defined by path
};




// a Alignment defines the alignement of the points of an input segment
// on a Path (interval)
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
    unsigned int l_size() const { return _seg_llen; }
    
    // index in segment of the first element of segment inside the fist half of this interval
    // out_of_range (= size of segment) id lsize() == 0
    unsigned int l_first() const { return _seg_lbeg; }

    // number of points of segment in the second half of this interval
    unsigned int r_size() const { return _seg_rlen; }
    
    // index in segment of the first element of segment inside the second half of this interval
    // out_of_range (= size of segment) id rsize() == 0
    unsigned int r_first() const { return _seg_rbeg; }
    
    bool habited() const { return ((_seg_llen + _seg_rlen) > 0); }
    
    // sub(k) return the list of subpaths obtained by
    // division of this Path into k segments.
    // k must be > 1.
    // the interval length must be divisible by k.
    vector<Alignment*> subs(unsigned int);
    
private:

    // resolution
    unsigned int _res;  // SUPPR (deja dans segment)
    
    Segment _seg;
    
    // number of segment points in the first half of this path
    unsigned int _seg_llen;

    // index of first segment point in the first half of this path
    // or out_of_bound (segment size) is there are none
    unsigned int _seg_lbeg;
    
    // number of segment points in the second half of this path
    unsigned int _seg_rlen;

    // index of first segment point in the second half of this path
    // or out_of_bound (segment size) is there are none
    unsigned int _seg_rbeg;
    
    
    // align(b) set the above values, starting from index b
    // and return the next index in segment to be processed
    // or the size of segment if end of segment is reached.
    unsigned int align(unsigned int);
    
    // Alignment(s, b, l) = alignement of the interval [b..b+l] subset [0,..,r]
    // to the input segment s.
    // WARNING: must be aligned afterwards.
    // use only internaly construction of recursive paths.
    Alignment(const Segment&, unsigned int, unsigned int);
    
};
