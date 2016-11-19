//
//  Onsets.cpp
//  qparse
//
//  Created by Florent Jacquemard on 18/11/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include "Onsets.hpp"

Onsets::Onsets(const DurationList& d)
{
    Rational accu;
    _content.push_back(Rational(0));

    if (d.cont() > Rational(0))
    {
        accu += d.cont();
        _content.push_back(accu);
    }

    for (list<Rational>::const_iterator i = d.begin(); i != d.end(); ++i)
    {
        accu += *i;
        _content.push_back(accu);
    }
}

DurationList Onsets::ioi() const
{
    DurationList res;
    list<Rational>::const_iterator i = _content.cbegin();
    assert (*i == Rational(0));
    Rational prev = *i;
    ++i; // first element after 0

    while (i != _content.cend())
    {
        res.add(*i - prev);
        prev = *i;
        ++i;
    }
    
    return res;
}

const Onsets operator+(const Onsets& lhs, const Onsets& rhs)
{
    Onsets res;
    int mode = 0;
    list<Rational>::const_iterator i = lhs._content.cbegin();
    list<Rational>::const_iterator j = rhs._content.cbegin();
    assert (*i == Rational(0));
    assert (*j == Rational(0));
    
    while (mode < 3)
    {
        if (i == lhs._content.cend())
        {
            if (j == rhs._content.cend())
            {
                mode = 3;
            }
            else
            {
                mode = 1;
            }
        }
        else if (j == lhs._content.cend())
        {
            mode = 2;
        }
        else
        {
            mode = 0;
        }

        switch (mode)
        {
            case 0: // i != end  j != end
                if (*i == *j)
                {
                    res.add(*i);
                    i++;
                    j++;
                }
                else if (*i < *j)
                {
                    res.add(*i);
                    i++;
                }
                else
                {
                    res.add(*j);
                    j++;
                }
                break;
                
            case 1:  // i == end  j != end
                res.add(*j);
                j++;
                break;

            case 2:  // i != end  j == end
                res.add(*i);
                i++;
                break;

            case 3:  // i == end  j == end
                break; // terminate loop
                
            default:
                assert (false); // should not happen
        }

    }
    
    return res;
}

