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
// could not separate .h and .cpp files for these template classes.
// "any translation unit (C++ source file) that wants to use a template class has to have access to the entire template definition".
// csq: implementation in the header file rather, and no .cpp file at all.


#ifndef Kbest_hpp
#define Kbest_hpp

#include <stdio.h>
#include <queue>
#include <vector>
#include <map>

#include "Run.hpp"


#endif /* Kbest_hpp */



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




template <typename Comp_t> class Ktable;

// comp is one of the above orderings
template <typename Comp_t> class Krecord
{
public:
    
    // should not be called
    // we want _cand empty iff no more run can be constructedg
    Krecord() { assert(false); }
    
    // fill the candidate list with the 1-best candidate for each transition
    // for transition (s1,...,sn), the 1-best is ([s1,1],...,[sn,1])
    Krecord(const TransitionList& tl, Ktable<Comp_t>* kt):
    _parent(kt)
    {
        assert(kt);
        for (TransitionList_const_iterator i = tl.begin(); i != tl.end(); ++i)
        {
            // 1-best run associated to transition
            //_cand.push(Run(*i));
            _cand.emplace(*i);
        }
    }

    
    ~Krecord()
    {
        // no clear() for priority_queues
        _cand = std::priority_queue<Run, vector<Run>, Comp_t>();
        _best.clear();
    }

    
    // best(k)
    // fill the table of best runs up to (at most) k
    // and returns the kth best in the table.
    // if less than k best can be constructed (table is complete),
    // return an empty run of weight 0.
    Run best(size_t k)
    {
        assert (k > 0);
        // k-best run already computed
        if (_best.size() >= k)
            return _best[k-1];
        
        // otherwise, we construct the next best run
        
        // cannot (all runs constructed in best table)
        if (_cand.empty())
        {
            return Run(); // empty run
        }
        
        // otherwise, process the best candidate
        Run r = _cand.top();
        assert (r.inner() || r.terminal()); // no null run candidate
        _cand.pop();
        
        // one candidate's weight not evaluated
        if (r.unknown())
        {
            assert(r.inner());
            eval(r);
            // evaluation succeeded:
            // push back with weight evaluated
            if (! r.unknown()) { _cand.push(r); }
            // if the evaluated weight is null,
            // it means than it cannot be evaluated
            // we do not push it back to cand
            // we do not push next runs because they can neither be evaluated

            return best(k); //tail recursive call to try to evaluate other cand
        }
        // all candidates have been evaluated (because weight 0 has priority)
        else
        {
            // compute next candidate if run is not terminal (leaf)
            if (r.inner()) { addNext(r); }
            _best.push_back(r); // add to best table
            return best(k);     //tail recursive call
                                // in case there is more than one best to construct
        }
    }

    
private:
    
    // it is empty iff no more k-best can be added
    std::priority_queue<Run, vector<Run>, Comp_t> _cand;

    // ordered list of best runs for this state
    std::vector<Run> _best;

    Ktable<Comp_t>* _parent;
    
    // bool _complete; not needed

    // eval(r) compute the weight of r emplace
    // the weight of r must be 0
    // the weight is left to 0 if new weight cannot be computed
    // (one k-best missing)
    void eval(Run& r)
    {
        // null runs should not be evaluated
        // terminal runs are evaluated at creation
        assert (r.inner());
        assert (r.unknown());

        r.weight = r.tweight;
        size_t a = r.arity();
        assert (a > 1);
        
        // composed run (depth > 0)
        for (int i = 0; i < a; i++)
        {
            Run rsk = _parent->best(r.getState(i), r.getRank(i)); // best(s, k)
            assert (rsk.null() || rsk.inner() || rsk.terminal());
            // the k-best for s does not exists
            // because there are less than k runs for s
            if ( rsk.unknown())
            {
                r.resetWeight(); // reset weight to 0
                return;
            }
            else r.weight += rsk.weight;
        }
    }
    
    
    // addNext(r) add candidates following r (lexico order for ranks)
    // to the table of candidates
    void addNext(Run& r)
    {
        assert(r.inner());
        // add next candidates
        for (int i = 0; i < r.arity(); i++)
        {
            // new run (copy) set to unknown (0 weight)
            Run nextr = Run(r); //Run nextr(r);
            assert(nextr.inner());
            //nextr._children[i].bp_rank = r.at(i).bp_rank + 1;
            nextr.setRank(i, r.getRank(i) + 1);
            nextr.resetWeight();
            _cand.push(nextr);
        }
    }
};


template <typename Comp_t> class Ktable
{
public:
    
    Ktable(const WTA* ta):_wta(ta),_init(false) { assert(ta); };
    
    // best(s, k)
    // returns the kth best for state s.
    // return an empty run of weight 0 when less than k best can be constructed.
    Run best(State s, size_t k)
    {
        typename std::map<State, Krecord<Comp_t>>::iterator i = _table.find(s);
        
        if (i == _table.end()) // s not found
        {
            // create an entry for s in the table
            assert (_wta);
            // assume that s is registered in _wta
            const TransitionList& tl = _wta->at(s);
            Krecord<Comp_t> kr = Krecord<Comp_t>(tl, this);

            std::pair<typename std::map<State, Krecord<Comp_t>>::iterator, bool> ret;
            ret = _table.insert(std::pair<State, Krecord<Comp_t>>(s, kr));
            assert (ret.second); // false if s already in _table
            // and call best on this entry
            return kr.best(k);
        }
        else // s found
        {
            return (i->second).best(k);
        }
    }
    
    
    // best(k)
    // returns the kth best for an initial state.
    // return an empty run of weight 0 when less than k best can be constructed.
    Run best(size_t k)
    {
        assert (k > 0);
        
        // initialization
        if (! _init)
        {
            for (set<State>::iterator i = _wta->initials.begin();
                 i != _wta->initials.end(); ++i)
            {
                pushinit(*i, 1);
            }
           _init = true;
        }
        
        
        // k-best run already computed
        if (_ibest.size() >= k)
            return _ibest[k-1];  //return Run(_ibest[k-1]);
        // otherwise, we try to construct the next best run
        
        // cannot (all runs constructed in best table)
        if (_icand.empty())
        {
            return Run(); // empty run
        }
        
        // otherwise, get the best candidate at the top of heap
        iRun r = _icand.top();
        _icand.pop();
        assert(! r.weight.null());
        // add it to the best-init table
        _ibest.push_back(r);
        // try to compute and push the next best for the same state
        // not lazy. on demand computation need more variables...
        pushinit(r.head, r.rank+1);
        
        return best(k); //tail recursive call in case there is more than one best to construct
    }

private:
    const WTA* _wta;
    
    map<State, Krecord<Comp_t>> _table;
    
    // stuff to compute bests for initial states
    
    // heap of weighted runs targeted to initial states
    std::priority_queue<iRun, vector<iRun>, Comp_t> _icand;

    // ordered list of best runs for all initial states
    std::vector<iRun> _ibest;

    // flag for initialization of table for initial states
    bool _init;
    
    
    void pushinit(State s, size_t k)
    {
        Run r = best(s, k);
        if (! r.weight.null())
        {
            _icand.push(iRun(r, s, k));
        }
        // do not add to vector if weight is null
    }

};


