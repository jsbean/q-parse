//
//  Run.hpp
//  qparse
//
//  Created by Florent Jacquemard on 24/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#ifndef Run_hpp
#define Run_hpp

#include <stdio.h>

#include "ComboWTA.hpp"

#endif /* Run_hpp */




struct bpointer
{
    State bp_state;
    size_t bp_rank;
    
    bpointer(State s, size_t k=1):bp_state(s),bp_rank(k) {}
};

// a run is associated to a parent transition (top),
// it has a weight, and a list of children represented by bpointers.
//
// a run can be of 3 kinds:
// - null run:
//   null parent transition, weight = 0, no children
// - terminal (leaf) run:
//   terminal (length 1) parent transition, weight > 0, no children
// - inner run:
//   inner (length > 1) parent transition,  weight == 0 (unknown) or
//   weight > 0 (evaluated), # children = length parent transition
class Run
{
    
public:
    // null run (special)
    Run();
    
    // construct a null run (special) if t = NULL or
    // construct the 1-best run for the given transition
    // with a zero (unknown) weight.
    // for transition (s1,...,sn), the 1-best is ([s1,1],...,[sn,1])
    Run(const Transition& t);
    
    // copy
    Run(const Run&);
    
    // null run - constructed with Run()
    bool null() const;
    
    // terminal (leaf) run
    bool terminal() const;

    // inner run
    bool inner() const;

    // inner run with unknown weight
    bool unknown() const;
    
    // return the number of children of this Run
    size_t arity();
    
    // at(i) returns the ith children of this run
    bpointer at(size_t) const;
    
    // getState(i) returns the state component of the ith children
    inline State getState(size_t i) const
    {
        assert (i < _children.size());
        return (_children[i].bp_state);
    }
    
    // getRank(i) returns the rank component of the ith children
    inline size_t getRank(size_t i) const
    {
        assert (i < _children.size());
        return (_children[i].bp_rank);
    }
    
    // set(i, k) sets the rank of the ith children of this run to k
    inline void setRank(size_t i, size_t k)
    {
        assert (i < _children.size());
        _children[i].bp_rank = k;
    }
    
    const Transition* top;
    
    Weight weight;

    
private:
    vector<bpointer> _children;

};




// an iRun extend a Run with
// - the target state
// - a rank (k-best number)
class iRun: public Run
{
public:
    iRun(const Run&, State, size_t);

    State head;
    size_t rank;

};


