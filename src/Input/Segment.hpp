//
//  Segment.hpp
//  qparse
//
//  Created by Florent Jacquemard on 06/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//
// Input Segment
// (fragment of Chord Seq)
//
// We do not import info on events in the chord seq
// only keep their indexes



#ifndef Segment_hpp
#define Segment_hpp

#include <stdio.h>
#include <vector>
#include <assert.h>

#include "Event.hpp"

#endif /* Segment_hpp */

using namespace std;


class Segment
{
public:
    // Segment(b,e,r) constructs an empty segment (no events) with
    // - start date b
    // - end date e
    // - resolution r
    Segment(double, double, int);
    
    //add new timestamped event
    void push(Event*);
    
    size_t size() const;
    
    int date(size_t) const;
    
    // return the number of elements of this segment contained in the given Path
    // int count(const Path&);

private:
    // resolution
    int _res;

    // start date (in seconds) of segment
    double _begin;
    
    // start date (in seconds) of segment
    double _end;
    
    //length (in seconds) of segment
    double _len;
    
    // event list
    vector<Event*> _event;
    
    // list of timestamps casted to [0.._res]
    vector<int> _date;
    
    // bit vector
};