//
// Created by samue on 2024-09-12.
//

#include "Orders.h"

namespace Orders {
    // Implementation of DeployOrder class
    bool DeployOrder::validate() {
        // Add validation logic for the deploy order
        std::cout << "Validating Deploy Order" << std::endl;
        return true;  // Assume valid for now
    }

    void DeployOrder::execute() {
        // Add execution logic for the deploy order
        std::cout << "Executing Deploy Order" << std::endl;
    }

    std::string DeployOrder::description() const {
        return "Deploy Order";
    }

    // Implementation of AdvanceOrder class
    bool AdvanceOrder::validate() {
        // Add validation logic for the advance order
        std::cout << "Validating Advance Order" << std::endl;
        return true;  // Assume valid for now
    }

    void AdvanceOrder::execute() {
        // Add execution logic for the advance order
        std::cout << "Executing Advance Order" << std::endl;
    }

    std::string AdvanceOrder::description() const {
        return "Advance Order";
    }

    // Implementation of BombOrder class
    bool BombOrder::validate() {
        // Add validation logic for the bomb order
        std::cout << "Validating Bomb Order" << std::endl;
        return true;  // Assume valid for now
    }

    void BombOrder::execute() {
        // Add execution logic for the bomb order
        std::cout << "Executing Bomb Order" << std::endl;
    }

    std::string BombOrder::description() const {
        return "Bomb Order";
    }

    // Implementation of BlockadeOrder class
    bool BlockadeOrder::validate() {
        // Add validation logic for the blockade order
        std::cout << "Validating Blockade Order" << std::endl;
        return true;  // Assume valid for now
    }

    void BlockadeOrder::execute() {
        // Add execution logic for the blockade order
        std::cout << "Executing Blockade Order" << std::endl;
    }

    std::string BlockadeOrder::description() const {
        return "Blockade Order";
    }

    // Implementation of AirliftOrder class
    bool AirliftOrder::validate() {
        // Add validation logic for the airlift order
        std::cout << "Validating Airlift Order" << std::endl;
        return true;  // Assume valid for now
    }

    void AirliftOrder::execute() {
        // Add execution logic for the airlift order
        std::cout << "Executing Airlift Order" << std::endl;
    }

    std::string AirliftOrder::description() const {
        return "Airlift Order";
    }

    // Implementation of NegotiateOrder class
    bool NegotiateOrder::validate() {
        // Add validation logic for the negotiate order
        std::cout << "Validating Negotiate Order" << std::endl;
        return true;  // Assume valid for now
    }

    void NegotiateOrder::execute() {
        // Add execution logic for the negotiate order
        std::cout << "Executing Negotiate Order" << std::endl;
    }

    std::string NegotiateOrder::description() const {
        return "Negotiate Order";
    }

    // Implementation of OrderList class
    void OrderList::addOrder(std::unique_ptr<Order> order) {
        orders.push_back(std::move(order));  // Add the order to the queue
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
            // Transferring ownerships because of unique_ptr to auto order
            auto order = std::move(orders[fromIndex]);
            // Erase the order in the fromIndex so that the vector class shifts everything down
            orders.erase(orders.begin() + fromIndex);
            // Now Transfer of ownership once more and back to the new location in the vector
            orders.insert(orders.begin() + toIndex, std::move(order));
        } else {
            std::cerr << "Invalid indices" << std::endl;
        }
    }

    void OrderList::executeOrders() {
        for (const auto& order : orders) {
            // Validate each order then execute
            if (order->validate()) {
                order->execute();
            } else {
                std::cout << "Invalid order, skipping execution." << std::endl;
            }
        }
        std::cout << "Completed execution of orders operation." << std::endl;
        // Clear order at the end of round
        orders.clear();
    }

    void OrderList::printOrders() const {
        // Displaying all the orders and their index in the queue
        for (size_t i = 0; i < orders.size(); ++i) {
            std::cout << "Order at index " << i << ": " << *orders[i] << std::endl;
        }
    }
}


