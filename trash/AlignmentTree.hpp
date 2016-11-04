//
//  AlignmentTree.hpp
//  
//
//  Created by Florent Jacquemard on 04/11/16.
//
//

#ifndef AlignmentTree_hpp
#define AlignmentTree_hpp

#include <stdio.h>

#endif /* AlignmentTree_hpp */



// a structure for optimizing the construction of ComboWTA
// by avoiding recomputation of Alignments
//
// a alignement tree is composed of
// - a root alignment
// - several tuples of subtrees, defining partitions of the root alignement
//   one tuple for each possible arity (length of partition).
class AlignmentTree
{
    friend class AlignmentTree;
    
public:
    AlignmentTree();
    
    AlignmentTree(const Segment&);
    
    AlignmentTree(Alignment* p):root(p){};
    
    ~AlignmentTree();
    
    Alignment* root;
    
    vector<AlignmentTree*> children(size_t);
    
private:
    
    // every entry in this map associate to
    // an arity n a partition a1,...,an of the root Alignment
    map<size_t, vector<AlignmentTree*>> _children;
    
};
