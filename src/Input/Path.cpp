//
//  Path.cpp
//  qparse
//
//  Created by Florent Jacquemard on 05/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include "Path.hpp"


Path::Path(unsigned int b, unsigned int l):_begin(b), _len(l)
{
    assert (b >= 0);
    assert (l > 0);
}

unsigned int Path::begin() const
{
    return _begin;
}

unsigned int Path::end() const
{
    assert(_len > 0);
    return _begin + _len - 1;
}

unsigned int Path::length() const
{
    return _len;
}

bool Path::member(unsigned int p)
{
    return ((_begin <= p) && (p < _begin+_len));
}

bool Path::aligned(unsigned int p)
{
    double mid = _begin + (_len / 2);
    return ((_begin <= p) && (p <= mid));
}

Path* Path::sub(unsigned int n, unsigned int i)
{
    assert (n > 0);
    assert (i > 0);
    // the interval length must be divisible by n
    assert ((_len % n) == 0);
    int len = _len / n;
    
    return new Path((_begin + ((i-1)* len)), len);
}





unsigned int Alignment::align(unsigned int b)
{
    size_t m = _seg.size(); // cast size_t (unsigned long) into unsigned int
    int mid = _begin + (_len / 2);
    int end = _begin + _len;
    int c = 0;
    unsigned int i;

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


Alignment::Alignment(const Segment& s, unsigned int b, unsigned int l):Path(b,l),_seg(s)
{
    _res = s.resolution();
    assert (b+l <= _res);
}


Alignment::~Alignment()
{
    // the passed segment is not deleted
}


//int Alignment::date(size_t i) const
//{
//    assert(i >= 0);
//    assert(i < _seg->size());
//    return _seg->date(i);
//}


vector<Alignment*> Alignment::subs(unsigned int n) const
{
    assert (n > 1);
    assert ((_len % n) == 0); // this interval length must be divisible by n
    assert(_begin >= 0);
    assert(_begin < _res);
    assert(_begin+_len <= _res);


    vector<Alignment*> v;
    int len = _len / n;
    int b = _begin;
    unsigned int j = _seg_lbeg;
    
    for (unsigned int i=0; i < n; i++)
    {
        Alignment* p = new Alignment(_seg, b, len);
        v.push_back(p);
        j = p->align(j); // align the newly created sub-Alignment
        b += len;
    }
    
    assert (v.size() == n);
    return v;
}





AlignmentTree::AlignmentTree()
{
    root = new Alignment();
}


AlignmentTree::AlignmentTree(const Segment& s)
{
    root = new Alignment(s);
}


AlignmentTree::~AlignmentTree()
{
    // recursive destruction
    for (map<int, vector<AlignmentTree*>>::iterator i = _children.begin();
         i != _children.end(); ++i)
    {
        vector<AlignmentTree*> v = i->second;
        for (vector<AlignmentTree*>::iterator j = v.begin();
             j != v.end(); ++j)
            delete *j;
    }
    _children.clear(); //destroy the content
    delete root;
}


vector<AlignmentTree*> AlignmentTree::children(int a)
{
    assert (a > 1);
    // create an empty vector if it is not found
    vector<AlignmentTree*> tv = _children[a];
    
    if (tv.size() == 0)
    {
        assert (a > 1);
        assert ((root->_len % a) == 0); // this interval length must be divisible by n
        assert(root->_begin >= 0);
        assert(root->_begin < root->_res);
        assert(root->_begin + root->_len <= root->_res);
        
        int len = root->_len / a;
        int b = root->_begin;
        unsigned int j = root->_seg_lbeg;
        
        for (unsigned int i = 0; i < a; i++)
        {
            // create sub-Alignment starting at b
            Alignment* p = new Alignment(root->_seg, b, len);
            // create child ATree and insets it at the end of tv
            //tv.emplace(tv.end(), p);
            AlignmentTree* t = new AlignmentTree(p);
            tv.push_back(t);
            j = p->align(j); // align the newly created sub-Alignment
            b += len;
        }
        
    }
    assert (tv.size() == a);
    
    return tv;
}


