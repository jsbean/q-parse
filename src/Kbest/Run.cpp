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
    assert (this->null());
}


// initialization of children from iterators to transition:
// uses constructor of bpointer from State
Run::Run(const Transition& t):
top(&t)
{
    // leaf transition
    if (t.terminal())
    {
        weight = t.weight();
        // children is empty vector
        assert(this->terminal());
        assert(_children.size() == 0);
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
        assert(this->inner());
        assert(_children.size() == t.size());
        weight = Weight(); // weight zero = unknown weight
    }
    else { assert(false); } //should not happen
}


// = default?
Run::Run(const Run& r):
top(r.top),
_children(r._children),
weight(r.weight)
{
    assert (r.null() || r.terminal() || r.inner());
//    assert(r.top || (r.weight.null()));
//    assert((r.top == NULL) ||
//           ((((_children.size() == 0) && top->size() == 1)) ||
//           (_children.size() == top->size())));
    assert (this->null() || this->terminal() || this->inner());
}


bool Run::null() const
{
    return ((top == NULL) && _children.empty() && weight.null());
}


bool Run::terminal() const
{
    return ((top != NULL) &&
            (top->size() == 1) &&
            (! weight.null()) &&
            _children.empty());
}

bool Run::inner() const
{
    return ((top != NULL) &&
            (top->size() > 1) &&
            (_children.size() == top->size()));
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




