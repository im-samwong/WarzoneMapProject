//
// Created by danielm on 10/20/24.
//

#include "../GameEngine/GameEngine.h"
#include <iostream>

/** void testGameStates() {
    std::cout << "Testing GameEngine.GameStates" << std::endl;

    std::vector<TransitionCommand> startStateTransitionCommands = {TransitionCommand::LOAD_MAP};
    StartState* startState = new StartState(
        GameStateTypes::STARTUP,
        GameStates::START,
        startStateTransitionCommands);

    std::vector<TransitionCommand> winTransitionCommands = {TransitionCommand::PLAY_AGAIN, TransitionCommand::END};
    WinState* winState = new WinState(
        GameStateTypes::PLAY,
        GameStates::WIN,
        winTransitionCommands);

    std::vector<TransitionCommand> executeOrdersTransitionCommands = {TransitionCommand::EXECUTE_ORDER, TransitionCommand::END_EXEC_ORDER, TransitionCommand::WIN_GAME};
    ExecuteOrdersState* executeOrdersState = new ExecuteOrdersState(
        GameStateTypes::PLAY,
        GameStates::EXECUTE_ORDERS,
        executeOrdersTransitionCommands);

    std::vector<TransitionCommand> issueOrdersTransitionCommands = {TransitionCommand::ISSUE_ORDER, TransitionCommand::END_ISSUE_ORDERS};
    IssueOrdersState* issueOrdersState = new IssueOrdersState(
        GameStateTypes::PLAY,
        GameStates::ISSUE_ORDERS,
        issueOrdersTransitionCommands);

    std::vector<TransitionCommand> assignReinforcementTransitionCommands = {TransitionCommand::ISSUE_ORDER};
    AssignReinforcementState* assignReinforcementState = new AssignReinforcementState(
        GameStateTypes::PLAY,
        GameStates::ASSIGN_REINFORCEMENT,
        assignReinforcementTransitionCommands);

    std::vector<TransitionCommand> playersAddedTransitionCommands = {TransitionCommand::ADD_PLAYER, TransitionCommand::ASSIGN_COUNTRIES};
    PlayersAddedState* playersAddedState = new PlayersAddedState(
        GameStateTypes::STARTUP,
        GameStates::PLAYERS_ADDED,
        playersAddedTransitionCommands);

    std::vector<TransitionCommand> mapValidatedTransitionCommands = {TransitionCommand::ADD_PLAYER};
    MapValidatedState* mapValidatedState = new MapValidatedState(
        GameStateTypes::STARTUP,
        GameStates::MAP_VALIDATED,
        mapValidatedTransitionCommands);

    std::vector<TransitionCommand> mapLoadedTransitionCommands = {TransitionCommand::LOAD_MAP, TransitionCommand::VALIDATE_MAP};
    MapLoadedState* mapLoadedState = new MapLoadedState(
        GameStateTypes::STARTUP,
        GameStates::MAP_LOADED,
        mapLoadedTransitionCommands);

    startState->setNextStates(std::vector<GameState*>{mapLoadedState});
    mapLoadedState->setNextStates(std::vector<GameState*>{mapValidatedState});
    mapValidatedState->setNextStates(std::vector<GameState*>{playersAddedState});
    playersAddedState->setNextStates(std::vector<GameState*>{assignReinforcementState});
    assignReinforcementState->setNextStates(std::vector<GameState*>{issueOrdersState});
    issueOrdersState->setNextStates(std::vector<GameState*>{executeOrdersState});
    executeOrdersState->setNextStates(std::vector<GameState*>{assignReinforcementState, winState});
    winState->setNextStates(std::vector<GameState*>{startState});

    GameEngine* gameEngine = GameEngine::getInstance();
    gameEngine->setGameOverStatus(false);
    gameEngine->setInputtedCommand("N/A");
    gameEngine->setCurrentGameState(startState);

    std::cout << "Game State is currently: " + mapEnumToString(GameStates::START) << std::endl;

    gameEngine->printCurrentStateCommands(gameEngine->getCurrentGameState()->getTransitionCommands(), mapEnumToString(gameEngine->getCurrentGameStateName()));

    std::cout << "\nPlease input one of the previous commands to continue.\n";

    while (!gameEngine->getGameOverStatus()) {
        std::string inputCommand = "INVALID_COMMAND";
        std::cin >> inputCommand;
        std::cout << "You inputted " + inputCommand + "\n";
        TransitionCommand transitionCommand = mapStringToTransitionCommand(inputCommand);
        if (gameEngine->transitionToNextState(transitionCommand)) {
            if (mapEnumToString(gameEngine->getCurrentGameStateName()) == "WIN" && transitionCommand == TransitionCommand::END) {
                std::cout << "The game state is currently: WIN and END command invoked.\n\nTerminating Program";
                gameEngine->setGameOverStatus(true);
            } else {
                gameEngine->printCurrentStateCommands(gameEngine->getCurrentGameState()->getTransitionCommands(), mapEnumToString(gameEngine->getCurrentGameStateName()));
            }
        }
    }

    exit(EXIT_SUCCESS);
}
**/
void testStartupPhase() {
    GameEngine* ge = new GameEngine();
    //Uncomment below line to get input from user.
   ge->readInputFromFile("../commands.txt");

    if(ge->startupPhase()){
        std::cout<<"Testing success";
    }

    delete ge;
}

void testMainGameLoop() {
    GameEngine* ge = new GameEngine();
    ge->readInputFromFile("../commands.txt");
    if(ge->startupPhase()){
        ge->mainGameLoop();
    }
    delete ge;
}