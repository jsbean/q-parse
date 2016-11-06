//
//  main.cpp
//  qparse
//
// https://github.com/florento/q-parse
//
//  Created by Florent Jacquemard on 01/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include <stdio.h>
#include <time.h>
#include <iostream>
#include <assert.h>
#include <string>
#include <vector>

//#include "ComboWTA.hpp"
#include "Kbest.hpp"


double duration(clock_t start)
{
    return ((double)(clock() - start)/CLOCKS_PER_SEC * 1000);
}

int main(int argc, const char * argv[])
{

    clock_t time_start;
    //clock_t time_end;
    
// test WTA file IO
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
    
// test Combo construction
    if (argc == 2) return 0;
    std::cout << "\n==== Read input segment from " << argv[2] << '\n';
    Segment seg = Segment(argv[2], res);
    std::cout << "segment size " << seg.size() << '\n';

    std::cout << "\n==== COnstruction COmbo\n";
    time_start = clock();
    ComboWTA* combo = new ComboWTA(seg, *schema, 0);
    cout << "time to compute ComboWTA : ";
    cout << duration(time_start) << "ms \n";

    delete schema;

    cout << "\nCombo:\n";
    combo->print();
    cout << *combo;
    
//    cout << "\n==== Clean Combo:\n";
//    time_start = clock();
//    combo->clean();
//    cout << "time to clean ComboWTA : ";
//    cout << duration(time_start) << "ms \n";
//    combo->print();
//    cout << *combo;




    
    
// test k-best for initial states of Combo
    int k = 30;
    time_start = clock();
    Ktable<WeightMin> kkt = Ktable<WeightMin>(combo);
    
    for (size_t pre = 0; pre <= combo->max_pre(); pre++)
    {
        for (size_t post = 0; post <= combo->max_post(); post++)
        {
            cout << "\n==== " << k << "-best for ";
            cout << "pre=" << pre << " post=" << post << "\n";
            for (int i = k; i > 0; i--)
            {
                Run r = kkt.best(pre, post, i);
                if (! r.unknown())
                {
                    cout << "weight " << i << "-best = " << r.weight;
                    cout << " " << r.duration << "\n";
                }
            }
        }
    }
    
    cout << "\ntime for " << k << "-best for all initial states ComboWTA : ";
    cout << duration(time_start) << "ms \n";
   
    delete combo;

    return 0;
}






