// Wahrscheinlichkeitstheorie.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Deck.hpp"
#include "HoldemInitializer.h"
//HOLDEM

double Calc(DeckImpl hand, size_t total_players, size_t players_in_game, DeckImpl board)
{
    unsigned cards_to_take = 5 - board.size();
    
    DeckImpl GameDeck = FullDeck() ^ hand;

    return 0;
}


inline int population_count64_max15(__int64 w)
{
    w -= (w >> 1) & 0x5555555555555555ULL;
    w = (w & 0x3333333333333333ULL) + ((w >> 2) & 0x3333333333333333ULL);

    return int((w * 0x1111111111111111ULL) >> 60);
}

int main()
{
    //int res = population_count64_max15(0x1111111110011011ULL);
    HoldemInitializer::Init_Combinations(5);
    return 0;
}

