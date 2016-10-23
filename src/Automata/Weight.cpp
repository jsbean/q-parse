//
//  Weight.cpp
//  qparse
//
//  Created by Florent Jacquemard on 04/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include "Weight.hpp"


std::ostream& operator<<(std::ostream& o, const Weight& w)
{
    o << w.value();
    return o;
}


// default to min
//void Weight::add(const Weight& w)
//{
//    if(_val > w._val)
//        _val = w._val;
//};


// default to plus
//void Weight::mult(const Weight& w)
//{
//    _val += w._val;
//}

