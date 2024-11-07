#include "../CommandProcessing/CommandProcessing.h"
#include "../Orders/Orders.h"
#include "../GameEngine/GameEngine.h"
#include "../LoggingObserver/LoggingObserver.h"

void testLoggingObserver() {
    Command* cmd = new Command("test");
    CommandProcessor* proc = new CommandProcessor();
    DeployOrder* order = new DeployOrder();
    OrderList* olist = new OrderList();
    GameEngine* engine = new GameEngine();

    std::cout << "Demonstrating Base classes:" << std::endl;
    std::cout << "Calling name() in Subject and ILoggable through Command: " << cmd->nameS() << ", " << cmd->nameI() << std::endl;
    std::cout << "Calling name() in Subject and ILoggable through CommandProcessor: " << proc->nameS() << ", " << proc->nameI() << std::endl;
    std::cout << "Calling name() in Subject and ILoggable through Order: " << order->nameS() << ", " << order->nameI() << std::endl;
    std::cout << "Calling name() in Subject and ILoggable through OrderList: " << olist->nameS() << ", " << olist->nameI() << std::endl;
    std::cout << "Calling name() in Subject and ILoggable through GameEngine: " << engine->nameS() << ", " << engine->nameI() << std::endl;
    std::cout << std::endl;

    // Create a LogObserver
    LogObserver* logObserver = new LogObserver("../gamelog.txt");

    // Attach observer
    cmd->attach(logObserver);
    proc->attach(logObserver);
    order->attach(logObserver);
    olist->attach(logObserver);
    engine->attach(logObserver);

    engine->readInputFromFile("../commands.txt");
    if(engine->startupPhase()){
        engine->mainGameLoop();
    }

    logObserver->closeFile();

    // Detach observer
    cmd->detach(logObserver);
    proc->detach(logObserver);
    order->detach(logObserver);
    olist->detach(logObserver);
    engine->detach(logObserver);
    
    // Clean up
    delete logObserver;
    delete cmd;
    delete proc;
    delete order;
    delete engine;

}
