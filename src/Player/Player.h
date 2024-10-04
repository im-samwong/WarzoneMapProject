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
    public:
        // Default constructor
        Player();

        // Parameterized constructor
        Player(const std::string& name);

        // Copy constructor
        Player(const Player& p);

        // Assignment operator
        Player& operator=(const Player& p);

        // Stream insertion operator
        friend std::ostream& operator<<(std::ostream& os, const Player& player);

        // Destructor
        ~Player();

        // Methods
            //toDefend()
            std::vector<Territory*> toDefend();

            //toAttack()
            std::vector<Territory*> toAttack();

            //issueOrder()
            void issueOrder(const std::string& orderType);

            // method to print hand of cards
            void printHand() const;

            // method to print orders
            void printOrders() const;

    private:
        std::string name;
        std::vector<Territory*> territories;
        Cards::Hand hand;
        Orders::OrderList orders;
    };
}

#endif //PLAYER_H
