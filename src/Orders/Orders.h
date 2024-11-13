#ifndef ORDERS_H
#define ORDERS_H

#include "../Map/Map.h"
#include "../Player/Player.h"
#include "../LoggingObserver/LoggingObserver.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Abstract Order class
class Order: public Subject, public ILoggable {
public:
    // Constructors
    Order();
    Order(Player* sourcePlayer, Player* targetPlayer, Territory* source, Territory* target, int* numUnits);
    Order(const Order& other);

    // Assignment operator
    Order& operator=(const Order& other);

    // Destructor
    virtual ~Order();

    // Pure virtual methods to be implemented by subclasses
    virtual bool validate() = 0;
    virtual void execute() = 0;
    std::string stringToLog() const override = 0;

    // Virtual clone method for deep copying
    virtual std::unique_ptr<Order> clone() const = 0;
    virtual std::string description() const = 0;  // For printing the order description

    // Stream insertion operator to describe the order
    friend std::ostream& operator<<(std::ostream& os, const Order& order);

protected:
    Player* sourcePlayer;
    Player* targetPlayer;
    Territory* source;
    Territory* target;
    int* numUnits;
    std::string* effects = nullptr;
};

// Subclass: DeployOrder
class DeployOrder : public Order {
public:
    // Constructors
    DeployOrder();
    DeployOrder(Player* sourcePlayer, Territory* source, int* numUnits);
    DeployOrder(const DeployOrder& other);

    // Assignment operator
    DeployOrder& operator=(const DeployOrder& other);

    // Override methods
    bool validate()  override;
    void execute() override;
    std::unique_ptr<Order> clone() const override;
    std::string stringToLog() const override;

    std::string description() const override;
};

// Subclass: AdvanceOrder
class AdvanceOrder : public Order {
public:
    // Constructors
    AdvanceOrder();
    AdvanceOrder(Player* sourcePlayer, Territory* source, Territory* target, int* numUnits);
    AdvanceOrder(const AdvanceOrder& other);

    // Assignment operator
    AdvanceOrder& operator=(const AdvanceOrder& other);

    // Override methods
    bool validate()  override;
    void execute() override;
    std::unique_ptr<Order> clone() const override;
    std::string stringToLog() const override;

    std::string description() const override;

};

// Subclass: BombOrder
class BombOrder : public Order {
public:
    // Constructors
    BombOrder();
    BombOrder(Player* sourcePlayer, Territory* target);
    BombOrder(const BombOrder& other);

    // Assignment operator
    BombOrder& operator=(const BombOrder& other);

    // Override methods
    bool validate() override;
    void execute() override;
    std::unique_ptr<Order> clone() const override;
    std::string stringToLog() const override;

    std::string description() const override;
};

// Subclass: BlockadeOrder
class BlockadeOrder : public Order {
public:
    // Constructors
    BlockadeOrder();
    BlockadeOrder(Player* sourcePlayer, Territory* target);
    BlockadeOrder(const BlockadeOrder& other);

    // Assignment operator
    BlockadeOrder& operator=(const BlockadeOrder& other);

    // Override methods
    bool validate() override;
    void execute() override;
    std::unique_ptr<Order> clone() const override;
    std::string stringToLog() const override;

    std::string description() const override;
};

// Subclass: AirliftOrder
class AirliftOrder : public Order {
public:
    // Constructors
    AirliftOrder();
    AirliftOrder(Player* sourcePlayer, Territory* source, Territory* target, int* numUnits);
    AirliftOrder(const AirliftOrder& other);

    // Assignment operator
    AirliftOrder& operator=(const AirliftOrder& other);

    // Override methods
    bool validate() override;
    void execute() override;
    std::unique_ptr<Order> clone() const override;
    std::string stringToLog() const override;

    std::string description() const override;
};

// Subclass: NegotiateOrder
class NegotiateOrder : public Order {
public:
    // Constructors
    NegotiateOrder();
    NegotiateOrder(Player* sourcePlayer, Player* targetPlayer);
    NegotiateOrder(const NegotiateOrder& other);

    // Assignment operator
    NegotiateOrder& operator=(const NegotiateOrder& other);

    // Override methods
    bool validate() override;
    void execute() override;
    std::unique_ptr<Order> clone() const override;
    std::string stringToLog() const override;

    std::string description() const override;
};

// OrderList class to manage a list of orders
class OrderList: public Subject, public ILoggable{
public:
    // Constructors
    OrderList();
    OrderList(const OrderList& other);

    // Assignment operator
    OrderList& operator=(const OrderList& other);

    // Destructor
    ~OrderList();

    // Methods to manage orders
    void addOrder(std::unique_ptr<Order> order);
    void removeOrder(int index);
    void moveOrder(int fromIndex, int toIndex);
    void printOrders() const;
    std::string stringToLog() const override;

    // Getters
    std::size_t getSize() const;
    const std::vector<std::unique_ptr<Order>>& getOrders() const;

private:
    std::vector<std::unique_ptr<Order>> orders;  // Vector of smart pointers to orders
};

#endif // ORDERS_H
