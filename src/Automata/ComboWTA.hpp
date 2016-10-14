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
#include <unordered_map>
#include <string>

#include "WTA.hpp"
#include "Path.hpp"

#endif /* ComboWTA_hpp */

using namespace std;


class ComboWeight;


//typedef struct _combostate
//{
//...    // state of base schema
//} ComboState;


// key for mapping of combo-states into their index in the constructed combo WTA
typedef std::array<unsigned int, 5> cs_key;


class ComboState
{
public:

    ~ComboState();
    
    // state of base schema
    State cs_state;

    // current augmented path (interval of points + alignment of input segment)
    Alignment* cs_path;
    
    // guess number of points aligned to right of previous segment
    int cs_rp;
    
    // guess number of points aligned to right of current segment
    int cs_rr;
    
    cs_key key();
};


//class ComboWTA: public WTA
//{
//public:
//    
//    // construction from input segment and WTA (base schema)
//    ComboWTA(const Segment&, const WTA&);
//   
//private:
//    // counter for new states
//    State _cpt;
//    
//    // compute and add transitions for one ComboState of the ComboWTA
//    // returns its state index in the ComboWTA
//    State addComboState(ComboState*);
//    
//    // map of ComboState into their index in ComboWTA
//    // tmp (only for constructor)
//    unordered_map<cs_key, State> _statemap;
//
//};
