#include <vector>
#include <algorithm>
#include <iostream>
#include <ctime>

typedef int Suit, Rank;

const Suit  hearts = 0, clubs = 1,   diamonds = 2,  spades = 3;
												  
const Rank  two = 0,    three = 1,   four = 2,      five = 3,
            six = 4,    seven = 5,   eight = 6,     nine = 7,
            ten = 8,     jack = 9,   queen = 10,    king = 11,
            ace = 12;

class Card
{
    Suit suit;
    Rank rank;
public:
    Card(Suit s, Rank r) : suit(s), rank(r) { }

    bool operator==(Card c) { return rank == c.rank; }
    bool operator< (Card c) { return rank <  c.rank; }
};


class CardStack
{
protected:
    std::vector<Card> cards;
public:
    Card Play()
    {
        Card c( *cards.begin() );
        cards.erase(cards.begin());
        return c;
    }

    std::vector<Card> Play(int i)
    {
        std::vector<Card> c;

        while (i-- && cards.size())
            c.push_back( Play() );

        return c;
    }

    void Draw(Card c)
    {
        cards.push_back(c);
    }
    void Draw(std::vector<Card> c)
    {
        for (std::vector<Card>::iterator it = c.begin(); it != c.end(); ++it)
            Draw(*it);
    }

    int CardsLeft() { return cards.size(); }
};

class Player : public CardStack {  };

class Deck : public CardStack
{
public:
    Deck() 
    {
        for (Rank r = two; r <= ace; ++r)
            for (Suit s = hearts; s <= spades; ++s)
                cards.push_back(Card(s,r));

        std::random_shuffle(cards.begin(), cards.end());
    }

    void Deal(Player& p1, Player& p2)
    {
        for(bool i = true; cards.size() ; i = !i)
        {
            if (i) p1.Draw( Play() );
            else   p2.Draw( Play() );
        }
    }
};

class Table
{
    CardStack theStack;
    Deck deck;
    Player p1, p2;
public:
    int iterations;

    Table() : iterations(0) {    deck.Deal(p1, p2);    }

    int Play()
    {
        int result = 0;

        while(result == 0)
            result = Attack();

        return result;
    }
    int Attack() // return 0 (no winner yet), 1 (p1 wins), 2 (p2 wins), 3 (tie)
    {
        if (++iterations > 15000) return 3;

        std::cout << std::endl << "p1: " << p1.CardsLeft() << "\t p2: " << p2.CardsLeft();

        if (p1.CardsLeft() < 1) return 2;
        if (p2.CardsLeft() < 1) return 1;

        Card left( p1.Play() ), right( p2.Play() );

        theStack.Draw(left);
        theStack.Draw(right);

        if (left == right) 
        {
            std::cout << " \t WAR!";
            if (p1.CardsLeft() < 3) return 2;
            if (p2.CardsLeft() < 3) return 1;

            theStack.Draw( p1.Play(3) );
            theStack.Draw( p2.Play(3) );

            return Attack();
        }
        else if (left < right)
        {
            std::cout << " \t p1 wins";
            p1.Draw( theStack.Play( theStack.CardsLeft() ) );
        }
        else
        {
            std::cout << " \t p2 wins";
            p2.Draw( theStack.Play( theStack.CardsLeft() ) );
        }

        return 0;
    }
};

int main()
{
    srand(time(NULL));

    Table t;
    t.Play();
}
