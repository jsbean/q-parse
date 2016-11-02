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
#include <assert.h>

#include "ComboWTA.hpp"
#include "DurationList.hpp"

#endif /* Run_hpp */




struct bpointer
{
    State bp_state;
    size_t bp_rank;
    
    bpointer(State s, size_t k=1):bp_state(s),bp_rank(k) {}
};



// a run is created from a transition,
// it stores the weith of the parent transition (parent weight),
// a list of children represented by bpointers.
// a current evaluated weight,
// a list of relative durations.
//
// a run can be of 3 kinds:
// - null run:
//   null parent weight, null current weight, no children, empty duration list.
// - terminal (leaf) run, created from terminal (length 1) parent transition:
//   parent weight = current weight > 0, no children, singleton duration list.
// - inner run, crated from inner (length > 1) parent transition:
//   parent weight > 0,
//   current weight == 0 (unknown) or current weight > 0 (evaluated)
//   # children = length parent transition
//   duration list == empty (unkown) or not (evaluated).
class Run
{
    
public:
    // construct a null run (special)
    Run();
    
    // construct
    // - a terminal run if t is terminal
    // - an unknown inner run if t is inner,
    //   with childrens list containing bptr to the 1-best runs for the given transition
    //   for transition (s1,...,sn), the 1-best is ([s1,1],...,[sn,1]).
    Run(const Transition& t);
    
    // copy: USELESS (= default, assert redundant)
    Run(const Run&);
    
    // null run - constructed with Run()
    inline bool null() const { return (tweight.null() &&
                                       weight.null() &&
                                       _children.empty()); }
    
    // terminal (leaf) run
    inline bool terminal() const { return ((! tweight.null()) &&
                                           (tweight == weight) &&
                                           _children.empty()); }

    // inner run
    inline bool inner() const { return ((! tweight.null()) &&
                                        (_children.size() > 1)); }


    // inner run with unevaluated weight
    inline bool unknown() const { return weight.null(); }
    
    // return the number of children of this Run
    inline size_t arity() { return (_children.size()); }

    
    // getState(i) returns the state component of the ith children
    inline State getState(size_t i) const { assert (i < _children.size());
                                            return (_children[i].bp_state); }
    
    // getRank(i) returns the rank component of the ith children
    inline size_t getRank(size_t i) const { assert (i < _children.size());
                                            return (_children[i].bp_rank); }
    
    // set(i, k) sets the rank of the ith children of this run to k
    inline void setRank(size_t i, size_t k) { assert (i < _children.size());
                                              _children[i].bp_rank = k; }

    // reset weight and duration to 0 (make run unknown)
    void reset();
    
    // weight of the parent (creator) transition
    Weight tweight;
    
    // current weight
    Weight weight;
    
    // to replace the criteria weight == 0?
    //bool evaluated;
    
    // list of relative durations
    DurationList duration;
    
private:
    vector<bpointer> _children;
    
    // at(i) returns the ith children of this run
    bpointer at(size_t) const;
    
};




// an iRun extend a Run with
// - the target state
// - a rank (k-best number)
class iRun: public Run
{
public:
    // null iRun,
    // = null Run
    // + target state = 0
    // + rank = 0
    iRun();
    
    iRun(const Run&, State, size_t);

    State head;
    size_t rank;

};


