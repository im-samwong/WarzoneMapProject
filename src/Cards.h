#ifndef CARDS_H
#define CARDS_H

#include "../Orders/Orders.h"
#include <vector>
#include <string>
#include <iostream>

class Card {
public:
    enum CardType { BOMB, REINFORCEMENT, BLOCKADE, AIRLIFT, DIPLOMACY };
    Card(CardType type); // Constructor
    Card(const Card& other);  // Copy constructor
    ~Card(); // Destructor
    
    Card& operator=(const Card& other);  // Assignment operator
    
    std::string getTypeAsString() const; // Get the card type as a string
    void play(Orders::OrderList* orderList);  // Play the card and return it to the deck

    // Overloaded stream operator to print card details
    friend std::ostream& operator<<(std::ostream& out, const Card& card);

private:
    CardType* type;  // Pointer type for class member
};

class Deck {
public:
    Deck(); // Constructor
    ~Deck(); // Destructor

    std::vector<Card*> getCards(); // Get the deck of cards
    Card* draw();  // Draw a random card from the deck
    
private:
    std::vector<Card*> cards;  // Pointer type for each card in the deck
};

class Hand {
public:
    Hand(); // Constructor
    ~Hand(); // Destructor
    
    std::vector<Card*> getHandCards(); // Get the cards in the hand
    void addCard(Card* card); // Add a card to the hand
    void playCard(Card* card, Deck* deck, Orders::OrderList* orderList); // Play a card from the hand
    
private:
    std::vector<Card*> handCards;  // Pointer type for each card in the hand
};

#endif
