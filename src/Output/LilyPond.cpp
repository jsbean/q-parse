//
//  LilyPond.cpp
//  qparse
//
//  Created by Florent Jacquemard on 19/11/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//
// Lilypond output for RT
// http://lilypond.org

#include "RT.hpp"


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
        else // [grace notes +] note
        {
            if (g > 0) // grace notes + note
            {
                res += "\\grace {";
                for (size_t i = 0; i < g; i++)
                {
                    res += "f";
                    res += ::to_string(depth*2);
                }
                res += "} ";
            }
            // note
            res += "e";
            res += ::to_string(depth);
            if (tie) { res += "~"; }
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


bool RhythmTree::dot_flag = false;


string RhythmTree::lilydot(int depth)
{
    RhythmTree::dot_flag = false;
    return lilydot(depth, false, false, false, false);
}


string RhythmTree::lilydot(int depth,
                           bool tie,
                           bool dot,
                           bool ignore_first,
                           bool ignore_second)
{
    if (dot) { RhythmTree::dot_flag = true; } // global variable
    
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
        else           //  note
        {
            if (g > 0) // grace notes + note
            {
                res += "\\grace {";
                for (size_t i = 0; i < g; i++)
                {
                    res += "f";
                    res += ::to_string(depth*2);
                }
                res += "} ";
            }
            // note (dotted or not)
            res += "e";
            res += ::to_string(depth);
            if (dot) { res += "."; }
            if (tie) { res += "~"; }
            return res;
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


