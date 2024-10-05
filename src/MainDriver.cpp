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

namespace Maps {
    void testLoadMaps();
}
int main() {
    Orders::testOrdersLists();
    std::cout << std::endl;

    Cards::testCards();
    std::cout << std::endl;

    Player::testPlayers();
    std::cout << std::endl;

    Maps::testLoadMaps();

    GameEngine::testGameStates();
    std::cout << std::endl;
    return 0;
}
