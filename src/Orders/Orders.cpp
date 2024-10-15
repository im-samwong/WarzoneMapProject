#include "Orders.h"
#include "../Player/Player.h"
#include "../Map/Map.h"
#include <iostream>

namespace Orders {

    // Implementation of DeployOrder class
    bool DeployOrder::validate(Player::Player* sourcePlayer, Player::Player* targetPlayer, Territory* source, Territory* target, int numUnits) const {
        if (target->getOwner() != sourcePlayer) {
            std::cout << "DeployOrder Validation Failed: Target territory is not owned by the player." << std::endl;
            return false;
        }
        if (numUnits > sourcePlayer->getReinforcementPool()) {
            std::cout << "DeployOrder Validation Failed: Not enough units in the reinforcement pool." << std::endl;
            return false;
        }
        return true;
    }

    void DeployOrder::execute(Player::Player* sourcePlayer, Player::Player* targetPlayer, Territory* source, Territory* target, int numUnits) {
        if (validate(sourcePlayer, targetPlayer, source, target, numUnits)) {
            target->addUnits(numUnits);
            sourcePlayer->removeFromReinforcementPool(numUnits);
            std::cout << "Executed DeployOrder: " << numUnits << " units deployed to " << target->getName() << std::endl;
        }
    }

    std::string DeployOrder::description() const {
        return "Deploy Order";
    }

    // Implementation of AdvanceOrder class
bool AdvanceOrder::validate(Player::Player* sourcePlayer, Player::Player* targetPlayer, Territory* source, Territory* target, int numUnits) const {
    if (source->getOwner() != sourcePlayer) {
        std::cout << "AdvanceOrder Validation Failed: Source territory is not owned by the player." << std::endl;
        return false;
    }
    if (numUnits > source->getUnits()) {
        std::cout << "AdvanceOrder Validation Failed: Not enough units in the source territory." << std::endl;
        return false;
    }
    if (!source->isAdjacentTo(target)) {
        std::cout << "AdvanceOrder Validation Failed: Target is not adjacent to the source territory." << std::endl;
        return false;
    }
    return true;
}

void AdvanceOrder::execute(Player::Player* sourcePlayer, Player::Player* targetPlayer, Territory* source, Territory* target, int numUnits) {
    if (validate(sourcePlayer, targetPlayer, source, target, numUnits)) {
        if (target->getOwner() == sourcePlayer) {
            // Move units between territories
            target->addUnits(numUnits);
            source->removeUnits(numUnits);
            std::cout << "Executed AdvanceOrder: Moved " << numUnits << " units from " << source->getName()
                      << " to " << target->getName() << std::endl;
        } else {
            // Simulate attack
            int attackingUnitsLost = numUnits * 0.6;
            int defendingUnitsLost = target->getUnits() * 0.7;
            source->removeUnits(attackingUnitsLost);
            target->removeUnits(defendingUnitsLost);

            if (target->getUnits() == 0) {
                target->setOwner(sourcePlayer);
                std::cout << "Executed AdvanceOrder: " << target->getName() << " has been conquered!" << std::endl;
            } else {
                std::cout << "Executed AdvanceOrder: Attack failed, " << target->getName()
                          << " still belongs to " << target->getOwner()->getName() << std::endl;
            }
        }
    }
}

    std::string AdvanceOrder::description() const {
        return "Advance Order";
    }

    // Implementation of BombOrder class
    bool BombOrder::validate(Player::Player* sourcePlayer, Player::Player* targetPlayer, Territory* source, Territory* target, int numUnits) const {
        if (target->getOwner() == sourcePlayer) {
            std::cout << "BombOrder Validation Failed: Cannot bomb your own territory." << std::endl;
            return false;
        }
        return true;
    }

    void BombOrder::execute(Player::Player* sourcePlayer, Player::Player* targetPlayer, Territory* source, Territory* target, int numUnits) {
        if (validate(sourcePlayer, targetPlayer, source, target, numUnits)) {
            int remainingUnits = target->getUnits() / 2;
            target->setUnits(remainingUnits);
            std::cout << "Executed BombOrder: Halved the units on " << target->getName() << std::endl;
        }
    }

    std::string BombOrder::description() const {
        return "Bomb Order";
    }

    // Implementation of BlockadeOrder class
    bool BlockadeOrder::validate(Player::Player* sourcePlayer, Player::Player* targetPlayer, Territory* source, Territory* target, int numUnits) const {
        if (target->getOwner() != sourcePlayer) {
            std::cout << "BlockadeOrder Validation Failed: Target territory is not owned by the player." << std::endl;
            return false;
        }
        return true;
    }

    void BlockadeOrder::execute(Player::Player* sourcePlayer, Player::Player* targetPlayer, Territory* source, Territory* target, int numUnits) {
        if (validate(sourcePlayer, targetPlayer, source, target, numUnits)) {
            target->setUnits(target->getUnits() * 2);
            target->setOwner(nullptr);  // Assuming nullptr represents neutral ownership
            std::cout << "Executed BlockadeOrder: Doubled units and transferred " << target->getName()
                      << " to neutral ownership." << std::endl;
        }
    }

    std::string BlockadeOrder::description() const {
        return "Blockade Order";
    }

    // Implementation of AirliftOrder class
    bool AirliftOrder::validate(Player::Player* sourcePlayer, Player::Player* targetPlayer, Territory* source, Territory* target, int numUnits) const {
        if (source->getOwner() != sourcePlayer || target->getOwner() != sourcePlayer) {
            std::cout << "AirliftOrder Validation Failed: Both source and target must be owned by the player." << std::endl;
            return false;
        }
        if (numUnits > source->getUnits()) {
            std::cout << "AirliftOrder Validation Failed: Not enough units in the source territory." << std::endl;
            return false;
        }
        return true;
    }

    void AirliftOrder::execute(Player::Player* sourcePlayer, Player::Player* targetPlayer, Territory* source, Territory* target, int numUnits) {
        if (validate(sourcePlayer, targetPlayer, source, target, numUnits)) {
            target->addUnits(numUnits);
            source->removeUnits(numUnits);
            std::cout << "Executed AirliftOrder: Moved " << numUnits << " units from " << source->getName()
                      << " to " << target->getName() << std::endl;
        }
    }

    std::string AirliftOrder::description() const {
        return "Airlift Order";
    }

    // Implementation of NegotiateOrder class
    bool NegotiateOrder::validate(Player::Player* sourcePlayer, Player::Player* targetPlayer, Territory* source, Territory* target, int numUnits) const {
        if (targetPlayer == sourcePlayer) {
            std::cout << "NegotiateOrder Validation Failed: Cannot negotiate with yourself." << std::endl;
            return false;
        }
        return true;
    }

    void NegotiateOrder::execute(Player::Player* sourcePlayer, Player::Player* targetPlayer, Territory* source, Territory* target, int numUnits) {
        if (validate(sourcePlayer, targetPlayer, source, target, numUnits)) {
            std::cout << "Executed NegotiateOrder: " << sourcePlayer->getName() << " and " << targetPlayer->getName()
                      << " will not attack each other this turn." << std::endl;
            // Add negotiation logic here (e.g., adding to a game state where attacks between players are prevented)
        }
    }

    std::string NegotiateOrder::description() const {
        return "Negotiate Order";
    }

    // Implementation of OrderList class
    // Default constructor
    OrderList::OrderList() = default;  // Default constructor implementation

    // Copy constructor for OrderList
    OrderList::OrderList(const OrderList& other) {
        for (const auto& order : other.orders) {
            orders.push_back(order->clone());  // Use the clone method for deep copy
        }
    }

    // Assignment operator for OrderList
    OrderList& OrderList::operator=(const OrderList& other) {
        if (this == &other) return *this;

        orders.clear();
        for (const auto& order : other.orders) {
            orders.push_back(order->clone());  // Use the clone method for deep copy
        }
        return *this;
    }

    // Add an order
    void OrderList::addOrder(std::unique_ptr<Order> order) {
        orders.push_back(std::move(order));
    }

    // Remove an order by index
    void OrderList::removeOrder(int index) {
        if (index >= 0 && index < orders.size()) {
            orders.erase(orders.begin() + index);
        } else {
            std::cerr << "Invalid index" << std::endl;
        }
    }

    // Move an order in the list
    void OrderList::moveOrder(int fromIndex, int toIndex) {
        if (fromIndex >= 0 && fromIndex < orders.size() && toIndex >= 0 && toIndex < orders.size()) {
            auto order = std::move(orders[fromIndex]);
            orders.erase(orders.begin() + fromIndex);
            orders.insert(orders.begin() + toIndex, std::move(order));
        } else {
            std::cerr << "Invalid indices" << std::endl;
        }
    }

    // Print the list of orders
    void OrderList::printOrders() const {
        for (std::size_t i = 0; i < orders.size(); ++i) {
            std::cout << "Order at index " << i << ": " << *orders[i] << std::endl;
        }
    }

    // Get the number of orders
    std::size_t OrderList::getSize() const {
        return orders.size();
    }

    // Get the list of orders
    const std::vector<std::unique_ptr<Order>>& OrderList::getOrders() const {
        return orders;
    }

}
