//
// Created by user on 14.08.16.
//
#pragma once

#include "stdafx.h"
#include "Deck.hpp"
#include <mutex>
#include "ThreadPool.h"
class Node
{
public:
    std::vector<std::shared_ptr<Node>> m_leafs;
    unsigned long long m_deck;
    unsigned m_cost;
    unsigned m_index;
};

class HoldemTree :
    public std::map<unsigned long long, unsigned>
{
public:
    HoldemTree()
    {
    }
    void insert(value_type type)
    {
        std::lock_guard<std::mutex> lock(m_mutex_);
        std::map<unsigned long long, unsigned>::insert(type);
    }
private:
    std::mutex m_mutex_;
    
};

static HoldemTree tree;

class HoldemInitializer {
public:

    enum HandMultiplyer : long long
    {
        Second      = 100,
        Best        = 1000,
        Pair        = 100000,
        DoublePair  = 1000000,
        Triple      = 10000000,
        Street      = 100000000,
        Flash       = 1000000000,
        FullHouse   = 10000000000,
        Quad        = 100000000000,
        StreetFlash = 1000000000000
    };

    static unsigned CalculatePower(unsigned long long hand)
    {
        DeckImpl deck = hand;

        unsigned first_value = 0;
        unsigned second_value = 0;

        if(deck.IsQuad(first_value))
        {
            return first_value  + Quad;
        }

        if (deck.IsTriple(first_value))
        {
            if (deck.IsPair(second_value))
            {
                return first_value * 10 + second_value + FullHouse;
            }

            return first_value + Triple;
        }

        if (deck.IsPair(first_value))
        {
            if (deck.IsPair(second_value, first_value - 3))
            {
                return first_value * 10 + second_value + DoublePair;
            }

            return first_value + Pair;
        }

        if(deck.IsStreet(first_value))
        {
            if (deck.IsFlash(first_value))
            {
                return first_value + StreetFlash;
            }
            return first_value + Street;
        }

        if (deck.IsFlash(first_value))
        {
            return first_value + Flash;
        }

        return 0;
    }

    static void CheckCombination(int cards_left, int total_cards,unsigned long long hand)
    {
        int begin_position = total_cards - 1;
        int end_position = --cards_left;
        for (int i = begin_position; i >= end_position; --i)
        {
            __int64 tmp = hand | 1ULL << i;

            if(cards_left == 0)
            {
//                unsigned power = CalculatePower(tmp);
//                if(power > 0) {
                    tree.insert(std::make_pair(tmp, CalculatePower(tmp)));
//                }
            }
            else
            {
                CheckCombination(cards_left, i, tmp);
            }
        }
    }

//    static void CheckCombination(size_t cards, size_t positions,unsigned long long hand, std::vector<std::shared_ptr<Node>>& nodes)
//    {
//        auto numberOfPositions = positions;
//        auto cards_left = cards - 1;
//        for (int i = positions; i >= 0; --i)
//        {
//            auto node = std::make_shared<Node>();
//            node->m_deck = hand | 1ULL << i;
//            nodes.push_back(node);
//            if (cards_left > 0 && i >= 0)
//            {
//                CheckCombination(cards_left, i - 1, hand | 1ULL << i, node->m_leafs);
//            }
//            //hash_table.insert_or_assign(hand | 1ULL << i, 1);
//        }
//    }

    static void Init_Combinations(size_t cards)
    {
        auto res = CalculatePower(0b1101100000000000000000000000000000000000000000000000);
        res = CalculatePower(0b0000000000000000000000000000001000001010010000110000);
        res = CalculatePower(1168239509512);

        HoldemTree hash_table;
        unsigned concurentThreadsSupported = std::thread::hardware_concurrency();
        if (concurentThreadsSupported == 0) concurentThreadsSupported = 1;

        int numberOfPositions = total_cards - 1;
        auto hand = 0ULL;
        //CheckCombination(cards, total_cards, 0ULL);
        ThreadPool pool(4);
        cards--;
        for(int i = numberOfPositions; i >= int(cards); --i)
        {
            pool.enqueue(HoldemInitializer::CheckCombination, cards, i , 1ULL << i);
        }

        pool.wait_until_nothing_in_flight();
        std::cout << tree.size();
        std::this_thread::sleep_for(std::chrono::seconds(30));
    }
};
