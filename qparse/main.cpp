//
//  main.cpp
//  qparse
//
// https://github.com/florento/q-parse
//
//  Created by Florent Jacquemard on 01/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include <iostream>
#include <assert.h>
#include <string>

#include "WTA.hpp"

int main(int argc, const char * argv[]) {

    // test maps
    map<State,vector<Transition*>> _table;
    // creation
    cout << "size table[0]=" << _table[0].size() << "\n";;
    vector<Transition*>* v = &_table[0]; // ptr
    cout << "size v=" << v->size() << "\n";;
    Weight w;
    Transition* t = new Transition(w);
    v->push_back(t);
    cout << "size v=" <<v->size() << "\n";;
    cout << "size _table[0]=" <<_table[0].size() << "\n";;
    
   
    
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
    
    
    
    // test WTA file IO
    assert (argc == 2);
    string filename = argv[1];
    std::cout << "Read " << filename << '\n';

    WTA* ta = new WTA(filename);
    ta->print();
    delete ta;
    return 0;
}

