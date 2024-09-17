//
// Created by samue on 2024-09-12.
//

#ifndef ORDERS_H
#define ORDERS_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>

namespace Orders {

    // Abstract Order class
    class Order {
    public:
        // Default destructor for Order class
        virtual ~Order() = default;

        // Abstract methods to be implemented by subclasses
        virtual bool validate() = 0;
        virtual void execute() = 0;

        // Stream insertion operator to describe the order so when you do << object of type Order, it will print immediately
        friend std::ostream& operator<<(std::ostream& os, const Order& order) {
            return os << "Order: " << order.description();
        }

    protected:
        virtual std::string description() const = 0;  // For printing the order
    };

    // Subclass: DeployOrder
    class DeployOrder : public Order {
    public:
        bool validate() override;
        void execute() override;

    protected:
        std::string description() const override;
    };

    // Subclass: AdvanceOrder
    class AdvanceOrder : public Order {
    public:
        bool validate() override;
        void execute() override;

    protected:
        std::string description() const override;
    };

    // Subclass: BombOrder
    class BombOrder : public Order {
    public:
        bool validate() override;
        void execute() override;

    protected:
        std::string description() const override;
    };

    // Subclass: BlockadeOrder
    class BlockadeOrder : public Order {
    public:
        bool validate() override;
        void execute() override;

    protected:
        std::string description() const override;
    };

    // Subclass: AirliftOrder
    class AirliftOrder : public Order {
    public:
        bool validate() override;
        void execute() override;

    protected:
        std::string description() const override;
    };

    // Subclass: NegotiateOrder
    class NegotiateOrder : public Order {
    public:
        bool validate() override;
        void execute() override;

    protected:
        std::string description() const override;
    };

    // OrderList class to manage a list of orders
    class OrderList {
    public:
        void addOrder(std::unique_ptr<Order> order);  // Add an order
        void removeOrder(int index);                  // Remove an order by index
        void moveOrder(int fromIndex, int toIndex);   // Move an order in the list
        void executeOrders();                         // Execute all valid orders
        void printOrders() const;                     // Print the list of orders

    private:
        std::vector<std::unique_ptr<Order>> orders;  // Vector of smart pointers to orders
    };

}

#endif // ORDERS_H

