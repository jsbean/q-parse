//
//  WTA.cpp
//  qparse
//
//  Created by Florent Jacquemard on 04/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include "WTA.hpp"

Transition::Transition(Weight w):_weight(w){}

void Transition::add(State s)
{
    _body.push_back(s);
}

void Transition::setWeight(Weight w)
{
    _weight = w;
}

size_t Transition::size() const
{
    return (_body.size());
}


State Transition::antecedent(size_t i)
{
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
        istringstream in(line);   //make a stream from the line
        
        // skip initial spaces
        //for(i=0; (i < line.size()) && (line[i] = ' '); i++);
        //if ((i >= line.size()) || (!isdigit(line[i])))  break;

        
        // process 1 line of the form "s (s0 ... sn) w"
        
        // extract s
        State s;
        if (!(in >> s))
            break;

        // extractc '('
        string buf;
        in >> buf;
        if (buf != "(")
            break;

        // extract s0 ... sn into vector body
        getline(in, buf, ')');
        istringstream ins(buf);
        vector<State> body;
        State q;
        while (ins >> q)
            body.push_back(q);

        float w;
        if (!(in >> w))
            break;

        // add transition to the table
        add(s, body,w);
    }
    file.close();
}


void WTA::dump(ostream& o)
{
    for (std::map<State,vector<Transition*>*>::iterator i = _table.begin();
         i != _table.end(); i++)
    {
        o << i->first << " ( ";
        vector<Transition*>* tv = i->second;
        for(size_t j = 0; j < tv->size(); j++)
        {
            Transition* t = tv->at(j);
            for(size_t k = 0; k < t->size(); k++)
                o << t->antecedent(k) << " ";
            
            o << ") " << (t->weight()).value() << '\n';
        }
        o << '\n';
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


vector<Transition*>* WTA::addState(State s)
{
    // returns new Transition vector if s is not a key in the table
    return (_table[s]); // new vector<Transition*>();
}


bool WTA::registered(State s) const
{
    return (_table.count(s));
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


// add a new transition for state s.
// if s is not registered, it is added to the table.
// return the index of this transition in the list of transitions of head s.
size_t WTA::add(State s, Weight w)
{
    vector<Transition*>* tv = _table[s];
    assert (tv != NULL);
    Transition* t = new Transition(w);
    tv->push_back(t);
    _cpt_tr++;
    _cpt_size++; // for the target of transition
    return(tv->size()-1);
}

// add(s, i, q) add the state q at the end of the body of the ith transition of head s.
// s must be a registered state and have at least i transitions
void WTA::add(State s, size_t i, State q)
{
    assert (registered(s));
    vector<Transition*>* tv = _table[s];
    assert (tv);
    assert (i < _table.size());
    Transition* t = tv->at(i);
    assert (t);
    _cpt_size++;
    t->add(q);
}

void WTA::add(State s, vector<State> sl, Weight w)
{
    vector<Transition*>* tv = _table[s];
    assert (tv != NULL);
    Transition* t = new Transition(w);
    // copy content of sl to body of new transition
    for(int i=0; i < sl.size(); i++)
        t->add(sl[i]);
    _cpt_tr++;
    _cpt_size += sl.size();
    tv->push_back(t); // add transition to transitions of s
}

Transition* WTA::at(State s, size_t i) 
{
    assert (registered(s));
    vector<Transition*>* tv = _table[s];
    assert(i < tv->size());
    return(tv->at(i));
}


