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


// test 1-best
    cout << "\n==== 1-best for all of " << schema->initials.size() << " initials\n";
    time_start = clock();
    Ktable<WeightMax> kt0 = Ktable<WeightMax>(schema);
    iRun r = kt0.best(1);
    cout << "weight 1-best = ";
    if (r.unknown()) { cout << "NaN\n"; }
    else { cout << r.weight << "\n"; }
    
    cout << "time to 1-best for all initial states ComboWTA : ";
    cout << duration(time_start) << "ms \n";

    return 0;


// test k-best
    cout << "\n==== 1-best for each of " << combo->initials.size() << " initials\n";
    time_start = clock();
    Ktable<WeightMin> kt = Ktable<WeightMin>(combo);
    for (set<State>::iterator i = combo->initials.begin();
         i != combo->initials.end(); ++i)
    {
        State s = *i;
        Run r = kt.best(s, 1);
        cout << "weight 1-best[" << s << "] = " << r.weight << " ";
        cout << r.duration << "\n";
    }
    cout << "time to compute 1-best for every initial state ComboWTA : ";
    cout << duration(time_start) << "ms \n";
    
// test k-best for initials
    int k = 50;
    cout << "\n==== " << k << "-best for " << combo->initials.size() << " initials altogether\n";
    time_start = clock();
    Ktable<WeightMin> kkt = Ktable<WeightMin>(combo);
    for (int i = k; i > 0; i--)
    {
        iRun r = kkt.best(i);
        if (! r.unknown())
        {
            cout << "weight " << i << "-best = " << r.weight;
            cout << " (" << r.rank << "-best for state " << r.head << ") ";
            cout << r.duration << "\n";
        }
    }
    cout << "time to " << k << "-best for all initial states ComboWTA : ";
    cout << duration(time_start) << "ms \n";
   
    delete combo;

    return 0;
}






