#include "Player.h"
#include "../Cards/Cards.h"
#include "../Map/Map.h"
#include "../Orders/Orders.h"
#include <set>

// Default constructor
Player::Player() : name(new std::string("Unknown")), territories(new std::vector<Territory*>()), hand(new Hand()), orders(new OrderList()), reinforcements(new int()) {}

// Parameterized constructor (name)
Player::Player(const std::string playerName) : name(new std::string(playerName)), territories(new std::vector<Territory*>()), hand(new Hand()), orders(new OrderList()),reinforcements(new int()) {}

// Parameterized constructor (name, territories)
Player::Player(const std::string playerName, const std::vector<Territory*>& terrs)
    : name(new std::string(playerName)), territories(new std::vector<Territory*>(terrs)), hand(new Hand()), orders(new OrderList()), reinforcements(new int()) {}

// Destructor
Player::~Player() {
    delete hand;
    delete orders;
    delete territories;
    delete reinforcements;
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

void Player::issueOrder() {
    std::cout << "Here are the territories you should defend:" << std::endl;
    for(Territory* territory: toDefend()) {
        std::cout << territory->getName() << std::endl;
    }

    std::cout << "\nFor now, use the deploy order until you have no more reinforcements to deploy.\n" << std::endl;

    while (*this->reinforcements != 0) {
        std::cout << "\nRemaining units: " << *this->reinforcements << std::endl;
        int unitsToUse = 0;
        std::cout << "How many units would you like to deploy?" << std::endl;
        std::cin >> unitsToUse;
        if(*this->reinforcements - unitsToUse < 0) {
            std::cout << "You cannot deploy more units than what you have. No units deployed, try again" << std::endl;
        } else {
            orders->addOrder(std::make_unique<DeployOrder>());
            *this->reinforcements -= unitsToUse;
        }
    }

    std::cout << "\nYou have now deployed all of your reinforcements units. You can now issue orders other than deploy now." << std::endl;
    //Once all reinforcements are deployed then show the toAttack stuff
    std::cout << "\nHere are the territories you should attack:" << std::endl;
    for(Territory* territory: toAttack()) {
        std::cout << territory->getName() << std::endl;
    }

    bool playerHasFinishedIssuingOrders = false;
    const std::vector<std::string> possibleOrders = {"AdvanceOrder","AirliftOrder","BlockadeOrder","BombOrder","NegotiateOrder"};
    const std::vector<std::string> stringEnumCards = {"Reinforcement","Airlift","Blockade","Bomb","Diplomacy"};

    std::vector<Card*> playerHandOrders = this->hand->getHandCards();

    while (!playerHasFinishedIssuingOrders) {
        std::cout << "\nYour available orders are:" << std::endl;
        int chosenOrderIndex = -1;
        for (const Card* card: playerHandOrders) {
            std::cout << card->getTypeAsString() << std::endl;
        }

        std::cout << "\nType the order as you see it if you wish to issue it or type endissueorders to stop issuing orders" << std::endl;

        std::string orderInput;
        std::cin >> orderInput;

        if (orderInput == "endissueorders") {
            playerHasFinishedIssuingOrders = true;
            std::cout << "Player has decided to stop issuing orders. Ending issue order turn for this player" << std::endl;
            break;
        }

        if (orderInput == stringEnumCards[0]) {
            this->orders->addOrder(std::make_unique<AdvanceOrder>());
            chosenOrderIndex = 0;
        } else if (orderInput == stringEnumCards[1]) {
            this->orders->addOrder(std::make_unique<AirliftOrder>());
            chosenOrderIndex = 1;
        } else if (orderInput == stringEnumCards[2]) {
            this->orders->addOrder(std::make_unique<BlockadeOrder>());
            chosenOrderIndex = 2;
        } else if (orderInput == stringEnumCards[3]) {
            this->orders->addOrder(std::make_unique<BombOrder>());
            chosenOrderIndex = 3;
        } else if (orderInput == stringEnumCards[4]) {
            this->orders->addOrder(std::make_unique<NegotiateOrder>());
            chosenOrderIndex = 4;
        } else {
            std::cout << "Invalid command. Did nothing, please re-issue your order" << std::endl;
        }

        if (chosenOrderIndex != -1) {
            for(int i = 0; i < playerHandOrders.size(); i++) {
                if (playerHandOrders[i]->getTypeAsString() == stringEnumCards[chosenOrderIndex]) {
                    delete playerHandOrders[i];
                    playerHandOrders.erase(playerHandOrders.begin() + i);
                    this->hand->setHandCards(playerHandOrders);//Update the player hand
                    break;
                }
            }
        }

        if(this->hand->getHandCards().empty()) {
            std::cout << "You have no cards in your hand. Your turn for issuing orders is over";
            playerHasFinishedIssuingOrders = true;
            break;
        }
    }
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

