//
//  Run.cpp
//  qparse
//
//  Created by Florent Jacquemard on 24/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include "Run.hpp"

//bpointer mk_bpointer(State s)
//{
//    return bpointer(s);
//}


Run::Run():
tweight(Weight()), // null parent weight
weight(Weight())   // null current weight
{
    assert (this->null());
}


// initialization of children from iterators to transition:
// uses constructor of bpointer from State
Run::Run(const Transition& t):
tweight(t.weight())
{
    // leaf transition
    if (t.terminal())
    {
        weight = t.weight();
        // children is empty vector
        assert(_children.size() == 0);
        assert(this->terminal());
    }
    // inner transition: fill _children with pointers to 1-bests
    else if (t.inner())
    {
        // children.resize(t->size());
        // mk 1-best for state *i
        // std::transform(t->begin(), t->end(), children.begin(), mk_bpointer);
        // equivalent to
        // for (Transition_const_iterator i = t->begin(); i != t->end(); ++i)
        //     children.push_back(bpointer(*i));
        _children = vector<bpointer>(t.begin(), t.end());
        weight = Weight(); // weight zero = unknown weight
        assert(_children.size() == t.size());
        assert(this->inner());
    }
    else { assert(false); } //should not happen
}


// = default
Run::Run(const Run& r):
tweight(r.tweight),
weight(r.weight),
_children(r._children)
{
    assert (r.null() || r.terminal() || r.inner());
    assert (this->null() || this->terminal() || this->inner());
}



bpointer Run::at(size_t i) const
{
    assert (i < _children.size());
    return (_children[i]);
}



iRun::iRun(const Run& r, State s, size_t k):
Run(r),
head(s),
rank(k)
{ assert (k > 0); }




