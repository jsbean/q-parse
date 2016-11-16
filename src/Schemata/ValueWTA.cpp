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
    return ((_state == rhs._state) && (_tree->top == rhs._tree->top));
}


std::ostream& operator<<(std::ostream& o, const ValueState& vs)
{
    o << vs._tree->top << "^" << vs._state;
    return o;
}



bool ValueWTA::compatible(State label, const ValueState& vs)
{
    return (
            // case of a continuation (tie)
            (Label::continuation(label) &&
             vs.value().single_continuation())
           ||
            // case of single note (no grace notes, no continuation)
            ((! Label::continuation(label)) &&
             (Label::nbGraceNotes(label) == 0) &&
              vs.value().single_event())
           ||
            // case of note and grace notes (no continuation)
            ((! Label::continuation(label)) && vs.value().event() &&
             (Label::nbGraceNotes(label) == vs.value().nbgn())));
}

State ValueWTA::addValueState(const ValueState& vs, bool initial)
{
    if (TRACE_LEVEL > 1) { cout << "value state: " << vs << " "; }
    
    // combo state found in map: combo state has been treated already
    unordered_map<ValueState, State>::const_iterator it = _statemap.find(vs);
    if (it != _statemap.end())
    {
        if (TRACE_LEVEL > 1) { cout << " = state " << it->second << " (done)\n"; }
        return it->second;
    }
    
    // otherwise, create new State
    State s = _cpt++;
    if (TRACE_LEVEL > 1) { cout << " = state " << s << "\n"; }
    
    // add map of current ComboState to new State
    _statemap[vs] = s;
    //register state
    TransitionList& tv = add(s, initial); // empty vector of Transitions
    
    State q = vs.state();                 // current state in schema

    
    // add transitions to new state s
    // vs._tree is the current node in tree of duration lists
    
    // enumerate the transitions to q in schema
    for (TransitionList_const_iterator it = _schema.begin(q);
         it != _schema.end(q); ++it)
    {
        const Transition& t = *it;    // transition of schema
        assert(t.inner() || t.terminal());
        size_t a = t.size();          //  arity of schema transition
        assert (a >= 1);
        const Weight& w = t.weight(); // weight of schema transition
        
        // leaf schema transition:
        // add at most one leaf transition to ValueWTA
        if (t.terminal())
        {
            assert (a == 1);
            State label = t.label(); // for such transition, the body is a singleton containing a label
            
            if (TRACE_LEVEL > 1)
            {
                cout << "...";
                cout << "lab=" << label << " ";
                //cout << "#gn=" << Label::nbGraceNotes(label) << " ";
                cout << vs.value() << " ";
                if (vs.value().single_continuation()) { cout << " (single cont) "; }
                if (vs.value().single_event()) { cout << " (single event) "; }
                if (vs.value().event()) { cout << " (event) "; }
            }
            if (compatible(label, vs))
            {
                if (TRACE_LEVEL > 1) { cout << "YES" << "\n"; }
                // add terminal transition from (leaf) label to s
                Transition newt = Transition(label, w);
                assert(newt.terminal());
                tv.add(newt);
            }
            // otherwise add no transition
            else {  if (TRACE_LEVEL > 1) { cout << "NO" << "\n"; } }
        }
        // inner schema transition:
        // add one inner transition to ValueWTA
        else if (t.inner() &&
                 (! vs.tree()->top.single_continuation()) &&
                 (! vs.tree()->top.single_event()) &&
                 (! vs.tree()->top.event()))
        {
            assert (a > 1);
            Transition newt = Transition(w); // new transition for ValueWTA
            for(size_t i = 0; i < a; i++)
            {
                // recursive registration of new ComboState i of transition
                // cannot be initial (TODO AV)
                State news = addValueState(ValueState(t.at(i), vs.tree()->sub(a,i)));
                newt.add(news);
            }
            assert(newt.inner());
            assert(newt.size() == a);
            tv.add(newt);
        }
    }
    return s;
}


ValueWTA::ValueWTA(const DurationList& dlist, const WTA& schema):
_schema(schema)
{
    _statemap.clear();
    _cpt = 0;
    _tree = new DurationTree(dlist);
    
    _initial = addValueState(ValueState(_schema.initial(), _tree), true);
    
    // end of construction
    // destroy tmp tree of duration lists
    delete _tree;
    // destroy all ValueStates
    _statemap.clear();
}

