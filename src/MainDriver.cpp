#include <iostream>

namespace Orders {
    void testOrdersLists();
}
namespace Cards {
    void testCards();
}

namespace Player {
    void testPlayers();
}

namespace GameEngine {
    void testGameStates();
}

int main() {
    Orders::testOrdersLists();
    std::cout << std::endl;

    Cards::testCards();
    std::cout << std::endl;

    Player::testPlayers();
    std::cout << std::endl;

    GameEngine::testGameStates();
    std::cout << std::endl;
    return 0;
}
