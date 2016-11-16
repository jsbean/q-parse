//
//  ValueWTA.hpp
//  qparse
//
//  Created by Florent Jacquemard on 15/11/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//
// Value WTA
// is a special kind of WTA associated to an initial WTA (schema)
// and a rhythmic value (DurationList).
// It characterizes the trees of the schema language (with weight defined by schema)
// having the given rhythmic value.


#ifndef ValueWTA_hpp
#define ValueWTA_hpp

#include <stdio.h>

#include <assert.h>
#include <array>
#include <queue>
#include <map>

#include "WTA.hpp"
#include "DurationList.hpp"
#include "Label.hpp"

#endif /* ValueWTA_hpp */


using namespace std;

class DurationList; // Rhythmic Value




// tmp state structure for construction of ValueWTA
// casted into size_t (aka type State) after construction
//
// value state = (q:int, d:DurationList) ou label (feuille)
// q: state base schema
// d: current duration list

class ValueState
{
public:
    
    ValueState();
    
    ValueState(State, DurationTree*);
    
    ~ValueState();
    
    State state() { return (_state); }

    DurationList* value() { assert(_tree); return (_tree->top); }
    
    bool operator==(const ValueState& s) const;
    
    friend std::ostream& operator<<(std::ostream&, const ValueState&);
    
    
private:
    // state of base schema
    State _state;

    // sequence of ratios associated to this state (shared structure)
    DurationTree* _tree;
    
};






