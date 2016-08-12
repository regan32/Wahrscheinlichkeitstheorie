// Wahrscheinlichkeitstheorie.cpp : Defines the entry point for the console application.
//

#ifdef _WIN32
#include "stdafx.h"
#endif

#include <unordered_map>
#include <iostream>
#include <functional>
#include <vector>

//HOLDEM
const size_t total_cards = 52;
const size_t total_combinations = 380204032; // 52^5;

enum HandMultiplyer
{
    Best = 1,
    Pair = 10,
    Triple = 100,
    Street = 1000,
    FullHouse = 10000,
    Flash = 100000,
    Quad = 1000000,
    StreetFlash = 10000000
};
struct DeckImpl
{
    DeckImpl()
        : mask(0)
    {
    }
    DeckImpl(unsigned long long cards)
        : mask(cards)
    {
    }
    unsigned long long operator^(const DeckImpl& rhs) const
    {
        return mask ^ rhs.mask;
    }

    unsigned long long mask;
    unsigned size()
    {
        unsigned cards = 0;
        for(size_t i = total_cards; i > 0; --i)
        {
            if ((1ULL << i & mask) != 0)
                cards++;
        }

        return cards;
    }
};

struct FullDeck : public DeckImpl
{
    FullDeck()
    {
        mask = (1ULL << total_cards + 1) - 1;
    }
};

class HoldemPower
{
    public:
        static unsigned long long Calculate_Power(DeckImpl hand)
        {
            m_functions.push_back([](DeckImpl hand){ return 0;});
        }

private:
    static std::vector<std::function<unsigned long long(DeckImpl)>> m_functions;

    unsigned long long IsQuad(DeckImpl hand)
    {
        unsigned single =0;
        for (int i = 0; i < 13; ++i)
        {
            if(Calc_matches(hand.mask >> (i * 4), 4))
                return (i+1) * HandMultiplyer::Quad;
        }
    }
    bool Calc_matches(unsigned long long hand, int matches)
    {
        int result = 0;
        for(int i = 4; i > 0; --i)
        {
            result += (hand >> i) & 0x1;
        }

        return result == matches;
    }
};


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

unsigned long long CalcDeckPower(DeckImpl hand)
{
    if (hand.size() > 5) return 0;


}
double Calc(DeckImpl hand, size_t total_players, size_t players_in_game, DeckImpl board)
{
    unsigned cards_to_take = 5 - board.size();
    
    DeckImpl GameDeck = FullDeck() ^ hand;

    return 0;
}
int main()
{
    Init_Combinations(5);
    return 0;
}

