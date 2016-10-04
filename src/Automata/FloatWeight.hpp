//
//  FloatWeight.hpp
//  qparse
//
//  Created by Florent Jacquemard on 04/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#ifndef FloatWeight_hpp
#define FloatWeight_hpp

#include <stdio.h>

#include "Weight.hpp"


#endif /* FloatWeight_hpp */


class FloatWeight : public Weight
{
public:
    FloatWeight(double v) { _val = v; };

    void add(const FloatWeight&);
    
    void mult(const FloatWeight&);    
};
