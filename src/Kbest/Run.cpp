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
    assert(r.top);
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



iRun::iRun(const Run& r, State s, size_t k):
Run(r),
head(s),
rank(k)
{ assert (k > 0); }




