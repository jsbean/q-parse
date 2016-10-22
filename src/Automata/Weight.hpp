//
//  Weight.hpp
//  qparse
//
//  Created by Florent Jacquemard on 04/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#ifndef Weight_hpp
#define Weight_hpp

#include <stdio.h>

#endif /* Weight_hpp */

// class for weights in automata transitions
// can be used for weights in file (serialized WTA)
class Weight
{
public:
    
    Weight(double v=0):_val(v) {};
        
    inline double value() const { return _val; }

    inline double null() const { return (_val == 0); }
    
    // add weight in parameter to this weight (modified in place)
    // use: this weight is the weight of a run, parameter is a weight in a transition
    // the value is updated
    // TBR
    //virtual void add(const Weight&); // =0;
    
    //static Weight zero();
    
    // multiply weight in parameter by this weight (modified in place)
    // use: this weight is the weight of a run, parameter is a weight in a transition
    // the value is updated
    // TBR
    //virtual void mult(const Weight&); // =0;
    
    friend Weight operator+(Weight, const Weight&);
    
    Weight& operator+=(const Weight& rhs)
    {
        _val += rhs._val;
        return *this;
    }

protected:
    
    // store
    double _val;
    
};


inline Weight operator+(Weight lhs, const Weight& rhs)
{
    lhs._val += rhs._val;
    return lhs;
}

inline bool operator==(const Weight& lhs, const Weight& rhs)
{
    return (lhs.value() == rhs.value());
    // it is the default -> TODO SUPPR.
}

inline bool operator!=(const Weight& lhs, const Weight& rhs)
{
    return !operator==(lhs,rhs);
}

// comparison class
// one ordering for k-best to compute the min weight
struct WeightMin
{
    bool operator()(const Weight& lhs, const Weight& rhs) const
    {
        return lhs.value() < rhs.value();
    }
};

// one ordering for k-best to compute the max weight
// where 0 considered the highest value
// 0 is the weight for initialization, computed weight is always > 0
struct WeightMax
{
    bool operator()(const Weight& lhs, const Weight& rhs) const
    {
        if (lhs.null())
        {
            if (rhs.null()) return false;
            else return true;
        }
        else
        {
            if (rhs.null()) return false;
            else return lhs.value() > rhs.value();
        }
    }
};


