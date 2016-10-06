//
//  Distance.hpp
//  qparse
//
//  Created by Florent Jacquemard on 04/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#ifndef Distance_hpp
#define Distance_hpp

#include <stdio.h>
#include <vector>

#include "Weight.hpp"

#endif /* Distance_hpp */

using namespace std;


// A Distance is a Weight measure made of
// - a vector of pointwise distances to an input onset vector
// - a mask (bitvector)
// - the dimension is the length of the input onset vector
// - the length (Euclidean) of the vector (norm) = Weight's val
class Distance : public Weight
{
public:
    Distance(int v) { };
    
    void add(const Distance&);
    
    void mult(const Distance&);

    
private:
    // vector length
    int _dim;
    
    vector<double> _dist;
    
    
    
};
