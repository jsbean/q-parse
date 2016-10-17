//
//  ComboWTA.cpp
//  qparse
//
//  Created by Florent Jacquemard on 11/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include "ComboWTA.hpp"

#include "ComboWeight.hpp"


ComboState::ComboState():cs_state(0), cs_rp(0), cs_rr(0)
{
    cs_path = new Alignment();
}


ComboState::ComboState(State s, Alignment* p, int rp, int rr):
cs_state(s),
cs_path(p),
cs_rp(rp),
cs_rr(rr)
{
    assert(p);
    rehash();
}


ComboState::ComboState(const ComboState& cs, int rp, int rr):
cs_state(cs.cs_state),
cs_path(cs.cs_path),
cs_rp(rp),
cs_rr(rr)
{
    assert(cs_path);
    rehash();
}

ComboState::~ComboState()
{
    delete cs_path;
}

void ComboState::rehash()
{
    assert(cs_path);
    _hash[0] = cs_state;
    _hash[1] = cs_path->begin();
    _hash[2] = cs_path->length();
    _hash[3] = cs_rp;
    _hash[4] = cs_rr;
}

bool ComboState::operator==(const ComboState& s) const
{   for (int i = 0; i < 5; i++)
{
    if (_hash[i] != s._hash[i]) return false;
}
    return true;
}

// lexicographic comparison on hash value (unsigned int[5])
bool ComboState::operator<(const ComboState& s) const
{
    for (int i = 0; i < 5; i++)
    {
        if (_hash[i] > s._hash[i]) return false;
        else if (_hash[i] < s._hash[i]) return true;
        //otherwise _hash[i] == s._hash[i]
    }
    return false;   //they are equal
}



State ComboWTA::addState(ComboState* cs)
{
    assert(cs);
    map<ComboState, State>::const_iterator it = _statemap.find(*cs);

    // combo state found in map: combo state has been treated already
    if (it != _statemap.end())
        return it->second;

    // otherwise, create new State
    State s = _cpt++;
    // add map of current ComboState to new State
    _statemap[*cs] = s;

    // add transitions to new state s

    State q = cs->cs_state;      // current state in schema
    Alignment* p = cs->cs_path;  // current Path (Alignment)

    // enumerate the transitions to q in schema
    for (vector<Transition*>::const_iterator it = _schema->begin(q); it != _schema->end(q); ++it)
    {
        Transition* t = *it;         // transition of schema
        unsigned int a = t->size(); // transition arity
        assert (a >= 1);
        Weight* w = t->weight();    // weight of schema transition

        // leaf schema transition:
        // add at most one leaf transition to ComboWTA
        if (a == 1)
        {
            State label = t->at(0); // for such transition, the body is a singleton containing a label

            // check that the label coincide with
            // the info in guess (# grace notes)
            // and the info in current Path (# points aligned on right)
            if ((Label::nbGraceNotes(label) ==
                (cs->cs_rp + p->l_size() - 1)) &&
                (cs->cs_rr == p->r_size()))
            {
                // compute distance to input segment
                Distance* dist = new Distance(cs->cs_path);
                ComboWeight* cw = new ComboWeight(w, dist);
                assert(cw);
                Transition* t = add(s, cw);
                assert(t);
                // add terminal transition to (leaf) label
                t->add(label);
            }
            // otherwise add no transition
        }

        // inner schema transition:
        // add zero or several transitions to ComboWTA (acc. to guesses for rr values)
        else if (a > 1)
        {
            // compute vector of children alignements
            vector<Alignment*> vp = p->subs(a);
            assert (vp.size() == a);
            // conditions: rr must be propagated from target cs to rightmost child
            // i.e. rr_a = rr
            int rr = vp[a-1]->r_size(); // rr of the last element of vp
            if (cs->cs_rr <= rr)        // is the max possible rr for rightmost child (propagated rr)
            {
                // the weight is the same for all the ComboWTA transitions (sharing)
                // it depends only on the weight of the original schema transition
                ComboWeight* cw = new ComboWeight(w);
                assert(cw);

                // this aux. vector will store all the possible values
                // of the a-1 first rr of child ComboStates,
                // between 0 and the max allowed rr value defined in vp (r_size)
                unsigned int rrs[a-1];
                // initialized to null vector
                for (int i = 0; i < a-1; i++)
                    rrs[i] = 0;
                
                // enumeration of all the possible rrs vectors (of length a-1)
                // and addition of one transition for each one
                bool cont = true;
                while (cont)
                {
                    // construct and add a new transition defined by the current rrs vector and computed Alignment's
                    Transition* newt = add(s, cw); // new transition for ComboWTA
                    assert(newt);
                    unsigned int rp = cs->cs_rp; // propagation of rp from target cs to leftmost child
                    for(int i = 0; i < a-1; i++)
                    {
                        // ComboState i of transition
                        ComboState* newcs = new ComboState(t->at(i), vp[i], rp, rrs[i]);
                        State news = addState(newcs); // recursive registration of new ComboState
                        newt->add(news);
                        rp = rr;
                    }
                    // last ComboState of transition
                    ComboState* newcs = new ComboState(t->at(a-1), vp[a-1], rp, rr);
                    State news = addState(newcs); // recursive registration of last ComboState
                    newt->add(news);

                    // construct next rrs vector (of length a-1)
                    for (int i = 0; ; )
                    {
                        rrs[i]++;
                        if (rrs[i] == vp[i]->r_size()) // we reach max value for rr[i]
                        {
                            if (i+1 == a-1)   // we have constructed all possible rrs
                            {
                                cont = false; // break the while loop
                                break;
                            }
                            else
                            {
                                rrs[i] = 0;
                                i++;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                } // while flag loop
            }
            // else (rr too big) do not add transitions
        } // end inner transition
    }
    return s;
}


ComboWTA::ComboWTA(const Segment& seg, const WTA* schema)
{
    assert(schema);
    _schema = schema;
    
    
    // initial ComboStates
    // TODO
    // addState(init_cs);
}
