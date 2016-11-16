//
//  ValueWTA.cpp
//  qparse
//
//  Created by Florent Jacquemard on 15/11/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include "ValueWTA.hpp"


ValueState::ValueState():_state(0)
{
    _tree = new DurationTree();
}


ValueState::ValueState(State s, DurationTree* t):
_state(s),
_tree(t)
{
    assert(t);
}


ValueState::~ValueState()
{
    // _tree deleted externaly (avoid double delete)
    // delete _tree;
}


bool ValueState::operator==(const ValueState& rhs) const
{
    return ((_state == rhs._state) &&
            *(_tree->top) == *(rhs._tree->top));
}



std::ostream& operator<<(std::ostream& o, const ValueState& vs)
{
    o << *(vs._tree->top) << "^" << vs._state;
    return o;
}


