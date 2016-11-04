//
//  ComboWTA.cpp
//  qparse
//
//  Created by Florent Jacquemard on 11/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//

#include "ComboWTA.hpp"
//#include "Distance.hpp"


ComboState::ComboState():cs_state(0), cs_rp(0), cs_rr(0)
{
    cs_path = new AlignmentTree();
}


ComboState::ComboState(State s, AlignmentTree* t, size_t rp, size_t rr):
cs_state(s),
cs_path(t),
cs_rp(rp),
cs_rr(rr)
{
    assert(t);
    rehash();
}


ComboState::ComboState(const ComboState& cs, size_t rp, size_t rr):
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
    // cs_path deleted with _tree (avoid double delete)
    // delete cs_path;
}

void ComboState::rehash()
{
    assert(cs_path);
    _hash[0] = cs_state;
    _hash[1] = cs_path->root->begin();
    _hash[2] = cs_path->root->length();
    _hash[3] = cs_rp;
    _hash[4] = cs_rr;
}

bool ComboState::operator==(const ComboState& s) const
{   for (int i = 0; i < _hash_len; i++)
    {
        if (_hash[i] != s._hash[i]) return false;
    }
    return true;
}

// lexicographic comparison on hash value (size_t[5])
bool ComboState::operator<(const ComboState& s) const
{
    for (int i = 0; i < _hash_len; i++)
    {
        if (_hash[i] > s._hash[i]) return false;
        else if (_hash[i] < s._hash[i]) return true;
        //otherwise _hash[i] == s._hash[i] : continue
    }
    return false;   //they are equal
}


std::ostream& operator<<(std::ostream& o, const ComboState& cs)
{
    o << "< " << cs.cs_state << " ";
    o << "[" << cs.cs_path->root->begin() << "-";
    o << cs.cs_path->root->end() << "] ";
    o << cs.cs_rp << " ";
    o << cs.cs_rr << ">";
    return o;
}



bool ComboWTA::compatible(State label, const ComboState& cs, const Alignment* p)
{
    return(
    (
     (
      // if it is a continuation (tie): there must be no point aligned to the left (of this Path)
      Label::continuation(label) &&
      (cs.cs_rp == 0) &&   // no point of previous Path aligned to left
      (p->l_size() == 0)  // no point of this Path aligned to left
     )
     ||
      // if it contains note and grace notes (no continuation):
      // the label must be consistent with
      // the info in guess (# grace notes)
     (
      (! Label::continuation(label)) &&
      (Label::nbGraceNotes(label) == (cs.cs_rp + p->l_size() - 1))
     )
    )
    &&
    // and check the info in current Path (# points aligned on right)
    // (must be true in both cases (continuation or not)
    (cs.cs_rr == p->r_size()));
}


State ComboWTA::addComboState(const ComboState& cs, bool initial)
{
    if (TRACE_LEVEL > 1) { cout << "combo state: " << cs << " "; }

    // combo state found in map: combo state has been treated already
    map<ComboState, State>::const_iterator it = _statemap.find(cs);
    if (it != _statemap.end())
    {
        if (TRACE_LEVEL > 1) { cout << " = state " << it->second << " (old)\n"; }
        return it->second;
    }

    // otherwise, create new State
    State s = _cpt++;
    if (TRACE_LEVEL > 1)
    {
        cout << " = state " << s;
        if (initial) { cout << "*"; }
    }
    
    // add map of current ComboState to new State
    _statemap[cs] = s;
    //register state
    TransitionList& tv = add(s, initial); // empty vector of Transitions
    
    // add transitions to new state s

    State q = cs.cs_state;              // current state in schema
                                        // cs.cs_path is current node in tree
    Alignment* p = cs.cs_path->root;    // current Path (Alignment)

    if (TRACE_LEVEL > 1)
    {
        if (p->habited()) cout << " (hab)\n";
        else cout << " (nhab)\n";
    }

    // enumerate the transitions to q in schema
    for (TransitionList_const_iterator it = _schema.begin(q);
         it != _schema.end(q); ++it)
    {
        const Transition& t = *it;    // transition of schema
        assert(t.inner() || t.terminal());
        size_t a = t.size();          //  arity of schema transition
        assert (a >= 1);
        const Weight& w = t.weight(); // weight of schema transition

        // leaf schema transition:
        // add at most one leaf transition to ComboWTA
        if (t.terminal())
        {
            assert (a == 1);
            State label = t.label(); // for such transition, the body is a singleton containing a label

            if (TRACE_LEVEL > 1)
            {
                cout << "lab=" << label << " ";
                cout << "#gn=" << Label::nbGraceNotes(label) << " ";
                cout << "cs.rp=" << cs.cs_rp << " ";
                cout << "seg_llen=" << p->l_size() << " ";
                cout << "cs.rr=" << cs.cs_rr << " ";
                cout << "seg_rlen=" << p->r_size() << "  ";
            }
            if (compatible(label, cs, p))
            {
                if (TRACE_LEVEL > 1) { cout << "YES" << "\n"; }
                // compute distance to input segment
                Distance dist = Distance(*p);  //ComboWeight* cw = new ComboWeight(w, dist);
                dist.combine(w);
                // add terminal transition from (leaf) label to s
                Transition newt = Transition(label, dist);
                assert(newt.terminal());
                tv.add(newt);
            }
            // otherwise add no transition
            else {  if (TRACE_LEVEL > 1) { cout << "NO" << "\n"; } }
        }

        // inner schema transition:
        // add zero or several transitions to ComboWTA (acc. to guesses for rr values)
        // do not descent if there are no point in Alignement
        else if (t.inner() && p->habited())
        {
            assert (a > 1);
            // compute vector of children alignements
            const vector<AlignmentTree*>& vp = cs.cs_path->children(a);             //vector<Alignment*> vp = p->subs(a);
            assert (vp.size() == a);
            // conditions: rr must be propagated from target cs to rightmost child
            // i.e. rr_a = rr
            size_t maxrr = vp[a-1]->root->r_size(); // rsize for the last element of vp
            if (cs.cs_rr <= maxrr) // is the max possible rr for rightmost child (propagated rr)
            {
                // the weight is a combination of null distance and the weight w of the original schema transition
                // It is the same for all the ComboWTA transitions
                // -> TODO shared pointers?
                //Distance cw = Distance(w);

                // this aux. vector will store all the possible values
                // of the a-1 first rr of child ComboStates,
                // between 0 and the max allowed rr value defined in vp (r_size)
                //size_t rrs[a-1];
                vector<size_t> rrs(a-1);
                // initialized to null vector
                for (int i = 0; i < a-1; i++)
                    rrs[i] = 0;
                
                // enumeration of all the possible rrs vectors (of length a-1)
                // and addition of one transition for each one
                bool cont = true;
                while (cont)
                {
                    // construct and add a new transition defined by the current rrs vector and computed Alignment's
                    Transition newt = Transition(Distance(w)); // new transition for ComboWTA
                    size_t rp = cs.cs_rp; // propagation of rp from target cs to leftmost child
                    for(int i = 0; i < a-1; i++)
                    {
                        // recursive registration of new ComboState i of transition
                        // cannot be initial (TODO AV)
                        State news = addComboState(ComboState(t.at(i), vp[i], rp, rrs[i]));
                        newt.add(news);
                        rp = rrs[i]; // next rp is current rr
                    }
                    // recursive registration of last ComboState of transition
                    State news = addComboState(ComboState(t.at(a-1), vp[a-1], rp, cs.cs_rr));
                    newt.add(news);
                    assert(newt.inner());
                    assert(newt.size() == a);
                    tv.add(newt);

                    // construct next rrs vector (of length a-1)
                    for (int i = 0; ; )
                    {
                        rrs[i]++;
                        if (rrs[i] > vp[i]->root->r_size()) // we reach max value for rr[i]
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


ComboWTA::ComboWTA(const Segment& seg, const WTA& schema, size_t rp):
_schema(schema)
{
    _statemap.clear();
    _cpt = 0;
    Alignment* full = new Alignment(seg);
    _tree = new AlignmentTree(full);
    
    // construction of initial ComboState's
    // with:
    // - initial schema state
    // - Alignment covering the whole segment
    // - rr values between 0 and the number of points in second half of segment
    // - rp given
//    for (set<State>::iterator it = schema.initials.begin();
//         it != schema.initials.end(); ++it)
    for (State s : schema.initials)
    {
//        State s = *it;
        size_t max_rr = full->r_size();
        for (size_t rr = 0; rr <= max_rr; rr++)
        {
            // arbitrarily
            ComboState cs = ComboState(s, _tree, rp, rr);
            addComboState(cs, true);
            // the other ComboState's will be added recursively
        }
    }

    // end of construction
    // destroy all Alignments (Paths)
    delete _tree;
    // destroy all Combostates
    _statemap.clear();
}



