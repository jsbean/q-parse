//
//  Kbest.hpp
//  qparse
//
//  Created by Florent Jacquemard on 21/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//
// k-best parsing algorithm, following
// "Better k-best parsing", Huang & Chiang 2005
//

#ifndef Kbest_hpp
#define Kbest_hpp

#include <stdio.h>
#include <queue>
#include <vector>
#include <map>

#include "WTA.hpp"


#endif /* Kbest_hpp */


struct bpointer
{
    State bp_state;
    size_t bp_rank;
    
    bpointer(State s, size_t k=1):bp_state(s),bp_rank(k) {}
};


class Run
{
    
public:
    // empty run
    Run();
    
    // construct the 1-best run for the given transition
    // with a zero (unknown) weight.
    // for transition (s1,...,sn), the 1-best is ([s1,1],...,[sn,1])
    Run(const Transition*);

    // copy 
    Run(const Run&);
    
    // the run was constructed with Run()
    bool empty() const;
    
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

    vector<bpointer> _children;
    
private:
    
};


// comparison class
// one ordering for k-best to select the min weight run
struct WeightMin
{
    bool operator()(const Run& lhs, const Run& rhs) const
    {
        return lhs.weight.value() > rhs.weight.value();
    }
};

// one ordering for k-best to select the max weight run
// where 0 considered the highest weight value
// (0 is the weight value for initialization, computed weight is always > 0)
struct WeightMax
{
    bool operator()(const Run& lhs, const Run& rhs) const
    {
        if (lhs.weight.null())
        {
            return false;
            // if rhs == 0 then lhs == rhs
            // if rhs > 0 then lhs > rhs;
        }
        else
        {
            if (rhs.weight.null()) return true;
            else return lhs.weight.value() < rhs.weight.value();
        }
    }
};





template <class Comp_t> class Ktable;

// comp is one of the above orderings
template <class Comp_t> class Krecord
{
public:
    
    // should not be called
    // we want _cand empty iff no more run can be constructedg
    Krecord() { assert(false); }
    
    // fill the candidate list with the 1-best candidate for each transition
    // for transition (s1,...,sn), the 1-best is ([s1,1],...,[sn,1])
    Krecord(const TransitionList&, Ktable<Comp_t>*);
    
    ~Krecord();
    
    // best(k)
    // fill the table of best runs up to (at most) k
    // and returns the kth best in the table.
    // if less than k best can be constructed (table is complete),
    // return an empty run of weight 0.
    Run best(size_t);
    
private:
    
    std::priority_queue<Run, vector<Run>, Comp_t> _cand;

    std::vector<Run> _best;

    // it is empty iff no more k-best can be added
    Ktable<Comp_t>* _parent;
    
    // bool _complete; not needed

    // eval(r) compute the weight of r emplace
    // the weight of r must be 0
    // the weight is left to 0 if new weight cannot be computed
    // (one k-best missing)
    void eval(Run&);
    
    // addNext(r) add candidates following r (lexico order for ranks)
    // to the table of candidates
    void addNext(Run& run);
    

};


template <class Comp_t> class Ktable
{
public:
    
    Ktable (const WTA* ta):_wta(ta) { assert(ta); };
    
    // best(s, k)
    // returns the kth best for state s.
    // return an empty run of weight 0
    // if less than k best can be constructed.
    Run best(State, size_t);

private:
    const WTA* _wta;
    
    map<State, Krecord<Comp_t>> _table;
};


