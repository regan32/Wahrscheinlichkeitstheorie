//
// Created by user on 14.08.16.
//
#pragma once

#include <memory>
#include "stdafx.h"
#include "Deck.hpp"

class Node
{
public:
    std::vector<std::shared_ptr<Node>> m_leafs;
    unsigned long long m_deck;
    unsigned m_cost;
    unsigned m_index;
};

class HoldemTree
{
public:
    std::vector<std::shared_ptr<Node>> m_roots;
};

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

    static void CheckCombination(size_t cards, size_t positions,unsigned long long hand, std::vector<std::shared_ptr<Node>>& nodes)
    {
        auto numberOfPositions = positions;
        auto cards_left = cards - 1;
        for (int i = positions; i > 0; --i)
        {
            auto node = std::make_shared<Node>();
            node->m_deck = hand | 1ULL << i;
            nodes.push_back(node);
            if (cards_left > 0 && i > 0)
            {
                CheckCombination(cards_left, i - 1, hand | 1ULL << i, node->m_leafs);
            }
            //hash_table.insert_or_assign(hand | 1ULL << i, 1);
        }
    }

    static void Init_Combinations(size_t cards)
    {
        HoldemTree trees;
        //std::unordered_map<unsigned long long, int> hash_table;
        CheckCombination(cards, total_cards, 0ULL, trees.m_roots);
        std::cout<< trees.m_roots.size();
    }
};
