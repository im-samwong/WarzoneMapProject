#include "../Map/Map.h"
#include "../Orders/Orders.h"
#include "../Cards/Cards.h"
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#ifndef PLAYER_H
#define PLAYER_H

namespace Player {
    class Player {
    private:
        std::string name;                      // Player's name
        std::vector<Territory*> territories;   // Collection of player's territories
        Cards::Hand* hand;                            // Player's hand of cards
        Orders::OrderList* orders;                    // List of orders the player has issued

    public:
        // Constructors
        Player();                              // Default constructor
        Player(const std::string& playerName); // Parameterized constructor (name)
        Player(const std::string& playerName, const std::vector<Territory*>& terrs); // Param. constructor (name, territories)

        // Destructor
        ~Player();

        // Copy constructor
        Player(const Player& other);

        // Assignment operator
        Player& operator=(const Player& other);

        // Stream insertion operator
        friend std::ostream& operator<<(std::ostream& out, const Player& player);

        // Functions
        std::vector<Territory*> toDefend();    // Return list of territories to defend
        std::vector<Territory*> toAttack();    // Return list of territories to attack
        void issueOrder(std::unique_ptr<Orders::Order> order);         // Issue an order

        // Other getters and setters as needed
        std::string getName() const;
        Cards::Hand& getHand();
    };
}

#endif //PLAYER_H
