// Wahrscheinlichkeitstheorie.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Deck.hpp"
#include "HoldemInitializer.h"
//HOLDEM

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
    HoldemInitializer::Init_Combinations(7);
    return 0;
}

