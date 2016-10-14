//
//  WTA.hpp
//  qparse
//
//  Created by Florent Jacquemard on 04/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//
// WTA schema = weighted CFG
//
// state (non-terminals): int
//
// transitions:
// state -> state list | weight
//   state: head state
//   state list: body states if length ≥ 2
//               label if length = 1
//
// in other terms transition rules have one of the forms
// s -> (s1,...,sk) w where k > 1, s, s1, ..., sk are states and w weight
// s -> (s1) w where s1 is an leaf label = int encoding
//
// leaf label (terminals):
// number of note + grace notes at (left of) current node
//  0 = continuation
//  1 = 1 note | rest      (au + 1 note)
//  2 = 1 grace notes + 1 note
//  3 = 2 grace notes + 1 note
// etc
// see Label for the functions managing these labels
//
// transition table:
// head state -> vector of (state vector, weight)


#ifndef WTA_hpp
#define WTA_hpp

#include <stdio.h>
#include <assert.h>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <unordered_map>

#include "Weight.hpp"

#endif /* WTA_hpp */

using namespace std;

typedef unsigned int State;

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
    
    ~Transition();
    
    // add antecedent at the end of the obdy of this transition
    void add(State);

    void setWeight(Weight w);
    
    // size of body
    size_t size() const;
    
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
    
    // empty automaton
    WTA();

    // read from file
    WTA(string);
    
    ~WTA();
    
    // addState(s) returns a pointer to the transition vector of s.
    // if s is not present in the table register it as new state (with empty vector of transitions)
    // a pointer to the transition vector of s if it is registered
    // vector<Transition*>* addState(State);

    bool registered(State) const;

   
    // add(s, w) add a new transition with weight w for state s.
    // if s is not registered, it is added to the table.
    // return the index of this transition in the list of transitions of head s.
    size_t add(State, Weight);
   
    // add(s, i, q) add the state q at the end of the body of the ith transition of head s.
    // s must be a registered state with at least i transitions.
    void add(State, size_t, State);

    // add(s, sl, w) add a new transition of head s and body sl and weight w
    // s must be a registered state
    // return the index of this transition in the list of transition of head s
    void add(State, vector<State>, Weight);
    
    // begin(s) returns an iterator pointing to the first transition
    // with head state s.
    // s must be registered.
    vector<Transition*>::iterator begin(State);

    // begin(s) returns an iterator pointing to the past-the-end transition
    // with head state s.
    // s must be registered.
    vector<Transition*>::iterator end(State);

    
    // size(s) return the number of transitions with head state s.
    // s must be registered.
    size_t size(State);
    
    // at(s, i) return the ith transition of head state s.
    // s must be registered.
    Transition* at(State, size_t);
    
    // save to file
    void save(string);
    
    // write table content to output stream
    void dump(ostream&);
    
    // print sizes and table content to std output
    void print();
    
    // number of states
    size_t countStates() const;
    
    // number of transition
    size_t countTransitions() const;
    
    // number of symbols (state occurences)
    size_t countAll() const;
    
protected:

    // number of transitions
    size_t _cpt_tr;

    // full size (number of occurences of states)
    size_t _cpt_size;
   
    
    //vector<vector<Transition*>*> _table;
    map<State,vector<Transition*>> _table;
    
    //vector<Transition*>* getTrs(State s);
    
};

