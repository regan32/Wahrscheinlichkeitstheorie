//
// Created by user on 14.08.16.
//
#pragma once

#include "stdafx.h"
#include "Deck.hpp"
#include <mutex>


class Node
{
public:
    std::vector<std::shared_ptr<Node>> m_leafs;
    unsigned long long m_deck;
    unsigned m_cost;
    unsigned m_index;
};

class HoldemTree :
    std::multimap<unsigned long long, unsigned>
{
public:
    void insert(value_type type)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        insert(type);
    }
private:
    std::mutex m_mutex;
    
};

class HoldemInitializer {
public:

    enum HandMultiplyer
    {
        Best = 1,
        Pair = 100,
        DoublePair = 1000,
        Triple = 10000,
        Street = 100000,
        FullHouse = 1000000,
        Flash = 10000000,
        Quad = 100000000,
        StreetFlash = 1000000000
    };

    static unsigned CalculatePower(unsigned long long hand)
    {
        DeckImpl deck = hand;

        unsigned card_value = 0;

        if(deck.IsQuad(card_value))
        {
            return card_value + Quad;
        }

        if (deck.IsTriple(card_value))
        {
            unsigned pair_value;
            if (deck.IsPair(pair_value))
            {
                return card_value * 10 + pair_value + FullHouse;
            }

            return card_value + Triple;
        }

        if (deck.IsPair(card_value))
        {
            unsigned second_pair;
            if (deck.IsPair(second_pair, card_value - 3))
            {
                return card_value * 10 + second_pair + DoublePair;
            }

            return card_value + Pair;
        }

        if(deck.IsStreet(card_value))
        {
            if (deck.IsFlash(card_value))
            {
                return card_value + StreetFlash;
            }
            return card_value + Street;
        }

        if (deck.IsFlash(card_value))
        {
            return card_value + Flash;
        }

        return 0;
    }
    static void CheckCombination(size_t cards_left, size_t positions,unsigned long long hand, HoldemTree& hash_table)
    {
        auto numberOfPositions = positions;
        for (int i = positions; i >= 0; --i)
        {
            if (cards_left > 0)
                CheckCombination(cards_left - 1, i - 1, hand | 1ULL << i, hash_table);
            else if(cards_left == 0)
            {
                __int64 tmp = hand | 1ULL << i;
                unsigned power = CalculatePower(tmp);
                if(power > 0)
                hash_table.insert(std::make_pair(tmp, power));
            }

            //hash_table.insert_or_assign(hand | 1ULL << i, 1);
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
        auto numberOfPositions = total_cards;
        auto hand = 0UL;

//        std::vector<std::shared_ptr<std::thread>> Pool;
//        //std::thread thread(HoldemInitializer::CheckCombination, cards - 1, 1 - 1, hand | 1ULL << 1, std::ref(hash_table));
//        for (int i = total_cards; i >= 0; --i)
//        {
//            std::thread thread(CheckCombination,cards - 1, i - 1, hand | 1ULL << i, std::ref(hash_table));
//            thread.detach();
//        }
//        std::this_thread::sleep_for(std::chrono::seconds(30));
//        for(auto it = Pool.rbegin(); it != Pool.rend(); ++it)
//        {
//            (*it)->join();
//        }
        // 
        CheckCombination(cards, total_cards, 0ULL, hash_table);
       std::thread thread(CheckCombination, cards, total_cards, 0ULL, std::ref(hash_table));
        //std::thread thread2(CheckCombination, cards, total_cards, 0ULL, std::ref(hash_table));
        bool f = thread.joinable();
        thread.detach();
        //thread2.join();
//        std::ofstream stream;
//        stream.open("C:\\hashMap", std::ios::trunc | std::ios::out);
//        if(stream.is_open())
//        {
//            for(auto it = hash_table.begin(); it != hash_table.end(); ++it)
//            {
//                stream << std::bitset<52>(it->first)<< " - " << it->second << std::endl;
//            }
//        }
        std::this_thread::sleep_for(std::chrono::seconds(30));
    }
};
