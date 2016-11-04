//
//  Path.cpp
//  qparse
//
//  Created by Florent Jacquemard on 05/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include "Path.hpp"


Path::Path(size_t b, size_t l):_begin(b), _len(l)
{
    assert (b >= 0);
    assert (l > 0);
}

Path::Path(const Path& p, size_t n, size_t i)
{
    assert (n > 0);
    assert (i > 0);
    // the interval length must be divisible by n
    assert ((p._len % n) == 0);
    _len = p._len / n;
    _begin = p._begin + ((i-1)*_len);
}


size_t Path::begin() const
{
    return _begin;
}

size_t Path::end() const
{
    assert(_len > 0);
    return _begin + _len;
}

size_t Path::length() const
{
    return _len;
}

bool Path::member(size_t p)
{
    return ((_begin <= p) && (p < _begin+_len));
}

bool Path::aligned(size_t p)
{
    double mid = _begin + (_len / 2);
    return ((_begin <= p) && (p <= mid));
}


size_t Alignment::align(size_t b)
{
    size_t m = _seg.size(); // cast size_t (unsigned long) into size_t
    double mid = double(_begin) + (double(_len) / 2);
    size_t end = _begin + _len;
    int c = 0;
    size_t i;

    // pre-cond: the first point must be after the left bound of the interval
    assert ((b >= m) || _seg.date(b) >= _begin);

    // count # segment points from b on in the first half of Alignment
    for(i = b; (i < m) && (_seg.date(i) < mid); i++)
        c++;

    _seg_llen = c;
    if (_seg_llen == 0) // no segment points in first half of interval
        _seg_lbeg = m;  // out_of_range (should not be used)
    else
        _seg_lbeg = b;  // _begin <= date(b) < mid

    if (i >= m)
    {
        _seg_rlen = 0;
        _seg_rbeg = m;
    }
    else
    {
        _seg_rbeg = i;
        // count # segment points from i on in the second half of Alignment
        for(c=0; (i < m) && (_seg.date(i) < end); i++)
            c++;

        _seg_rlen = c;
        if (_seg_rlen == 0) // no segment points in second half of interval
            _seg_rbeg = m;
    }

    // post-conds:
    assert (_seg_llen >= 0);
    assert (_seg_llen < m);
    assert (_seg_lbeg >= 0);
    
    assert ((_seg_llen >  0) || (_seg_lbeg >= m));
    assert ((_seg_llen == 0) || (_seg_lbeg < m));
    assert ((_seg_llen == 0) || (_seg_lbeg+_seg_llen-1 < m));
    assert ((_seg_llen == 0) || (_seg.date(_seg_lbeg) >= _begin));
    assert ((_seg_llen == 0) || (_seg.date(_seg_lbeg) < mid));
    assert ((_seg_llen == 0) || (_seg.date(_seg_lbeg+_seg_llen-1) >= _begin));
    assert ((_seg_llen == 0) || (_seg.date(_seg_lbeg+_seg_llen-1) < mid));
    
    assert (_seg_rlen >= 0);
    assert (_seg_rlen < m);
    assert (_seg_rbeg >= 0);

    assert ((_seg_rlen >  0) || (_seg_rbeg >= m));
    assert ((_seg_rlen == 0) || (_seg_rbeg < m));
    assert ((_seg_rlen == 0) || (_seg_rbeg+_seg_rlen-1 < m));
    assert ((_seg_rlen == 0) || (_seg.date(_seg_rbeg) >= mid));
    assert ((_seg_rlen == 0) || (_seg.date(_seg_rbeg) < end));
    assert ((_seg_rlen == 0) || (_seg.date(_seg_rbeg+_seg_rlen-1) >= mid));
    assert ((_seg_rlen == 0) || (_seg.date(_seg_rbeg+_seg_rlen-1) < end));

    // i (next point) out of interval
    assert ((i >= m) || (_seg.date(i) >= end));
    
    return i;
}


Alignment::Alignment():_seg_llen(0),_seg_lbeg(0),_seg_rlen(0),_seg_rbeg(0)
{
    _seg = *(new Segment());
}


Alignment::Alignment(const Segment& s) :_seg(s)
{
    _res = s.resolution();
    _begin = 0;
    _len = _res;
    align(0);
}


Alignment::Alignment(const Segment& s, size_t b, size_t l):Path(b,l),_seg(s)
{
    _res = s.resolution();
    assert (b+l <= _res);
}


Alignment::~Alignment()
{
    // recursive destruction
    for (map<size_t, vector<Alignment*>>::iterator i = _children.begin();
         i != _children.end(); ++i)
    {
        vector<Alignment*> v = i->second;
        for (vector<Alignment*>::iterator j = v.begin();
             j != v.end(); ++j)
            delete *j;
    }
    _children.clear(); //destroy the content
    // the passed segment is not deleted
}


//int Alignment::date(size_t i) const
//{
//    assert(i >= 0);
//    assert(i < _seg->size());
//    return _seg->date(i);
//}


Alignment* Alignment::sub(size_t a, size_t n)
{
    assert (a > 1);
    assert (n < a);

    // creates an empty vector if it is not found
    vector<Alignment*> v = _children[a];
    
    if (v.size() == 0) // newly created, fill it
    {
        assert ((_len % a) == 0); // this interval length must be divisible by n
        assert(_begin >= 0);
        assert(_begin < _res);
        assert(_begin + _len <= _res);
        
        size_t len = _len / a;
        size_t b = _begin;
        size_t j = _seg_lbeg;
        
        for (int i = 0; i < a; i++)
        {
            // create sub-Alignment starting at b
            Alignment* p = new Alignment(_seg, b, len);
            // create child ATree and insets it at the end of tv
            //tv.emplace(tv.end(), p);
            v.push_back(p);
            j = p->align(j); // align the newly created sub-Alignment
            b += len;
        }
    }
    assert (v.size() == a);
    
    return v[n];
}




