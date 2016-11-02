//
//  DurationList.cpp
//  qparse
//
//  Created by Florent Jacquemard on 02/11/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include "DurationList.hpp"

// empty duration list
DurationList::DurationList():
_continuation(Rational(0)),
_main()
{
    assert (this->empty());
}

DurationList::DurationList(const DurationList& l, Rational q):
_main()
{
    _continuation = l._continuation * q;
    for(list<Rational>::const_iterator i = l._main.begin(); i != l._main.end(); ++i)
    {
        _main.push_back((*i)*q);
    }
}

bool DurationList::empty() const
{
    return ( _continuation.null() && _main.empty());
}

void DurationList::add(Rational q)
{
    _main.push_back(q);
}

void DurationList::addcont(Rational q)
{
    _continuation += q;
}

void DurationList::mult(Rational q)
{
    _continuation *= q;
    // multiply every element of _main in place
    for(Rational &x : _main) { x *= q; }
}

std::ostream& operator<<(std::ostream& o, const DurationList& l)
{
    o << '[';
    assert(l._continuation >= 0);
    if (l._continuation > 0)
    {
        o << " " << -(l._continuation);
    }
    for(list<Rational>::const_iterator i = l._main.cbegin();
        i != l._main.cend();
        ++i)
    {
        assert (*i >= 0);
        o << " " << *i;
    }
    o << " ]";
    return o;
}

