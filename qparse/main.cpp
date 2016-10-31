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
    clock_t time_end;
    
// test WTA file IO
    assert (argc >= 2);
    std::cout << "\n==== Read schema WTA from " << argv[1] << '\n';

    WTA* ta = new WTA(argv[1]);
//    ta->print();  // cout << ta;

    cout << "\n==== Clean schema:\n";
    time_start = clock();
    ta->clean();
    //time_end = clock();
    cout << "time to clean WTA : ";
    cout << duration(time_start) << "ms \n";
    ta->print();  // cout << ta;

    time_start = clock();
    unsigned int res = ta->resolution();
    cout << "time to compute resolution : ";
    cout << duration(time_start) << "ms \n";
    std::cout << "\nresolution = " << res << '\n';

// test Combo construction
    if (argc == 2) return 0;
    std::cout << "\n==== Read input segment from " << argv[2] << '\n';
    Segment seg = Segment(argv[2], res);
    std::cout << " segment size " << seg.size() << '\n';

    std::cout << "\n==== COnstruction COmbo\n";
    time_start = clock();
    ComboWTA* combo = new ComboWTA(seg, *ta, 0);
    cout << "time to compute ComboWTA : ";
    cout << duration(time_start) << "ms \n";

    delete ta;

    cout << "\nCombo:\n";
    combo->print();
    
    cout << "\n==== Clean Combo:\n";
    time_start = clock();
    combo->clean();
    cout << "time to clean ComboWTA : ";
    cout << duration(time_start) << "ms \n";
    combo->print();  // cout << ta;

    
// test k-best
    cout << "\n==== 1-best (" << combo->initials.size() << " initials)\n";
    time_start = clock();
    Ktable<WeightMin> kt = Ktable<WeightMin>(combo);
    for (set<State>::iterator i = combo->initials.begin();
         i != combo->initials.end(); ++i)
    {
        State s = *i;
        Run r = kt.best(s, 1);
        cout << "weight 1-best[" << s << "] = " << r.weight << "\n";
    }
    cout << "time to compute best for initial states ComboWTA : ";
    cout << duration(time_start) << "ms \n";
    
//    Run r = kt.best(1);
//    cout << "weight 1-best = " << r.weight << "\n";
   
    delete combo;

    return 0;
}






