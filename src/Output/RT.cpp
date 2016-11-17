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


string RhythmTree::to_string() const
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
                return (::to_string(Label::nbGraceNotes(label()))+"+n");
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


string RhythmTree::lily(int depth, bool tie) const
{
    std::string res;
    if (terminal())
    {
        size_t a = this->label();
        size_t g = Label::nbGraceNotes(a);
        // tie
        if (Label::continuation(a))
        {
            res += "e";
            res += ::to_string(depth);
            if (tie) { res += "~"; }
            return res;
        }
        else if (g == 0) // single note
        {
            res += "e";
            res += ::to_string(depth);
            if (tie) { res += "~"; }
            return res;
        }
        else // note + grace notes
        {
            // TODO
            return res;
        }
    }
    else
    {
        size_t a = label();
        assert (a > 1);
        if (a > 2)
        {
            res += "\\tuplet ";
            res += ::to_string(a);
            res += "/2";
        }
        res += " {";
        for (size_t i = 0; i < a-1; i++)
        {
            bool nexttie = _children[i+1]->tied();
            res += " ";
            res += _children[i]->lily(depth*2, nexttie);
        }
        // last child, propagate parent's tie flag
        res += " ";
        res += _children[a-1]->lily(depth*2, tie);
        res += " } ";
        return res;
    }
}

bool RhythmTree::tied() const
{
    if (terminal())
    {
        return (Label::continuation(this->label()));
    }
    else //
    {
        assert (_children.size() > 1);
        return (_children[0]->tied());
    }
}


