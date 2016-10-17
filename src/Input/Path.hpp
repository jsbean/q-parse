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
    
    Path(int b, int l): _begin(b), _len(l) { assert (b >= 0); assert (l > 0); }
    

    // return the left bound of the interval defined by this Path (in samples)
    int begin() const;

    // return the right bound of the interval defined by this Path (in samples)
    // WARNING: the right bound is the last position of the interval +1
    // i.e. it does not belong to the interval
    int end() const;

    // return the length of the interval defined by this Path (in samples)
    int length() const;

    // the position in param belongs to the interval defined by this Path
    bool member(int);

    // the position in param belongs to the interval defined by this Path
    // and is closer to the left bound than to the right bound
    bool aligned(int);
    
    // sub(n, i) return a new path defined as the ith subsegment
    // of a division of this Path into n segments
    // the interval length must be divisible by n
    Path* sub(int, int);
    
protected:
    
    // position of left bound of interval (in samples)
    int _begin;
    
    // length of interval (in samples)
    int _len;
    
    // _dur : inverse of duration defined by path
};




// a Alignment defines the alignement of the points of an input segment
// on a Path (interval)
class Alignment: public Path
{
public:
    Alignment():_seg_llen(0),_seg_lbeg(0),_seg_rlen(0),_seg_rbeg(0) { _seg = new Segment(); }
    
    // Alignment(s) = alignement of the input segment s to the interval [0..r]
    // (initial (toplevel) path).
    Alignment(Segment*);
    
    // Alignment(s, b, l) = alignement of the interval [b..b+l] subset [0,..,r]
    // to the input segment s.
    // construction of recursive paths.
    // WARNING: must be aligned.
    Alignment(Segment*, int, int);
    
    ~Alignment();
    
    // input segment
    Segment* input() const { return _seg; }
    
    // date in stored Segment
    //int date(size_t) const;
    
    // number of points of segment in the fist half of this interval
    int l_size() const { return _seg_llen; }
    
    // index in segment of the first element of segment inside the fist half of this interval
    // out_of_range (= size of segment) id lsize() == 0
    size_t l_first() const { return _seg_lbeg; }

    // number of points of segment in the second half of this interval
    int r_size() const { return _seg_rlen; }
    
    // index in segment of the first element of segment inside the second half of this interval
    // out_of_range (= size of segment) id rsize() == 0
    size_t r_first() const { return _seg_rbeg; }
    
    // sub(k) return the list of subpaths obtained by
    // division of this Path into k segments.
    // the interval length must be divisible by k.
    vector<Alignment*> subs(int);
    
private:

    // resolution
    int _res;  // SUPPR (deja dans segment)
    
    Segment* _seg;
    
    // number of segment points in the first half of this path
    int _seg_llen;

    // index of first segment point in the first half of this path
    // or out_of_bound (segment size) is there are none
    size_t _seg_lbeg;
    
    // number of segment points in the second half of this path
    int _seg_rlen;

    // index of first segment point in the second half of this path
    // or out_of_bound (segment size) is there are none
    size_t _seg_rbeg;
    
    
    // align(b) set the above values, starting from index b
    // and return the next index in segment to be processed
    // or the size of segment if end of segment is reached.
    size_t align(size_t);
    
};
