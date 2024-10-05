#include "Orders.h"

namespace Orders {

    // Implementation of DeployOrder class
    bool DeployOrder::validate() {
        std::cout << "Validating Deploy Order" << std::endl;
        return true;
    }

    void DeployOrder::execute() {
        std::cout << "Executing Deploy Order" << std::endl;
    }

    std::string DeployOrder::description() const {
        return "Deploy Order";
    }

    // Implementation of AdvanceOrder class
    bool AdvanceOrder::validate() {
        std::cout << "Validating Advance Order" << std::endl;
        return true;
    }

    void AdvanceOrder::execute() {
        std::cout << "Executing Advance Order" << std::endl;
    }

    std::string AdvanceOrder::description() const {
        return "Advance Order";
    }

    // Implementation of BombOrder class
    bool BombOrder::validate() {
        std::cout << "Validating Bomb Order" << std::endl;
        return true;
    }

    void BombOrder::execute() {
        std::cout << "Executing Bomb Order" << std::endl;
    }

    std::string BombOrder::description() const {
        return "Bomb Order";
    }

    // Implementation of BlockadeOrder class
    bool BlockadeOrder::validate() {
        std::cout << "Validating Blockade Order" << std::endl;
        return true;
    }

    void BlockadeOrder::execute() {
        std::cout << "Executing Blockade Order" << std::endl;
    }

    std::string BlockadeOrder::description() const {
        return "Blockade Order";
    }

    // Implementation of AirliftOrder class
    bool AirliftOrder::validate() {
        std::cout << "Validating Airlift Order" << std::endl;
        return true;
    }

    void AirliftOrder::execute() {
        std::cout << "Executing Airlift Order" << std::endl;
    }

    std::string AirliftOrder::description() const {
        return "Airlift Order";
    }

    // Implementation of NegotiateOrder class
    bool NegotiateOrder::validate() {
        std::cout << "Validating Negotiate Order" << std::endl;
        return true;
    }

    void NegotiateOrder::execute() {
        std::cout << "Executing Negotiate Order" << std::endl;
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

    // Execute all valid orders
    void OrderList::executeOrders() {
        for (const auto& order : orders) {
            if (order->validate()) {
                order->execute();
            } else {
                std::cout << "Invalid order, skipping execution." << std::endl;
            }
        }
        orders.clear();
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
