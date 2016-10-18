//
//  WTA.cpp
//  qparse
//
//  Created by Florent Jacquemard on 04/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include "WTA.hpp"

#include "Weight.hpp"


Transition::Transition(Weight* w):_weight(w){}

Transition::~Transition()
{
    _body.clear();
}


void Transition::add(State s)
{
    _body.push_back(s);
}

void Transition::setWeight(Weight* w)
{
    assert (w);
    _weight = w;
}

size_t Transition::size() const
{
    return (_body.size());
}


State Transition::at(int i)
{
    assert (0 <= i);
    assert (i < _body.size());
    return (_body[i]);
}










WTA::WTA():_cpt_tr(0), _cpt_size(0)
{ }


WTA::WTA(string filename):_cpt_tr(0), _cpt_size(0)
{
    ifstream file;
    
    file.open(filename, ios_base::in);
    if(!file.is_open())
        throw "cannot open file";
    
    //read stream line by line
    for(string line; getline(file, line); )
    {
        // skip empty line
        if (line.size() == 0) continue;

        istringstream in(line);   //make a stream from the line
        
        // skip initial spaces
        //for(i=0; (i < line.size()) && (line[i] = ' '); i++);
        //if ((i >= line.size()) || (!isdigit(line[i])))  break;
        
        // process 1 line of the form "s (s0 ... sn) w"
        
        // extract s
        State s;
        if (!(in >> s)) continue; // parse error: skip line

        // extractc '('
        string buf;
        in >> buf;
        if (buf != "(") continue; // parse error: skip line

        // extract s0 ... sn into vector body
        getline(in, buf, ')');
        istringstream ins(buf);
        vector<State> body;
        State q;
        while (ins >> q)
            body.push_back(q);

        float val;
        if (!(in >> val)) continue; // parse error: skip line
        Weight* w = new Weight(val);
        // add transition to the table
        assert (w);
        Transition* t = add(s, w);
        assert(t);
        // copy content of sl to body of new transition
        size_t a = body.size();
        for(int i=0; i < a; i++)
        {
            State si = body[i];
            // register every read body state (harmless if already registered)
            if (a > 1) add(si);
            t->add(si);
        }
        _cpt_tr++;
        _cpt_size += body.size();
        cout << "add " << s << "( " << body.size() << " ) " << w->value();
        cout << " size table = " << _table.size() << '\n';
    }
    file.close();
    init = { 0 };
}

WTA::~WTA()
{
    for (std::map<State,vector<Transition*>>::iterator i = _table.begin();
         i != _table.end(); ++i)
    {
        vector<Transition*> tv = i->second;
        for (vector<Transition*>::iterator j = tv.begin(); j != tv.end(); ++j)
            delete *j;
        tv.clear();
    }
    _table.clear();
}

void WTA::dump(ostream& o)
{
    for (std::map<State,vector<Transition*>>::iterator i = _table.begin();
         i != _table.end(); ++i)
    {
        State s = i->first;
        vector<Transition*> tv = i->second;
        for(size_t j = 0; j < tv.size(); j++)
        {
            Transition* t = tv[j];
            assert (t);
            o << s << " ( ";
            for(size_t k = 0; k < t->size(); k++)
                o << t->at(k) << " ";
            
            o << ") " << (t->weight())->value() << '\n';
        }
    }
}

void WTA::print()
{
    cout << '\n';
    cout << this->countStates() << " states\n";
    cout << this->countTransitions() << " transitions\n";
    cout << this->countAll() << " total symbols\n\n";
    dump(cout);
}
               
void WTA::save(string filename)
{
    ofstream file;
    
    file.open(filename, ios_base::out);
    if(!file.is_open())
        throw "cannot open file";

    dump(file);
    
    file.close();
}



bool WTA::isRegistered(State s) const
{
    return (_table.count(s));
}


bool WTA::isInitial(State s) const
{
    return (init.count(s));
}





size_t WTA::countStates() const
{
    return _table.size();
}


size_t WTA::countTransitions() const
{
    return _cpt_tr;
}


size_t WTA::countAll() const
{
    return _cpt_size;
}


vector<Transition*>::const_iterator WTA::add(State s, bool initial)
{
    // _table[s]: if there is no entry for s, one is created with empty vector of transition (see stl::map)
    vector<Transition*>* tv = &_table[s];
    assert (tv);
    if (initial) init.insert(s);
    return(tv->begin());
}


Transition* WTA::add(State s, Weight* w)
{
    assert (w);
    // _table[s]: if there is no entry for s, one is created with empty vector of transition (see stl::map)
    vector<Transition*>* tv = &_table[s];
    assert (tv);
    Transition* t = new Transition(w);
    tv->push_back(t);
    _cpt_tr++;
    _cpt_size++; // for the target of transition
    return(t);
}


Transition* WTA::add(State s, vector<State> sl, Weight* w)
{
    assert (w);
    cout << "add " << s << "( " << sl.size() << " ) " << w->value();
    cout << " size table = " << _table.size() << '\n';
    Transition* t = add(s, w);
    assert(t);
    // copy content of sl to body of new transition
    for(int i=0; i < sl.size(); i++)
        t->add(sl[i]);
    _cpt_tr++;
    _cpt_size += sl.size();
    return (t);
}

vector<Transition*>::const_iterator WTA::begin(State s) const
{
    map<State,vector<Transition*>>::const_iterator it = _table.find(s);
    assert(it != _table.end());
    return it->second.begin();
    //return _table[s].begin(); // not const
}

vector<Transition*>::const_iterator WTA::end(State s) const
{
    map<State,vector<Transition*>>::const_iterator it = _table.find(s);
    assert(it != _table.end());
    return it->second.end();
    //return _table[s].end(); // not const
}

size_t WTA::size(State s) const
{
    map<State,vector<Transition*>>::const_iterator it = _table.find(s);
    assert(it != _table.end());
    return it->second.size();
    // return _table[s].size(); // not const
}

Transition* WTA::at(State s, size_t i) const
{
    map<State,vector<Transition*>>::const_iterator it = _table.find(s);
    assert(it != _table.end());
    assert(i < it->second.size());
    return (it->second.at(i));
}


unsigned int gcd(unsigned int a, unsigned int b)
{
    if( b==0 )
        return a;
    return gcd(b, a%b);
}


unsigned int lcm(unsigned int a, unsigned int b)
{
    return a*b / gcd(a,b);
}


// over approx.
// could be optimized
unsigned int WTA::resolution() const
{
    // start with initial states
    set<State>* from = new set<State>(init);
    // initialy empty
    set<State>* reach = new set<State>();

    unsigned int res = 1;

    while (! from->empty())
    {
        unsigned int res1 = 1;
        // for all state in reached set
        for (set<State>::iterator is = from->begin();
             is != from->end(); ++is)
        {
            State s = *is;
//            cout << "state: " << s << '\n';
            // for all transition headed by the current state
            for (vector<Transition*>::const_iterator it = begin(s);
                 it != end(s); ++it)
            {
                Transition* t = *it;
                assert(t);
                size_t a = t-> size();
                if (a > 1) // exclude leaf transitions (to terminal symbol)
                {
                    res1 = lcm(res1, a);
                    // add states in the body of the transition to reach set
                    for (int i = 0; i < a; i++)
                        reach->insert(t->at(i));
                }
            }
        }
        //cout << "res1 = " <<  res1 << "\n";
        set<State>* aux = from;
        from = reach;
        reach = aux;
        reach->clear();
        res *= res1;
    }
    delete from;
    delete reach;
    return res;
}


set<State> WTA::step(set<State>& sin)
{
    set<State> sout; // empty set
    assert (sout.empty());
    // for all state in given set
    for (set<State>::iterator is = sin.begin(); is != sin.end(); ++is)
    {
        State s = *is;
        // for all transition headed by the current state
        for (vector<Transition*>::const_iterator it = begin(s); it != end(s); ++it)
        {
            Transition* t = *it;
            assert(t);
            if (t->size() > 1) // exclude leaf transition (to terminal symbol)
            {
                // for all state in the body of the transition
                for (int i = 0; i < t->size(); i++)
                    sout.insert(t->at(i));
            }
        }
    }
    return sout;
}

