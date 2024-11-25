#include "Player.h"
#include "../Cards/Cards.h"
#include "../Map/Map.h"
#include "../Orders/Orders.h"
#include "../PlayerStrategies/PlayerStrategies.h"
#include <set>
#include <algorithm>
#include <limits>

// Default constructor
Player::Player() : name(new std::string("Unknown")), territories(new std::vector<Territory*>()), hand(new Hand()), orders(new OrderList()), reinforcements(new int()) {
    strategy = new HumanPlayer(this);
}

// Parameterized constructor (name)
Player::Player(const std::string playerName) : name(new std::string(playerName)), territories(new std::vector<Territory*>()), hand(new Hand()), orders(new OrderList()),reinforcements(new int()) {
    strategy = new HumanPlayer(this);
}

// Parameterized constructor (name, territories)
Player::Player(const std::string playerName, const std::vector<Territory*>& terrs)
    : name(new std::string(playerName)), territories(new std::vector<Territory*>(terrs)), hand(new Hand()), orders(new OrderList()), reinforcements(new int()) {
    strategy = new HumanPlayer(this);
}

// Destructor
Player::~Player() {
    delete hand;
    delete orders;
    delete territories;
    delete reinforcements;
    delete strategy;
}

// Copy constructor (using move semantics for unique_ptr)
Player::Player(const Player& other)
    : name(other.name), hand(new Hand(*other.hand)), territories(other.territories), reinforcements(new int(*other.reinforcements)) {
    this->orders = new OrderList(std::move(*other.orders)); // Move the orders
}

// Assignment operator (using move semantics for unique_ptr)
Player& Player::operator=(const Player& other) {
    if (this == &other)
        return *this;

    // Clean up current resources
    delete hand;
    delete orders;
    delete territories;
    delete reinforcements;

    // Copy new resources and move orders
    this->name = new std::string(*other.name);
    this->hand = new Hand(*other.hand);
    this->territories = new std::vector<Territory*>(*other.territories);
    this->orders = new OrderList(std::move(*other.orders));
    this->reinforcements = new int(*other.reinforcements); // Move the orders

    return *this;
}

// Stream insertion operator
std::ostream& operator<<(std::ostream& out, const Player& player) {
    out << "Player: " << *player.name << " has " << player.territories->size() << " territories and has issued the following orders:" << std::endl;
    player.orders->printOrders();
    return out;
}

// Return territories to defend
std::vector<Territory*> Player::toDefend() {
    return *territories;
}

void Player::emptyToDefend() {
    this->territories->clear(); // Clear the list but do not delete the objects
}

// Return territories to attack
std::vector<Territory*> Player::toAttack() {
    std::set<Territory*> uniqueAtkTargets;
    for(const Territory* territory: *territories) {
        for(const std::vector<Territory*> targets = *territory->getNeighbors(); Territory* target: targets) {
            if(target->getOwner()->getName() != *this->name) {
                uniqueAtkTargets.insert(target);
            }
        }
    }

    std::vector<Territory*> territoriesToAttack;
    territoriesToAttack.assign(uniqueAtkTargets.begin(), uniqueAtkTargets.end());
    return territoriesToAttack;
}

// Issue an order (with unique_ptr)
void Player::issueOrder(std::unique_ptr<Order> order) {
    orders->addOrder(std::move(order));
}

void Player::issueOrder(std::vector<Player*>* players) {
    strategy->issueOrder(players);
}

void Player::addTerritory(Territory* t) {
    territories->push_back(t);
}

void Player::changeReinforcements(int i) {
    *reinforcements = i + *reinforcements;
}

int Player::getReinforcements() const {
    return *reinforcements;
}

PlayerStrategy* Player::getPlayerStrategy() const {
    return strategy;
}

void Player::setPlayerStrategy(PlayerStrategy *ps) {
    delete strategy;
    strategy = ps;
}

// Other getters and setters
std::string Player::getName() const {
    return *name;
}

Hand& Player::getHand() {
    return *hand;
}

OrderList* Player::getOrdersList() {
    return orders;
}

void Player::setOrdersList(const OrderList& ol) {
    delete orders;
    orders = new OrderList(ol);
}

std::vector<Territory *> Player::getAllTerritories() const {
    return *this->territories;
}
