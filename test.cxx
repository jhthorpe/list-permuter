#include <vector>
#include <array>
#include <stdio.h>
#include "lists.hpp"

int main()
{
    const orb_type E(orb_type::e, 0); const orb_type e(orb_type::e, 1);
    const orb_type A(orb_type::a, 0); const orb_type a(orb_type::a, 1);
    const orb_type I(orb_type::i, 0); const orb_type i(orb_type::i, 1);
    const orb_type M(orb_type::m, 0); const orb_type m(orb_type::m, 1);

    std::vector<orb_type> occA{I, M};
    std::vector<orb_type> occb{i, m};
    std::vector<orb_type> vrtA{A, E};
    std::vector<orb_type> vrtb{a, e};

//TESTING
/*
    std::vector<orb_type> occA{I};
    std::vector<orb_type> occb{i};
    std::vector<orb_type> vrtA{A};
    std::vector<orb_type> vrtb{a};
*/

    //STEP 1: register all lists you want to check uniqueness of
    std::vector<list_type> AbIj; 
    for (auto& a : vrtA)
    for (auto& b : vrtb)
    for (auto& i : occA)
    for (auto& j : occb)
    {
        AbIj.emplace_back(a, b, i, j);
        AbIj.emplace_back(a, j, i, b);
        AbIj.emplace_back(i, b, a, j);
        AbIj.emplace_back(i, j, a, b);
    }

    //STEP 2: Check for unique lists based off of permutations of indices
    std::vector<list_type> unique_pphh;
    for (auto& pphh : AbIj)
    {
        //"sort" via permutations 
        pphh.permute_sort();
        bool is_unique = true; 
        for (const auto& list : unique_pphh)
        {
            if (pphh == list)
            {
                is_unique = false;
                break;
            } 
        } 
        if (is_unique) unique_pphh.push_back(pphh);
    }
 
    printf("Unique lists w/o spin-adaptation:\n");
    for (const auto& pphh : unique_pphh)
        printf("%s\n", pphh.str().c_str());
    printf("\n");

    //STEP 3: Determine which lists are unique after "spin-adaptation" 

     
    return 0; 
}
