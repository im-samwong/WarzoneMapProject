//
// Created by samue on 2024-09-12.
//

#include "../Orders/Orders.h"

namespace Orders {
    void testOrdersLists() {
        // Create an OrderList object
        OrderList orderList;

        // Add all types of orders to the list
        orderList.addOrder(std::make_unique<DeployOrder>());
        orderList.addOrder(std::make_unique<AdvanceOrder>());
        orderList.addOrder(std::make_unique<BombOrder>());
        orderList.addOrder(std::make_unique<BlockadeOrder>());
        orderList.addOrder(std::make_unique<AirliftOrder>());
        orderList.addOrder(std::make_unique<NegotiateOrder>());

        // Print the orders in the list
        std::cout << "Orders in list:" << std::endl;
        orderList.printOrders();

        // Move the second order to the first position
        orderList.moveOrder(1, 0);
        std::cout << "Moving orders from index 1 to index 0" << std::endl;
        std::cout << "Orders after moving:" << std::endl;
        orderList.printOrders();

        // Remove the first order
        orderList.removeOrder(0);
        std::cout << "Removing order at index 0" << std::endl;
        std::cout << "Orders after removal:" << std::endl;
        orderList.printOrders();

        // Execute remaining orders
        orderList.executeOrders();
    }
}
