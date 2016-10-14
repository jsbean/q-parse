//
//  Distance.cpp
//  qparse
//
//  Created by Florent Jacquemard on 04/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include "Distance.hpp"


Distance::Distance(const Alignment* const p)
{
    assert (p != NULL);
    
    const Segment* s = p->input();
    assert (s != NULL);
//    const size_t m = p->input()->size();
    const int b = p->begin();
    const int e = p->end();
    const int ll = p->l_size();
    const int rl = p->r_size();
    
    _val = 0;

    if (ll > 0)
    {
        size_t j = p->l_first();
        for(int i = 0; i < ll; i++)
        {
            assert (s->date(j) >= b);
            assert (s->date(j) < e);
            _val += (s->date(j++) - b);
        }
    }

    if (rl > 0)
    {
        size_t j = p->r_first();
        for(int i = 0; i < rl; i++)
        {
            assert (s->date(j) >= b);
            assert (s->date(j) < e);
            _val += (e - s->date(j++));
        }
    }
    
    //normalization
    const int r = p->input()->resolution();
    const size_t d = p->input()->size();

    _val *= (2 / (d*r)); // TBC
}


void Distance::add(const Distance& d)
{
    _val += d._val;
}


void Distance::mult(const Distance& d)
{
    _val *= d._val;
}
