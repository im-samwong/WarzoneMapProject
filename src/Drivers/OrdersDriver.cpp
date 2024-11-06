//
// Created by samue on 2024-09-12.
//

#include "../Orders/Orders.h"
#include "../GameEngine/GameEngine.h"

// Free function to test order execution
void testOrderExecution() {
    // Create players
    Player player1("Player 1");
    Player player2("Player 2");

    // Create territories and assign owners
    Territory territory1("Territory 1", 0, 0, nullptr);
    Territory territory2("Territory 2", 1, 0, nullptr);
    Territory enemyTerritory1("Enemy Territory 1", 2, 0, nullptr);
    Territory enemyTerritory2("Enemy Territory 2", 3, 0, nullptr);


    // Set initial ownership and armies
    territory1.setOwner(&player1);
    territory2.setOwner(&player1);
    enemyTerritory1.setOwner(&player2);
    enemyTerritory2.setOwner(&player2);

    territory1.modifyArmies(10);
    territory2.modifyArmies(5);
    enemyTerritory1.modifyArmies(5);
    enemyTerritory2.modifyArmies(5);

    // Set territory neighbours
    territory1.addNeighbor(&territory2);
    territory2.addNeighbor(&territory1);
    territory2.addNeighbor(&enemyTerritory1);
    territory2.addNeighbor(&enemyTerritory2);
    enemyTerritory1.addNeighbor(&territory2);
    enemyTerritory2.addNeighbor(&territory2);

    // Add to Player 1 reinforcement pool
    player1.changeReinforcements(5);

    // Display initial setup
    std::cout << "\nInitial Setup:\n";
    std::cout << "Territory 1 (Player 1): " << territory1.getArmies() << " units.\n";
    std::cout << "Territory 2 (Player 1): " << territory2.getArmies() << " units.\n";
    std::cout << "Enemy Territory (Player 2): " << enemyTerritory1.getArmies() << " units.\n\n";

    // Testing each order type

    // 1. DeployOrder
    std::cout << "\nTesting DeployOrder:\n";
    DeployOrder* deployOrder = new DeployOrder(&player1, &territory1, new int(5));
    deployOrder->execute();
    std::cout << "Territory 1 (Player 1): " << territory1.getArmies() << " units after deploy.\n";

    // 2. AdvanceOrder (Move units within the same player's territories)
    std::cout << "\nTesting AdvanceOrder (Same Player):\n";
    AdvanceOrder* advanceOrder = new AdvanceOrder(&player1, &territory1, &territory2, new int(3));
    advanceOrder->execute();
    std::cout << "Territory 1 (Player 1): " << territory1.getArmies() << " units after move.\n";
    std::cout << "Territory 2 (Player 1): " << territory2.getArmies() << " units after move.\n";

    // 3. BombOrder (Bombing enemy territory)
    std::cout << "\nTesting BombOrder:\n";
    BombOrder* bombOrder = new BombOrder(&player1, &enemyTerritory1);
    bombOrder->execute();  // Source is set to Player 1's territory for context
    std::cout << "Enemy Territory 1 (Player 2): " << enemyTerritory1.getArmies() << " units after bombing.\n";

    // 4. AdvanceOrder (Attack enemy territory)
    std::cout << "\nTesting AdvanceOrder (Attack Enemy):\n";
    AdvanceOrder* advanceOrder2 = new AdvanceOrder(&player1, &territory2, &enemyTerritory1, new int(5));
    advanceOrder2->execute();
    std::cout << "Territory 2 (Player 1): " << territory2.getArmies() << " units after attack.\n";
    std::cout << "Enemy Territory 1 (Player 2): " << enemyTerritory1.getArmies() << " units after attack.\n";

    // 5. AirliftOrder (Move units between non-adjacent territories)
    std::cout << "\nTesting AirliftOrder:\n";
    AirliftOrder* airliftOrder = new AirliftOrder(&player1, &territory1, &enemyTerritory1, new int(2));
    airliftOrder->execute();
    std::cout << "Territory 1 (Player 1): " << territory1.getArmies() << " units after airlift.\n";
    std::cout << "Enemy Territory 1 (Conquored by Player 1): " << enemyTerritory1.getArmies() << " units after airlift.\n";

    // 6. BlockadeOrder (Transfer ownership to Neutral - nullptr)
    std::cout << "\nTesting BlockadeOrder:\n";
    BlockadeOrder* blockadeOrder = new BlockadeOrder(&player1, &territory1);
    blockadeOrder->execute(); // Source and target both set to territory1
    std::cout << "Territory 1 ownership transferred to Neutral (nullptr) with " << territory1.getArmies() << " units after blockade.\n";

    // 7. NegotiateOrder (Player 1 and Player 2 cannot attack each other)
    std::cout << "\nTesting NegotiateOrder:\n";
    NegotiateOrder* negotiateOrder = new NegotiateOrder(&player1, &player2);
    negotiateOrder->execute();

    // Attempt another attack between Player 1 and Player 2 to test negotiation
    std::cout << "\nTesting AdvanceOrder after NegotiateOrder (should be prevented):\n";
    AdvanceOrder* advanceOrder3 = new AdvanceOrder(&player1, &territory2, &enemyTerritory2, new int(3));
    advanceOrder3->execute();

    // Check for card reward
    Deck* deck = new Deck();
    if (GameState::hasConqueredTerritory(&player1)) {
        player1.getHand().addCard(deck->draw());
        std::cout << player1.getName() << " receives a card for conquering a territory this turn.\n";
    }

    GameState::resetConqueredTerritories();  // Reset the conquest status for the next turn
    GameState::resetNegotiations();

    // Display final setup
    std::cout << "\nFinal Setup:\n";
    std::cout << "Territory 1: " << territory1.getArmies() << " units, owned by " << (territory1.getOwner() ? territory1.getOwner()->getName() : "Neutral (nullptr)") << "\n";
    std::cout << "Territory 2: " << territory2.getArmies() << " units, owned by " << (territory2.getOwner() ? territory2.getOwner()->getName() : "Neutral (nullptr)") << "\n";
    std::cout << "Enemy Territory 1 (Conquored by Player 1): " << enemyTerritory1.getArmies() << " units, owned by " << (enemyTerritory1.getOwner() ? enemyTerritory1.getOwner()->getName() : "Neutral (nullptr)") << "\n";
    std::cout << "Enemy Territory 1 (Player 2): " << enemyTerritory2.getArmies() << " units, owned by " << (enemyTerritory2.getOwner() ? enemyTerritory2.getOwner()->getName() : "Neutral (nullptr)") << "\n";
}
