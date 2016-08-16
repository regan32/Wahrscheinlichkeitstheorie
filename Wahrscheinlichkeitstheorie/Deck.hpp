#pragma once

const size_t total_cards = 52;
const size_t total_game_combinations = 674274182400; // 52!/45!;
const size_t total_card_combinations = 311875200; // 52!/47!;
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

    bool IsQuad(unsigned& card_value)
    {
        for (int i = 0; i < 13; ++i)
        {
            if ((mask >> (i * 4) & 0xF) == 0xF)
            {
                card_value = (i + 2);
                return true;
            }
        }

        return false;
    }

    bool IsPairAndTriple(unsigned& triple_card_value, unsigned& pair_card_value)
    {
        return IsTriple(triple_card_value) && IsPair(pair_card_value);
    }
    bool IsStreet(unsigned& card_value)
    {
        int counter = 0;
        bool ace = false;
        for (int i = 12; i >= 0; --i)
        {
            if ((mask >> (i * 4) & 0xF) > 0)
            {
                counter++;
                if(i == 12)
                {
                    ace = true;
                }

                if (counter == 5)
                {
                    card_value = i + 6;
                    return true;
                }

                if(i == 0 && counter == 4 && ace) //Lowest street
                {
                    card_value = 5;
                    return true;
                }
            }
        }
    }
    bool IsFlash(unsigned& card_value)
    {
        for (int i = 0; i < 4; ++i)
        {
            int cards = population_count64_max15(mask >> i & 0x1111111111111);
            if(cards == 5)
            {
                card_value = 0;
                return true;
            }
            if (cards > 1)
                return false; //For 5 cards only
        }
        
        return false;
    }

    bool IsStreetFlash(unsigned& card_value)
    {
        return IsFlash(card_value) && IsStreet(card_value);
    }
    bool IsTriple(unsigned& card_value)
    {
        for (int i = 0; i < 13; ++i)
        {
            if (Calc_matches(mask >> (i * 4), 3))
            {
                card_value = (i + 2);
                return true;
            }
        }

        return false;
    }

    bool IsPair(unsigned& card_value, unsigned offset = 12)
    {
        for (int i = offset; i >= 0; --i)
        {
            if (Calc_matches(mask >> (i * 4), 2))
            {
                card_value = (i + 2);
                return true;
            }
        }

        return false;
    }

    int population_count64_max15(unsigned __int64 w)
    {
        w -= (w >> 1) & 0x5555555555555555ULL;
        w = (w & 0x3333333333333333ULL) + ((w >> 2) & 0x3333333333333333ULL);

        return int((w * 0x1111111111111111ULL) >> 60);
    }


    bool Calc_matches(unsigned long long hand, int matches)
    {
        int result = 0;
        for (int i = 3; i >= 0; --i)
        {
            result += (hand >> i) & 0x1;
        }

        return result == matches;
    }
};

struct FullDeck : public DeckImpl
{
    FullDeck()
    {
        mask = (1ULL << total_cards + 1) - 1;
    }
};