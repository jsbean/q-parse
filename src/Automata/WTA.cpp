//
//  WTA.cpp
//  qparse
//
//  Created by Florent Jacquemard on 04/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include "WTA.hpp"


Transition::Transition(State s, const Weight& w):_weight(w)
{
    assert (_body.empty());
    _body.push_back(s);
}

Transition::~Transition()
{
    _body.clear();
//    delete _weight;   // TBC free weight?
}


void Transition::add(State s)
{
    _body.push_back(s);
}

void Transition::set(const Weight& w)
{
    _weight = w;
}


size_t Transition::size() const
{
    return (_body.size());
}


State Transition::at(int i) const
{
    assert (0 <= i);
    assert (i < _body.size());
    return (_body[i]);
}


bool Transition::member(State s) const
{
    if (_body.size() == 0)
        return false;
    
    for (State i : _body)
        if (i == s) return true;
    
//    for (vector<State>::const_iterator i = _body.begin(); i != _body.end(); i++)
//        if (*i == s) return true;

    return false;
}


bool Transition::allin(const std::set<State>& e) const
{
    if (_body.size() == 1) return false; //singleton body contains terminal symbol
    
    for (vector<State>::const_iterator i = _body.begin(); i != _body.end(); ++i)
    {
        if (e.count(*i) == 0) return false;
    }
    return true;
}


bool Transition::nonein(const std::set<State>& e) const
{
    if (_body.size() == 1) return true; //singleton body contains terminal symbol
    
    for (vector<State>::const_iterator i = _body.begin(); i != _body.end(); ++i)
    {
        if (e.count(*i) != 0) return false;
    }
    return true;
}


//void Transition::dump(ostream& o)
//{
//    o << " ( ";
//    for(vector<State>::iterator i = _body.begin(); i != _body.end(); ++i)
//        o << *i << " ";
//    
//    o << ") " << _weight->value() << '\n';
//}


std::ostream& operator<<(std::ostream& o, const Transition& t)
{
    o << " ( ";
    for(Transition_const_iterator i = t._body.begin(); i != t._body.end(); ++i)
        o << *i << " ";
    
    o << ") " << t._weight.value();
    return o;
}




















TransitionList::~TransitionList()
{
    clear();
}


bool TransitionList::empty() const
{
    return (_table.empty());
}


size_t TransitionList::size() const
{
    return _table.size();
}


void TransitionList::add(const Transition& t)
{
    _table.push_back(t);
    _cpt_size += t.size() + 1;
    
    assert (_parent);
    _parent->_cpt_tr++;
    _parent->_cpt_size += (t.size() + 1);
}


void TransitionList::clear()
{
    //for(vector<Transition*>::iterator i = _table.begin(); i != _table.end(); ++i)
    //    delete *i;
    
    _table.clear();
}


void TransitionList::remove(State s)
{
    for(list<Transition>::iterator i = _table.begin(); i != _table.end(); i++)
    {
        Transition& t = *i;
        // we do not remove transition to terminal symbols
        if ((t.size() > 1) && (t.member(s)))
        {
            _cpt_size -= (t.size() + 1);
            _table.erase(i); // remove transition from vector (destructor called)
        }
    }
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

        //cout << "state " << s << "\n";
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
        // add transition to the table
        // copy content of vector body to new transition
        add(s, Transition(body, Weight(val)));
        cout << " - add tr. " << s << Transition(body, Weight(val)) << "\n";
    }
    file.close();
    initials = { 0 };
}


WTA::~WTA()
{
    for (std::map<State,TransitionList>::iterator i = _table.begin();
         i != _table.end(); ++i)
    {
        TransitionList tv = i->second;
        tv.clear();
    }
    // TBC destroy the TransitionList contents?
    _table.clear();
}



bool WTA::isRegistered(State s) const
{
    return (_table.count(s) > 0);
}


bool WTA::isInitial(State s) const
{
    return (initials.count(s));
}


TransitionList& WTA::add(State s, bool initial)
{
    // _table[s]:
    // if there is an entry for s, return it
    // if there is no entry for s, one is created with empty vector of transition (see stl::map)a
    TransitionList& tv = _table[s];
    tv._parent = this;
    if (initial) initials.insert(s);
    return(tv);
}


TransitionList& WTA::add(State s, const Transition& t, bool initial)
{
    TransitionList& tv = add(s, initial); // updates the counters _cpt_tr and _cpt_size
    tv.add(t);
    return tv;
}


void WTA::remove(State s)
{
    size_t s_nb = 0;   // number of transitions headed to s
    size_t s_size = 0; // sum of sizes of transitions headed to s
    
    // first traversal transition map:
    // to remove transition with s occurring in body
    // and to update size counters
    for (map<State,TransitionList>::iterator i = _table.begin();
         i != _table.end(); ++i)
    {
        State q = i->first;
        TransitionList& tv = i->second;

        // save for later the sizes of the TransitionList with head s
        if (q == s)
        {
            s_nb = tv.size();
            s_size = tv.fullsize();
        }
        // delete the transitions containing s in the TransitionList of q
        else
        {
            _cpt_tr -= tv.size();
            _cpt_size -= tv.fullsize();
            tv.remove(s);
            _cpt_tr += tv.size();
            _cpt_size += tv.fullsize();
        }
    }

    // the elements removed are destroyed,
    // destructor of transition list of s is called
    _table.erase(s);
    _cpt_tr -= s_nb;
    _cpt_size -= s_size;
    
    initials.erase(s);
}


TransitionList_const_iterator WTA::begin(State s) const
{
    map<State,TransitionList>::const_iterator i = _table.find(s);
    assert(i != _table.end());
    return i->second.begin();
}

TransitionList_const_iterator WTA::end(State s) const
{
    map<State,TransitionList>::const_iterator it = _table.find(s);
    assert(it != _table.end());
    return it->second.end();
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


TransitionList WTA::at(State s) const
{
    map<State,TransitionList>::const_iterator it = _table.find(s);
    assert(it != _table.end());
    return (it->second);
}


//Transition* WTA::at(State s, size_t i) const
//{
//    map<State,TransitionList>::const_iterator it = _table.find(s);
//    assert(it != _table.end());
//    assert(i < it->second.size());
//    return (it->second.at(i));
//}


unsigned int gcd(unsigned int a, unsigned int b)
{
    if( b == 0 )
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
    // start with copy of initial state set
    set<State>* from = new set<State>(initials);
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
            for (TransitionList_const_iterator it = begin(s);
                 it != end(s); ++it)
            {
                const Transition& t = *it;
                size_t a = t.size();
                if (a > 1) // exclude leaf transitions (to terminal symbol)
                {
                    res1 = lcm(res1, a);
                    // add states in the body of the transition to reach set
                    for (Transition_const_iterator i = t.begin(); i != t.end(); i++)
                        reach->insert(*i);
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


set<State> WTA::step(const set<State>& sin)
{
    set<State> sout; // empty set
    assert (sout.empty());
    // for all state in given set
    for (set<State>::iterator is = sin.begin(); is != sin.end(); ++is)
    {
        State s = *is;
        // for all transition headed by the current state
        for (TransitionList_const_iterator it = begin(s); it != end(s); ++it)
        {
            const Transition& t = *it;
            if (t.size() > 1) // exclude leaf transition (to terminal symbol)
            {
                // for all state in the body of the transition
                for (Transition_const_iterator i = t.begin(); i != t.end(); i++)
                    sout.insert(*i);
            }
        }
    }
    return sout;
}

//set of all states occuring in wta (in head or body)
set<State> WTA::allstates()
{
    set<State> res;
    
    for (map<State, TransitionList>::iterator i = _table.begin();
         i != _table.end(); ++i)
    {
        
        res.insert(i->first);
        for (TransitionList_const_iterator it = (i->second).begin();
             it != (i->second).end(); ++it)
        {
            const Transition& t = *it;
            if (t.size() > 1)
            {
                for (Transition_const_iterator is = t.begin();
                     is != t.end(); is++)
                    res.insert(*is);
            }
        }
    }

    return res;
}


void WTA::clean()
{
    // first determine the set of empty states

    set<State> empty = allstates();

    
    bool change = true;
    while(change)
    {
        change = false;
        // for all state s
        for (map<State,TransitionList>::iterator i = _table.begin();
             i != _table.end(); ++i)
        {
            State s = i->first;
            // the state is already marked nonempty
            if (empty.count(s) == 0) continue;
            // otherwise try to mark s
            // for all transition headed by s
            for (TransitionList_const_iterator it = (i->second).begin();
                 it != (i->second).end(); ++it)
            {
                const Transition& t = *it;
                // transition from terminal symbol or from a body of all nonempty states
                if ((t.size() == 1) || (t.nonein(empty)))
                {
                    empty.erase(s); // in this case s is not empty
                    change = true;
                }
            }
        }
    }

    // next erase empty states
    
    for (std::set<State>::iterator i = empty.begin();
         i != empty.end(); ++i)
        remove(*i);
}


//void WTA::dump(ostream& o)
//{
//    for (std::map<State,TransitionList>::iterator i = _table.begin();
//         i != _table.end(); ++i)
//    {
//        State s = i->first;
//        TransitionList tv = i->second;
//        o << s;
//
//        for(TransitionList_const_iterator j = tv.begin(); j != tv.end(); j++)
//        {
//            Transition* t = *j;
//            assert (t);
//            t->dump(o);
//            o << '\n';
//        }
//    }
//}

std::ostream& operator<<(std::ostream& o, const WTA& a)
{
    for (std::map<State,TransitionList>::const_iterator i = a._table.begin();
         i != a._table.end(); ++i)
    {
        State s = i->first;
        TransitionList tv = i->second;
        for(TransitionList_const_iterator j = tv.begin(); j != tv.end(); j++)
        {
            const Transition& t = *j;
            o << s << t << " \n";
        }
    }
    return o;
}


void WTA::print()
{
    cout << '\n';
    cout << this->countStates() << " states\n";
    cout << this->countTransitions() << " transitions\n";
    cout << this->countAll() << " total symbols\n\n";
    cout << *this;
}

void WTA::save(string filename)
{
    ofstream file;
    
    file.open(filename, ios_base::out);
    if(!file.is_open())
        throw "cannot open file";
    
    file << *this;
    
    file.close();
}


