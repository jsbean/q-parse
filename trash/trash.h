//
//  trash.h
//  qparse
//
//  Created by Florent Jacquemard on 17/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#ifndef trash_h
#define trash_h


#endif /* trash_h */


// test schema convertion
dagSchema sche = dagSchema();
sche.add(ds_transition(0, 2, 2));
sche.add(ds_transition(2, 2, 3));
sche.add(ds_transition(2, 3, 3));
sche.add(ds_transition(2, 5, 3));
sche.add(ds_transition(3, 3, 1));

sche.rename(2, 4);



// ENUM


// just for the sample -- print the data sets
std::ostream& operator<<(std::ostream& os, const std::vector<int>& vi)
{
    os << "( ";
    std::copy(vi.begin(), vi.end(), std::ostream_iterator<int>(os, " "));
    os << ")\n";
    return os;
}


void product(std::vector<int> b, size_t len)
{
    std::vector<int> result;
    
    // initial result = null vector
    for (int i = 0; i < len; i++)
        result.push_back(0);
    
    while (true)
    {
        std::cout << result;
        for (int i = 0; ; )
        {
            result[i]++;
            if (result[i] == b[i]) // we reach max value for i
            {
                if (i+1 == len) // last digit
                {
                    return;
                }
                else
                {
                    result[i] = 0;
                    i++;
                }
            }
            else
            {
                break;
            }
        }
    }
}

int main(int argc, const char * argv[])
{
    std::vector<int> bases = { 3, 3, 3 };
    product(bases, bases.size());
    return 0;
}




//////////////
//   Weight
//////////////



// generic compare
// returns
// -1 if this weight is smaller than the weight given
//  0 if this weight is equal to the weight given
//  1 if this weight is larger than the weight given
int Weight::compare(const Weight& w)
{
    if(_val < w._val)
        return -1;
    else if(_val == w._val)
        return 0;
    else return 1;
}





typedef struct _transition
{
    vector<State> tr_body;
    Weight tr_weight;
} Transition;


vector<Transition*>* WTA::getTrs(State s)
{
    vector<Transition*>* tv = _table[s];
    if (tv == NULL) // not found
    {
        tv = new vector<Transition*>;
        _table[s] = tv;
    }
    return tv;
}



// add(s, i, q) add the state q at the end of the body of the ith transition of head s.
// s must be a registered state with at least i transitions.
// DO NOT USE.
void WTA::add(State s, size_t i, State q)
{
    vector<Transition*>* tv = &_table[s];
    assert (tv);
    // if s was not registered it will fail because size _state[s] = 0
    assert (i < _table.size());
    Transition* t = tv->at(i);
    assert (t);
    _cpt_size++;
    t->add(q);
}


Transition* WTA::at(State s, size_t i) const
{
    return _table[s][i]; //not const
    //    vector<Transition*>* tv = &_table[s]; // not const
    //    assert(tv);
    //    assert(i < tv->size());
    //    return(tv->at(i));
}





//////////////
// ComboWTA
//////////////

//typedef struct _combostate
//{
//...    // state of base schema
//} ComboState;


// key for mapping of combo-states into their index in the constructed combo WTA
//typedef std::array<unsigned int, 5> cs_key;

struct cs_key
{
    unsigned int csk_state;
    int csk_pathbegin;
    int csk_pathend;
    int csk_rp;
    int csk_rr;
};



// hash function for unordered_map of ComboStates

void hash_combine(std::size_t& seed, std::size_t value) {
    seed ^= value + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

struct container_hasher {
    template<class T>
    std::size_t operator()(const T& c) const {
        std::size_t seed = 0;
        for(const auto& elem : c) {
            hash_combine(seed, std::hash<typename T::value_type>()(elem));
        }
        return seed;
    }
};



class ComboState
{
public:

    bool operator==(const ComboState& s) const
    {
        return((this->cs_state == s.cs_state)&&
               (this->cs_path->begin() == s.cs_path->begin())&&
               (this->cs_path->length() == s.cs_path->length())&&
               (this->cs_rp == s.cs_rp)&&
               (this->cs_rr == s.cs_rr));
    }
};


void ComboState::rehash()
{
    assert(cs_path);
    _hash.csk_state = cs_state;
    _hash.csk_pathbegin = cs_path->begin();
    _hash.csk_pathend = cs_path->end();
    _hash.csk_rp = cs_rp;
    _hash.csk_rr = cs_rr;
}


cs_key const ComboState::key()
{
    return (_hash);
}



ComboWTA::ComboWTA(const Segment& seg, const WTA* schema)
{
    assert(schema);
    _schema = schema;
    
    // FIFO of ComboStates to add
    //queue<ComboState*> _csqueue;
    
    // map of ComboState into their index in ComboWTA
    // tmp (only for constructor)
    //map<ComboState, State> _statemap;
    
    
    // initial ComboStates
    
    
    
    //    while (! _csqueue.empty())
    //    {
    //        ComboState* cs = _csqueue.front();
    //        _csqueue.pop();
    
    
}

// addState(s) returns a pointer to the transition vector of s.
// if s is not present in the table register it as new state (with empty vector of transitions)
// a pointer to the transition vector of s if it is registered
vector<Transition*>* addState(State);


vector<Transition*> WTA::addState(State s)
{
    // returns new Transition vector if s is not a key in the table
    return (_table[s]); // new vector<Transition*>();
}



Transition* WTA::add(State s, Weight* w, bool initial)
{
    assert (w);
    Transition* t = new Transition(w);
    assert (t);
    TransitionList* tv = add(s, t, initial);
    _cpt_tr++;
    _cpt_size++; // for the target of transition
    return t;
}


Transition* WTA::add(State s, vector<State> sl, Weight* w)
{
    assert (w);
    Transition* t = add(s, w);
    assert(t);
    // copy content of sl to body of new transition
    for(int i=0; i < sl.size(); i++)
        t->add(sl[i]);
    _cpt_tr++;
    _cpt_size += sl.size();
    return (t);
}


// size(s) return a pair made of
//   the number of transitions with head state s.
//   the sum of sizes of transitions with head state s.
std::pair<size_t,size_t> WTA::size(State s) const
{
    size_t nb = 0;
    size_t sum = 0;
    map<State,TransitionList>::const_iterator i = _table.find(s);
    
    // state not registered
    if (i == _table.end()) return std::pair<size_t,size_t>(nb, sum);
    
    // otherwise, sum transitions sizes
    for (TransitionList_const_iterator it = i->second.begin();
         it != i->second.end(); ++it)
    {
        nb++;
        sum += ((*it)->size()+1); // size body + target
    }
    return std::pair<size_t,size_t>(nb, sum);
}


class DurationList
{
public:
    
    // the Run's node is a leaf, labeled by tie
    bool isTie() { return  (_continuation.size() == 1) && _main.empty(); }
    
    // the Run's node is a leaf not labeled by tie (event or grace note)
    bool isEvent() { return  (_continuation.empty()) && _main.size() == 1; }
    
    bool hasTies() { return  (! _continuation.empty()); }

};



// test k-best for initial states of Combo
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

