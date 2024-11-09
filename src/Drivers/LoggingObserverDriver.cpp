//
// Created by USER on 2024-11-08.
//

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

    // Clean up
    delete cmd;
    delete proc;
    delete order;
    delete olist;
    delete engine;

    // Create a LogObserver
    logObserver = new LogObserver("../gamelog.txt");

    std::cout << std::endl;


    // testCommandProcessing
    std::cout << "Testing Command and CommandProcessing: " << std::endl;
    CommandProcessor consoleProcessor;
    consoleProcessor.attach(logObserver);

    std::string currentGameState = "start";

    std::cout << "Welcome to the Warzone!" << std::endl;
    std::cout << "If you would like to play using the command line, type 0. (i.e. 0)" << std::endl;
    std::cout << "If you would like to play using a file of commands, type 1. (i.e. 1)" << std::endl;

    std::string inputType;
    std::string command;
    std::getline(std::cin, inputType);

    if (inputType == "0") { //reading from command line
        std::cout << "INPUT 0 CHOSEN: Reading from command line " << "\n" << std::endl;
        std::cout << "Enter a command (i.e. loadmap fileName.map): " << std::endl;
        while (true) {
            Command* cmd = consoleProcessor.getCommand();
            cmd->attach(logObserver);
            if (cmd->getCommand() == "quit") {
                break;
            }
            std::cout << "Current game state: " << currentGameState << "\n" << std::endl;
            //validating the command
            if (consoleProcessor.validate(*cmd, currentGameState)) {
                cmd->saveEffect("The command '" + cmd->getCommand() + "' is valid!");
                std::cout << "Effect: " << cmd->getEffect() << std::endl;

                //update state based on command
                if (cmd->getCommand() == "loadmap") {
                    currentGameState = "maploaded";
                } else if (cmd->getCommand() == "validatemap") {
                    currentGameState = "mapvalidated";
                } else if (cmd->getCommand() == "addplayer") {
                    currentGameState = "playersadded";
                } else if (cmd->getCommand() == "gamestart") {
                    currentGameState = "assignreinforcement";
                } else if (cmd->getCommand() == "issueorder") {
                    currentGameState = "issueorders";
                } else if (cmd->getCommand() == "issueordersend") {
                    currentGameState = "executeorders";
                } else if (cmd->getCommand() == "execorder") {
                    currentGameState = "executeorders";
                } else if (cmd->getCommand() == "endexecorders") {
                    currentGameState = "assignreinforcement";
                } else if (cmd->getCommand() == "win") {
                    currentGameState = "win";
                } else if (cmd->getCommand() == "replay") {
                    currentGameState = "start";
                }

                std::cout << "Current game state: " << currentGameState << "\n" << std::endl;
            } else {
                std::cout << "The command '"+ cmd->getCommand() +"' is invalid for the game state '" << currentGameState << "'\n" << std::endl;
            }
        }
    } else if (inputType == "1") { //reading from file
        std::cout << "INPUT 1 CHOSEN: Reading from file" << "\n" << std::endl;

            FileCommandProcessorAdapter fileProcessor("commands.txt");
            currentGameState = "start";
            //reading commands from file
            while (true) {
                Command* cmd = fileProcessor.getCommand();
                if (cmd->getCommand() == "") {
                    break;  //reached the end of the file
                }

                //validating commands
                if (fileProcessor.validate(*cmd, currentGameState)) { // issue here
                    cmd->saveEffect("The command '" + cmd->getCommand() + "' is valid!");
                    std::cout << "Effect: " << cmd->getEffect() << std::endl;

                    //update state based on command
                    if (cmd->getCommand() == "loadmap") {
                        currentGameState = "maploaded";
                    } else if (cmd->getCommand() == "validatemap") {
                        currentGameState = "mapvalidated";
                    } else if (cmd->getCommand() == "addplayer") {
                        currentGameState = "playersadded";
                    } else if (cmd->getCommand() == "gamestart") {
                        currentGameState = "assignreinforcement";
                    } else if (cmd->getCommand() == "issueorder") {
                        currentGameState = "issueorders";
                    } else if (cmd->getCommand() == "issueordersend") {
                        currentGameState = "executeorders";
                    } else if (cmd->getCommand() == "execorder") {
                        currentGameState = "executeorders";
                    } else if (cmd->getCommand() == "endexecorders") {
                        currentGameState = "assignreinforcement";
                    } else if (cmd->getCommand() == "win") {
                        currentGameState = "win";
                    } else if (cmd->getCommand() == "replay") {
                        currentGameState = "start";
                    }

                    std::cout << "Current game state: " << currentGameState << "\n" << std::endl;
                } else {
                    std::cout << "Command: " << cmd->getCommand() << std::endl; //debug
                    std::cout << "Invalid command for the game state: '" << currentGameState << "'\n" << std::endl;
                }
            }

    } else {
        std::cout << "Invalid input!" << std::endl;
    }

    // testOrderExecution
    std::cout << "Testing Order and OrderList: " << "\n" << std::endl;
    // Create players
    Player player1("Player 1");
    Player player2("Player 2");

    // Create territories and assign owners
    Territory territory1("Territory 1", 0, 0, nullptr);
    Territory territory2("Territory 2", 1, 0, nullptr);
    Territory enemyTerritory1("Enemy Territory 1", 2, 0, nullptr);
    Territory enemyTerritory2("Enemy Territory 2", 3, 0, nullptr);


    // Set initial ownership and armies
    territory1.setOwner(&player1);
    territory2.setOwner(&player1);
    enemyTerritory1.setOwner(&player2);
    enemyTerritory2.setOwner(&player2);

    territory1.modifyArmies(10);
    territory2.modifyArmies(5);
    enemyTerritory1.modifyArmies(5);
    enemyTerritory2.modifyArmies(5);

    // Set territory neighbours
    territory1.addNeighbor(&territory2);
    territory2.addNeighbor(&territory1);
    territory2.addNeighbor(&enemyTerritory1);
    territory2.addNeighbor(&enemyTerritory2);
    enemyTerritory1.addNeighbor(&territory2);
    enemyTerritory2.addNeighbor(&territory2);

    // Add to Player 1 reinforcement pool
    player1.changeReinforcements(5);

    // Display initial setup
    std::cout << "\nInitial Setup:\n";
    std::cout << "Territory 1 (Player 1): " << territory1.getArmies() << " units.\n";
    std::cout << "Territory 2 (Player 1): " << territory2.getArmies() << " units.\n";
    std::cout << "Enemy Territory (Player 2): " << enemyTerritory1.getArmies() << " units.\n\n";

    // Testing each order type

    // 1. DeployOrder
    std::cout << "\nTesting DeployOrder:\n";
    DeployOrder* deployOrder = new DeployOrder(&player1, &territory1, new int(5));
    deployOrder->attach(logObserver);
    deployOrder->execute();
    std::cout << "Territory 1 (Player 1): " << territory1.getArmies() << " units after deploy.\n";

    // 2. AdvanceOrder (Move units within the same player's territories)
    std::cout << "\nTesting AdvanceOrder (Same Player):\n";
    AdvanceOrder* advanceOrder = new AdvanceOrder(&player1, &territory1, &territory2, new int(3));
    advanceOrder->attach(logObserver);
    advanceOrder->execute();
    std::cout << "Territory 1 (Player 1): " << territory1.getArmies() << " units after move.\n";
    std::cout << "Territory 2 (Player 1): " << territory2.getArmies() << " units after move.\n";

    // 3. BombOrder (Bombing enemy territory)
    std::cout << "\nTesting BombOrder:\n";
    BombOrder* bombOrder = new BombOrder(&player1, &enemyTerritory1);
    bombOrder->attach(logObserver);
    bombOrder->execute();  // Source is set to Player 1's territory for context
    std::cout << "Enemy Territory 1 (Player 2): " << enemyTerritory1.getArmies() << " units after bombing.\n";

    // 4. AdvanceOrder (Attack enemy territory)
    std::cout << "\nTesting AdvanceOrder (Attack Enemy):\n";
    AdvanceOrder* advanceOrder2 = new AdvanceOrder(&player1, &territory2, &enemyTerritory1, new int(5));
    advanceOrder2->attach(logObserver);
    advanceOrder2->execute();
    std::cout << "Territory 2 (Player 1): " << territory2.getArmies() << " units after attack.\n";
    std::cout << "Enemy Territory 1 (Player 2): " << enemyTerritory1.getArmies() << " units after attack.\n";

    // 5. AirliftOrder (Move units between non-adjacent territories)
    std::cout << "\nTesting AirliftOrder:\n";
    AirliftOrder* airliftOrder = new AirliftOrder(&player1, &territory1, &enemyTerritory1, new int(2));
    airliftOrder->attach(logObserver);
    airliftOrder->execute();
    std::cout << "Territory 1 (Player 1): " << territory1.getArmies() << " units after airlift.\n";
    std::cout << "Enemy Territory 1 (Conquored by Player 1): " << enemyTerritory1.getArmies() << " units after airlift.\n";

    // 6. BlockadeOrder (Transfer ownership to Neutral - nullptr)
    std::cout << "\nTesting BlockadeOrder:\n";
    BlockadeOrder* blockadeOrder = new BlockadeOrder(&player1, &territory1);
    blockadeOrder->attach(logObserver);
    blockadeOrder->execute(); // Source and target both set to territory1
    std::cout << "Territory 1 ownership transferred to Neutral (nullptr) with " << territory1.getArmies() << " units after blockade.\n";

    // 7. NegotiateOrder (Player 1 and Player 2 cannot attack each other)
    std::cout << "\nTesting NegotiateOrder:\n";
    NegotiateOrder* negotiateOrder = new NegotiateOrder(&player1, &player2);
    negotiateOrder->attach(logObserver);
    negotiateOrder->execute();

    // Attempt another attack between Player 1 and Player 2 to test negotiation
    std::cout << "\nTesting AdvanceOrder after NegotiateOrder (should be prevented):\n";
    AdvanceOrder* advanceOrder3 = new AdvanceOrder(&player1, &territory2, &enemyTerritory2, new int(3));
    advanceOrder3->attach(logObserver);
    advanceOrder3->execute();

    // Check for card reward
    Deck* deck = new Deck();
    if (GameState::hasConqueredTerritory(&player1)) {
        player1.getHand().addCard(deck->draw());
        std::cout << player1.getName() << " receives a card for conquering a territory this turn.\n";
    }

    GameState::resetConqueredTerritories();  // Reset the conquest status for the next turn
    GameState::resetNegotiations();

    // Display final setup
    std::cout << "\nFinal Setup:\n";
    std::cout << "Territory 1: " << territory1.getArmies() << " units, owned by " << (territory1.getOwner() ? territory1.getOwner()->getName() : "Neutral (nullptr)") << "\n";
    std::cout << "Territory 2: " << territory2.getArmies() << " units, owned by " << (territory2.getOwner() ? territory2.getOwner()->getName() : "Neutral (nullptr)") << "\n";
    std::cout << "Enemy Territory 1 (Conquored by Player 1): " << enemyTerritory1.getArmies() << " units, owned by " << (enemyTerritory1.getOwner() ? enemyTerritory1.getOwner()->getName() : "Neutral (nullptr)") << "\n";
    std::cout << "Enemy Territory 1 (Player 2): " << enemyTerritory2.getArmies() << " units, owned by " << (enemyTerritory2.getOwner() ? enemyTerritory2.getOwner()->getName() : "Neutral (nullptr)") << "\n";

    // testOrderList
    OrderList* orderList = new OrderList();
    orderList->attach(logObserver);
    orderList->addOrder(std::unique_ptr<Order>(bombOrder));


    std::cout << "Testing main game loop:" << std::endl;
    GameEngine* ge = new GameEngine();
    ge->readInputFromFile("../commands.txt");
    if(ge->startupPhase()){
        ge->mainGameLoop();
    }

    logObserver->closeFile();

     // Detach observer
     consoleProcessor.detach(logObserver);
     deployOrder->detach(logObserver);
     advanceOrder->detach(logObserver);
     bombOrder->detach(logObserver);
     advanceOrder2->detach(logObserver);
     airliftOrder->detach(logObserver);
     blockadeOrder->detach(logObserver);
     negotiateOrder->detach(logObserver);
     advanceOrder3->detach(logObserver);
     orderList->detach(logObserver);

    // Clean up
    // delete deployOrder;
    // delete advanceOrder;
    // delete bombOrder;
    // delete advanceOrder2;
    // delete airliftOrder;
    // delete blockadeOrder;
    // delete negotiateOrder;
    // delete advanceOrder3;
     delete deck;
    // delete orderList;

     delete logObserver;
    // delete ge;
}