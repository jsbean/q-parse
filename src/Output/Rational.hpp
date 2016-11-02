//
//  Rational.hpp
//  qparse
//
//  Created by Florent Jacquemard on 02/11/16.
//
// rational.h
// class of rational numbers


#ifndef Rational_hpp
#define Rational_hpp

#include <iostream>

#endif /* Rational_hpp */


using namespace std;


class Rational
{
public:
    Rational() : num(0), den(1) {}
    Rational(long n, long d = 1);
    
    // copy constructor
    Rational(const Rational& rhs) : num(rhs.num), den(rhs.den) {}
    
    ~Rational(void) {}
    
    long numerator(void) const { return num; }
    long denominator(void) const { return den; }
    
    // assignment operators
    Rational& operator=(const Rational& rhs);
    Rational& operator=(long rhs);
    
    // unary operators
    Rational operator+(void) const { return *this; }
    Rational operator-(void) const { return Rational(-num, den); }
    Rational invert(void) const { return Rational(den, num); }
    
    // binary shortcut operators
    const Rational& operator+=(const Rational& rhs);
    const Rational& operator-=(const Rational& rhs);
    const Rational& operator*=(const Rational& rhs);
    const Rational& operator/=(const Rational& rhs);
    const Rational& operator+=(long rhs);
    const Rational& operator-=(long rhs);
    const Rational& operator*=(long rhs);
    const Rational& operator/=(long rhs);
    
    // increment/decrement iterators
    const Rational& operator++();
    const Rational operator++(int);
    const Rational& operator--();
    const Rational operator--(int);
    
    // -- better implemented as explicit conversion
    // -- function toDouble (see below)
    // operator double(void) const { return double(num)/den; }
    
private:
    // Data
    long num;  // numerator
    long den;  // denominator (keep > 0!)
    
    // auxillary helper function to normalize the rationals
    long gcd(long, long);
};


// assignment operators
inline Rational& Rational::operator=(const Rational& rhs)
{
    num = rhs.num;
    den = rhs.den;
    return *this;
}

inline Rational& Rational::operator=(long rhs)
{
    num = rhs;
    den = 1;
    return *this;
}

// Rational -> double conversion
inline double toDouble(const Rational& r)
{
    return double(r.numerator())/r.denominator();
}

// Rational -> long conversions
inline long trunc(const Rational& r)
{
    return r.numerator() / r.denominator();
}

inline long floor(const Rational& r)
{
    long q = r.numerator() / r.denominator();
    return (r.numerator() < 0 && r.denominator() != 1) ? --q : q;
}

inline long ceil(const Rational& r)
{
    long q = r.numerator() / r.denominator();
    return (r.numerator() >= 0 && r.denominator() != 1) ? ++q : q;
}

// double -> Rational conversion
Rational toRational(double x, int iterations = 5);

// binary operators
const Rational operator+(const Rational& lhs, const Rational& rhs);
const Rational operator-(const Rational& lhs, const Rational& rhs);
const Rational operator*(const Rational& lhs, const Rational& rhs);
const Rational operator/(const Rational& lhs, const Rational& rhs);
Rational rabs(const Rational& rhs);

// boolean operators
bool operator==(const Rational& lhs, const Rational& rhs);
bool operator!=(const Rational& lhs, const Rational& rhs);
bool operator<=(const Rational& lhs, const Rational& rhs);
bool operator>=(const Rational& lhs, const Rational& rhs);
bool operator<(const Rational& lhs, const Rational& rhs);
bool operator>(const Rational& lhs, const Rational& rhs);

// output operator
ostream& operator<< (ostream& ostr, const Rational& r);
istream& operator>> (istream& istr, Rational& r);

