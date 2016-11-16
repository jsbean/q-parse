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
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <list>
#include <map>
#include <vector>

#include "Rational.hpp"

using namespace std;


// a duration list is made of 2 parts:
// - an initial duration (possibly null) ties to a previous duration list
// - a main list of the other events (without ties)
class DurationList
{
    friend class DurationTree;
    
public:
    // empty duration list
    DurationList();
    
    // copy of duration list where all elements are multiplied by given Ratio
    DurationList(const DurationList&);

    // copy of duration list where all elements are multiplied by given Ratio
    DurationList(const DurationList&, Rational);
    
    // read duration list from file
    // one ratio per line
    // if the first line is negative ratio, it is a continuation
    // all other line must contain positive or null ratios
    // the file must not be empty
    DurationList(string);

    bool empty() const;
    
    size_t size() const { return (_main.size() + ((_continuation > 0)?1:0)); }
    
    Rational cont() const { return _continuation; }
    
    // one (non null) continuation and no event in the main list.
    bool single_continuation() const;

    // no continuation and only one event in the main list.
    bool single_event() const;

    // no continuation and some grace notes (dur=0) + one event (dur>0) in the main list.
    bool event() const;
    
    // number of grace note
    // must be an event()
    size_t nbgn() const;
    
    // sum of the elements of the duration list (including continuation)
    Rational length() const;
    
    // add the event at the end of the main list
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


namespace std {
    template <> struct hash<DurationList>
    {
        size_t operator()(const DurationList& d) const
        {
            return (hash<size_t>()(d.size()) ^
                    hash<double>()(toDouble(d.cont())));
        }
    };
}


inline bool operator==(const DurationList& lhs, const DurationList& rhs)
{
    if ((lhs._continuation == rhs._continuation) &&
        (lhs._main.size() == rhs._main.size()))
    {
        list<Rational>::const_iterator il = lhs._main.begin();
        list<Rational>::const_iterator ir = rhs._main.begin();
        while (il != lhs._main.end())
        {
            assert (ir != lhs._main.end());
            if (*il != *ir) { return false; }
            il++;
            ir++;
        }
        return true;
    }
    else
        return false;
}

inline bool operator!=(const DurationList& lhs, const DurationList& rhs)
{
    return !operator==(lhs,rhs);
}






// a tree container for duration lists,
// to avoid recomputation of division of duration lists
class DurationTree
{
public:
    DurationTree():top() {};
    DurationTree(const DurationList& d);
    
    ~DurationTree();
    
    DurationList top;
    
    DurationTree* sub(size_t, size_t);
    
   
private:
    // length of the top DurationList
    Rational _length;
    
    // every entry in this map associate to
    // an arity n a partition a1,...,an of this DurationList
    std::map<size_t, vector<DurationTree*>> _children;
    
};



#endif /* DurationList_hpp */
