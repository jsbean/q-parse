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
    else // inner()
    {
        size_t a = this->arity();
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

bool RhythmTree::_dot = false;

string RhythmTree::lilydot(int depth)
{
    RhythmTree::_dot = false;
    return lilydot(depth, false, false, false, false);
}

string RhythmTree::lilydot(int depth,
                           bool tie,
                           bool dot,
                           bool ignore_first,
                           bool ignore_second) 
{
    if (dot) { RhythmTree::_dot = true; } // global variable
    
    std::string res;
    if (terminal())
    {
        size_t a = this->label();
        size_t g = Label::nbGraceNotes(a);

        if (Label::continuation(a)) // tie
        {
            res += "e";
            res += ::to_string(depth);
            if (dot) { res += "."; }
            if (tie) { res += "~"; }
            return res;
        }
        else if (g == 0)            // single note
        {
            res += "e";
            res += ::to_string(depth);
            if (dot) { res += "."; }
            if (tie) { res += "~"; }
            return res;
        }
        else                        // note + grace notes
        {
            return res;             // TODO
        }
    }
    else // inner()
    {
        assert (! dot); // dot flag is only for terminals
        size_t a = this->arity();
        assert (a > 1);
        
        if (ignore_first && (! ignore_second))
        {
            assert (a == 2);
            res += _children[1]->lilydot(depth*2, tie, false, false, false);
        }
        else if (ignore_second && (! ignore_first))
        {
            assert (a == 2);
            res += _children[0]->lilydot(depth*2, tie, false, false, false);
        }
        else if (ignore_first && ignore_second)
        {
            assert (a == 2);
            // skip
        }
        else
        {
            if (a > 2)
            {
                res += "\\tuplet ";
                res += ::to_string(a);
                res += "/2";
            }
            res += " {";
            bool next_ignore_first = false;
            bool next_dot = false;
            for (size_t i = 0; i < a-1; i++)
            {
                res += " ";
                // the first leaf of next sibling is a dot for current sibling
                if (_children[i]->terminal() &&
                    _children[i+1]->binary() &&
                    _children[i+1]->_children[0]->tie() &&
                    (! next_dot))
                {
                    assert (! next_ignore_first);
                    res += _children[i]->lilydot(depth*2,
                                                 _children[i+1]->second_tied(), // curent tie,
                                                 true,                          // current dot,
                                                 false,                         // ignore_first
                                                 false                          // ignore_second
                                                );
                    next_ignore_first = true;
                    next_dot = false;
                }
                // the second leaf of this sibling is a dot for next sibling
                else if (_children[i]->binary() &&
                         _children[i]->_children[1]->terminal() &&
                         _children[i+1]->terminal() &&
                         _children[i+1]->tie())
                {
                    res += _children[i]->lilydot(depth*2,
                                                 false,             // curent tie,
                                                 false,             // current dot,
                                                 next_ignore_first, // ignore_first
                                                 true               // ignore_second
                                                 );
                    next_ignore_first = false;
                    next_dot = true;
                }
                else
                {
                    res += _children[i]->lilydot(depth*2,
                                                 _children[i+1]->tied(), // curent tie,
                                                 next_dot,               // current dot,
                                                 next_ignore_first,      // ignore_first
                                                 false                   // ignore_second
                                                 );

                    next_ignore_first = false;
                    next_dot = false;
                }
            }
            // last child, propagate parent's tie flag
            // cannot be dotted
            res += " ";
            res += _children[a-1]->lilydot(depth*2,
                                           tie,
                                           next_dot,
                                           next_ignore_first,
                                           false);
            res += " } ";
        }
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



