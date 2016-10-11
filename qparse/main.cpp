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
    // insert code here...
    assert (argc == 2);
    string filename = argv[1];
    std::cout << "Read " << filename << '\n';

    WTA* ta = new WTA(filename);
    ta->print();
    delete ta;
    return 0;
}

