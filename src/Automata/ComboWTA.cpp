//
//  ComboWTA.cpp
//  qparse
//
//  Created by Florent Jacquemard on 11/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include "ComboWTA.hpp"




ComboState::~ComboState()
{
    delete cs_path;
}


cs_key ComboState::key()
{
    cs_key k;
    k[0] = cs_state;
    k[1] = cs_path->begin();
    k[2] = cs_path->end();
    k[3] = cs_rp;
    k[4] = cs_rr;
    return (k);
}



//State ComboWTA::addComboState(ComboState* s)
//{
//assert(s);
//    cs_key k = s->key();
//    
//}
//
//
//ComboWTA::ComboWTA(const Segment& seg, const WTA& schema)
//{
//    
//    //FIFO queue of states to treat
//    // queue<ComboState*> states;
//    
//
//    
//    
//}
