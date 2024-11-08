#include "Player.h"

#include "../Cards/Cards.h"
#include "../Map/Map.h"
#include "../Orders/Orders.h"
#include <set>
#include <algorithm>
#include <limits>

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

void Player::issueOrder(std::vector<Player*>* players) {
    std::cout << "Here are the territories you should defend:" << std::endl;
    std::vector<Territory*> playerTerritories = toDefend();
    std::vector<Territory*> territoriesToAttack = toAttack();
    std::vector<Territory*> allTerritories;

    allTerritories.reserve(playerTerritories.size() + territoriesToAttack.size());
    allTerritories.insert(allTerritories.end(),playerTerritories.begin(),playerTerritories.end());
    allTerritories.insert(allTerritories.end(), territoriesToAttack.begin(),territoriesToAttack.end());

    for(Territory* territory: playerTerritories) {
        std::cout << territory->getName() << std::endl;
    }

    std::cout << "\nFor now, use the deploy order until you have no more reinforcements to deploy." << std::endl;

    int reinforcements = this->getReinforcements();
    while (reinforcements != 0) {
        std::cout << "\nRemaining units: " << reinforcements << std::endl;

        int unitsToUse = 0;

        std::cout << "How many units would you like to deploy?" << std::endl;
        std::cin >> unitsToUse;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "To which territory do you want to deploy these " << unitsToUse << "? Type the name of the territory as you see it." << std::endl;
        std::string territoryName;
        std::getline(std::cin,territoryName);

        auto target = std::ranges::find_if(playerTerritories, [&territoryName](Territory* territory) {
                return territory->getName() == territoryName;
        });

        reinforcements -= unitsToUse;
        if(target != playerTerritories.end()) {
            orders->addOrder(std::make_unique<DeployOrder>(this,*target,new int(unitsToUse)));
        } else {
            orders->addOrder(std::make_unique<DeployOrder>(this,nullptr,new int(unitsToUse)));
        }
    }

    if(this->hand->getHandCards().empty()) {
        std::cout << "You have no cards in your hand. Your turn for issuing orders is over" << std::endl;
        return;
    }

    std::cout << "\nYou have now issue deployment orders for all of your reinforcements units. You can now issue orders other than deploy now." << std::endl;
    //Once all reinforcements are deployed then show the toAttack stuff
    std::cout << "\nHere are the territories you should attack:" << std::endl;
    for(Territory* territory: territoriesToAttack) {
        std::cout << territory->getName() << std::endl;
    }

    bool playerHasFinishedIssuingOrders = false;
    const std::vector<std::string> possibleOrders = {"AdvanceOrder","AirliftOrder","BlockadeOrder","BombOrder","NegotiateOrder"};
    const std::vector<std::string> stringEnumCards = {"Reinforcement","Airlift","Blockade","Bomb","Diplomacy"};

    std::vector<Card*> playerHandOrders = this->hand->getHandCards();

    while (!playerHasFinishedIssuingOrders) {
        if(this->hand->getHandCards().empty()) {
            std::cout << "You have no cards in your hand. Your turn for issuing orders is over" << std::endl;
            playerHasFinishedIssuingOrders = true;
            break;
        }
        std::cout << "\nYour available orders are:" << std::endl;
        int chosenOrderIndex = -1;
        for (const Card* card: playerHandOrders) {
            std::cout << card->getTypeAsString() << std::endl;
        }

        std::cout << "\nType the order as you see it if you wish to issue it or type endissueorders to stop issuing orders" << std::endl;

        std::string orderInput;
        std::cin >> orderInput;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (orderInput == "endissueorders") {
            playerHasFinishedIssuingOrders = true;
            std::cout << "Player has decided to stop issuing orders. Ending issue order turn for this player" << std::endl;
            break;
        }

        if (orderInput == stringEnumCards[0]) {
            std::string sourceTerritory;
            std::string targetTerritory;
            int numOfUnits = 0;

            std::cout << "From which territory do you wish to move units from?" << std::endl;
            std::getline(std::cin,sourceTerritory);
            auto source = std::ranges::find_if(allTerritories, [&sourceTerritory](Territory* territory) {
                return territory->getName() == sourceTerritory;
            });


            std::cout << "To what territory should these units be deployed?" << std::endl;
            std::getline(std::cin,targetTerritory);
            auto target = std::ranges::find_if(allTerritories, [&targetTerritory](Territory* territory) {
                return territory->getName() == targetTerritory;
            });

            std::cout << "Lastly, how many units do you want to move." << std::endl;
            std::cin >> numOfUnits;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (source != allTerritories.end() && target != allTerritories.end()) {
                this->orders->addOrder(std::make_unique<AdvanceOrder>(this, *source, *target, new int(numOfUnits)));
                chosenOrderIndex = 0;
            } else {
                std::cout << "The target or source territory is not in the list of territories to defend or attack. Try again" << std::endl;
            }
        } else if (orderInput == stringEnumCards[1]) {
            std::string sourceTerritory;
            std::string targetTerritory;
            int numOfUnits = 0;

            std::cout << "From which territory do you wish to airlift units from?" << std::endl;
            std::getline(std::cin,sourceTerritory);
            auto source = std::ranges::find_if(allTerritories, [&sourceTerritory](Territory* territory) {
                return territory->getName() == sourceTerritory;
            });

            std::cout << "To what territory should these units be deployed?" << std::endl;
            std::getline(std::cin,targetTerritory);
            auto target = std::ranges::find_if(allTerritories, [&targetTerritory](Territory* territory) {
                return territory->getName() == targetTerritory;
            });

            std::cout << "Lastly, how many units do you want to airlift. Source territory has "<< (*source)->getArmies() << " units you can move" << std::endl;
            std::cin >> numOfUnits;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (source != allTerritories.end() && target != allTerritories.end()) {
                this->orders->addOrder(std::make_unique<AirliftOrder>(this,  *source, *target, new int(numOfUnits)));
                chosenOrderIndex = 1;
            } else {
                std::cout << "The target or source territory is not in the list of territories to defend or attack. Try again" << std::endl;
            }
        } else if (orderInput == stringEnumCards[2]) {
            std::string targetTerritory;
            std::cout << "What territory do you want to blockade?" << std::endl;
            std::getline(std::cin,targetTerritory);

            auto target = std::ranges::find_if(allTerritories, [&targetTerritory](Territory* territory) {
                return territory->getName() == targetTerritory;
            });

            if (target != playerTerritories.end()) {
                this->orders->addOrder(std::make_unique<BlockadeOrder>(this, *target));
                chosenOrderIndex = 2;
            } else {
                std::cout << "The target territory was not found. Try again" << std::endl;
            }
        } else if (orderInput == stringEnumCards[3]) {
            std::string targetTerritory;
            std::cout << "What territory do you want to bomb?" << std::endl;
            std::getline(std::cin,targetTerritory);

            auto target = std::ranges::find_if(allTerritories, [&targetTerritory](Territory* territory) {
                return territory->getName() == targetTerritory;
            });

            if (target != playerTerritories.end()) {
                this->orders->addOrder(std::make_unique<BombOrder>(this, *target));
                chosenOrderIndex = 3;
            } else {
                std::cout << "The target territory was not found. Try again" << std::endl;
            }
        } else if (orderInput == stringEnumCards[4]) {
            std::string targetPlayer;
            std::cout << "With who do you want to negotiate with? Type their name as it appears:" << std::endl;
            for(Player* player: *players) {
                std::cout << player->getName() << std::endl;
            }

            std::getline(std::cin, targetPlayer);

            auto target = std::ranges::find_if(*players, [&targetPlayer](Player* player) {
                return player->getName() == targetPlayer;
            });

            if (target != players->end()) {
                this->orders->addOrder(std::make_unique<NegotiateOrder>(this,*target));
                chosenOrderIndex = 4;
            } else {
                std::cout << "The target player was not found. Try again" << std::endl;
            }
        } else {
            std::cout << "Invalid command. Did nothing, please re-issue your order" << std::endl;
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

