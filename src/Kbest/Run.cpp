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
tweight(), // null parent weight
weight(),  // null current weight
duration() // empty current duration
{
    assert (this->null());
}


// initialization of children from iterators to transition:
// uses constructor of bpointer from State
Run::Run(const Transition& t):
tweight(t.weight()),
duration()
{
    // leaf transition
    if (t.terminal())
    {
        weight = t.weight();

        State label = t.label();
        if (Label::continuation(label))
        {
            duration.addcont(Rational(1)); // relative duration of leaf is 1
        }
        else // leaf contains 0 or several grace notes and one event
        {
            for (int i = 0; i < Label::nbGraceNotes(label); i++)
            {
                duration.add(Rational(0)); // every grace note has duration 0
            }
            duration.add(Rational(1)); // the event in leaf has relative duration 1
        }
        // children is singleton vector
        _children.push_back(bpointer(label, 0));
        assert(_children.size() == 1);
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
duration(r.duration),
_children(r._children)
{
    assert (r.null() || r.terminal() || r.inner());
    assert (this->null() || this->terminal() || this->inner());
}


size_t Run::label() const
{
    assert (this->terminal());    // 1 children
    assert (_children.front().bp_rank == 0);
    return _children.front().bp_state;
}


void Run::reset()
{
    weight = Weight();
    duration = DurationList();
}

size_t Run::arity()
{
    size_t a = _children.size();
    switch (a)
    {
        case 0:
        case 1:
            return 0;
            
        default:
            return a;
    }
}


bpointer Run::at(size_t i) const
{
    assert (i < _children.size());
    return (_children[i]);
}


iRun::iRun():
Run(),
head(0),
rank(0)
{}

iRun::iRun(const Run& r, State s, size_t k):
Run(r),
head(s),
rank(k)
{ assert (k > 0); }

