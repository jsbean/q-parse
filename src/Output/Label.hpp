//
//  Label.hpp
//  qparse
//
//  Created by Florent Jacquemard on 04/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//
// labels for nodes of output Rhythm Trees
//
// Inner nodes are simply labeled by arity
// Leaves contain either events or ties
// a Tie leaf contains no info
// an Event leaf points to input events (vector of input events)
// the pointed event is
// - number of grace notes



#ifndef Label_hpp
#define Label_hpp

#include <stdio.h>
#include <assert.h>
#include <vector>


//#include "Event.hpp"

#define MAX_AR 13


using namespace std;

class Event;

enum LabelKind
{
    EVENT,
    TIE,    // continuation
    INNER,  // inner node
};


// generic label with a kind
// class Label
class Label
{
public:
    // tie if arity = 0
    // inner node if arity > 0
    Label(int a=0);
    
    size_t arity() const;
    
    bool isLeaf() const { return (_ar == 0); };

    bool isInner() const { return (_ar > 0); };
    
    LabelKind kind() const { return _type; };
    
    // return the number of grace node encoded in given leaf label
    static size_t nbGraceNotes(size_t);

    // return the leaf label encoding the given number of grace nodes
    //static size_t leafLabel(size_t);

    static bool continuation(size_t);

protected:
    
    LabelKind _type;

    // arity (0 for leaf)
    size_t _ar;

};



// label for inner node.
// contains only arity (more info later?)
class InnerLabel: Label
{
public:
    // Label of inner node with arity given in param
    // 0 grace note, kind note if leaf (a == 0)
    InnerLabel(int);

};


class EventLabel: Label
{
public:
    // Label for leaf of kind tie
    EventLabel();

    // Label for leaf of kind event,
    // number of grace notes given in param
    // TODO: change to list events ?
    EventLabel(int n=0);
    
    // to be called only on a leaf
    size_t nbGraceNotes() const;

    // add number of grace notes given in param
    // the type of this Label must be NOTE
    void addGraceNotes(size_t);
    
    void pushEvent(Event*);

private:
    // number of events (grace notes + note)
    // 0 for tie
    // > 0 for grace notes + event
    size_t _nbe;
    
    // in MIDIcent
    // not used
    vector<Event*> _events;
    
};




#endif /* Label_hpp */
