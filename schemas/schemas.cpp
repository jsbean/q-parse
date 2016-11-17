//
//  schemas.cpp
//  qparse
//
//  Created by Florent Jacquemard on 17/11/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//
// management of schema files

#include <stdio.h>
#include <time.h>
#include <iostream>
#include <assert.h>
#include <string>
#include <vector>

#include "Kbest.hpp"
#include "SubdivisionSchema.hpp"


double duration(clock_t start)
{
    return ((double)(clock() - start)/CLOCKS_PER_SEC * 1000);
}




int main(int argc, const char * argv[])
{
    
    clock_t time_start;
    //clock_t time_end;
    
    // test WTA file input
    assert (argc >= 2);
    std::cout << "\n==== Read schema WTA from " << argv[1] << '\n';
    
    WTA* schema = new WTA(argv[1]);
    schema->print();
    cout << *schema;
    
    cout << "\n==== Clean schema:\n";
    time_start = clock();
    schema->clean();
    //time_end = clock();
    cout << "time to clean WTA : ";
    cout << duration(time_start) << "ms \n";
    schema->print();
    cout << *schema;
    
    time_start = clock();
    size_t res = schema->resolution();
    cout << "time to compute resolution : ";
    cout << duration(time_start) << "ms \n";
    std::cout << "\nresolution = " << res << '\n';
    
    
    // test 1-best (compute max weight of schema)
    cout << "\n==== 1-best for all of " << schema->initials.size() << " initials\n";
    time_start = clock();
    Ktable<WeightMax> kt0 = Ktable<WeightMax>(schema);
    Run r = kt0.best(1);
    cout << "weight 1-best (max) = ";
    if (r.unknown()) { cout << "NaN\n"; }
    else { cout << r.weight << "\n"; }
    
    cout << "time to 1-best (max) for initial state of schema : ";
    cout << duration(time_start) << "ms \n";
    
    // test normalization schema

    
    // test WTA file output
    if (argc == 2) return 0;
    std::cout << "\n==== Write normalized WTA to " << argv[2] << '\n';
    schema->save(argv[2]);
    
    return 0;
}






