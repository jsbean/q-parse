//
//  Label.cpp
//  qparse
//
//  Created by Florent Jacquemard on 04/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include <assert.h>

#include "Label.hpp"



Label::Label(int a): _ar(a), _event(NULL), _gn(0)
{
    assert (a >= 0);
    if(a > 0)
        _type = INNER;
    else
    {
        _type = TIE;
    }
};

Label::Label(Event* e, int g): _ar(0), _type(EVENT), _gn(g)
{
    assert (e != NULL);
    assert (g >=0);
//    assert ((g == 0) || (k == NOTE));
}


int Label::arity() const
{
    assert(_ar >= 0);
    assert(_ar <= MAX_AR);
    return _ar;
}


int Label::nbGraceNotes() const
{
    assert(isLeaf());
    return _gn;
}


bool Label::isRest() const
{
    if (_type == EVENT)
    {
        assert (_event != NULL);
        return (_event->kind() == REST);
    }
    else
        return false;
}

bool Label::isNote() const
{
    if (_type == EVENT)
    {
        assert (_event != NULL);
        return (_event->kind() == REST);
    }
    else
        return false;
}

bool Label::isTie() const
{
    assert (_ar >= 0);
    return ((_ar == 0) && (_type == TIE));
}


void Label::addGraceNotes(int g)
{
    assert (_type == EVENT);
    assert (g >=0);
    _gn += g;
}


