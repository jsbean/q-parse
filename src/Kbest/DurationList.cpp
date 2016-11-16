//
//  DurationList.cpp
//  qparse
//
//  Created by Florent Jacquemard on 02/11/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include "DurationList.hpp"

// empty duration list
DurationList::DurationList():
_continuation(Rational(0)),
_main()
{
    assert (this->empty());
}

DurationList::DurationList(const DurationList& l):
_continuation(l._continuation),
_main(l._main)
{ }

DurationList::DurationList(const DurationList& l, Rational q):
_main()
{
    _continuation = l._continuation * q;
    for(list<Rational>::const_iterator i = l._main.begin(); i != l._main.end(); ++i)
    {
        _main.push_back((*i)*q);
    }
}


DurationList::DurationList(string filename)
{
    ifstream file;
    
    file.open(filename, ios_base::in);
    if(!file.is_open())
        throw "cannot open file";
    
    bool first = true;
    
    for(string line; getline(file, line); )
    {
        Rational d;
        
        // skip empty line
        if (line.size() == 0) continue;
        
        istringstream in(line);   //make a stream from the line
        if (!(in >> d)) continue; // parse error: skip line

        if (first && (d < 0))
        {
            _continuation = d;
            first = false;
        }
        else
        {
            assert (d >= 0);
            add(d);
        }
    }
    file.close();
}


bool DurationList::empty() const
{
    return ( _continuation.null() && _main.empty());
}

bool DurationList::single_continuation() const
{
    return ( (! _continuation.null()) && _main.empty());
}

bool DurationList::single_event() const
{
    return ( _continuation.null() && (_main.size() == 1));
}

bool DurationList::event() const
{
    if ( _continuation.null() && (_main.size() >= 1))
    {
        list<Rational>::const_reverse_iterator i = _main.crbegin();
        assert (i != _main.crend());
        assert (*i >= 0);
        // sequence of grace notes followed by non null event
        if ( *i == 0 ) return false;
        while (i != _main.crend())
        {
            assert (*i >= 0);
            if ( *i > 0 ) return false;
            ++i;
        }
        return true;
    }
    else
    {
        return false;
    }
}

size_t DurationList::nbgn() const
{
    assert ( _continuation.null());
    assert (_main.size() >= 1);
    list<Rational>::const_reverse_iterator i = _main.crbegin();
    assert (i != _main.crend());
    size_t res = 0;
    assert (*i > 0); // last element is the event
    // previouses are the grace notes
    while (i != _main.crend())
    {
        assert (*i == 0);
        ++res;
        ++i;
    }
    return res;        
}

Rational DurationList::length() const
{
    Rational res = _continuation;
    for(list<Rational>::const_iterator i = _main.cbegin(); i != _main.cend(); ++i)
    {
        res += *i;
    }
    return res;
}


void DurationList::add(Rational q)
{
    _main.push_back(q);
}

void DurationList::addcont(Rational q)
{
    _continuation += q;
}

void DurationList::mult(Rational q)
{
    _continuation *= q;
    // multiply every element of _main in place
    for(Rational &x : _main) { x *= q; }
}

std::ostream& operator<<(std::ostream& o, const DurationList& l)
{
    o << '[';
    assert(l._continuation >= 0);
    if (l._continuation > 0)
    {
        o << " " << -(l._continuation);
    }
    for(list<Rational>::const_iterator i = l._main.cbegin();
        i != l._main.cend();
        ++i)
    {
        assert (*i >= 0);
        o << " " << *i;
    }
    o << " ]";
    return o;
}








DurationTree::~DurationTree()
{
    // recursive destruction
    for (map<size_t, vector<DurationTree*>>::iterator i = _children.begin();
         i != _children.end(); ++i)
    {
        vector<DurationTree*> v = i->second;
        for (vector<DurationTree*>::iterator j = v.begin();
             j != v.end(); ++j)
            delete *j;
    }
    _children.clear(); //destroy the content
    // the passed segment is not deleted
}

DurationTree::DurationTree(const DurationList& d):top(d)
{
    _length = d.length();
}

DurationTree* DurationTree::sub(size_t a, size_t n)
{
    assert (a > 1);
    assert (n < a);
    assert (! top.empty());
    assert (! top.single_continuation());
    assert (! top.single_event());
    assert (! top.event());

    // creates an empty vector if there is none associated to a
    vector<DurationTree*>& v = _children[a];
    
    if (v.size() == 0) // newly created, fill it
    {
        // length of each part of the division in a parts
        Rational len = _length/Rational(a);
        // copy
        DurationList rest(top);
        
        while (! rest.empty())
        {
            // we construct duration subtree by hand
            DurationTree* dt = new DurationTree();
            dt->top = DurationList();
            dt->_length = len;
            assert (dt->top._main.empty());
            assert (dt->_children.empty());

            if (rest._continuation >= len)
            {
                dt->top._continuation = len;
                rest._continuation -= len;
            }
            else
            {
                dt->top._continuation = rest._continuation;
                Rational accu = rest._continuation;
                assert (! rest._main.empty());
                Rational next;
                for (next = rest._main.front();
                     accu + next < len;
                     next = rest._main.front())
                {
                    accu += next;
                    dt->top.add(next);
                    rest._main.pop_front();
                    assert (! rest._main.empty());
                }
                if (accu + next == len)
                {
                    dt->top.add(next);
                    rest._continuation = Rational(0);
                    rest._main.pop_front();
                }
                else // accu + next > len
                {
                    assert (accu < len);
                    // the next is split in 2 parts
                    // part 1 in current sub
                    dt->top.add(len - accu);
                    // part 2 as continuation in next sub
                    rest._continuation = accu + next - len;
                    rest._main.pop_front();
                }
            }
            assert (dt->top.length() == dt->_length);
            v.push_back(dt);
        }
    }
    assert (v.size() == a);
    
    return v[n];
}




