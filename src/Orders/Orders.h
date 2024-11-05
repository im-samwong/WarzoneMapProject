#ifndef ORDERS_H
#define ORDERS_H

#include "../Map/Map.h"
#include "../Player/Player.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Abstract Order class
class Order {
public:
    Order() = default;
    Order(const Order& other) = default;
    Order& operator=(const Order& other) = default;
    virtual ~Order() = default;  // Virtual destructor for polymorphic deletion

    // Pure virtual methods to be implemented by subclasses
    virtual bool validate(Player* sourcePlayer, Player* targetPlayer, Territory* source, Territory* target, int numUnits) const = 0;
    virtual void execute(Player* sourcePlayer, Player* targetPlayer, Territory* source, Territory* target, int numUnits) = 0;

    // Virtual clone method for deep copying
    virtual std::unique_ptr<Order> clone() const = 0;

    // Stream insertion operator to describe the order
    friend std::ostream& operator<<(std::ostream& os, const Order& order) {
        return os << "Order: " << order.description();
    }

protected:
    virtual std::string description() const = 0;  // For printing the order description
};

// Subclass: DeployOrder
class DeployOrder : public Order {
public:
    DeployOrder() = default;
    DeployOrder(const DeployOrder& other) = default;
    DeployOrder& operator=(const DeployOrder& other) = default;

    bool validate(Player* sourcePlayer, Player* targetPlayer, Territory* source, Territory* target, int numUnits) const override;
    void execute(Player* sourcePlayer, Player* targetPlayer, Territory* source, Territory* target, int numUnits) override;

    // Override clone to create a deep copy of DeployOrder
    std::unique_ptr<Order> clone() const override {
        return std::make_unique<DeployOrder>(*this);
    }

protected:
    std::string description() const override;
};

// Subclass: AdvanceOrder
class AdvanceOrder : public Order {
public:
    AdvanceOrder() = default;
    AdvanceOrder(const AdvanceOrder& other) = default;
    AdvanceOrder& operator=(const AdvanceOrder& other) = default;

    bool validate(Player* sourcePlayer, Player* targetPlayer, Territory* source, Territory* target, int numUnits) const override;
    void execute(Player* sourcePlayer, Player* targetPlayer, Territory* source, Territory* target, int numUnits) override;

    std::unique_ptr<Order> clone() const override {
        return std::make_unique<AdvanceOrder>(*this);
    }

protected:
    std::string description() const override;
};

// Subclass: BombOrder
class BombOrder : public Order {
public:
    BombOrder() = default;
    BombOrder(const BombOrder& other) = default;
    BombOrder& operator=(const BombOrder& other) = default;

    bool validate(Player* sourcePlayer, Player* targetPlayer, Territory* source, Territory* target, int numUnits) const override;
    void execute(Player* sourcePlayer, Player* targetPlayer, Territory* source, Territory* target, int numUnits) override;

    std::unique_ptr<Order> clone() const override {
        return std::make_unique<BombOrder>(*this);
    }

protected:
    std::string description() const override;
};

// Subclass: BlockadeOrder
class BlockadeOrder : public Order {
public:
    BlockadeOrder() = default;
    BlockadeOrder(const BlockadeOrder& other) = default;
    BlockadeOrder& operator=(const BlockadeOrder& other) = default;

    bool validate(Player* sourcePlayer, Player* targetPlayer, Territory* source, Territory* target, int numUnits) const override;
    void execute(Player* sourcePlayer, Player* targetPlayer, Territory* source, Territory* target, int numUnits) override;

    std::unique_ptr<Order> clone() const override {
        return std::make_unique<BlockadeOrder>(*this);
    }

protected:
    std::string description() const override;
};

// Subclass: AirliftOrder
class AirliftOrder : public Order {
public:
    AirliftOrder() = default;
    AirliftOrder(const AirliftOrder& other) = default;
    AirliftOrder& operator=(const AirliftOrder& other) = default;

    bool validate(Player* sourcePlayer, Player* targetPlayer, Territory* source, Territory* target, int numUnits) const override;
    void execute(Player* sourcePlayer, Player* targetPlayer, Territory* source, Territory* target, int numUnits) override;

    std::unique_ptr<Order> clone() const override {
        return std::make_unique<AirliftOrder>(*this);
    }

protected:
    std::string description() const override;
};

// Subclass: NegotiateOrder
class NegotiateOrder : public Order {
public:
    NegotiateOrder() = default;
    NegotiateOrder(const NegotiateOrder& other) = default;
    NegotiateOrder& operator=(const NegotiateOrder& other) = default;

    bool validate(Player* sourcePlayer, Player* targetPlayer, Territory* source, Territory* target, int numUnits) const override;
    void execute(Player* sourcePlayer, Player* targetPlayer, Territory* source, Territory* target, int numUnits) override;

    std::unique_ptr<Order> clone() const override {
        return std::make_unique<NegotiateOrder>(*this);
    }

protected:
    std::string description() const override;
};

// OrderList class to manage a list of orders
class OrderList {
public:
    void addOrder(std::unique_ptr<Order> order);  // Add an order
    void removeOrder(int index);                  // Remove an order by index
    void moveOrder(int fromIndex, int toIndex);   // Move an order in the list
    void printOrders() const;                     // Print the list of orders

    OrderList() = default;  // Default constructor

    // Copy constructor and assignment operator to ensure deep copy of OrderList
    OrderList(const OrderList& other);

    OrderList& operator=(const OrderList& other);

    // Getters
    std::size_t getSize() const;
    const std::vector<std::unique_ptr<Order>>& getOrders() const;

private:
    std::vector<std::unique_ptr<Order>> orders;  // Vector of smart pointers to orders
};

#endif // ORDERS_H
