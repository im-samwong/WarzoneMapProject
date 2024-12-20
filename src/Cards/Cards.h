#ifndef CARDS_H
#define CARDS_H
#include "../Orders/Orders.h"
#include <iostream>
#include <string>
#include <vector>

// Forward declaration of OrderList to avoid circular dependencies
class OrderList;

class Card {
public:
    enum CardType { BOMB,
                    REINFORCEMENT,
                    BLOCKADE,
                    AIRLIFT,
                    DIPLOMACY };
    Card(CardType type);     // Constructor
    Card(const Card& other); // Copy constructor
    ~Card();                 // Destructor

    Card& operator=(const Card& other); // Assignment operator

    std::string getTypeAsString() const; // Get the card type as a string
    void play(OrderList* orderList);     // Play the card and return it to the deck

    // Overloaded stream operator to print card details
    friend std::ostream& operator<<(std::ostream& out, const Card& card);

private:
    CardType* type; // Pointer type for class member
};

class Deck {
public:
    Deck();                  // Constructor
    Deck(const Deck& other); // Copy Constructor
    ~Deck();                 // Destructor

    // Assignment operator
    Deck& operator=(const Deck& other);

    std::vector<Card*> getCards(); // Get the deck of cards
    Card* draw();                  // Draw a random card from the deck

    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& out, const Deck& deck);

private:
    std::vector<Card*> cards; // Pointer type for each card in the deck
};

class Hand {
public:
    Hand();                  // Constructor
    Hand(const Hand& other); // Copy Constructor
    ~Hand();                 // Destructor

    // Assignment operator
    Hand& operator=(const Hand& other);

    std::vector<Card*> getHandCards();                           // Get the cards in the hand
    void setHandCards(const std::vector<Card*> &newCards);
    void addCard(Card* card);                                    // Add a card to the hand
    void playCard(Card* card, Deck* deck, OrderList* orderList); // Play a card from the hand

    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& out, const Hand& hand);

private:
    std::vector<Card*> handCards; // Pointer type for each card in the hand
};

#endif
