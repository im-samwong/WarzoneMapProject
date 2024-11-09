#include <iostream>

void testOrderExecution();
void testCards();
void testLoadMaps();
void testPlayers();
void testGameStates();
void testCommandProcessor();
void testMainGameLoop();
void testStartupPhase();
void testLoggingObserver();

int main() {
    testLoggingObserver();
    testOrderExecution();
    //std::cout << std::endl;
    //
    // testCards();
    // std::cout << std::endl;
    //
    // testPlayers();
    // std::cout << std::endl;
    //
     testCommandProcessor();
    // std::cout << std::endl;
    //
    // testLoadMaps();
    //
    // testGameStates();
    // std::cout << std::endl;
     testStartupPhase();
     testMainGameLoop();

    return 0;
}
