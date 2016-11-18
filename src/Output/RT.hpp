//
//  RT.hpp
//  qparse
//
//  Created by Florent Jacquemard on 16/11/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//
// Rhythm Trees

#ifndef RT_hpp
#define RT_hpp

#include <stdio.h>
#include <vector>
#include <string>

#include "Label.hpp"



class RhythmTree
{
public:
    RhythmTree(size_t lab=0):_label(lab) {};
    
    ~RhythmTree();

    bool terminal() const { return _children.empty(); }

    bool inner() const { return (! terminal()); }
    
    // label for terminal node
    // must be a terminal node
    size_t label() const;

    // arity for inner node
    // must be an inner node
    size_t arity() const;

    // add a subtree
    void add(RhythmTree*);

    string to_string() const;
    
    // LilyPond format
    string lily(int depth, bool tie=false) const;
    
    // LilyPond format with dots
    string lilydot(int depth);
    
    // global variable set if a dot is added in lilydot
    static bool _dot;

private:
    // type state. for leaves only (no significance for inner nodes)
    size_t _label;
    
    // every entry in this map associate to
    // an arity n a partition a1,...,an of this DurationList
    std::vector<RhythmTree*> _children;
    
    // utilities for LilyPond convert
    
    // return whether this tree is a continuation (a leaf)
    bool tie() const;

    // return whether this tree is binary
    bool binary() const;

    // return whether the leftmost innermost leaf is a tie (continuation)
    bool tied() const;

    // return whether this tree is binary and the second child is tied
    bool second_tied() const;
    
    // return whether this tree is binary
    // and the left son is a dot (continuation after the dotted note)
    bool dot_after() const;

    // return whether this tree is binary
    // and the right son is a dot (continuation before the dotted note)
    bool dot_before() const;
    
    // LilyPond format with dots
    string lilydot(int depth,
                   bool tie,
                   bool dot,
                   bool ignore_first,
                   bool ignore_second);

};




#endif /* RT_hpp */
