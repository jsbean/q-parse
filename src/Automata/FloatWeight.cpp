//
//  FloatWeight.cpp
//  qparse
//
//  Created by Florent Jacquemard on 04/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include "FloatWeight.hpp"


void FloatWeight::add(const FloatWeight& w)
{
    if(_val > w.value())
        _val = w.value();
}


void FloatWeight::mult(const FloatWeight& w)
{
    _val += w.value();
}
