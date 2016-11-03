//
//  Segment.cpp
//  qparse
//
//  Created by Florent Jacquemard on 06/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//


#include <math.h> 

#include "Segment.hpp"



Segment::Segment(double b, double e, unsigned int r):_begin(b),_end(e),_res(r)
{
    assert(r >= 1);
    assert(_begin <= _end);
    _len = (e-b);
    //_event = new vector<Event*>();
    //_date = new vector<int>();
}


Segment::Segment(string filename, unsigned int r):_res(r)
{
    assert(r >= 1);

    ifstream file;
    
    file.open(filename, ios_base::in);
    if(!file.is_open())
        throw "cannot open file";
    
    vector<double> v;
    
    for(string line; getline(file, line); )
    {
        double d;

        // skip empty line
        if (line.size() == 0) continue;
        
        istringstream in(line);   //make a stream from the line
        if (!(in >> d)) continue; // parse error: skip line
        
        v.push_back(d);
    }
    file.close();

    assert (v.size() > 2);
    size_t last = v.size()-1;
    
    _begin = v[0];
    _end = v[last];
    assert(_begin < _end);
    _len = (_end - _begin);
    double prev = v[0];
    
    assert (_event.size() == 0);
    assert (_date.size() == 0);
    for (int i = 1; i < last; i++)
    {
        double d = v[i];
        assert((i > 1) || (d >= prev)); // the first point can be at segment beggining
        assert((i <= 1) || (d > prev)); // the next points must be distinct
        prev = d;
        TimestampedEvent* e = new TimestampedEvent(Event(), d);
        _event.push_back(e);
        _date.push_back(round(((d - _begin)/_len) * _res));
        if (TRACE_LEVEL > 0) { cout << "input[" << i-1 << "] = " << d << "  -> " << round(((d - _begin)/_len) * _res) << "\n"; }
    }
}



void Segment::push(TimestampedEvent* e)
{
    assert(_res > 0);
    assert (e != NULL);
    assert (e->timestamp() >= _begin);
    assert (e->timestamp() <= _end);
    _event.push_back(e);
    double date = ((e->timestamp() - _begin)/_len) * _res;
    _date.push_back(date);
}

size_t Segment::size() const
{
    return _date.size();
}

unsigned int Segment::date(size_t i) const
{
    assert(i < _date.size());
    return _date[i];
}


//int Segment::count(const Path& p)
//{
//    int pl = p.begin();
//    assert (pl >= 0);
//    int rl = p.end();
//    assert (rl <= _res);
//    size_t m = _date.size();
//    int c = 0;
//    int i = 0;
//    for(i=0; (i < m) && (_date[i] < pl); i++);
//    if (i >= m)
//        return c;
//    for(c=0; (i < m) && (_date[i] < rl); i++)
//        c++;
//    return c;
//}
