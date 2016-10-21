//
//  main.cpp
//  qparse
//
// https://github.com/florento/q-parse
//
//  Created by Florent Jacquemard on 01/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//



#define TRACE 1


#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <string>
#include <vector>

#include "ComboWTA.hpp"


int main(int argc, const char * argv[])
{
    
// test WTA file IO
    assert (argc >= 2);
    std::cout << "\n Read schema WTA from " << argv[1] << '\n';

    WTA* ta = new WTA(argv[1]);
//    ta->print();  // cout << ta;
    
    cout << "\n clean:\n";
    ta->clean();
    ta->print();  // cout << ta;
    
    unsigned int res = ta->resolution();
    std::cout << "\n Resolution = " << res << '\n';
    
// test Combo construction
    if (argc == 2) return 0;
    std::cout << "\n Read input segment from " << argv[2] << '\n';
    Segment seg = Segment(argv[2], res);
    std::cout << " segment size " << seg.size() << '\n';
    
    std::cout << "\n COnstruction COmbo\n";
    ComboWTA* combo = new ComboWTA(seg, *ta, 0);
    
    cout << "\n Combo:\n";
    combo->print();
    
    delete ta;
    
    return 0;
}


//    test maps

//    map<State,vector<Transition*>> _table;
//    // creation
//    cout << "size table[0]=" << _table[0].size() << "\n";;
//    vector<Transition*>* v = &_table[0]; // ptr
//    cout << "size v=" << v->size() << "\n";;
//    Weight w;
//    Transition* t = new Transition(w);
//    v->push_back(t);
//    cout << "size v=" <<v->size() << "\n";;
//    cout << "size _table[0]=" <<_table[0].size() << "\n";;


//    vector<Transition*>* tv = _table[0];
//if (tv == NULL)
//        cout << "NULL\n";
//    else
//        cout << "not NULL\n";
//
//    _table[0] = new vector<Transition*>;
//    tv = _table[0];
//    if (tv == NULL)
//        cout << "NULL\n";
//    else
//        cout << "not NULL\n";

