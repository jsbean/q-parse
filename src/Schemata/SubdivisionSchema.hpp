//
//  SubdivisionSchema.hpp
//  qparse
//
//  Created by Florent Jacquemard on 07/11/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//
// Subdivision schemas used in OM RQ lib
// and conversion to WTAs



#ifndef SubdivisionSchema_hpp
#define SubdivisionSchema_hpp

#include <stdio.h>
#include <assert.h>
#include <vector>

#include "Complexity.hpp"


// AND-OR alternating nested lists used by Adrien in RQ.
//
// from RQ doc:
// A subdivision schema of this kind is presented in the form of a nested list.
//
// For example : (2 2 3) means that each measure is divided in two,
// then each part is again divided in two,
// and finally, each of these parts is divided in 3.
// (2 2 3) = ∧(2 2 3) = 2.2.3  (AND ∧ means sequential composition also denoted .)
//
// If an element of the list is a list itself,
// it means that the choice is given between the values of the list.
// For example : (2 (2 3 5) 3) means (2 2 3) or (2 3 3) or (2 5 3).
// (2 (2 3 5) 3) = ∧(2 ∨(2 3 5) 3) = 2.(2|3|5).3
//
// If an element of the list is a list of lists,
// it means that the choice is given between various successions of subdivisions.
// For example : ( ((2 3) (3 2)) ) means (2 3) or (3 2).
// ( ((2 3) (3 2)) ) = ∧( ∨(∧(2 3) ∧(3 2)) ) =  (2.3) | (3.2)
// For example : ((2 3) ((2 3) ((3 5) 2))) means
// (2 2 3) or (2 3 2) or (2 5 2) or (3 2 3) or (3 3 2) or (3 5 2).
// ((2 3) ((2 3) ((3 5) 2))) = ∧(∨(2 3) ∨(∧(2 3) ∧(∨(3 5) 2)))
//                           = (2|3) . ((2.3) | ((3|5).2))
//
class AONode
{
public:
    AONode(size_t a):_arity(a) { }
    
    bool inner() const { return (_arity == 0); }

    bool leaf() const { return (_arity > 1); }

    size_t value() const { return _arity; }
    
protected:
    // 0 for inner nodes, arity > 1 for leaf
    size_t _arity;
};

class ONode;

class ANode: public AONode
{
public:
    ANode():AONode(0) {}
    ANode(size_t a):AONode(a) {}

    void add(const ONode& n) { children.push_back(n); }
    
    std::vector<ONode> children;
private:
};

class ONode: public AONode
{
public:
    ONode():AONode(0) {}
    ONode(size_t a):AONode(a) {}
    
    void add(const ANode& n) { children.push_back(n); }

    std::vector<ANode> children;
private:
};




// dag schema

struct ds_transition
{
    unsigned int dst_source;
    size_t dst_label;
    unsigned int dst_target;
    
    // ds_transition(source_state, arity_val, target_state)
    ds_transition(unsigned int s, size_t l, unsigned int t):
    dst_source(s),
    dst_label(l),
    dst_target(t)
    {}

    ds_transition(const ds_transition& dst):
    dst_source(dst.dst_source),
    dst_label(dst.dst_label),
    dst_target(dst.dst_target)
    {}
    
    void rename(unsigned int s, unsigned int u);
    
    // increase source and target state by n
    void shift(unsigned int n);

    // increase source and target state by n, if they are not 0
    void shift0(unsigned int n);

};


// dag whose edges are labeled by arity values
// two distinguished nodes:
// a source node: 0
// a target node: _max_state
class dagSchema
{
public:
    dagSchema():_max_state(0) {};
    
    // translation of AND-OR alternating nested lists into dag-schemas
    dagSchema(const ANode&);
    dagSchema(const ONode&);

    unsigned int max() const { return _max_state; }
    
    unsigned int max(const dagSchema& lhs, const dagSchema& rhs)
    {
        unsigned int lm = lhs.max();
        unsigned int rm = rhs.max();
        return (lm >= rm)?lm:rm;
    }
    

    // for testing. do not use
    void add(const ds_transition& dst);
    
  
    
private:
    std::vector<ds_transition> _table;
    
    // max_state occurring in table
    unsigned int _max_state;
    
    // rename state i into state j
    void rename(unsigned int i, unsigned int j);

    // rename every state s into s + n
    // the result is not a dag-schema
    void shift(unsigned int n);

    // rename every state s into s + n except 0
    void shift0(unsigned int n);

};




#endif /* SubdivisionSchema_hpp */
