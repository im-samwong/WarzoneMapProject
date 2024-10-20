#include <iostream>

void testOrdersLists();
void testCards();
void testLoadMaps();
void testPlayers();
void testGameStates();

int main() {
    testOrdersLists();
    std::cout << std::endl;

    testCards();
    std::cout << std::endl;

    testPlayers();
    std::cout << std::endl;

    testLoadMaps();

    testGameStates();
    std::cout << std::endl;
    return 0;
}
