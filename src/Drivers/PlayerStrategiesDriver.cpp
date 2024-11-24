#include "../GameEngine/GameEngine.h"

void testPlayerStrategies() {
    GameEngine* ge = new GameEngine();
    ge->readInputFromFile("../commands.txt");
    if(ge->startupPhase()){
        ge->mainGameLoop();
    }
    //delete ge;
    std::cout<<"\nTesting success\n\n" << std::endl;
}
