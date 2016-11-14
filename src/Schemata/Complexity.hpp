//
//  Complexity.hpp
//  qparse
//
//  Created by Florent Jacquemard on 13/11/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//
// Generic weight values for construction of schemas

#ifndef Complexity_hpp
#define Complexity_hpp

#include <stdio.h>
#include <assert.h>
#include <math.h> 


#endif /* Complexity_hpp */

class Weight;

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
