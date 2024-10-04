#include "../Player/Player.h"
#include "../Map/Map.h"
#include "../Orders/Orders.h"
#include "../Cards/Cards.h"
#include <vector>
#include <string>
#include <iostream>
#include <memory>

namespace Player {
    void testPlayers() {
        // Create a player using the parameterized constructor (name)
        Player* player1 = new Player("Alice");

        // Create some sample territories
        Territory* t1 = new Territory();
        Territory* t2 = new Territory();

        // Create a player using the parameterized constructor (name, territories)
        std::vector<Territory*> territories = {t1, t2};
        Player* player2 = new Player("Bob", territories);

        // Create a dummy order and issue it
        std::unique_ptr<Orders::Order> order = std::make_unique<Orders::DeployOrder>();
        player2->issueOrder(std::move(order));

        // Print out player details
        std::cout << *player1 << std::endl;
        std::cout << *player2 << std::endl;

        // Clean up
        delete player1;
        delete player2;
    }
}
