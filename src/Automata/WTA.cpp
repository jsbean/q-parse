//
//  WTA.cpp
//  qparse
//
//  Created by Florent Jacquemard on 04/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include "WTA.hpp"

Transition::Transition(Weight w):_weight(w){}

void Transition::add(State s)
{
    _body.push_back(s);
}

void Transition::setWeight(Weight w)
{
    _weight = w;
}

State Transition::antecedent(size_t i)
{
    assert (i < _body.size());
    return (_body[i]);
}


WTA::WTA():_cptState(0), _cptTr(0)
{ }

size_t WTA::newState()
{
    size_t pos = _cptState;
    vector<Transition*>* tl = new vector<Transition*>();
    
    assert (_cptState == _table.size());
    _table.push_back(tl);
    _cptState++;
   
    return pos;
}

bool WTA::registered(State s) const
{
    return (s < _table.size());
}


size_t WTA::size() const
{
    return _table.size();
}

size_t WTA::add(State s, Weight w)
{
    assert (s < _table.size());
    vector<Transition*>* tv = _table[s];
    Transition* t = new Transition(w);
    tv->push_back(t);
    _cptTr++;
    return(tv->size()-1);
}

// add(s, i, q) add the state q at the end of the body of the ith transition of head s.
// s must be a registered state.
// return the index of this transition in the list of transition of head s.
void WTA::add(State s, size_t i, State q)
{
    assert (s < _table.size());
    vector<Transition*>* tv = _table[s];
    assert (i < _table.size());
    Transition* t = tv->at(i);
    t->add(q);
}

// do not use
size_t WTA::add(State s, vector<State> sl, Weight w)
{
    size_t i = add(s, w);
    //assert (all_of(sl.begin(), sl.end(), [](State i){registered(i)}));
    //for(int i=0; i < sl.size(); i++){ assert (registered(sl[i])); };
    for(int i=0; i < sl.size(); i++){ add(s, i, sl[i]); }
    return(i);
}

Transition* WTA::get(State s, size_t i) const
{
    assert (s < _table.size());
    vector<Transition*>* tv = _table[s];
    assert(i < tv->size());
    return(tv->at(i));
}





