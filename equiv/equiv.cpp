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
#include <assert.h>
#include <iostream>
#include <string>
#include <vector>

#include "ValueWTA.hpp"
#include "Kbest.hpp"
#include "DurationList.hpp"
#include "Onsets.hpp"
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
    
    // read rhythmic value (2 options)
    DurationList seg;

    if (argc == 2) // schema
    {
        // nothing to do
    }
    else if (argc == 3) // schema, rythmic value
    {
        std::cout << "\n==== Read rhythmic value from " << argv[2] << ".txt\n";
        string valuename = string(argv[2])+".txt";
        seg = DurationList(valuename);
        std::cout << "length rhythmic value: " << seg.size() << '\n';
    }
    else if (argc == 4) // schema, first rythmic value, second rythmic value
    {
        std::cout << "\n==== Merge rhythmic values from " << argv[2] << ".txt";
        std::cout << " and " << argv[3] << ".txt\n";
        string valuename1 = string(argv[2])+".txt";
        Onsets on1 = Onsets(DurationList(valuename1));
        string valuename2 = string(argv[3])+".txt";
        Onsets on2 = Onsets(DurationList(valuename2));
        Onsets on = on1+on2;
        seg = (on1+on2).ioi();
        cout << "merge=" << seg << "\n";
    }
    else
    {
        cout << "usage: equiv schema.txt or \n";
        cout << "       equiv schema.txt IOIlist or \n";
        cout << "       equiv schema.txt IOIlist1 IOIlist2 \n";
        return 0;
    }
    
    Ktable<WeightMin>* kkt = NULL;
    
    if (argc == 2) // enumeration of schema
    {
        kkt = new Ktable<WeightMin>(schema);
    }
    else if ((argc == 3) || (argc == 4)) // enumeration of schema inter rhythmic values
    {
        // test ValueWTA construction
        std::cout << "\n==== Construction ValueWTA\n";
        time_start = clock();
        ValueWTA* vwta = new ValueWTA(seg, *schema);
        cout << "time to compute ComboWTA : ";
        cout << duration(time_start) << "ms \n";
        
        // delete schema;
        
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

        assert (vwta);
        kkt = new Ktable<WeightMin>(vwta);
    }
    assert (kkt);
    
    // test k-best for initial states of ValueWTA
    int k = 100;
    time_start = clock();
    
    cout << "\n==== " << k << "-best equiv\n";
    for (int i = k; i > 0; i--)
    {
        Run r = kkt->best(i);
        if (! r.unknown())
        {
            RhythmTree* t = kkt->tree(r);
            cout << i << "-best: ";
            //cout << kkt.tree(r)->to_string();
            cout << t->to_string();
            cout << "\n";
            cout << "        ";
            cout << t->lily(4);
            cout << "\n";
            cout << "        (weight=" << r.weight << ")";
            cout << " " << r.duration << "\n";
        }
    }
    
    cout << "\ntime for " << k << "-best for ValueWTA : ";
    cout << duration(time_start) << "ms \n";
    
    cout << "\n==== LilyPOnd output for " << k << "-best equiv\n";
    string schemafile = string(argv[1]);
    size_t lastindex = schemafile.find_last_of(".");
    string schemaprefix = schemafile.substr(0, lastindex);
    string filename;
    
    if (argc == 2)
    {
        filename = schemaprefix+".ly";
    }
    else if (argc == 3)
    {
        filename = string(argv[2])+"-"+schemaprefix+".ly";
    }
    else if (argc == 4)
    {
        filename = string(argv[2])+"+"+string(argv[3])+"-"+schemaprefix+".ly";
    }

    cout << "write output to file "+filename+"\n";
    ofstream file;
    file.open(filename, ios_base::out);
    if(!file.is_open())
    {
        cerr << "cannot open file "+filename+"\n";
        return 1;
    }
    
    // try 1-best (for duration list)
    Run r0 = kkt->best(1);
    if (! r0.unknown())
    {
        // LilyPond header
        file << "\\header{ \n";
        file << "  title = \"rhythm notations of value ";
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
            Run r = kkt->best(i);
            if (! r.unknown())
            {
                RhythmTree* t = kkt->tree(r);
                // without dots
                file << t->lily(4);
                // file << "\n";
                // with dots (test)
                string d = t->lilydot(4);
                if (t->dot_flag) { file << "\n" << d; }
                file << " \\bar \"||\" ";
                file << "\n";
                //file << "}\n";
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
    
    // TODO delete kkt->_wta
    if (kkt) { delete kkt; }
    delete schema;
    
    return 0;
}
