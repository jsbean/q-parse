//
//  Event.cpp
//  qparse
//
//  Created by Florent Jacquemard on 06/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include "Event.hpp"


TimestampedEvent::TimestampedEvent(const Event& e, double d):Event(e),_date(d) {}
