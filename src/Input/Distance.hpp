//
//  Distance.hpp
//  qparse
//
//  Created by Florent Jacquemard on 04/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//
// simple version: we just keep the sum of the components


#ifndef Distance_hpp
#define Distance_hpp

#include <stdio.h>

#include "Weight.hpp"
#include "Path.hpp"

#endif /* Distance_hpp */

using namespace std;


class Distance : public Weight
{
public:
    Distance(const Alignment&);
    
    void add(const Distance&);
    
    void mult(const Distance&);

    
private:
    // OLD version
    // A Distance is a Weight measure made of
    // - a vector of pointwise distances to an input onset vector
    // - a mask (bitvector)
    // - the dimension is the length of the input onset vector
    // - the length (Euclidean) of the vector (norm) = Weight's val
    
    // vector length
    //int _dim;
    
    //vector<double> _dist;
    
};
