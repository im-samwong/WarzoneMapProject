#include "Player.h"
#include "../Map/Map.h"
#include "../Orders/Orders.h"
#include "../Cards/Cards.h"

namespace Player {
    // Default constructor
    Player::Player() : name("No name"), hand(), orders(), territories()
    {}

    // Constructor
    Player::Player(const std::string& name) : name(name), hand(), orders(), territories()
    {}

    // Copy constructor
    Player::Player(const Player& p) :
    name(p.name), hand(p.hand), orders(p.orders)
    {
        // the territories vector keeps the default initialization of empty because two players cannot own the same territory
    }

    // Assignment operator
    Player& Player::operator=(const Player& p) {
        if (!(this == &p)) // ensures that the resources aren't reallocated if this == p
        {
            // clearing territories, cards, and orders
            territories.clear();

            // creating a copy of the name
            name = p.name;

            // creating a deep copy of the hand
            hand = p.hand;

            // creating a deep copy of the orders list
            orders = p.orders;
        }
        return *this; // returning the object in case we want to chain assign
    }

    // Default destructor
    Player::~Player() = default;

    // Stream insertion operator
    std::ostream& operator<<(std::ostream& os, const Player& p) {
        os << "Player: " << p.name << "\n";
        os << "Number of Territories owned: " << p.territories.size() << "\n";
        os << "Orders:\n";
        p.printOrders();
        os << "Hand contains: the following cards:\n";
        p.printHand();
        return os;
    }

    //toDefend() returns a list of territories that are to be defended
    std::vector<Territory*> Player::toDefend() {
        std::vector<Territory*> toDefendList;
        // add logic
        return toDefendList; // returns an empty list for now -- Assignment 1
    }

    //toAttack() returns a list of territories that are to be attacked
    std::vector<Territory*> Player::toAttack() {
        std::vector<Territory*> toAttackList;
        // add logic
        return toAttackList; // returns an empty list for now -- Assignment 1
    }

    //issueOrder() creates an order object and adds it to the list of orders
    void Player::issueOrder(const std::string& orderType) {
        // Create the new order based on orderType
        std::unique_ptr<Orders::Order> newOrder;

        if (orderType == "deploy") {
            newOrder = std::make_unique<Orders::DeployOrder>();
        } else if (orderType == "advance") {
            newOrder = std::make_unique<Orders::AdvanceOrder>();
        } else if (orderType == "bomb") {
            newOrder = std::make_unique<Orders::BombOrder>();
        } else if (orderType == "blockade") {
            newOrder = std::make_unique<Orders::BlockadeOrder>();
        } else if (orderType == "airlift") {
            newOrder = std::make_unique<Orders::AirliftOrder>();
        } else if (orderType == "negotiate") {
            newOrder = std::make_unique<Orders::NegotiateOrder>();
        } else {
            std::cerr << "Unknown order type: " << orderType << std::endl; //or std::cout
            return;
        }

        // Add the new order to the list of orders
        orders.addOrder(std::move(newOrder));
    }

    // Method to print hand of cards
    void Player::printHand() const{
    /*    std::vector<Cards::Card*> handCards = hand.getHandCards();
        for (const auto& card : handCards) {
            std::cout << *card << std::endl;
        } */
    }

    // Method to print orders
    void Player::printOrders() const {
        orders.printOrders();
    }

}