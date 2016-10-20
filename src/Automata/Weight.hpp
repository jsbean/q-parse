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
    
    Weight(double v=0): _val(v) {};
        
    double value() const { return _val; }
    
    // add weight in parameter to this weight (modified in place)
    // use: this weight is the weight of a run, parameter is a weight in a transition
    // the value is updated
    virtual void add(const Weight&); // =0;
    
    //static Weight zero();
    
    // multiply weight in parameter by this weight (modified in place)
    // use: this weight is the weight of a run, parameter is a weight in a transition
    // the value is updated
    virtual void mult(const Weight&); // =0;
    
    // returns
    // -1 if the value of this weight is smaller than the value of the parameter
    //  0 if the value of this weight is equal to the value of the parameter
    //  1 if the value of this weight is larger than the value of the parameter
    int compare(const Weight&);
    
    
protected:
    
    // store
    double _val;
    
};
