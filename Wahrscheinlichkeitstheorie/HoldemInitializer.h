//
// Created by user on 14.08.16.
//
#pragma once
#include "stdafx.h"
#include "Deck.hpp"

class HoldemInitializer {
public:

    void CheckCombination(size_t cards, size_t positions,unsigned long long hand, std::unordered_map<unsigned long long, int>& hash_table)
    {
        auto numberOfPositions = positions;
        auto cards_left = cards - 1;
        for (int i = positions; i > 0; --i)
        {
            if (cards_left > 0 && i > 0)
                CheckCombination(cards_left, i - 1, hand | 1ULL << i, hash_table);
            else if(cards_left == 0)
                hash_table.insert(std::make_pair(hand | 1ULL << i, 1));
            //hash_table.insert_or_assign(hand | 1ULL << i, 1);
        }
    }

    void Init_Combinations(size_t cards)
    {
        std::unordered_map<unsigned long long, int> hash_table;
        CheckCombination(cards, total_cards, 0ULL, hash_table);
        std::cout<< hash_table.size();
    }
};


#endif //WAHRSCHEINLICHKEITSTHEORIE_HOLDEMINITIALIZER_H
