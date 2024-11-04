//
// Created by samue on 2024-09-12.
//

#include "../Orders/Orders.h"

void testOrderExecution() {
    // Create players
    Player player1("Player 1");
    Player player2("Player 2");

    // Create territories
    Territory territory1{};
    territory1.modifyArmies(10);
    Territory territory2{};  // Belongs to Player 1
    Territory enemyTerritory{};  // Belongs to Player 2

    // Display initial territory ownership and unit setup
    std::cout << "\nInitial Setup:\n";
    std::cout << "Territory 1 (Player 1): " << territory1.getArmies() << " units.\n";
    std::cout << "Territory 2 (Player 1): " << territory2.getArmies() << " units.\n";
    std::cout << "Enemy Territory (Player 2): " << enemyTerritory.getArmies() << " units.\n\n";

    // Example 1: DeployOrder - Player 1 deploys 5 units to Territory 1
    std::cout << "\nTesting DeployOrder:" << std::endl;
    DeployOrder deployOrder;
    deployOrder.execute(&player1, nullptr, nullptr, &territory1, 5);

    // Example 2: AdvanceOrder - Player 1 advances 3 units from Territory 1 to Territory 2 (same player)
    std::cout << "\nTesting AdvanceOrder (Same Player):" << std::endl;
    AdvanceOrder advanceOrder;
    advanceOrder.execute(&player1, nullptr, &territory1, &territory2, 3);

    // Example 3: AdvanceOrder - Player 1 attacks Enemy Territory with 4 units
    std::cout << "\nTesting AdvanceOrder (Attack Enemy):" << std::endl;
    advanceOrder.execute(&player1, &player2, &territory1, &enemyTerritory, 4);

    // Example 4: BombOrder - Player 1 bombs the Enemy Territory
    std::cout << "\nTesting BombOrder:" << std::endl;
    BombOrder bombOrder;
    bombOrder.execute(&player1, &player2, nullptr, &enemyTerritory, 0);

    // Example 5: AirliftOrder - Player 1 airlifts 2 units from Territory 2 to Territory 1
    std::cout << "\nTesting AirliftOrder:" << std::endl;
    AirliftOrder airliftOrder;
    airliftOrder.execute(&player1, nullptr, &territory2, &territory1, 2);

    // Example 6: BlockadeOrder - Player 1 blockades Territory 1
    std::cout << "\nTesting BlockadeOrder:" << std::endl;
    BlockadeOrder blockadeOrder;
    blockadeOrder.execute(&player1, nullptr, nullptr, &territory1, 0);

    // Example 7: NegotiateOrder - Player 1 negotiates with Player 2 (no attacks between them)
    std::cout << "\nTesting NegotiateOrder:" << std::endl;
    NegotiateOrder negotiateOrder;
    negotiateOrder.execute(&player1, &player2, nullptr, nullptr, 0);

    // Display final territory ownership and unit setup
    std::cout << "\nFinal Setup:\n";
    std::cout << "Territory 1 (Player 1): " << territory1.getArmies() << " units.\n";
    std::cout << "Territory 2 (Player 1): " << territory2.getArmies() << " units.\n";
    std::cout << "Enemy Territory (Player 2): " << enemyTerritory.getArmies() << " units.\n\n";
}
