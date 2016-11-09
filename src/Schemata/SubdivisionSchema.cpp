//
//  SubdivisionSchema.cpp
//  qparse
//
//  Created by Florent Jacquemard on 07/11/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include "SubdivisionSchema.hpp"



void ds_transition::rename(unsigned int i, unsigned int j)
{
    assert (dst_source != dst_target);
    if (dst_source == i) { dst_source = j; }
    if (dst_target == i) { dst_target = j; }
    assert (dst_source != dst_target);
}


void ds_transition::shift(unsigned int n)
{
    if (dst_source > 1) { dst_source += n; }
    if (dst_target > 1) { dst_target += n; }
}




dagSchema& dagSchema::operator+=(dagSchema& rhs)
{
    _table.insert(_table.end(), rhs._table.begin(), rhs._table.end());
    unsigned int tm = max();
    unsigned int rm = rhs.max();
    _max_state = (tm >= rm)?tm:rm;
    return *this; // return the result by reference
}


// collage par fusion source de this avec source de rhs
// et target de this avec target de rhs.
dagSchema& dagSchema::operator/=(dagSchema& rhs)
{
    rhs.shift(rhs.max());
    // union of tables
    _table.insert(_table.end(), rhs._table.begin(), rhs._table.end());
    return *this; // return the result by reference
}

// concatenation par fusion target de this avec source de rhs.
// le source est le source de this, la target est la target de rhs.
dagSchema& dagSchema::operator*=(dagSchema& rhs)
{
    unsigned int tm = max();
    unsigned int rm = rhs.max();
    unsigned int m = (tm >= rm)?tm:rm;
    untarget(m+1);
    rhs.unsource(m+1);
    rhs.shift(m);
    // union of tables
    _table.insert(_table.end(), rhs._table.begin(), rhs._table.end());
    _max_state =  rhs.max();
    return *this; // return the result by reference
}


dagSchema::dagSchema(const ANode& an)
{
    // single edge from source 0 to target 1
    if (an.leaf())
    {
        _table.push_back(ds_transition(0, an.value(), 1));
        _max_state = 1;
    }
    else
    {
        assert (an.children.size() > 0);
        std::vector<ONode>::const_iterator i = an.children.begin();
        dagSchema first = dagSchema(*i); // dagschema from first (ONode) children
        _table = first._table;
        _max_state = first._max_state;
        i++;
        for(; i !=  an.children.end(); ++i)
        {
            dagSchema next = dagSchema(*i);
            unsigned int m = (max() >= next.max())?max():next.max();
            next.shift(m);
            untarget(m+1);
            next.unsource(m+1);
            // union of tables
            _table.insert(_table.end(), next._table.begin(), next._table.end());
            _max_state =  next.max();
        }
    }
}


dagSchema::dagSchema(const ONode& on)
{
    // single edge from source 0 to target 1
    if (on.leaf())
    {
        _table.push_back(ds_transition(0, on.value(), 1));
        _max_state = 1;
    }
    else
    {
        assert (on.children.size() > 0);
        std::vector<ANode>::const_iterator i = on.children.begin();
        dagSchema first = dagSchema(*i); // dagschema from first (ONode) children
        _table = first._table;
        _max_state = first._max_state;
        i++;
        for(; i !=  on.children.end(); ++i)
        {
            dagSchema next = dagSchema(*i);
            unsigned int m = (max() >= next.max())?max():next.max();
            next.shift(m);
            // union of tables
            _table.insert(_table.end(), next._table.begin(), next._table.end());
            _max_state =  next.max();
        }
    }
}


void dagSchema::add(const ds_transition& dst)
{
    _table.push_back(dst);
}

void dagSchema::rename(unsigned int i, unsigned int j)
{
    for (int s = 0; s < _table.size(); s++)
    {
        _table[s].rename(i, j); // modif en place
    }
    if (j > _max_state) { _max_state = j; }
}


void dagSchema::shift(unsigned int n)
{
    for (int s = 0; s < _table.size(); s++)
    {
        _table[s].shift(n); // modif en place
    }
    _max_state += n;
}

