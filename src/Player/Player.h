#ifndef PLAYER_H
#define PLAYER_H

#include "../Cards/Cards.h"
#include "../Map/Map.h"
#include "../Orders/Orders.h"
#include <memory>
#include <string>
#include <vector>

// Forward declarations to avoid circular dependencies
class Territory;
class Order;
class OrderList;
class Hand;
class PlayerStrategy;

class Player {
public:
    // Constructors
    Player();                                                                   // Default constructor
    Player(const std::string playerName);                                       // Parameterized constructor (name)
    Player(const std::string playerName, const std::vector<Territory*>& terrs); // Param. constructor (name, territories)

    // Destructor
    ~Player();

    // Copy constructor
    Player(const Player& other);

    // Assignment operator
    Player& operator=(const Player& other);

    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& out, const Player& player);

    // Functions
    std::vector<Territory*> toDefend();            // Return list of territories to defend
    std::vector<Territory*> toAttack();            // Return list of territories to attack
    void issueOrder(std::unique_ptr<Order> order); // Issue an order
    void issueOrder(std::vector<Player*>* players);
    void addTerritory(Territory* t);
    std::vector<Territory*> getAllTerritories() const;
    void emptyToDefend();
    void changeReinforcements(int i);
    int getReinforcements() const;

    void setPlayerStrategy(PlayerStrategy* ps);
    PlayerStrategy* getPlayerStrategy() const;

    // Other getters and setters as needed
    std::string getName() const;
    Hand& getHand();
    OrderList* getOrdersList();
    void setOrdersList(const OrderList& ol);

private:
    std::string* name;
    std::vector<Territory*>* territories; // Collection of player's territories
    Hand* hand;                           // Player's hand of cards
    OrderList* orders;                    // List of orders the player has issued
    int* reinforcements;
    PlayerStrategy* strategy;
};

#endif // PLAYER_H
