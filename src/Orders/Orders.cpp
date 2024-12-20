#include "Orders.h"
#include "../GameEngine/GameEngine.h"
#include <cstdlib>

// clone method implementations
std::unique_ptr<Order> DeployOrder::clone() const {
    return std::make_unique<DeployOrder>(*this);
}

std::unique_ptr<Order> AdvanceOrder::clone() const {
    return std::make_unique<AdvanceOrder>(*this);
}

std::unique_ptr<Order> BombOrder::clone() const {
    return std::make_unique<BombOrder>(*this);
}

std::unique_ptr<Order> BlockadeOrder::clone() const {
    return std::make_unique<BlockadeOrder>(*this);
}

std::unique_ptr<Order> AirliftOrder::clone() const {
    return std::make_unique<AirliftOrder>(*this);
}

std::unique_ptr<Order> NegotiateOrder::clone() const {
    return std::make_unique<NegotiateOrder>(*this);
}

// Order Base Class

Order::Order()
    : sourcePlayer(nullptr), targetPlayer(nullptr), source(nullptr), target(nullptr), numUnits(nullptr) {}

Order::Order(Player* sourcePlayer, Player* targetPlayer, Territory* source, Territory* target, int* numUnits)
    : sourcePlayer(sourcePlayer),
      targetPlayer(targetPlayer),
      source(source),
      target(target),
      numUnits(numUnits) {}

Order::Order(const Order& other)
    : sourcePlayer(other.sourcePlayer ? new Player(*other.sourcePlayer) : nullptr),
      targetPlayer(other.targetPlayer ? new Player(*other.targetPlayer) : nullptr),
      source(other.source ? new Territory(*other.source) : nullptr),
      target(other.target ? new Territory(*other.target) : nullptr),
      numUnits(other.numUnits ? new int(*other.numUnits) : nullptr) {}

Order& Order::operator=(const Order& other) {
    if (this != &other) {
        delete sourcePlayer;
        delete targetPlayer;
        delete source;
        delete target;
        delete numUnits;

        sourcePlayer = other.sourcePlayer ? new Player(*other.sourcePlayer) : nullptr;
        targetPlayer = other.targetPlayer ? new Player(*other.targetPlayer) : nullptr;
        source = other.source ? new Territory(*other.source) : nullptr;
        target = other.target ? new Territory(*other.target) : nullptr;
        numUnits = other.numUnits ? new int(*other.numUnits) : nullptr;
    }
    return *this;
}

// Stream insertion operator for Order
std::ostream& operator<<(std::ostream& os, const Order& order) {
    os << order.description();  // Calls the subclass's description() method
    return os;
}

Order::~Order() {
    //Don't delete these because we're actually passing them and not copies --> Very risky
    // delete sourcePlayer;
    // delete targetPlayer;
    // delete source;
    // delete target;
    delete numUnits;
}

// DeployOrder Class

DeployOrder::DeployOrder() : Order() {}

DeployOrder::DeployOrder(Player* sourcePlayer, Territory* target, int* numUnits)
    : Order(sourcePlayer, nullptr, nullptr, target, numUnits) {}

DeployOrder::DeployOrder(const DeployOrder& other) : Order(other) {}

DeployOrder& DeployOrder::operator=(const DeployOrder& other) {
    if (this != &other) {
        Order::operator=(other);
    }
    return *this;
}

bool DeployOrder::validate()  {
    effects = new std::string("");

    if (target == nullptr || target->getOwner() != sourcePlayer) {
        *effects += "DeployOrder Validation Failed: Target territory is not owned by the player.\n";
        std::cout << "DeployOrder Validation Failed: Target territory is not owned by the player." << std::endl;
        return false;
    }
    if (*numUnits > sourcePlayer->getReinforcements()) {
        *effects += "DeployOrder Validation Failed: Not enough units in the reinforcement pool.\n";
        std::cout << "DeployOrder Validation Failed: Not enough units in the reinforcement pool." << std::endl;
        return false;
    }
    return true;
}

void DeployOrder::execute() {
    if (validate()) {
        target->modifyArmies(*numUnits);
        sourcePlayer->changeReinforcements(-(*numUnits));
        *effects += "Executed DeployOrder: " + std::to_string(*numUnits) + " units deployed to " + target->getName() + " for player " + sourcePlayer->getName() + "\n";
        std::cout << "Executed DeployOrder for player " << sourcePlayer->getName() << ": " << *numUnits << " units deployed to " << target->getName() << std::endl;
    }

    if (logObserver != nullptr) {
        notify(this);
    }

    delete effects;
    effects = nullptr;
}

std::string DeployOrder::stringToLog() const {
    return *effects;
}

std::string DeployOrder::description() const {
    return "Deploy Order";
}

// AdvanceOrder Class

AdvanceOrder::AdvanceOrder() : Order() {}

AdvanceOrder::AdvanceOrder(Player* sourcePlayer, Territory* source, Territory* target, int* numUnits)
    : Order(sourcePlayer, nullptr, source, target, numUnits) {}

AdvanceOrder::AdvanceOrder(const AdvanceOrder& other) : Order(other) {}

AdvanceOrder& AdvanceOrder::operator=(const AdvanceOrder& other) {
    if (this != &other) {
        Order::operator=(other);
    }
    return *this;
}

bool AdvanceOrder::validate()  {
    effects = new std::string("");

    // Check if the source territory is owned by the sourcePlayer
    if (source->getOwner() != sourcePlayer) {
        *effects += "AdvanceOrder Validation Failed: Source territory is not owned by the player.\n";
        std::cout << "AdvanceOrder Validation Failed: Source territory is not owned by the player." << std::endl;
        return false;
    }

    // Check if there are enough units in the source territory
    if (*numUnits > source->getArmies()) {
        *effects += "AdvanceOrder Validation Failed: Not enough units in the source territory.\n";
        std::cout << "AdvanceOrder Validation Failed: Not enough units in the source territory." << std::endl;
        return false;
    }

    // Check if the target territory is a neighbor of the source territory
    const std::vector<Territory*>* neighbors = source->getNeighbors();
    bool isAdjacent = false;
    for (const Territory* neighbor : *neighbors) {
        if (neighbor == target) {
            isAdjacent = true;
            break;
        }
    }

    if (!isAdjacent) {
        *effects += "AdvanceOrder Validation Failed: Target is not adjacent to the source territory.\n";
        std::cout << "AdvanceOrder Validation Failed: Target is not adjacent to the source territory." << std::endl;
        return false;
    }

    // Check for a truce only if the target is owned by another player
    if (target->getOwner() != sourcePlayer && GameState::hasNegotiation(sourcePlayer, target->getOwner())) {
        *effects += "AdvanceOrder Validation Failed: A truce is in place between " + sourcePlayer->getName()
                  + " and " + target->getOwner()->getName() + ". Attack not allowed.\n";
        std::cout << "AdvanceOrder Validation Failed: A truce is in place between " << sourcePlayer->getName()
                  << " and " << target->getOwner()->getName() << ". Attack not allowed." << std::endl;
        return false;
    }

    return true;
}

void AdvanceOrder::execute() {
     if (validate()) {
        if (target->getOwner() == sourcePlayer) {
            // Move units between territories owned by the same player
            target->modifyArmies(*numUnits);
            source->modifyArmies(-*numUnits);
            *effects += "Executed AdvanceOrder: Moved " + std::to_string(*numUnits) + " units from " + source->getName()
                      + " to " + target->getName() + ".\n";
            std::cout << "Executed AdvanceOrder: Moved " << *numUnits << " units from " << source->getName()
                      << " to " << target->getName() << "." << std::endl;
        } else {
            // Begin battle simulation if target belongs to another player
            int attackingUnits = *numUnits;
            int defendingUnits = target->getArmies();

            // Variables to track remaining units after each round of attacks
            int survivingAttackers = attackingUnits;
            int survivingDefenders = defendingUnits;

            // Battle simulation loop
            while (survivingAttackers > 0 && survivingDefenders > 0) {
                // Each attacking unit has a 60% chance of killing a defending unit
                for (int i = 0; i < survivingAttackers; ++i) {
                    if (static_cast<float>(rand()) / RAND_MAX < 0.6) {
                        survivingDefenders--;
                        if (survivingDefenders == 0) {
                            break;  // Stop if all defenders are killed
                        }
                    }
                }

                // Each defending unit has a 70% chance of killing an attacking unit
                for (int i = 0; i < survivingDefenders; ++i) {
                    if (static_cast<float>(rand()) / RAND_MAX < 0.7) {
                        survivingAttackers--;
                        if (survivingAttackers == 0) {
                            break;  // Stop if all attackers are killed
                        }
                    }
                }
            }

            // Apply the results of the battle
            source->modifyArmies(-attackingUnits);  // Remove all units initially sent from the source territory
            target->modifyArmies(-defendingUnits);  // Remove all units initially in the target territory

            if (survivingDefenders == 0) {
                // Attacker wins and captures the territory
                target->setOwner(sourcePlayer);
                target->modifyArmies(survivingAttackers);  // Occupy the territory with surviving attackers
                *effects += "Executed AdvanceOrder: " + target->getName() + " has been conquered!\n";
                std::cout << "Executed AdvanceOrder: " << target->getName() << " has been conquered!" << std::endl;

                // Set conquered territory status for the player to receive a card
                GameState::setConqueredTerritory(sourcePlayer, true);
            } else {
                // Attack failed, so target retains its ownership, and all attacking units are lost
                *effects += "Executed AdvanceOrder: Attack failed. " + target->getName()
                          + " remains under control of " + target->getOwner()->getName() + ".\n";
                std::cout << "Executed AdvanceOrder: Attack failed. " << target->getName()
                          << " remains under control of " << target->getOwner()->getName() << "." << std::endl;
                target->modifyArmies(survivingDefenders);  // Restore the surviving defenders in the target territory
            }

            // Output the battle result
            *effects += "Battle Result: " + std::to_string(attackingUnits - survivingAttackers) + " attacking units lost, "
                      + std::to_string(defendingUnits - survivingDefenders) + " defending units lost.\n";
            std::cout << "Battle Result: " << (attackingUnits - survivingAttackers) << " attacking units lost, "
                      << (defendingUnits - survivingDefenders) << " defending units lost." << std::endl;
        }
    }

    if (logObserver != nullptr) {
        notify(this);
    }

    delete effects;
    effects = nullptr;
}

std::string AdvanceOrder::stringToLog() const {
    return *effects;
}

std::string AdvanceOrder::description() const {
    return "Advance Order";
}

// BombOrder Class

BombOrder::BombOrder() : Order() {}

BombOrder::BombOrder(Player* sourcePlayer, Territory* target)
    : Order(sourcePlayer, nullptr, nullptr, target, nullptr) {}

BombOrder::BombOrder(const BombOrder& other) : Order(other) {}

BombOrder& BombOrder::operator=(const BombOrder& other) {
    if (this != &other) {
        Order::operator=(other);
    }
    return *this;
}

bool BombOrder::validate()  {
    effects = new std::string("");

    // Check if the target territory belongs to the issuing player
    if (target->getOwner() == sourcePlayer) {
        *effects += "BombOrder Validation Failed: Cannot bomb your own territory\n.";
        std::cout << "BombOrder Validation Failed: Cannot bomb your own territory." << std::endl;
        return false;
    }

    const std::vector<Territory*>* neighbors = target->getNeighbors();
    if (!neighbors) {
        *effects += "BombOrder Validation Failed: Source territory has no neighbors.\n";
        std::cout << "BombOrder Validation Failed: Source territory has no neighbors." << std::endl;
        return false;
    }

    // Check if the target territory is adjacent to source player territory
    bool isAdjacent = false;
    for (const Territory* neighbor : *neighbors) {
        if (neighbor->getOwner() == sourcePlayer) {
            isAdjacent = true;
            break;
        }
    }

    if (!isAdjacent) {
        *effects += "BombOrder Validation Failed: Target territory is not adjacent to any territory owned by the player.\n";
        std::cout << "BombOrder Validation Failed: Target territory is not adjacent to any territory owned by the player." << std::endl;
        return false;
    }

    // If all checks pass, the order is valid
    return true;
}

void BombOrder::execute() {
    if (validate()) {
        // Calculate units to remove (half of the army units)
        int unitsToRemove = target->getArmies() / 2;
        target->modifyArmies(-unitsToRemove);
        *effects += "Executed BombOrder: Halved the units on " + target->getName() + ". Remaining units: " + std::to_string(target->getArmies());
        std::cout << "Executed BombOrder: Halved the units on " << target->getName() << ". Remaining units: " << target->getArmies() << std::endl;
    }

    if (logObserver != nullptr) {
        notify(this);
    }

    delete effects;
    effects = nullptr;
}

std::string BombOrder::stringToLog() const {
    return *effects;
}

std::string BombOrder::description() const {
    return "Bomb Order";
}

// BlockadeOrder Class

BlockadeOrder::BlockadeOrder() : Order() {}

BlockadeOrder::BlockadeOrder(Player* sourcePlayer, Territory* target)
    : Order(sourcePlayer, nullptr, nullptr, target, nullptr) {}

BlockadeOrder::BlockadeOrder(const BlockadeOrder& other) : Order(other) {}

BlockadeOrder& BlockadeOrder::operator=(const BlockadeOrder& other) {
    if (this != &other) {
        Order::operator=(other);
    }
    return *this;
}

bool BlockadeOrder::validate() {
    effects = new std::string("");

    if (target->getOwner() != sourcePlayer) {
        *effects += "BlockadeOrder Validation Failed: Target territory is not owned by the player.\n";
        std::cout << "BlockadeOrder Validation Failed: Target territory is not owned by the player." << std::endl;
        return false;
    }
    return true;
}

void BlockadeOrder::execute() {
    if (validate()) {
        int currentArmies = target->getArmies();
        target->modifyArmies(currentArmies);
        target->setOwner(nullptr);  // Assuming nullptr represents neutral ownership
        *effects += "Executed BlockadeOrder: Doubled units and transferred " + target->getName()
                  + " to neutral ownership.\n";
        std::cout << "Executed BlockadeOrder: Doubled units and transferred " << target->getName()
                  << " to neutral ownership." << std::endl;
    }

    if (logObserver != nullptr) {
        notify(this);
    }

    delete effects;
    effects = nullptr;
}

std::string BlockadeOrder::stringToLog() const {
    return *effects;
}

std::string BlockadeOrder::description() const {
    return "Blockade Order";
}

// AirliftOrder Class

AirliftOrder::AirliftOrder() : Order() {}

AirliftOrder::AirliftOrder(Player* sourcePlayer, Territory* source, Territory* target, int* numUnits)
    : Order(sourcePlayer, nullptr, source, target, numUnits) {}

AirliftOrder::AirliftOrder(const AirliftOrder& other) : Order(other) {}

AirliftOrder& AirliftOrder::operator=(const AirliftOrder& other) {
    if (this != &other) {
        Order::operator=(other);
    }
    return *this;
}

bool AirliftOrder::validate() {
    effects = new std::string("");

    if (source->getOwner() != sourcePlayer || target->getOwner() != sourcePlayer) {
        *effects += "AirliftOrder Validation Failed: Both source and target must be owned by the player.\n";
        std::cout << "AirliftOrder Validation Failed: Both source and target must be owned by the player." << std::endl;
        return false;
    }
    if (*numUnits > source->getArmies()) {
        *effects += "AirliftOrder Validation Failed: Not enough units in the source territory.\n";
        std::cout << "AirliftOrder Validation Failed: Not enough units in the source territory." << std::endl;
        return false;
    }
    return true;
}

void AirliftOrder::execute() {
    if (validate()) {
        target->modifyArmies(*numUnits);
        source->modifyArmies(-*numUnits);
        *effects += "Executed AirliftOrder: Moved " + std::to_string(*numUnits) + " units from " + source->getName()
                  + " to " + target->getName() + "\n";
        std::cout << "Executed AirliftOrder: Moved " << *numUnits << " units from " << source->getName()
                  << " to " << target->getName() << std::endl;
    }

    if (logObserver != nullptr) {
        notify(this);
    }

    delete effects;
    effects = nullptr;
}

std::string AirliftOrder::stringToLog() const {
    return *effects;
}

std::string AirliftOrder::description() const {
    return "Airlift Order";
}

// NegotiateOrder Class

NegotiateOrder::NegotiateOrder() : Order() {}

NegotiateOrder::NegotiateOrder(Player* sourcePlayer, Player* targetPlayer)
    : Order(sourcePlayer, targetPlayer, nullptr, nullptr, nullptr) {}

NegotiateOrder::NegotiateOrder(const NegotiateOrder& other) : Order(other) {}

NegotiateOrder& NegotiateOrder::operator=(const NegotiateOrder& other) {
    if (this != &other) {
        Order::operator=(other);
    }
    return *this;
}

bool NegotiateOrder::validate() {
    effects = new std::string("");

    if (targetPlayer == sourcePlayer) {
        *effects += "NegotiateOrder Validation Failed: Cannot negotiate with yourself.\n";
        std::cout << "NegotiateOrder Validation Failed: Cannot negotiate with yourself." << std::endl;
        return false;
    }
    return true;
}

void NegotiateOrder::execute() {
    if (validate()) {
        GameState::addNegotiation(sourcePlayer, targetPlayer);
        *effects +="Executed NegotiateOrder: " + sourcePlayer->getName() + " and " + targetPlayer->getName()
                  + " will not attack each other this turn.";
        std::cout << "Executed NegotiateOrder: " << sourcePlayer->getName() << " and " << targetPlayer->getName()
                  << " will not attack each other this turn." << std::endl;
    }

    if (logObserver != nullptr) {
        notify(this);
    }

    delete effects;
    effects = nullptr;
}

std::string NegotiateOrder::stringToLog() const {
    return *effects;
}

std::string NegotiateOrder::description() const {
    return "Negotiate Order";
}

// OrderList Class

OrderList::OrderList() = default;

OrderList::OrderList(const OrderList& other) {
    for (const auto& order : other.orders) {
        orders.push_back(order->clone());
    }
}

OrderList& OrderList::operator=(const OrderList& other) {
    if (this != &other) {
        orders.clear();
        for (const auto& order : other.orders) {
            orders.push_back(order->clone());
        }
    }
    return *this;
}

OrderList::~OrderList() = default;

void OrderList::addOrder(std::unique_ptr<Order> order) {
    orders.push_back(std::move(order));

    if (logObserver != nullptr) {
        notify(this);
    }
}

std::string OrderList::stringToLog() const {
    Order* order = orders.back().get();
    std::string toLog = "Order added to list: " + order->description();
    order = nullptr;
    return toLog;
}

void OrderList::removeOrder(int index) {
    if (index >= 0 && index < orders.size()) {
        orders.erase(orders.begin() + index);
    } else {
        std::cerr << "Invalid index" << std::endl;
    }
}

void OrderList::moveOrder(int fromIndex, int toIndex) {
    if (fromIndex >= 0 && fromIndex < orders.size() && toIndex >= 0 && toIndex < orders.size()) {
        auto order = std::move(orders[fromIndex]);
        orders.erase(orders.begin() + fromIndex);
        orders.insert(orders.begin() + toIndex, std::move(order));
    } else {
        std::cerr << "Invalid indices" << std::endl;
    }
}

void OrderList::printOrders() const {
    for (std::size_t i = 0; i < orders.size(); ++i) {
        std::cout << "Order at index " << i << ": " << *orders[i] << std::endl;
    }
}

std::size_t OrderList::getSize() const {
    return orders.size();
}

const std::vector<std::unique_ptr<Order>>& OrderList::getOrders() const {
    return orders;
}
