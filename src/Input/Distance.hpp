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


class Distance: public Weight
{
public:
    // weight which is the distance defined by alignment for input segment
    Distance(const Alignment&);

    // copy of weight = combination of given weight and a null distance value.
    Distance(const Weight&);

    
    void add(const Distance&);
    
    void mult(const Distance&);
    
    // linear combination of this distance value and the given weight
    void combine(const Weight&);

    
    static void setFactor(double a) { _alpha = a ;};
    
private:
    
    // factor for linear combinations weight - distance
    static double _alpha;

    
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
