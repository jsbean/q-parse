//
//  ComboWeight.cpp
//  qparse
//
//  Created by Florent Jacquemard on 04/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//
// OBSOLETE


#include "ComboWeight.hpp"



//default
double ComboWeight::_alpha = 0.5;


ComboWeight::ComboWeight(const Weight* const w, const Distance* const d)
{
    _val = (1 - _alpha) * w->value() + _alpha * d->value();
}


ComboWeight::ComboWeight(const Weight* const w)
{
    _val = (1 - _alpha) * w->value();
}


void ComboWeight::add(const ComboWeight& w)
{
    _val += w._val;
}


void ComboWeight::mult(const ComboWeight& w)
{
    _val *= w._val;
}


