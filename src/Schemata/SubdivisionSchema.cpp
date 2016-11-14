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
    assert (dst_source != j);
    assert (dst_target != j);
    if (dst_source == i) { dst_source = j; }
    if (dst_target == i) { dst_target = j; }
    assert (dst_source != dst_target);
}


void ds_transition::shift(unsigned int n)
{
    dst_source += n;
    dst_target += n;
}


void ds_transition::shift0(unsigned int n)
{
    if (dst_source > 0) { dst_source += n; }
    if (dst_target > 0) { dst_target += n; }
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
            unsigned int m = next.max();
            next.shift(_max_state); // the source of next is shifted to _max_state
            // union of tables into this _table
            _table.insert(_table.end(), next._table.begin(), next._table.end());
            _max_state += m;
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
        assert (_max_state > 0);
        i++;
        for(; i !=  on.children.end(); ++i)
        {
            dagSchema next = dagSchema(*i);
            unsigned int m = next.max();
            assert (m > 0);
            if ((_max_state >1) || (m > 1))
            {
                next.shift0(_max_state);
                assert (next._max_state = _max_state + m);
                rename(_max_state, _max_state+m);
                _max_state += m;
            }
            // union of tables
            _table.insert(_table.end(), next._table.begin(), next._table.end());
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

void dagSchema::shift0(unsigned int n)
{
    assert (_max_state > 0);
    for (int s = 0; s < _table.size(); s++)
    {
        _table[s].shift0(n); // modif en place
    }
    _max_state += n;
}

