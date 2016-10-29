//
//  WTA.hpp
//  qparse
//
//  Created by Florent Jacquemard on 04/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//
// WTA schema = weighted CFG
//
// state (and non-terminals): int
//
// transitions:
// state -> state list, weight
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
#include <list>
#include <map>
#include <set>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
//#include <unordered_map>

#include "Distance.hpp" // includes class Weight

#endif /* WTA_hpp */

using namespace std;

typedef unsigned int State;

// poor man's definition of iterators
typedef vector<State>::iterator Transition_iterator;
typedef vector<State>::const_iterator Transition_const_iterator;


// a Transition is defined by a sequence of antecedent states (body) and a weight.
// a transition can be of two kinds:
// - inner transition:
//   the body has length > 1, the weight is not null.
// - terminal (leaf) transition:
//   the body has length 1 and contains a leaf label
//   the weight is not null.
class Transition
{
public:

    // Transition(w) creates a transition of weight w and empty body
    Transition(const Weight& w):_weight(w){ assert(_body.size() == 0); }
    
    // Transition(v, w) creates a transition of weight w and body a copy of v
    Transition(vector<State>, const Weight&);
    
    // Transition(s, w) creates a transition of weight w from the terminal symbol s
    // (body of size 1)
    Transition(State, const Weight&);

    ~Transition();
    
    
    bool inner() const;

    bool terminal() const;
    
    inline Weight weight() const { return _weight; }

    // at(i) returns the ith state in the body
    // i must be an index of the body
    State at(int) const;
    
    // add given State at the end of the body of this transition
    void add(State);

    // change weight
    void set(const Weight& w);
    
    // size of body
    size_t size() const;
    
    
    
    // iterator pointing to the first state in the body of the transition
    Transition_const_iterator begin() const { return _body.begin(); }

    // iterator pointing to the end of the body of the transition
    Transition_const_iterator end() const { return _body.end(); }
    
    // whether the given State belongs to the body of this transition
    bool member(State) const;
    
    // every State of the body is in the given set
    bool allin(const std::set<State>&) const;

    // no State of the body is in the given set
    bool nonein(const std::set<State>&) const;

    // write content of body and weight to output stream
    //void dump(ostream&);

    // write content of body and weight to output stream
    friend std::ostream& operator<<(std::ostream&, const Transition&);

private:
    vector<State> _body;
    Weight _weight;
};




class WTA;

typedef list<Transition>::iterator TransitionList_iterator;
typedef list<Transition>::const_iterator TransitionList_const_iterator;


class TransitionList
{
    
    friend class WTA;
    
public:
    TransitionList():_cpt_size(0) {}
    
    ~TransitionList();

    bool empty() const;
    
    // number of transitions
    size_t size() const;
    
    // total size of transition table (sum of transition sizes = number of occurences of states)
    size_t fullsize() const { return _cpt_size; }
    
    TransitionList_const_iterator begin() const { return _table.begin(); }
    
    TransitionList_const_iterator end() const { return _table.end(); }

    void add(const Transition&);

    
    // remove all transitions of length > 1 in the list
    // containing the given state
    // do not remove length 1 transitions to terminal symbols
    void remove(State);
    
    void clear();
    
private:

    // full size (number of occurences of states)
    size_t _cpt_size;
    
    // transition list
    list<Transition> _table;

    // container WTA
    WTA* _parent;
    
    //set of all states occuring in wta (in head or body)
    set<State> allstates();
};





class WTA
{
    friend class TransitionList;

public:
    
    // empty automaton
    WTA();

    // read WTA from file
    // the initial state set is set to singleton { 0 }
    WTA(string);
    
    ~WTA();
    

    // the state is present in the automaton
    bool isRegistered(State) const;
    
    // the state is an initial state
    bool isInitial(State) const;
    
    unsigned int resolution() const;
    
    // add(s, i) register state s
    // if s was already registered, return a reference to its transition list.
    // otherwise, create state s with an empty transition list and returns a reference to it.
    // moreover s is set as initial if i = true.
    TransitionList& add(State, bool initial=false);
    
    // add(s, t) add a transition with head s and with body/weight described in t
    // if s was not registered, it is registered
    // the transition t is added to the transition list of s
    // and a reference to this transition list is returned
    // moreover s is set as initial if i = true.
    TransitionList& add(State, const Transition&, bool initial=false);
    
    // remove the entry for given state s in the table of the table
    // i.e. all transitions targeted to s,
    // and all the transitions with s in their body.
    // if s was in the initial set, it is also removed from this set.
    // s must be registered.
    void remove(State);
    
    // begin(s) returns an iterator pointing to the first transition with head state s.
    // s must be registered.
    // not for modifying transition list of s. use add(...) methods for this.
    TransitionList_const_iterator begin(State) const;

    // begin(s) returns an iterator pointing to the past-the-end transition with head state s.
    // s must be registered.
    // not for modifying transition list of s. use add(...) methods for this.
    TransitionList_const_iterator end(State) const;
    
    // at(s) return the transition list of head state s.
    // s must be registered.
    TransitionList at(State) const;

    // at(s, i) return the ith transition of head state s.
    // s must be registered.
    // TODO REMOVE - REPL. by begin(State)
    //Transition* at(State, size_t) const;

    // remove states not inhabited and transitions containing these states
    void clean();
    
    // save to file
    void save(string);
    
    // number of states
    size_t countStates() const;
    
    // number of transition
    size_t countTransitions() const;
    
    // number of symbols (state occurences)
    size_t countAll() const;
    
    // TODO ajouter clean (avec decision du vide)

    // write table content to output stream
    //void dump(ostream&);
    
    // write table content to output stream
    friend std::ostream& operator<<(std::ostream&, const WTA&);
    
    // print sizes and table content to std output
    void print();
    
    // set of initial states
    std::set<State> initials;

    
protected:

    // number of transitions
    size_t _cpt_tr;

    // full size (number of occurences of states)
    size_t _cpt_size;

    // transition table
    //map<State,vector<Transition*>> _table;
    map<State, TransitionList> _table;
    
    // add(s, sl, w) add a new transition of head s and body sl and weight w
    // if s is not registered, it is added to the table.
    // returns a pointer to the created transition.
    // TODO SUPPR.?
//    Transition* add(State, vector<State>, Weight*);
    
    // step(s) returns the set of states reachable in one transition step by this WTA from the given state set s.
    // all the states in the set s must be registered.
    std::set<State> step(const std::set<State>&);
   
    // returns the set of all states occuring in wta (in head or body)
    set<State> allstates();
    
};

