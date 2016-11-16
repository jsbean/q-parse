//
//  RT.cpp
//  qparse
//
//  Created by Florent Jacquemard on 16/11/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include "RT.hpp"


RhythmTree::~RhythmTree()
{
    // recursive destruction
    for (vector<RhythmTree*>::iterator i = _children.begin();
         i != _children.end(); ++i)
    {
            delete *i;
    }
    _children.clear(); //destroy the content
}


size_t RhythmTree::label() const
{
    if (inner())
    {
        return (_children.size());
    }
    else
    {
        return _label;
    }
}

void RhythmTree::add(RhythmTree* t)
{
    assert(t);
    _children.push_back(t);
}


string RhythmTree::to_string()
{
    if (terminal())
    {
        size_t a = this->label();
        switch (a)
        {
            case 0:
                return "o";
            case 1:
                return "n";
            default:
                return (::to_string(label())+"n");
        }
    }
    else
    {
        size_t a = label();
        assert (a > 1);
        std::string res = ::to_string(a);
        res += "(";
        for (size_t i = 0; i < a; i++)
        {
            res += _children[i]->to_string();
            if (i < a-1) { res += ", "; }
        }
        res += ")";
        return res;
    }
}


