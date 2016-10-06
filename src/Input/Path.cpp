//
//  Path.cpp
//  qparse
//
//  Created by Florent Jacquemard on 05/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include "Path.hpp"



int Path::begin()
{
    return _begin;
}

int Path::end()
{
    assert(_len > 0);
    return _begin + _len;
}

int Path::length()
{
    return _len;
}

bool Path::member(int p)
{
    return ((_begin <= p) && (p < _begin+_len));
}

bool Path::aligned(int p)
{
    double mid = _begin + (_len / 2);
    return ((_begin <= p) && (p <= mid));
}

Path* Path::sub(int n, int i)
{
    assert (n > 0);
    assert (i > 0);
    // the interval length must be divisible by n
    assert ((_len % n) == 0);
    int len = _len / n;
    
    return new Path((_begin + ((i-1)* len)), len);
}

