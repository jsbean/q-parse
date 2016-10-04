//
//  Weight.cpp
//  qparse
//
//  Created by Florent Jacquemard on 04/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include "Weight.hpp"



// generic compare
// AV: specific compare's not based on value?
int Weight::compare(const Weight& w)
{
    if(_val < w.value())
        return -1;
    else if(_val == w.value())
        return 0;
    else return 1;
}


// default to min
void Weight::add(const Weight& w)
{
    if(_val > w.value())
        _val = w.value();
};


// default to plus
void Weight::mult(const Weight& w)
{
    _val += w.value();
}
