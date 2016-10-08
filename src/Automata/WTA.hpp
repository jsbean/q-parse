//
//  WTA.hpp
//  qparse
//
//  Created by Florent Jacquemard on 04/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//
// WTA schema = stochastic CFG
//
// label: int (terminals)
// number of note + grace notes at (left of) current node
//  0 = continuation
//  1 = 1 note | rest      (au + 1 note)
//  2 = 1 grace notes + 1 note
//  3 = 2 grace notes + 1 note
// etc
//
// state: int
//
// transitions (table):
// state -> state list | weight:float
//   state: head state
//   state list: body states if length ≥ 2
//               label if length = 1
//   w: weight
//

#ifndef WTA_hpp
#define WTA_hpp

#include <stdio.h>
#include <assert.h>
#include <vector>
#include <unordered_map>

#include "Weight.hpp"

#endif /* WTA_hpp */

using namespace std;

typedef size_t State;

//typedef struct _transition
//{
//    vector<State> tr_body;
//    Weight tr_weight;
//} Transition;


class Transition
{
public:
    // Transition(w) create a transition of weight w and empty body
    Transition(Weight);
    
    // add antecedent at the end of the obdy of this transition
    void add(State);

    void setWeight(Weight w);
    
    // antecedent(i) returns the ith state in the body
    State antecedent(size_t);
    
    Weight weight(){ return _weight; }

    
private:
    vector<State> _body;
    Weight _weight;
};


class WTA
{
public:
    
    WTA();
    
    // register a new state
    size_t newState();

    bool registered(State) const;

    // number of states
    size_t size() const;
    
    // add(s, sl, w) add a new transition of head s and body sl and weight w
    // s must be a registered state
    // return the index of this transition in the list of transition of head s
    size_t add(State, vector<State>, Weight);

    // add(s, sl, w) add a new transition of head s with empty body and weight w.
    // s must be a registered state.
    // return the index of this transition in the list of transition of head s.
    size_t add(State, Weight);
    
    // add(s, i, q) add the state q at the end of the body of the ith transition of head s.
    // s must be a registered state
    void add(State, size_t, State);
  
    
    // get(s, i) return the ith transition of head state s
    Transition* get(State, size_t) const;
    
    
private:
    
    size_t _cptState;
    
    size_t _cptTr;
    
    vector<vector<Transition*>*> _table;
    
};

