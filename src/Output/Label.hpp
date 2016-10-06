//
//  Label.hpp
//  qparse
//
//  Created by Florent Jacquemard on 04/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//
// labels for output RT


#ifndef Label_hpp
#define Label_hpp

#include <stdio.h>
#include <assert.h>

#include "Event.hpp"

#define MAX_AR 13

#endif /* Label_hpp */



enum LabelKind
{
    EVENT,
    TIE,    // continuation
    INNER,  // inner node
};



// labels for nodes of Rhythm Trees
class Label
{
public:
    
    // Label with arity given in param, 0 grace note, kind note if leaf (a == 0)
    Label(int a);

    // Label for leaf of kind event,
    // number of grace notes given in param
    // TODO: change to list of grave notes as events ?
    Label(Event*, int g=0);
    
    int arity() const;
    
    bool isLeaf() const { return (_ar == 0); };

    bool isInner() const { return (_ar > 0); };
    
    
    // to be called only on a leaf
    int nbGraceNotes() const;

    LabelKind kind() const { return _type; };
    
    // return true if this node is a leaf and a rest
    bool isRest() const;

    // return true if this node is a leaf and a note
    bool isNote() const;

    // return true if this node is a leaf and a chord
    bool isChord() const;

    // return true if this node is a leaf and a tie
    bool isTie() const;
    
    // add number of grace notes given in param
    // the type of this Label must be NOTE
    void addGraceNotes(int);

    
private:
    // arity (0 for leaf)
    int _ar;

    // number of grace notes (only for leaf containing event)
    int _gn;
    
    LabelKind _type;

    // in MIDIcent
    // not used
    Event* _event; 

};
