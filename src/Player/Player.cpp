#include "Player.h"
#include "../Cards/Cards.h"
#include "../Map/Map.h"
#include "../Orders/Orders.h"

// Default constructor
Player::Player() : name(new std::string("Unknown")), territories(new std::vector<Territory*>()), hand(new Hand()), orders(new OrderList()) {}

// Parameterized constructor (name)
Player::Player(const std::string playerName) : name(new std::string(playerName)), territories(new std::vector<Territory*>()), hand(new Hand()), orders(new OrderList()) {}

// Parameterized constructor (name, territories)
Player::Player(const std::string playerName, const std::vector<Territory*>& terrs)
    : name(new std::string(playerName)), territories(new std::vector<Territory*>(terrs)), hand(new Hand()), orders(new OrderList()) {}

// Destructor
Player::~Player() {
    delete hand;
    delete orders;
    delete territories;
}

// Copy constructor (using move semantics for unique_ptr)
Player::Player(const Player& other)
    : name(other.name), hand(new Hand(*other.hand)), territories(other.territories) {
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

    // Copy new resources and move orders
    this->name = new std::string(*other.name);
    this->hand = new Hand(*other.hand);
    this->territories = new std::vector<Territory*>(*other.territories);
    this->orders = new OrderList(std::move(*other.orders)); // Move the orders

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

// Return territories to attack
std::vector<Territory*> Player::toAttack() {
    Continent* dummyContinent = nullptr;
    Territory* t = new Territory("Greenland", 140, 240, dummyContinent);
    std::vector<Territory*> territories = {t};
    // return std::vector<Territory*>();
    return territories; // arbitrary list of territories
}

// Issue an order (with unique_ptr)
void Player::issueOrder(std::unique_ptr<Order> order) {
    orders->addOrder(std::move(order));
}

// Other getters and setters
std::string Player::getName() const {
    return *name;
}

Hand& Player::getHand() {
    return *hand;
}
