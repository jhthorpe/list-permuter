#ifndef _LIST_PERMUTER_LISTS_HPP_
#define _LIST_PERMUTER_LISTS_HPP_

#include <stdio.h>
#include <algorithm>
#include <vector>
#include <array>
#include <string>

// BASIC TYPES
//Types
// 0 -> pocc
// 1 -> aocc
// 2 -> avrt
// 3 -> pvrt
// 
// 4 -> spin-adapted pocc,pocc
// 5 -> spin-adapted aocc,pocc
// 6 -> spin-adapted aocc,aocc
// 7 -> spin-adapted avrt,avrt
// 8 -> spin-adapted pvrt,avrt
// 9 -> spin-adapted pvrt,apvrt
// 
// Spins
// 0 -> alpha
// 1 -> beta
// 2 -> spin-adapted 
//
// Electron
// 0,1,2,3,... indicates which electron this orb is for 
//
// Electron2
// Used to indicate a spin-adapted electron
//
// SPINADA
struct orb_type
{
    int spatial;
    int spin;
    int electron{0};
    int electron2{0};

    //integer identifiers for types
    const static int m{0}; 
    const static int i{1};
    const static int a{2};
    const static int e{3};

    //spin adapted tytpes
    const static int mn{4};
    const static int im{5};
    const static int ij{6};
    const static int ab{7};
    const static int ea{8};
    const static int ef{9};

    //Constructors
    orb_type() {}
    orb_type(const orb_type&) = default;
    orb_type(orb_type&&) = default;
    orb_type& operator=(const orb_type&) = default;
    orb_type& operator=(orb_type&&) = default;
    orb_type(int sa, int sp, int en=0, int en2=0)
    {
        spatial = sa;
        spin = sp;
        electron = en;
        electron2 = en2;
    }
    orb_type(std::initializer_list<orb_type>) {}

    bool operator==(const orb_type& other) const 
    {
        return (spatial == other.spatial) && 
               (spin == other.spin) &&
               (electron == other.electron) &&
               (electron2 == other.electron2); 
    }
    bool operator>(const orb_type& other) const {return (spatial > other.spatial);}
    bool operator<=(const orb_type& other) const {return (spatial <= other.spatial);}

};//end orb_type

/*
extern int orb_type::m;
extern int orb_type::i;
extern int orb_type::a;
extern int orb_type::e;
extern int orb_type::mn;
extern int orb_type::im;
extern int orb_type::ij;
extern int orb_type::ab;
extern int orb_type::ea;
extern int orb_type::ef;
*/

//Orbitals can be permuated if they correspond to the same electron, and if they are the
//an alpha/beta type (not spin-adapted or whatever)
bool can_permute(const orb_type& a, const orb_type& b)
{
    return (
               a.electron == b.electron &&
               (a.spin == 0 || a.spin == 1) &&
               (b.spin == 0 || b.spin == 1)
           );
}

//Orbitals can be spin adapted if they are both in the occupied/virtual space, 
//  and if they have alpha,beta or beta/alpha spin 
bool can_spinad(const orb_type& a, const orb_type& b)
{
    return (
               ((a.spatial == 0 || a.spatial == 1) && (b.spatial == 0 || b.spatial == 1)) ||
               ((a.spatial == 2 || a.spatial == 3) && (b.spatial == 2 || b.spatial == 3))
           )
           &&
           (
               (a.spin == 0 && b.spin == 1) || 
               (a.spin == 1 && b.spin == 0)    
           );
}

/*
 * A list of orbitals, in mulikan ordering: ex <Ab|Ij>  
 *
 * Note that it this list is assumed to be non-antisymmetric, and that the 
 * positions of the orbitals infer their electron number : ex <Ab|Ij> is (1,2,1,2) 
 * 
 */
struct list_type
{
    using strs_t = std::array<std::array<std::array<std::string, 4>,2>,4>;

    const static strs_t strs;

    std::array<orb_type, 4> orbs;

    list_type(const list_type&) = default;
    list_type(list_type&&) = default;
    list_type& operator=(const list_type&) = default;
    list_type& operator=(list_type&&) = default;

    list_type(orb_type p, orb_type q, orb_type r, orb_type s)
    {
        if (p.electron == 0) p.electron = 1;
        if (q.electron == 0) q.electron = 2;
        if (r.electron == 0) r.electron = 1;
        if (s.electron == 0) s.electron = 2;
        orbs = std::array<orb_type, 4>{p, q, r, s};    
    } 

    const auto& type(int i) const {return orbs[i];} 
    auto& type(int i) {return orbs[i];} 

    bool operator==(const list_type& other) const
    {
        return (type(0) == other.type(0) &&
                type(1) == other.type(1) &&
                type(2) == other.type(2) &&
                type(3) == other.type(3));
    }

    std::string str() const 
    {
        std::array<int, 4> num{0, 0, 0, 0};
        std::string st = "<";
        for (const auto& orb : orbs)
            st += strs[orb.spatial][orb.spin][num[orb.spatial]++]; 
        st += ">";
        st += "(";
        for (const auto& orb : orbs)
        {
            st += std::to_string(orb.electron);
            if (orb.electron2 != 0 )
            {
                st += "+";
                st += std::to_string(orb.electron2);
            }
            st += ",";
        }
        st += ")";
        return st;
    } 


    //Determine if can permute or can spinad
    bool can_permute_12() const {return can_permute(orbs[0], orbs[1]);} 
    bool can_permute_13() const {return can_permute(orbs[0], orbs[2]);} 
    bool can_permute_14() const {return can_permute(orbs[0], orbs[3]);} 
    bool can_permute_23() const {return can_permute(orbs[1], orbs[2]);} 
    bool can_permute_24() const {return can_permute(orbs[1], orbs[3]);} 
    bool can_permute_34() const {return can_permute(orbs[2], orbs[3]);} 
    bool can_permute_1324() const {return can_permute_13() && can_permute_24();}
    bool can_permute_1234() const {return can_permute_12() && can_permute_34();}

    void permute(orb_type& a, orb_type& b)
    {
        std::swap(a, b);
    }
    void permute_12()   { permute(orbs[0], orbs[1]);}
    void permute_13()   { permute(orbs[0], orbs[2]);}
    void permute_14()   { permute(orbs[0], orbs[3]);}
    void permute_23()   { permute(orbs[1], orbs[2]);}
    void permute_24()   { permute(orbs[1], orbs[3]);}
    void permute_34()   { permute(orbs[2], orbs[3]);}
    void permute_1324() { permute(orbs[0], orbs[2]); permute(orbs[1], orbs[3]); }; 


    bool can_spinad_12() const {return can_spinad(orbs[0], orbs[1]);} 
    bool can_spinad_34() const {return can_spinad(orbs[2], orbs[3]);} 

    //Sorts the list via permutations from left to right 
    //Annoyingly, because you are not guarenteed that a permutation CAN be performed,
    // you cannot use an intelligent sorting algorithm 
    void permute_sort() 
    {
        for (auto i = 0;   i < orbs.size(); i++)
        for (auto j = i+1; j < orbs.size(); j++)
            if (orbs[j] > orbs[i] && can_permute(orbs[i], orbs[j])) permute(orbs[i], orbs[j]);
    }

}; //end list type


//extern list_type::strs_t list_type::strs;
const list_type::strs_t list_type::strs{" M'", " N'", " O'", " P'",
                                  " m'", " n'", " o'", " p'",
                                  " I ", " J ", " K ", " L ",
                                  " i ", " j ", " k ", " l ",
                                  " A ", " B ", " C ", " D ",
                                  " a ", " b ", " c ", " d ",
                                  " E'", " F'", " G'", " H'",
                                  " e'", " f'", " g'", " h'"};


#endif
