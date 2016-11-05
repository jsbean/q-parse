//
//  CONSTANT.h
//  qparse
//
//  Created by Florent Jacquemard on 21/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#ifndef CONSTANT_h
#define CONSTANT_h

#include <stdio.h>
//#include <iostream>
#include <string>


#endif /* CONSTANT_h */


const int TRACE_LEVEL = 1;

const double CST_ALPHA = 0.5;

const char CMD_NAME[] = "qparse";



#if defined(__GNUC__) && (__GNUC__ < 3) && !defined(__SPSL__)
#include <limits.h>


template <class T> class numeric_limits
{
public:
    static T min() {return T();}
    static T max() {return T();}
};

template<> class numeric_limits <char>
{
public:
    static long min() { return CHAR_MIN; }
    static long max() { return CHAR_MAX; }
};

template<> class numeric_limits <short>
{
public:
    static long min() { return SHRT_MIN; }
    static long max() { return SHRT_MAX; }
};

template<> class numeric_limits <int>
{
public:
    static long min() { return INT_MIN; }
    static long max() { return INT_MAX; }
};

template<> class numeric_limits <long>
{
public:
    static long min() { return LONG_MIN; }
    static long max() { return LONG_MAX; }
};

template<> class numeric_limits <long long>
{
public:
    static long long min() {
        return -1-(long long)((~(unsigned long long)0)>>1); }
    static long long max() {
        return (long long)((~(unsigned long long)0)>>1); }
};

#else
#include <limits>
using std::numeric_limits;
#endif

