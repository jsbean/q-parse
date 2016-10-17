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
#include <vector>

#include "ComboWTA.hpp"


// just for the sample -- print the data sets
std::ostream& operator<<(std::ostream& os, const std::vector<int>& vi)
{
    os << "( ";
    std::copy(vi.begin(), vi.end(), std::ostream_iterator<int>(os, " "));
    os << ")\n";
    return os;
}


void product(std::vector<int> b, size_t len)
{
    std::vector<int> result;
    
    // initial result = null vector
    for (int i = 0; i < len; i++)
        result.push_back(0);
    
    while (true)
    {
        std::cout << result;

        for (int i = 0; ; )
        {
            result[i]++;
            if (result[i] == b[i]) // we reach max value for i
            {
                if (i+1 == len) // last digit
                {
                    return;
                }
                else
                {
                    result[i] = 0;
                    i++;
                }
            }
            else
            {
                break;
            }
        }
    }
}


int main(int argc, const char * argv[])
{

    std::vector<int> bases = { 3, 3, 3 };
    
    product(bases, bases.size());
    
    
//    // test maps
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
    
    
    
    // test WTA file IO
//    assert (argc == 2);
//    string filename = argv[1];
//    std::cout << "Read " << filename << '\n';
//
//    WTA* ta = new WTA(filename);
//    ta->print();
//    delete ta;
    
    return 0;
}

