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
    {
        _type = INNER;
        _pitch = 0;
    }
    else
    {
        _type = NOTE;
        _pitch = 6000;
    }
    _gn = 0;
};

Label::Label(EventKind k, int g): _ar(0), _type(k), _gn(g)
{
    assert ((k == NOTE) || (k == REST) || (k == TIE));
    assert (g >=0);
    assert ((g == 0) || (k == NOTE));
    if (k == NOTE)
        _pitch = 6000;
    else
        _pitch = 0;
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
    assert (_ar >= 0);
    return ((_ar == 0) && (_type == REST));
}

bool Label::isNote() const
{
    assert (_ar >= 0);
    return ((_ar == 0) && (_type == NOTE));
}

bool Label::isTie() const
{
    assert (_ar >= 0);
    return ((_ar == 0) && (_type == TIE));
}


void Label::addGraceNotes(int g)
{
    assert (_type == NOTE);
    assert (g >=0);
    _gn += g;
}


