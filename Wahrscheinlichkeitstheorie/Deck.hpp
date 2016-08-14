#pragma once

const size_t total_cards = 52;
//const size_t total_combinations = 380204032; // 52^5;

class DeckImpl
{
public:
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
    unsigned long long operator=(const DeckImpl& deck) const
    {
        return deck.mask;
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