//
// Created by samue on 2024-09-12.
//

#include "../Player/Player.h"
#include "../Map/Map.h"
#include "../Orders/Orders.h"

namespace Orders {
    void testOrderExecution() {
        // Create players
        Player::Player player1("Player 1");
        Player::Player player2("Player 2");

        // Create territories
        Territory territory1("Territory 1", &player1);
        Territory territory2("Territory 2", &player1);  // Belongs to Player 1
        Territory enemyTerritory("Enemy Territory", &player2);  // Belongs to Player 2

        // Display initial territory ownership and unit setup
        std::cout << "\nInitial Setup:\n";
        std::cout << "Territory 1 (Player 1): " << territory1.getUnits() << " units.\n";
        std::cout << "Territory 2 (Player 1): " << territory2.getUnits() << " units.\n";
        std::cout << "Enemy Territory (Player 2): " << enemyTerritory.getUnits() << " units.\n\n";

        // Example 1: DeployOrder - Player 1 deploys 5 units to Territory 1
        std::cout << "\nTesting DeployOrder:" << std::endl;
        Orders::DeployOrder deployOrder;
        deployOrder.execute(&player1, nullptr, nullptr, &territory1, 5);

        // Example 2: AdvanceOrder - Player 1 advances 3 units from Territory 1 to Territory 2 (same player)
        std::cout << "\nTesting AdvanceOrder (Same Player):" << std::endl;
        Orders::AdvanceOrder advanceOrder;
        advanceOrder.execute(&player1, nullptr, &territory1, &territory2, 3);

        // Example 3: AdvanceOrder - Player 1 attacks Enemy Territory with 4 units
        std::cout << "\nTesting AdvanceOrder (Attack Enemy):" << std::endl;
        advanceOrder.execute(&player1, &player2, &territory1, &enemyTerritory, 4);

        // Example 4: BombOrder - Player 1 bombs the Enemy Territory
        std::cout << "\nTesting BombOrder:" << std::endl;
        Orders::BombOrder bombOrder;
        bombOrder.execute(&player1, &player2, nullptr, &enemyTerritory, 0);

        // Example 5: AirliftOrder - Player 1 airlifts 2 units from Territory 2 to Territory 1
        std::cout << "\nTesting AirliftOrder:" << std::endl;
        Orders::AirliftOrder airliftOrder;
        airliftOrder.execute(&player1, nullptr, &territory2, &territory1, 2);

        // Example 6: BlockadeOrder - Player 1 blockades Territory 1
        std::cout << "\nTesting BlockadeOrder:" << std::endl;
        Orders::BlockadeOrder blockadeOrder;
        blockadeOrder.execute(&player1, nullptr, nullptr, &territory1, 0);

        // Example 7: NegotiateOrder - Player 1 negotiates with Player 2 (no attacks between them)
        std::cout << "\nTesting NegotiateOrder:" << std::endl;
        Orders::NegotiateOrder negotiateOrder;
        negotiateOrder.execute(&player1, &player2, nullptr, nullptr, 0);

        // Display final territory ownership and unit setup
        std::cout << "\nFinal Setup:\n";
        std::cout << "Territory 1 (Player 1): " << territory1.getUnits() << " units.\n";
        std::cout << "Territory 2 (Player 1): " << territory2.getUnits() << " units.\n";
        std::cout << "Enemy Territory (Player 2): " << enemyTerritory.getUnits() << " units.\n\n";
    }

    // void testOrdersLists() {
    //     // Create an OrderList object
    //     OrderList orderList;
    //
    //     // Add all types of orders to the list
    //     orderList.addOrder(std::make_unique<DeployOrder>());
    //     orderList.addOrder(std::make_unique<AdvanceOrder>());
    //     orderList.addOrder(std::make_unique<BombOrder>());
    //     orderList.addOrder(std::make_unique<BlockadeOrder>());
    //     orderList.addOrder(std::make_unique<AirliftOrder>());
    //     orderList.addOrder(std::make_unique<NegotiateOrder>());
    //
    //     // Print the orders in the list
    //     std::cout << "Orders in list:" << std::endl;
    //     orderList.printOrders();
    //
    //     // Move the second order to the first position
    //     orderList.moveOrder(1, 0);
    //     std::cout << "Moving orders from index 1 to index 0" << std::endl;
    //     std::cout << "Orders after moving:" << std::endl;
    //     orderList.printOrders();
    //
    //     // Remove the first order
    //     orderList.removeOrder(0);
    //     std::cout << "Removing order at index 0" << std::endl;
    //     std::cout << "Orders after removal:" << std::endl;
    //     orderList.printOrders();
    //
    // }
}
