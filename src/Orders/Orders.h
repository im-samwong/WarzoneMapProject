#ifndef ORDERS_H
#define ORDERS_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Abstract Order class
class Order {
public:
    // Default constructor
    Order() = default;

    // Virtual destructor for Order class
    virtual ~Order() = default;

    // Pure virtual methods to be implemented by subclasses
    virtual bool validate() = 0;
    virtual void execute() = 0;

    // Stream insertion operator to describe the order
    friend std::ostream &operator<<(std::ostream &os, const Order &order) {
        return os << "Order: " << order.description();
    }

    // Virtual clone method for deep copying
    virtual std::unique_ptr<Order> clone() const = 0;

protected:
    virtual std::string description() const = 0; // For printing the order
};

// Subclass: DeployOrder
class DeployOrder : public Order {
public:
    DeployOrder() = default;
    DeployOrder(const DeployOrder &other) = default;            // Copy constructor
    DeployOrder &operator=(const DeployOrder &other) = default; // Assignment operator

    bool validate() override;
    void execute() override;

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
    AdvanceOrder(const AdvanceOrder &other) = default;
    AdvanceOrder &operator=(const AdvanceOrder &other) = default;

    bool validate() override;
    void execute() override;

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
    BombOrder(const BombOrder &other) = default;
    BombOrder &operator=(const BombOrder &other) = default;

    bool validate() override;
    void execute() override;

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
    BlockadeOrder(const BlockadeOrder &other) = default;
    BlockadeOrder &operator=(const BlockadeOrder &other) = default;

    bool validate() override;
    void execute() override;

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
    AirliftOrder(const AirliftOrder &other) = default;
    AirliftOrder &operator=(const AirliftOrder &other) = default;

    bool validate() override;
    void execute() override;

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
    NegotiateOrder(const NegotiateOrder &other) = default;
    NegotiateOrder &operator=(const NegotiateOrder &other) = default;

    bool validate() override;
    void execute() override;

    std::unique_ptr<Order> clone() const override {
        return std::make_unique<NegotiateOrder>(*this);
    }

protected:
    std::string description() const override;
};

// OrderList class to manage a list of orders
class OrderList {
public:
    void addOrder(std::unique_ptr<Order> order); // Add an order
    void removeOrder(int index);                 // Remove an order by index
    void moveOrder(int fromIndex, int toIndex);  // Move an order in the list
    void executeOrders();                        // Execute all valid orders
    void printOrders() const;                    // Print the list of orders

    OrderList(); // Default constructor

    // Copy constructor and assignment operator
    OrderList(const OrderList &other);
    OrderList &operator=(const OrderList &other);

    // Getters
    std::size_t getSize() const;
    const std::vector<std::unique_ptr<Order>> &getOrders() const;

private:
    std::vector<std::unique_ptr<Order>> orders; // Vector of smart pointers to orders
};

#endif // ORDERS_H
