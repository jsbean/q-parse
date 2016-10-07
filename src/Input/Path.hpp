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
#include <vector>
#include <assert.h>

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



class PathInput: public Path
{
public:

    // PathInput(r, s) = path associated to a input segment s and the interval [0..r]
    PathInput(int, const Segment* const);
    
    // PathInput(r, s, b, l) = path associated to a input segment s, the interval [b..b+l].
    PathInput(int r, const Segment* const, int, int);

    // sub(k) return the list of subpaths obtained by
    // division of this Path into k segments
    // the interval length must be divisible by n
    vector<PathInput*> subs(int);
    
private:

    // resolution
    int _res;
    
    const Segment* const _seg;
    
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
