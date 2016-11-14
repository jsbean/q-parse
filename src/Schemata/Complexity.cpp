//
//  Complexity.cpp
//  qparse
//
//  Created by Florent Jacquemard on 13/11/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include "Complexity.hpp"
#include "Weight.hpp"


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



