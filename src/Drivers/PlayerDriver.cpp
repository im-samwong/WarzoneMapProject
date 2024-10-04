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
        // Creating player objects
        Player p1("Caitlyn");

        // Printing player info
        std::cout << p1;

        //Territory t1("Territory1");
        //Territory t2("Territory2");

        // Using Player methods (toDefend & toAttack)
        std::vector<Territory*> toDefendP1 = p1.toDefend();
        std::cout << "Player 1 has to defend the following territories: \n" << endl;
        for (const auto& territory : toDefendP1) {
            std::cout << territory->getName() << std::endl;
        }

        std::vector<Territory*> toAttackP1 = p1.toAttack();
        std::cout << "Player 1 has to attack the following territories: \n" << endl;
        for (const auto& territory : toAttackP1) {
            std::cout << territory->getName() << std::endl;
        }

        // Issuing orders
        p1.issueOrder("deploy");
        p1.issueOrder("advance");

        // Printing orders
        std::cout << "Player 1 Orders:\n";
        p1.printOrders();

        // Printing hand of cards
        p1.printHand();
    }
}
