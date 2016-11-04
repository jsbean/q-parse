//
//  AlignmentTree.cpp
//  
//
//  Created by Florent Jacquemard on 04/11/16.
//
//

#include "AlignmentTree.hpp"



AlignmentTree::AlignmentTree()
{
    root = new Alignment();
}


AlignmentTree::AlignmentTree(const Segment& s)
{
    root = new Alignment(s);
}


AlignmentTree::~AlignmentTree()
{
    // recursive destruction
    for (map<size_t, vector<AlignmentTree*>>::iterator i = _children.begin();
         i != _children.end(); ++i)
    {
        vector<AlignmentTree*> v = i->second;
        for (vector<AlignmentTree*>::iterator j = v.begin();
             j != v.end(); ++j)
            delete *j;
    }
    _children.clear(); //destroy the content
    delete root;
}


vector<AlignmentTree*> AlignmentTree::children(size_t a)
{
    assert (a > 1);
    // create an empty vector if it is not found
    vector<AlignmentTree*> tv = _children[a];
    
    if (tv.size() == 0)
    {
        assert (a > 1);
        assert ((root->_len % a) == 0); // this interval length must be divisible by n
        assert(root->_begin >= 0);
        assert(root->_begin < root->_res);
        assert(root->_begin + root->_len <= root->_res);
        
        size_t len = root->_len / a;
        size_t b = root->_begin;
        size_t j = root->_seg_lbeg;
        
        for (int i = 0; i < a; i++)
        {
            // create sub-Alignment starting at b
            Alignment* p = new Alignment(root->_seg, b, len);
            // create child ATree and insets it at the end of tv
            //tv.emplace(tv.end(), p);
            AlignmentTree* t = new AlignmentTree(p);
            tv.push_back(t);
            j = p->align(j); // align the newly created sub-Alignment
            b += len;
        }
        
    }
    assert (tv.size() == a);
    
    return tv;
}

