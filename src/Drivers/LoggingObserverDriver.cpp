//
// Created by USER on 2024-11-08.
//

#include "../CommandProcessing/CommandProcessing.h"
#include "../Orders/Orders.h"
#include "../GameEngine/GameEngine.h"
#include "../LoggingObserver/LoggingObserver.h"

void testLoggingObserver() {

    logObserver = new LogObserver("../gamelog.txt");

    std::cout << "Testing main game loop:" << std::endl;
    GameEngine* ge = new GameEngine();
    ge->readInputFromFile("../commands.txt");
    if(ge->startupPhase()){
        ge->mainGameLoop();
    }

    logObserver->closeFile();

    delete logObserver;
    logObserver = nullptr;
}