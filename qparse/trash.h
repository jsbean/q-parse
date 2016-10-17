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



//////////////
//   WTA
//////////////

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

