//
//  Distance.cpp
//  qparse
//
//  Created by Florent Jacquemard on 04/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include "Distance.hpp"


//default
double Distance::_alpha = CST_ALPHA;


Distance::Distance(Alignment* p)
{
    const Segment& s = p->input();
//    const size_t m = p.input()->size();
    const size_t b = p->begin();
    const size_t e = p->end();
    const size_t ll = p->l_size();
    const size_t rl = p->r_size();
    
    _val = 0;

    if (ll > 0)
    {
        size_t j = p->l_first();
        assert (j < s.size());
        for(int i = 0; i < ll; i++)
        {
            assert (s.date(j) >= b);
            assert (s.date(j) < e);
            _val += (s.date(j++) - b);
        }
    }

    if (rl > 0)
    {
        size_t j = p->r_first();
        assert (j < s.size());
        for(int i = 0; i < rl; i++)
        {
            assert (s.date(j) >= b);
            assert (s.date(j) <= e);
            _val += (e - s.date(j++));
        }
    }
    
    //normalization
    const size_t r = p->input().resolution();
    const size_t d = p->input().size();

    _val *= (2 / (d*r)); // TBC
}


Distance::Distance(const Weight& w)
{
    _val = (1 - _alpha) * w.value();
}


//void Distance::add(const Distance& d)
//{
//    _val += d._val;
//}
//
//
//void Distance::mult(const Distance& d)
//{
//    _val *= d._val;
//}


void Distance::combine(const Weight& w)
{
    _val = (1 - _alpha) * w.value() + _alpha * _val;
}


