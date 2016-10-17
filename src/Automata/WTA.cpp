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

        float val;
        if (!(in >> val))
            break;
        Weight* w = new Weight(val);
        // add transition to the table
        assert (w);
        Transition* t = add(s, w);
        assert(t);
        // copy content of sl to body of new transition
        for(int i=0; i < body.size(); i++)
            t->add(body[i]);
        _cpt_tr++;
        _cpt_size += body.size();
        cout << "add " << s << "( " << body.size() << " ) " << w->value();
        cout << " size table = " << _table.size() << '\n';
    }
    file.close();
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


//vector<Transition*> WTA::addState(State s)
//{
//    // returns new Transition vector if s is not a key in the table
//    return (_table[s]); // new vector<Transition*>();
//}


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

