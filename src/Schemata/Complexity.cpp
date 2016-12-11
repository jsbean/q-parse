//
//  Complexity.cpp
//  qparse
//
//  Created by Florent Jacquemard on 13/11/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include "Complexity.hpp"
#include "Weight.hpp"



Weight Complexity::penalty_arity[18] =
{
    Weight(0.01),  // x2
    Weight(0.02),  // x3
    Weight(0.03),  // x4
    Weight(0.04),  // x5
    Weight(0.05),  // x6
    Weight(0.06),  // x7
    Weight(0.07),  // x8
    Weight(0.08),  // x9
    Weight(0.09),  // x10
    Weight(0.10),  // x11
    Weight(0.11),  // x12
    Weight(0.12),  // x13
    Weight(0.13),  // x14
    Weight(0.14),  // x15
    Weight(0.15),  // x16
    Weight(0.16),  // x17
    Weight(0.17),  // x18
    Weight(0.18)   // x19
};


Weight Penalty::inner(size_t arity)
{
    assert (arity > 1);

    if (arity < 10)
    {
        switch (arity)
        {
        case 2:
            return Weight(0.05);
        case 4:
            return Weight(0.10);
        case 3:
            return Weight(0.15);
        case 6:
            return Weight(0.20);
        case 8:
            return Weight(0.25);
        case 5:
            return Weight(0.35);
        case 7:
            return Weight(0.40);
        case 9:
            return Weight(0.45);
        default:
            assert (false);
        }
    }
    else
    {
        return Weight(0.05 + (0.05 * arity));
    }
}


Weight Penalty::tie()
{
    return Weight(0.10);
}

Weight Penalty::gracenote(size_t n)
{
    return Weight(0.05 * exp(0.9 * n));
}



