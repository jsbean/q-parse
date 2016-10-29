//
//  Label.cpp
//  qparse
//
//  Created by Florent Jacquemard on 04/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include <assert.h>

#include "Label.hpp"

Label::Label(int a): _ar(a)
{
    assert (a >= 0);
    if(a > 0)
        _type = INNER;
    else
        _type = TIE;
};

int Label::arity() const
{
    assert(_ar >= 0);
    assert(_ar <= MAX_AR);
    return _ar;
}


int Label::nbGraceNotes(unsigned int n)
{
    if (n > 0)
        return n - 1;
    else return 0;  // 1 or 0 event
}

bool Label::continuation(unsigned int n)
{
    return (n == 0);
}


//bool Label::isRest() const
//{
//    if (_type == EVENT)
//    {
//        assert (_event);
//        return (_event->kind() == REST);
//    }
//    else
//        return false;
//}
//
//bool Label::isNote() const
//{
//    if (_type == EVENT)
//    {
//        assert (_event != NULL);
//        return (_event->kind() == REST);
//    }
//    else
//        return false;
//}
//
//bool Label::isTie() const
//{
//    assert (_ar >= 0);
//    return ((_ar == 0) && (_type == TIE));
//}


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

int EventLabel::nbGraceNotes() const
{
    assert(isLeaf());
    return (Label::nbGraceNotes(_nbe));
}

void EventLabel::addGraceNotes(int g)
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



