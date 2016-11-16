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
//#include <map>
#include <unordered_map>

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
    
    State state() const { return _state; }

    DurationList value() const { assert(_tree); return _tree->top; }
    DurationTree* tree() const { assert(_tree); return _tree; }
    
    bool operator==(const ValueState& s) const;
    
    friend std::ostream& operator<<(std::ostream&, const ValueState&);
    
    
private:
    // state of base schema
    State _state;

    // sequence of ratios associated to this state (shared structure)
    DurationTree* _tree;
    
};


namespace std {
    template <> struct hash<ValueState>
    {
        size_t operator()(const ValueState& vs) const
        {
            return hash<State>()(vs.state()) ^ hash<DurationList>()(vs.value());
        }
    };
}



// table of transitions
// construction top-down, given an initial schema (WTA) and a DurationList
class ValueWTA: public WTA
{
public:
    
    // construction from given initial duration list and WTA (base schema)
    ValueWTA(const DurationList&, const WTA&);
    
    
private:
    // Global variables for the ComboWTA construction
    
    const WTA& _schema;
    
    // counter for new ComboWTA states
    State _cpt;
    
    // map of ComboState into their index in ComboWTA
    // tmp (only for constructor)
    //unordered_map<cs_key, State, container_hasher> _statemap;
    unordered_map<ValueState, State> _statemap;
    
    // tree of Alignments (Path with info on alignment to input segment)
    // organized hierarchically by sub-alignments
    // the ComboStates use th epointers stored in this tree
    DurationTree* _tree;
    
    // addValueState(vs, flag) returns the ValueWTA state associated to the ValueState vs
    // if there exists one.
    // otherwise:
    //     a new ValueWTA State s is created,
    //     s is associated to vs,
    //     s is added to the transition table
    //     the ValueWTA transitions with head s are computed and added to the table
    //     s is returned
    State addValueState(const ValueState&, bool initial=false);
    
    bool compatible(State label, const ValueState& vs);
    
};


