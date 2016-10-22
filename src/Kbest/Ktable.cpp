//
//  Ktable.cpp
//  qparse
//
//  Created by Florent Jacquemard on 21/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include "Ktable.hpp"

Run::Run(const Transition* t)
{
    assert(t);
    top = t;
    weight = Weight(); // weight zero = unknown weight
}

