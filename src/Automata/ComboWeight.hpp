//
//  ComboWeight.hpp
//  qparse
//
//  Created by Florent Jacquemard on 04/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#ifndef ComboWeight_hpp
#define ComboWeight_hpp

#include <stdio.h>

#include "Distance.hpp"
//#include "Weight.hpp"

#endif /* ComboWeight_hpp */

//class Weight;
//class Distance;


// complexity + alpha.distance
// constructor(alpha)
class ComboWeight : public Weight
{
public:
    // new weight combining (mult) a weight and a distance weight
    ComboWeight(const Weight* const, const Distance* const);
 
    // default where distance is neutral elemet for mult
    ComboWeight(const Weight* const);
    
    
    void add(const ComboWeight&);
    
    void mult(const ComboWeight&);
    
    static void setFactor(double a) { _alpha = a ;};
    
private:
    static double _alpha;
};

