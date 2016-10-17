//
//  ComboWTA.hpp
//  qparse
//
//  Created by Florent Jacquemard on 11/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#ifndef ComboWTA_hpp
#define ComboWTA_hpp

#include <stdio.h>
#include <assert.h>
#include <array>
#include <queue>
#include <map>

#include "WTA.hpp"
#include "Label.hpp"

#endif /* ComboWTA_hpp */

using namespace std;


class Weight;
class Distance;
class ComboWeight;
class Alignment; // Path
class Segment;
class Label;


// tmp state structure for construction of ComboWTA
// casted into unsigned int (state index)
class ComboState
{
public:

    ComboState();
    
    ComboState(State, Alignment*, int rp=0, int rr=0);

    ComboState(const ComboState&, int rp=0, int rr=0);

    ~ComboState();
    
    // state of base schema
    State cs_state;

    // current augmented path (interval of points + alignment of input segment)
    Alignment* cs_path;
    
    // guess number of points aligned to right of previous segment
    int cs_rp;
    
    // guess number of points aligned to right of current segment
    int cs_rr;
    
    // cs_key const key();
    
    bool operator==(const ComboState& s) const;
    
    // lexicographic comparison on hash value (array[5])
    bool operator<(const ComboState& s) const;
    
    
private:
    //cs_key _hash;
    unsigned int _hash[5];
    
    void rehash();
};





class ComboWTA: public WTA
{
public:
    
    // construction from input segment and WTA (base schema)
    ComboWTA(const Segment&, const WTA*);
   
private:
    // Global variables for the ComboWTA construction
    
    const WTA* _schema;
    
    // counter for new ComboWTA states
    State _cpt;

    // map of ComboState into their index in ComboWTA
    // tmp (only for constructor)
    //unordered_map<cs_key, State, container_hasher> _statemap;
    map<ComboState, State> _statemap;
    
    
    // toState(cs) returns the ComboWTA state associated to the CoboState cs if there is one
    // otherwise:
    //     a new ComboWTA state s is created,
    //     s is associated to cs,
    //     s is added to the table
    //     the ComboWTA transitions with head s are computed and added to the table
    //     r is returned
    State addState(ComboState*);

};
