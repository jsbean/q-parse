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
    assert (this->terminal());
    return _label;
}


size_t RhythmTree::arity() const
{
    assert (this->inner());
    return (_children.size());
}

void RhythmTree::add(RhythmTree* t)
{
    assert(t);
    _children.push_back(t);
}


string RhythmTree::to_string() const
{
    if (terminal())
    {
        size_t a = this->label();
        size_t g = Label::nbGraceNotes(a);

        if (Label::continuation(a)) // tie
        { return "o"; }
        else if (g == 0)            // single note
        { return "n"; }
        else                        // graces notes + note
        { return (::to_string(Label::nbGraceNotes(a))+"g+n"); }
//        switch (a)
//        {
//            case 0:
//                return "o";
//            case 1:
//                return "n";
//            default:
//                return (::to_string(Label::nbGraceNotes(a))+"g+n");
//        }
    }
    else // inner()
    {
        size_t a = this->arity();
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


bool RhythmTree::tie() const
{
    return ((this->terminal()) && (Label::continuation(this->label())));
}

bool RhythmTree::tied() const
{
    if (terminal())
    {
        return (Label::continuation(this->label()));
    }
    else // inner()
    {
        assert (_children.size() > 1);
        return (_children[0]->tied());
    }
}

bool RhythmTree::binary() const
{
    return ((this->inner()) && (this->arity() == 2));
}

bool RhythmTree::second_tied() const
{
    return ((this->binary()) &&
            (_children[1]->tied()));
}

bool RhythmTree::dot_after() const
{
    return ((this->binary()) &&
            (_children[0]->tie()));
}

bool RhythmTree::dot_before() const
{
    return ((this->binary()) &&
            (_children[1]->tie()));
}



