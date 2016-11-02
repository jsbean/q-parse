//
//  DurationList.hpp
//  qparse
//
//  Created by Florent Jacquemard on 02/11/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//
// list of rational durations to label nodes of WTA Runs for Kbest enum.
// Duration is either
// positive (event w or wo continuations -ties)
// or null (grace note).
//
// in quatization (ComboWTA)
// the length of list for a run correspoding to a segment is equal to the length of the segment
//


#ifndef DurationList_hpp
#define DurationList_hpp

#include <stdio.h>
#include <assert.h>
#include <list>

#include "Rational.hpp"

#endif /* DurationList_hpp */

using namespace std;


// a duration list is made of 2 parts:
// - an initial list of events ties to previous
// - a main list of the other events (without ties)
class DurationList
{
public:
    // empty duration list
    DurationList();
    
    // copy of duration list where all elements are multiplied by given Ratio
    DurationList(const DurationList&, Rational);

    bool empty() const;
    
    // add the event at the end of he main list
    void add(Rational);

    // push a continuation value
    void addcont(Rational);
    
    // multiply all elements given Ratio
    void mult(Rational);

    // concatenation
    DurationList& operator+=(const DurationList& rhs)
    {
        if (_main.empty())
        {
            _continuation += rhs._continuation;
        }
        else
        {
            list<Rational>::reverse_iterator last = _main.rbegin();
            assert (last != _main.rend());
            *last += rhs._continuation;
        }
        
        _main.insert(_main.end(), rhs._main.begin(), rhs._main.end());

        return *this;
    }
    
    friend std::ostream& operator<<(std::ostream&, const DurationList&);
    
    friend inline bool operator==(const DurationList&, const DurationList&);
    friend inline bool operator!=(const DurationList&, const DurationList&);

    
private:
    // duration to tie when appending to other list
    // zero if there is no continuation
    Rational _continuation;

    // list of events (without ties)
    list<Rational> _main;    
};


inline bool operator==(const DurationList& lhs, const DurationList& rhs)
{
    return ((lhs._continuation == rhs._continuation) &&
            (lhs._main == rhs._main)); // TBC defini?
}

inline bool operator!=(const DurationList& lhs, const DurationList& rhs)
{
    return !operator==(lhs,rhs);
}





