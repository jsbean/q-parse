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
    
    // arity for inner node
    // label for terminal node
    size_t label() const;
    
    // add a subtree
    void add(RhythmTree*);

    string to_string();

private:
    // type state. for leaves only (no significance for inner nodes)
    size_t _label;
    
    // every entry in this map associate to
    // an arity n a partition a1,...,an of this DurationList
    std::vector<RhythmTree*> _children;
    
};




#endif /* RT_hpp */
