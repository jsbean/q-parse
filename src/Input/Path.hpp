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

#endif /* Path_hpp */

// a path defines an interval of positions in [0..RES]
// where RES is the resolution associated to the given schema

class Path
{
public:
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
    
    
private:
    
    // position of left bound of interval (in samples)
    int _begin;
    
    // length of interval (in samples)
    int _len;
    

    // _dur : inverse of duration defined by path

};
