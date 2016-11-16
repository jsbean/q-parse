//
//  Label.cpp
//  qparse
//
//  Created by Florent Jacquemard on 04/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include <assert.h>

#include "Label.hpp"



// static function
//size_t Label::nbGraceNotes(size_t);
size_t Label::nbGraceNotes(size_t n)
{
    if (n > 0)
        return n - 1;
    else return 0;  // 1 event or 0 event (continuation)
}

// static function
//bool Label::continuation(size_t);
bool Label::continuation(size_t n)
{
    return (n == 0);
}



Label::Label(int a): _ar(a)
{
    assert (a >= 0);
    if(a > 0)
        _type = INNER;
    else
        _type = TIE;
};

size_t Label::arity() const
{
    assert(_ar >= 0);
    assert(_ar <= MAX_AR);
    return _ar;
}



InnerLabel::InnerLabel(int a)
{
    _type = INNER;
    _ar = a;
}


EventLabel::EventLabel():_nbe(0)
{
    _type = TIE;
    _ar = 0;
}

EventLabel::EventLabel(int n):_nbe(n)
{
    _ar = 0;
    if (n == 0)
        _type = TIE;
    else
        _type = EVENT;
}

size_t EventLabel::nbGraceNotes() const
{
    assert(isLeaf());
    return (Label::nbGraceNotes(_nbe));
}

void EventLabel::addGraceNotes(size_t g)
{
    assert (_type == EVENT);
    assert (g >=0);
    _nbe += g;
}

void EventLabel::pushEvent(Event* e)
{
    assert(e);
    assert(_events.size() < _nbe);
    _events.push_back(e);
}



