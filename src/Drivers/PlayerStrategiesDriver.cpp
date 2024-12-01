#include "../GameEngine/GameEngine.h"
#include <iostream>

void testPlayerStrategies() {
    //TODO
    GameEngine* ge = new GameEngine();
    ge->readInputFromFile("../commands.txt");
    std::cout << "Player Strategies Driver Running..." << std::endl;
    if(ge->startupPhase()){
        ge->mainGameLoop();
    }
    //delete ge;
    std::cout<<"\nTesting success\n\n" << std::endl;
}
