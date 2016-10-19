//
//  ComboWTA.hpp
//  qparse
//
//  Created by Florent Jacquemard on 11/10/16.
//  Copyright © 2016 Florent Jacquemard. All rights reserved.
//
//
// WTA combo
//
// labels comme schema ci-=dessus
// i.e. continuation or number of note + grace notes at left of the current path
//
// states (q:int, p:Path, rp:int list, rr:int list) ou label (feuille)
// q: state schema de base
// p: current path (interval of points)
// rp: guess number of points aligned to right of previous segment
// rr: guess number of points aligned to right of current segment
//
// transitions
// construction  top-down, connaissant l’input et le schema
//
// principe:
// - rp se propage du pere au leftmost child
// - rr se propage du pere au rightmost child
// - pour 2 états s1, s2 sibling successifs s1.rr = s2.rp
//
// given q state of schema, p path, k ≤ max{ n | q -> q1,...,qn | w transition of schema}
// mright(q, p, k) = # point d’input dans la derniere 2k partie de p
//
// on commence avec les états (q0, p0, rp0, rr0) (en file d’attente)
// q0 = état initial du schema
// p0 = [0,...,RES-1]
// rp0 arbitraire (donnée du problème = arbre précédent). typiquement 0.
// rr0 in [0, mright(q, p, min{ n > 1 | q0 → q1,…,qn | w transition of schema} )
//        
//        pour tout état (q, p, rp, rr)  sorti de la file d’attente
//        pour toute transition (q,q1,…,qk,w) du schema
//        
//        si k=1 i.e. q1 est un label  (feuille)
//        tel que
//        - q1 = rp + lalign(p)
//        - rr = ralign(p)
//        alors on ajoute la transition
//        (q, p, rp, rr) -> q1 | weight(w, dist(input,p))
//        
//        si k > 1  (inner node)
//        et rr in [0..mright(q, p, k)]
//        on ajoute les transitions
//        (q, p, rp, rr) -> (q1, p1, rp1, rr1),..., (qn, pk, rpk, rrk) | weight(w, 0)
//        tels que
//        - p1,...,pk forme une k-partition de p (qui doit être divisible par k)
//        - rrk = rr
//        - rp1 = rp
//        - pour tout 1 ≤ i < k, rri = rpi+1
//        
//        on ajoute les états (q1, p1, rp1, rr1),..., (qn, pk, rpk, rrk) en file d’attente
//        
//        REM: avec la distance à 0, on fait simplement la somme des distances des children
//        
//  pour un interval (path) p et un input,
//        lalign(p) = # de points de input alignés à gauche de p
//        ralign(p) = # de points de input alignés à droite de p
//  évalués par des bitset operations avec des mask ?
//  (avec ll = lalign(p) et rr = ralign(p))
//




#ifndef ComboWTA_hpp
#define ComboWTA_hpp

#include <stdio.h>
#include <assert.h>
#include <array>
#include <queue>
#include <map>

#include "WTA.hpp"
#include "Label.hpp"

#endif /* ComboWTA_hpp */

using namespace std;


class Weight;
class Distance;
class ComboWeight;
class Alignment; // Path
class Segment;
class Label;


// tmp state structure for construction of ComboWTA
// casted into unsigned int (state index)
class ComboState
{
public:

    ComboState();
    
    ComboState(State, Alignment*, int rp=0, int rr=0);

    // NOT USED? TODO: REM
    ComboState(const ComboState&, int rp=0, int rr=0);

    ~ComboState();
    
    // state of base schema
    State cs_state;

    // current augmented path (interval of points + alignment of input segment)
    Alignment* cs_path;
    
    // guess number of points aligned to right of previous segment
    unsigned int cs_rp;
    
    // guess number of points aligned to right of current segment
    unsigned int cs_rr;
    
    // cs_key const key();
    
    bool operator==(const ComboState& s) const;
    
    // lexicographic comparison on hash value (array[5])
    bool operator<(const ComboState& s) const;
    
    
private:
    //cs_key _hash;
    unsigned int _hash[5];
    
    static int const _hash_len = 5;
    
    void rehash();
};





class ComboWTA: public WTA
{
public:
    
    // construction from input segment and WTA (base schema)
    ComboWTA(const Segment&, const WTA&, unsigned int rp=0);
   
private:
    // Global variables for the ComboWTA construction
    
    WTA _schema;
    
    // counter for new ComboWTA states
    State _cpt;

    // map of ComboState into their index in ComboWTA
    // tmp (only for constructor)
    //unordered_map<cs_key, State, container_hasher> _statemap;
    map<ComboState, State> _statemap;
    
    
    // toState(cs) returns the ComboWTA state associated to the CoboState cs if there is one
    // otherwise:
    //     a new ComboWTA state s is created,
    //     s is associated to cs,
    //     s is added to the table
    //     the ComboWTA transitions with head s are computed and added to the table
    //     r is returned
    // moreover, the state is added to the initial state set of the ComboWTA if initial=true
    State addComboState(ComboState*, bool initial=false);

};
