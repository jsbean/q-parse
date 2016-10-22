//
//  Ktable.hpp
//  qparse
//
//  Created by Florent Jacquemard on 21/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#ifndef Ktable_hpp
#define Ktable_hpp

#include <stdio.h>

#include "WTA.hpp"


#endif /* Ktable_hpp */


typedef struct
{
    State bp_state;
    size_t bp_rank;
} bpointer;


class Run
{

public:
    Run(const Transition*);
    
    const Transition* top;
    
    vector<bpointer> children;
    
    Weight weight;
};


class Candidates
{
public:
    Candidates();
    
    
private:
//    map<State,
    
    
};
