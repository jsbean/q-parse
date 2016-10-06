//
//  Segment.cpp
//  qparse
//
//  Created by Florent Jacquemard on 06/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include "Segment.hpp"



Segment::Segment(double b, double e, size_t r):_begin(b),_end(e),_res(r)
{
    assert(r >= 1);
    assert(_begin <= _end);
    _len = (e-b);
    //_event = new vector<Event*>();
    //_date = new vector<int>();
}

void Segment::push(Event* e)
{
    assert(_res > 0);
    assert (e != NULL);
    assert (e->timestamp() >= _begin);
    assert (e->timestamp() <= _end);
    _event.push_back(e);
    int date = ((e->timestamp() - _begin)/_len) * _res;
    _date.push_back(date);
}

int Segment::count(const Path& p)
{
    int pl = p.begin();
    assert (pl >= 0);
    int rl = p.end();
    assert (rl <= _res);
    size_t m = _date.size();
    int c = 0;
    int i = 0;
    for(i=0; (i < m) && (_date[i] < pl); i++);
    if (i >= m)
        return c;
    for(c=0; (i < m) && (_date[i] < rl); i++)
        c++;
    return c;
}

