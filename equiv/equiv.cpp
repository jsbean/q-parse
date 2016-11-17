//
//  equiv.cpp
//  qparse
//
//  Created by Florent Jacquemard on 16/11/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//
// main for target equiv
// enumeration of rhythms of the same duration value

#include <stdio.h>
#include <time.h>
#include <iostream>
#include <assert.h>
#include <string>
#include <vector>

#include "ValueWTA.hpp"
#include "Kbest.hpp"
#include "RT.hpp"


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
    // cout << *schema;
    
    cout << "\n==== Clean schema:\n";
    time_start = clock();
    schema->clean();
    //time_end = clock();
    cout << "time to clean WTA : ";
    cout << duration(time_start) << "ms \n";
    schema->print();
    cout << *schema;
    
//    time_start = clock();
//    size_t res = schema->resolution();
//    cout << "time to compute resolution : ";
//    cout << duration(time_start) << "ms \n";
//    std::cout << "\nresolution = " << res << '\n';
    
    
    // test ValueWTA construction
    if (argc == 2) return 0;
    std::cout << "\n==== Read rhythmic value from " << argv[2] << ".txt\n";
    string valuename = string(argv[2])+".txt";
    DurationList seg = DurationList(valuename);
    std::cout << "length rhythmic value: " << seg.size() << '\n';
    
    std::cout << "\n==== COnstruction ValueWTA\n";
    time_start = clock();
    ValueWTA* vwta = new ValueWTA(seg, *schema);
    cout << "time to compute ComboWTA : ";
    cout << duration(time_start) << "ms \n";
    
    delete schema;
    
    cout << "\nValueWTA:\n";
    vwta->print();
    cout << *vwta;
    
    //    cout << "\n==== Clean ValueWTA:\n";
    //    time_start = clock();
    //    vwta->clean();
    //    cout << "time to clean ValueWTA : ";
    //    cout << duration(time_start) << "ms \n";
    //    vwta->print();
    //    cout << *vwta;
    
    
    
    // test k-best for initial states of ValueWTA
    int k = 100;
    time_start = clock();
    Ktable<WeightMin> kkt = Ktable<WeightMin>(vwta);
    
    cout << "\n==== " << k << "-best equiv\n";
    for (int i = k; i > 0; i--)
    {
        Run r = kkt.best(i);
        if (! r.unknown())
        {
            RhythmTree* t = kkt.tree(r);
            cout << i << "-best: ";
            //cout << kkt.tree(r)->to_string();
            cout << t->to_string();
            cout << " ";
            cout << t->lily(4);
            cout << " ";
            cout << " (weight=" << r.weight << ")";
            cout << " " << r.duration << "\n";
        }
    }
    
    cout << "\ntime for " << k << "-best for ValueWTA : ";
    cout << duration(time_start) << "ms \n";
    
    cout << "\n==== LilyPOnd output for " << k << "-best equiv\n";
    ofstream file;
    string filename = string(argv[2])+".ly";
    file.open(filename, ios_base::out);
    if(!file.is_open())
    {
        cerr << "cannot open file "+filename+"\n";
        return 1;
    }
    
    // try 1-best (for duration list)
    Run r0 = kkt.best(1);
    if (! r0.unknown())
    {
        // LilyPond header
        file << "\\header{ \n";
        file << "  title = \"rhythm notation of value ";
        file << r0.duration;
        file << " (";
        file << argv[1];
        file << ")";
        file << "\"\n";
        file << "}\n";
        file << "\n";
        
        // beginning of score
        file << "\\score {\n";
        file << "<<\n";
        file << "\\new RhythmicStaff {\n";
        file << "\\time 1/4\n";

        // one best sol per bar
        for (int i = 1; i <= k; i++)
        {
            Run r = kkt.best(i);
            if (! r.unknown())
            {
                RhythmTree* t = kkt.tree(r);
                file << t->lily(4);
                file << "\n";
            }
        }

        // end of score
        file << "}\n";
        file << ">>\n";
        file << "}\n";
        file << "\n";
        file << "\\version \"2.18.2\"\n";
    }
    
    file.close();
    
    delete vwta;
    
    return 0;
}
