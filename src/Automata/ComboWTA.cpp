//
//  ComboWTA.cpp
//  qparse
//
//  Created by Florent Jacquemard on 11/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include "ComboWTA.hpp"

#include "ComboWeight.hpp"


ComboState::ComboState():cs_state(0), cs_rp(0), cs_rr(0)
{
    cs_path = new Alignment();
}


ComboState::ComboState(State s, Alignment* p, int rp, int rr):
cs_state(s),
cs_path(p),
cs_rp(rp),
cs_rr(rr)
{
    assert(p);
    rehash();
}


ComboState::ComboState(const ComboState& cs, int rp, int rr):
cs_state(cs.cs_state),
cs_path(cs.cs_path),
cs_rp(rp),
cs_rr(rr)
{
    assert(cs_path);
    rehash();
}

ComboState::~ComboState()
{
    delete cs_path;
}

void ComboState::rehash()
{
    assert(cs_path);
    _hash[0] = cs_state;
    _hash[1] = cs_path->begin();
    _hash[2] = cs_path->length();
    _hash[3] = cs_rp;
    _hash[4] = cs_rr;
}

bool ComboState::operator==(const ComboState& s) const
{   for (int i = 0; i < 5; i++)
{
    if (_hash[i] != s._hash[i]) return false;
}
    return true;
}

// lexicographic comparison on hash value (unsigned int[5])
bool ComboState::operator<(const ComboState& s) const
{
    for (int i = 0; i < 5; i++)
    {
        if (_hash[i] > s._hash[i]) return false;
        else if (_hash[i] < s._hash[i]) return true;
        //otherwise _hash[i] == s._hash[i]
    }
    return false;   //they are equal
}










State ComboWTA::addState(ComboState* cs)
{
    assert(cs);
    map<ComboState, State>::const_iterator it = _statemap.find(*cs);

    // combo state found in map: combo state has been treated already
    if (it != _statemap.end())
        return it->second;

    // otherwise, create new state and add transitions to it
    State s = _cpt++;
    _statemap[*cs] = s;

    State q = cs->cs_state; // state in schema
    //Alignment* p = cs->cs_path;

    // enumerate the transitions to q in schema
    for (vector<Transition*>::const_iterator i = _schema->begin(q); i != _schema->end(q); ++i)
    {
        Transition* t = *i;     // transition of schema
        size_t a = t->size();   // transition arity
        assert (a >= 1);
        Weight* w = t->weight(); // trandition weight  // TODO ptr
        // leaf transition
        if (a == 1)
        {
            State label = t->antecedent(0);
            // the label coincide with the info in guess and path
            if ((Label::nbGraceNotes(label) ==
                (cs->cs_rp + cs->cs_path->l_size() - 1)) &&
                (cs->cs_rr == cs->cs_path->r_size()))
            {
                // compute distance
                Distance* dist = new Distance(cs->cs_path);
                
                // add terminal transition to (leaf) label
                
            }
            // otherwise add no transition
        }
        // inner transition
        else if (a > 1)
        {
            vector<Alignment*> vp = cs->cs_path->subs(0);
        }
    }

    return s;
}


ComboWTA::ComboWTA(const Segment& seg, const WTA* schema)
{
    assert(schema);
    _schema = schema;
    
    // FIFO of ComboStates to add
    //queue<ComboState*> _csqueue;
    
    // map of ComboState into their index in ComboWTA
    // tmp (only for constructor)
    //map<ComboState, State> _statemap;

    
    // initial ComboStates


    
//    while (! _csqueue.empty())
//    {
//        ComboState* cs = _csqueue.front();
//        _csqueue.pop();
    
    
}
