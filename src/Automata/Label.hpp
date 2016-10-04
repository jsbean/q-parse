//
//  Label.hpp
//  qparse
//
//  Created by Florent Jacquemard on 04/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#ifndef Label_hpp
#define Label_hpp

#include <stdio.h>
#include <assert.h>

#define MAX_AR 13

#endif /* Label_hpp */



enum EventKind
{
    REST,
    NOTE,
    TIE,    // continuation
    INNER,  // inner node
};



// labels for nodes of Rhythm Trees
class Label
{
public:
    
    // Label with arity given in param, 0 grace note, kind note if leaf (a == 0)
    Label(int a);

    // Label for leaf of kind given in param, number of grace notes given in param
    // WARNING: no grace notes for rests and ties
    Label(EventKind k, int g=0);
    
    int arity() const;
    
    bool isLeaf() const { return (_ar == 0); };

    bool isInner() const { return (_ar > 0); };
    
    
    // to be called only on a leaf
    int nbGraceNotes() const;

    EventKind kind() const { return _type; };
    
    // return true if this node is a leaf and a rest
    bool isRest() const;

    // return true if this node is a leaf and a notes
    bool isNote() const;

    // return true if this node is a leaf and a tie
    bool isTie() const;
    
    // add number of grace notes given in param
    // the type of this Label must be NOTE
    void addGraceNotes(int);

    
private:
    // arity (0 for leaf)
    int _ar;

    // number of grace notes (only for leaf)
    int _gn;
    
    EventKind _type;

    // in MIDIcent
    // not used
    int _pitch;
};
