//
//  Event.hpp
//  qparse
//
//  Created by Florent Jacquemard on 06/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//
// input events
// STUB. to be completed/instanciated if needed.
//
// input interface with OpenMusic or MEI etc
// keep track of input event list
// event are not stored internaly (in WTA). we just preserve the order.
// and remap afterwards to input event list (with dfs)


#ifndef Event_hpp
#define Event_hpp

#include <stdio.h>

#endif /* Event_hpp */



enum EventKind
{
    REST,
    NOTE,
    CHORD,
};



class Event
{
public:
    Event (EventKind k, double d):_type(k), _date(d) {}
    
    EventKind kind() const { return _type; };

    double timestamp() const { return _date; };

    
private:
    
    EventKind _type;
    
    double _date;
};





