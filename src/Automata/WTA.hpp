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
#include <set>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <unordered_map>

#endif /* WTA_hpp */

using namespace std;

typedef unsigned int State;

typedef vector<State>::iterator Transition_iterator;


class Weight;

class Transition
{
public:
    // Transition(w) create a transition of weight w and empty body
    Transition(Weight*);
    
    ~Transition();
    
    // add antecedent at the end of the obdy of this transition
    void add(State);

    void setWeight(Weight* w);
    
    // size of body
    size_t size() const;
    
    // antecedent(i) returns the ith state in the body
    // i must be an index of the body
    State at(int);
    
    Weight* weight(){ return _weight; }

    
protected:
    vector<State> _body;
    Weight* _weight;
};


class WTA
{
public:
    
    // empty automaton
    WTA();

    // read WTA from file
    // the initial state set is set to singleton { 0 }
    WTA(string);
    
    ~WTA();
    

    // the state is present in the automaton
    bool registered(State) const;
    
    // the state is an initial state
    bool initial(State) const;
    
    unsigned int resolution() const;

    
    // add(s) register state s
    // if s was already, return an iterator pointing to its transition list.
    // otherwise, create state s with an empty transition list and returns an iterator to it.
    vector<Transition*>::const_iterator add(State);
   
    // add(s, w) add a new transition with weight w for state s.
    // if s is not registered, it is added to the table.
    // return a pointer to the created transition.
    Transition* add(State, Weight*);
      
    // begin(s) returns an iterator pointing to the first transition with head state s.
    // s must be registered.
    // not for modifying transition list of s. use add(...) methods for this.
    vector<Transition*>::const_iterator begin(State) const;

    // begin(s) returns an iterator pointing to the past-the-end transition with head state s.
    // s must be registered.
    // not for modifying transition list of s. use add(...) methods for this.
    vector<Transition*>::const_iterator end(State) const;
    
    // size(s) return the number of transitions with head state s.
    // s must be registered.
    // TODO UNUSED?
    size_t size(State) const;
    
    // at(s, i) return the ith transition of head state s.
    // s must be registered.
    // TODO REMOVE - REPL. by begin(State)
    Transition* at(State, size_t) const;
    
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
   
    // set of initial states
    set<State> _init;
    
    //vector<vector<Transition*>*> _table;
    map<State,vector<Transition*>> _table;
    
    //vector<Transition*>* getTrs(State s);
    
    // add(s, sl, w) add a new transition of head s and body sl and weight w
    // if s is not registered, it is added to the table.
    // returns a pointer to the created transition.
    // TODO SUPPR.?
    Transition* add(State, vector<State>, Weight*);
    
    // step(s) returns the set of states reachable in one transition step by this WTA from the given state set s.
    // all the states in the set s must be registered.
    set<State> step(set<State>&);
    
};

