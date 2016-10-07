//
//  Path.cpp
//  qparse
//
//  Created by Florent Jacquemard on 05/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include "Path.hpp"


int Path::begin() const
{
    return _begin;
}

int Path::end() const
{
    assert(_len > 0);
    return _begin + _len;
}

int Path::length() const
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


size_t PathInput::align(size_t b)
{
    size_t m = _seg->size();
    int mid = _len / 2;
    int end = this->end();
    int c = 0;
    size_t i;

    // pre-cond: the first point must be after the left bound of the interval
    assert ((b >= m) || _seg->date(b) >= _begin);

    for(i = b; (i < m) && (_seg->date(i) <= mid); i++)
        c++;

    _seg_llen = c;
    if (_seg_llen == 0) // no segment points in first half of interval
        _seg_lbeg = m;  // out_of_range (should not be used)
    else
        _seg_lbeg = b;  // _begin <= b <= mid

    if (i >= m)
    {
        _seg_rlen = 0;
        _seg_rbeg = m;
    }
    else
    {
        _seg_rbeg = i;
        for(c=0; (i < m) && (_seg->date(i) < end); i++)
            c++;

        _seg_rlen = c;
        if (_seg_rlen == 0) // no segment points in second half of interval
            _seg_rbeg = m;
    }

    // post-conds:
    assert ((_seg_llen > 0)  || (_seg_lbeg >= m));
    assert ((_seg_llen == 0)  || (_seg_lbeg < m));
    assert ((_seg_llen == 0)  || (_seg_lbeg+_seg_llen-1 < m));
    assert ((_seg_llen == 0) || ((_seg->date(_seg_lbeg) >= _begin) &&
                                 (_seg->date(_seg_lbeg) <= mid)));
    assert ((_seg_llen == 0) || ((_seg->date(_seg_lbeg+_seg_llen-1) >= _begin) &&
                                (_seg->date(_seg_lbeg+_seg_llen-1) <= mid)));
    
    assert ((_seg_rlen > 0) || (_seg_rbeg >= m));
    assert ((_seg_rlen == 0)  || (_seg_rbeg < m));
    assert ((_seg_rlen == 0)  || (_seg_rbeg+_seg_rlen-1 < m));
    assert ((_seg_llen == 0) || ((_seg->date(_seg_rbeg) > mid) &&
                                 (_seg->date(_seg_lbeg) < end)));
    assert ((_seg_llen == 0) || ((_seg->date(_seg_rbeg+_seg_rlen-1) > mid) &&
                                 (_seg->date(_seg_lbeg+_seg_rlen-1) < end)));

    // i (next point) out of interval
    assert ((i >= m) || (_seg->date(i) >= end));
    
    return i;
}


PathInput::PathInput(int r, const Segment* const s) :_res(r),_seg(s)
{
    assert(s != NULL);
    assert (r >= 0);
    _begin = 0;
    _len = r;
    align(0);
}


PathInput::PathInput(int r, const Segment* const s, int b, int l) :_res(r),_seg(s)
{
    assert(s != NULL);
    assert (r >= 0);
    assert (b >= 0);
    assert (l > 0);
    
    _begin = b;
    _len = l;
}



vector<PathInput*> PathInput::subs(int n)
{
    assert (n > 0);
    // this interval length must be divisible by n
    assert ((_len % n) == 0);
    assert(_begin >= 0);
    assert(_begin < _res);

    vector<PathInput*> v;
    int len = _len / n;
    int b = _begin;
    size_t j = _seg_lbeg;
    
    for (size_t i=0; i < n; i++)
    {
        PathInput* p = new PathInput(_res, _seg, b, len);
        v.push_back(p);
        b += len;
        j = p->align(j);
    }
    
    assert (v.size() == n);
    return v;
}




//vector<PathInput*> PathInput::subs(int k)
//{
//}
