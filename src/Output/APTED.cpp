//
//  APTED.cpp
//  qparse
//
//  Created by Florent Jacquemard on 11/12/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//
// RT output format for Tree Edit Distance library
// APTED algorithm of M. Pawlik and N. Augsten
// http://tree-edit-distance.dbresearch.uni-salzburg.at

#include "RT.hpp"


string RhythmTree::APTED() const
{
    if (terminal())
    {
        size_t a = this->label();
        size_t g = Label::nbGraceNotes(a);
        
        if (Label::continuation(a)) // tie
        { return "{O}"; }
        else if (g == 0)            // single note
        { return "{X}"; }
        else                        // graces notes + note
        { return ("{X"+(::to_string(Label::nbGraceNotes(a)))+"}"); }
    }
    else // inner()
    {
        size_t a = this->arity();
        assert (a > 1);
        std::string res = "{A";
        res += ::to_string(a);
        for (size_t i = 0; i < a; i++)
        {
            res += " ";
            res += _children[i]->APTED();
        }
        res += "}";
        return res;
    }
}
