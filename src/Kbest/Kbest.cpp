//
//  Kbest.cpp
//  qparse
//
//  Created by Florent Jacquemard on 21/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include "Kbest.hpp"

//bpointer mk_bpointer(State s)
//{
//    return bpointer(s);
//}

Run::Run():
top(NULL),
weight(Weight()) // null weight
{
    assert (_children.empty());
}


// initialization of children from iterators to transition:
// uses constructor of bpointer from State
Run::Run(const Transition* t):
top(t),
_children(vector<bpointer>(t->begin(), t->end())),
weight(Weight())     // weight zero = unknown weight
{
    assert(t);

    // children.resize(t->size());
    // mk 1-best for state *i
    // std::transform(t->begin(), t->end(), children.begin(), mk_bpointer);
    // equivalent to
    // for (Transition_const_iterator i = t->begin(); i != t->end(); ++i)
    //     children.push_back(bpointer(*i));
}


// = default?
Run::Run(const Run& r):
top(r.top),
_children(r._children),
weight(r.weight)
{
}


bool Run::empty() const
{
    return (_children.empty());
}


size_t Run::arity()
{
    return (_children.size());
}


bpointer Run::at(size_t i) const
{
    assert (i < _children.size());
    return (_children[i]);
}



template <class Comp_t> Krecord<Comp_t>::Krecord(const TransitionList& tl,
                                                 Ktable<Comp_t>* kt):
_parent(kt)
{
    assert(kt);
    assert (tl.empty());
    for (TransitionList_const_iterator i = tl.begin(); i != tl.end(); ++i)
    {
        _cand.push(Run(&(*i)));
    }
}

template <class Comp_t> Krecord<Comp_t>::~Krecord()
{
    // no clear() for priority_queues
    _cand = std::priority_queue<Run, vector<Run>, Comp_t>();
    _best.clear();
}

template <class Comp_t> void Krecord<Comp_t>::eval(Run& r)
{
    assert (r.weight.null());
    r.weight = Weight(r.top->weight());
    
    assert (r.arity() == r.top->size());
    for (int i = 0; i < r.arity(); i++)
    {
        Run& rsk = _parent->best(r.getState(i), r.getRank(i)); // best(s, k)

        // the k-best for s does not exists (less than k runs for s)
        if ( rsk.weight.null())
        {
            r.weight = Weight(); // reset weight to 0
            return;
        }
        else r.weight += rsk.weight;
    }
}


template <class Comp_t> void Krecord<Comp_t>::addNext(Run& r)
{
    assert (r.arity() == r.top->size());
    // add next candidates
    for (int i = 0; i < r.arity(); i++)
    {
        assert (r.getState(i) == r.top->at(i));
        // new run
        Run nextr = Run(r);
        
        //nextr._children[i].bp_rank = r.at(i).bp_rank + 1;
        nextr.setRank(i, r.getRank(i) + 1);
        nextr.weight = Weight();
        _cand.push(nextr);
    }
}


template <class Comp_t> Run Krecord<Comp_t>::best(size_t k)
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
    _cand.pop();

    // one candidate not evaluated
    if (r.weight.null())
    {
        eval(r);
        if (! r.weight.null())
        {
            _cand.push(r); // push back with weight evaluated
        }
        // if the evaluated weight is null, it means than it cannot be evaluated
        // we do not push it back to cand
        // we do not push next because they can neither be evaluated
        best(k); //terminal recursive call to try to evaluate other cand
    }
    // all candidates have been evaluated (because weight 0 has priority)
    else
    {
        addNext(r);
        _best.push_back(r); // add to best table
        best(k); // recursive call in case there is more than one best to construct
    }
    
}




template <class Comp_t> Run Ktable<Comp_t>::best(State s, size_t k)
{
    typename std::map<State, Krecord<Comp_t>>::const_iterator i = _table.find(s);
    
    if (i == _table.end()) // s not found
    {
        // create an entry for s in the table
        assert (_wta);

        // assume that s is registered in _wta
        const TransitionList& tl = _wta->at(s);
        std::pair<typename std::map<State, Krecord<Comp_t>>::iterator, bool> ret;
        Krecord<Comp_t>& kr(tl, this);
        ret = _table.insert(std::pair<State, Krecord<Comp_t>>(s, kr));
        assert (ret.second); // false if s already in _table
        // and call best on this entry
        kr.best(k);
    }
    else // s found
    {
        return (i->second).best(k);
    }
    
}




