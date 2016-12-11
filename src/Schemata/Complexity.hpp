//
//  Complexity.hpp
//  qparse
//
//  Created by Florent Jacquemard on 13/11/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//
// Generic weight values for construction of schemas
//
// xp = coeff penalty for arity p
//
// ordering defined in Kant paper
// x2 < x4 < x3 < x6 < x8 < x5 < x7 < x9 < x12 < x10 < x11  < x13
//
// constraints
//
// x4  = 3 x2
// x6  = x2 + 2 x3
// x6  = x3 + 3 x2
// x8  = 7 x2
// x8  = x2 + 2 x4
// x8  = x4 + 4 x2
// x9  = 4 x3
// x10 = x2 + 2 x5
// x10 = x5 + 5 x2
// x12 = 3 x2 + 4 x3
// x12 = x3 + 9 x2
// x12 = x3 + 6 x2 + x4
// x12 = x3 + 3 x2 + 2 x4
// x12 = x3 + 3 x4
// x12 = x4 + 4 x3
// x12 = x2 + 2 x6
// x14 = x2 + 2 x7
// x14 = x7 + 7 x2
// x15 = x3 + 3 x5
// x15 = x5 + 5 x3
//
// solutions
//
// x2
// x4  = 3 x2
// x3  = 2 x2        (eqs for x6)
// x6  = 5 x2
// x8  = 7 x2
// x5  = 4 x2        (eqs for x10)
// x5  = 2 x3 = 4 x2 (eqs for x15)
// x7  = 6 x2        (eqs for x14)
// x9  = 8 x2
// x12 = 11 x2
// x10 = 9 x2
// x11
// x13
// x14 = 13 x2
// x15 = 14 x2


#ifndef Complexity_hpp
#define Complexity_hpp

#include <stdio.h>
#include <assert.h>
#include <math.h> 


class Weight;

class Complexity
{
public:
    // penalty for an inner node
    static Weight penalty_arity[18];
    
    // penalty for a tie
    static Weight penalty_tie;

    // penalty for given number of grace notes in a leaf
    // 0 = 1 event, no grace note
    // 1 = 1 event, 1 grace note
    // 2 = 1 event, 2 grace notes etc
    static Weight gracenote(size_t);
    
};

class Penalty
{
public:
    // penalty for an inner node
    static Weight inner(size_t arity);

    // penalty for a tie
    static Weight tie();

    // penalty for given number of grace notes in a leaf
    // 0 = 1 event, no grace note
    // 1 = 1 event, 1 grace note
    // 2 = 1 event, 2 grace notes etc
    static Weight gracenote(size_t);      
};


#endif /* Complexity_hpp */
