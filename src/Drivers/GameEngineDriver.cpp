//
// Created by danielm on 10/20/24.
//

#include "../GameEngine/GameEngine.h"
#include <iostream>

void testGameStates() {
    cout << "Testing GameEngine.GameStates" << endl;

    vector<TransitionCommand> startStateTransitionCommands = {TransitionCommand::LOAD_MAP};
    StartState* startState = new StartState(
        GameStateTypes::STARTUP,
        GameStates::START,
        startStateTransitionCommands);

    vector<TransitionCommand> winTransitionCommands = {TransitionCommand::PLAY_AGAIN, TransitionCommand::END};
    WinState* winState = new WinState(
        GameStateTypes::PLAY,
        GameStates::WIN,
        winTransitionCommands);

    vector<TransitionCommand> executeOrdersTransitionCommands = {TransitionCommand::EXECUTE_ORDER, TransitionCommand::END_EXEC_ORDER, TransitionCommand::WIN_GAME};
    ExecuteOrdersState* executeOrdersState = new ExecuteOrdersState(
        GameStateTypes::PLAY,
        GameStates::EXECUTE_ORDERS,
        executeOrdersTransitionCommands);

    vector<TransitionCommand> issueOrdersTransitionCommands = {TransitionCommand::ISSUE_ORDER, TransitionCommand::END_ISSUE_ORDERS};
    IssueOrdersState* issueOrdersState = new IssueOrdersState(
        GameStateTypes::PLAY,
        GameStates::ISSUE_ORDERS,
        issueOrdersTransitionCommands);

    vector<TransitionCommand> assignReinforcementTransitionCommands = {TransitionCommand::ISSUE_ORDER};
    AssignReinforcementState* assignReinforcementState = new AssignReinforcementState(
        GameStateTypes::PLAY,
        GameStates::ASSIGN_REINFORCEMENT,
        assignReinforcementTransitionCommands);

    vector<TransitionCommand> playersAddedTransitionCommands = {TransitionCommand::ADD_PLAYER, TransitionCommand::ASSIGN_COUNTRIES};
    PlayersAddedState* playersAddedState = new PlayersAddedState(
        GameStateTypes::STARTUP,
        GameStates::PLAYERS_ADDED,
        playersAddedTransitionCommands);

    vector<TransitionCommand> mapValidatedTransitionCommands = {TransitionCommand::ADD_PLAYER};
    MapValidatedState* mapValidatedState = new MapValidatedState(
        GameStateTypes::STARTUP,
        GameStates::MAP_VALIDATED,
        mapValidatedTransitionCommands);

    vector<TransitionCommand> mapLoadedTransitionCommands = {TransitionCommand::LOAD_MAP, TransitionCommand::VALIDATE_MAP};
    MapLoadedState* mapLoadedState = new MapLoadedState(
        GameStateTypes::STARTUP,
        GameStates::MAP_LOADED,
        mapLoadedTransitionCommands);

    startState->setNextStates(vector<GameState*>{mapLoadedState});
    mapLoadedState->setNextStates(vector<GameState*>{mapValidatedState});
    mapValidatedState->setNextStates(vector<GameState*>{playersAddedState});
    playersAddedState->setNextStates(vector<GameState*>{assignReinforcementState});
    assignReinforcementState->setNextStates(vector<GameState*>{issueOrdersState});
    issueOrdersState->setNextStates(vector<GameState*>{executeOrdersState});
    executeOrdersState->setNextStates(vector<GameState*>{assignReinforcementState, winState});
    winState->setNextStates(vector<GameState*>{startState});

    GameEngine* gameEngine = GameEngine::getInstance();
    gameEngine->setGameOverStatus(false);
    gameEngine->setInputtedCommand("N/A");
    gameEngine->setCurrentGameState(startState);

    cout << "Game State is currently: " + mapEnumToString(GameStates::START) << endl;

    gameEngine->printCurrentStateCommands(gameEngine->getCurrentGameState()->getTransitionCommands(), mapEnumToString(gameEngine->getCurrentGameStateName()));

    cout << "\nPlease input one of the previous commands to continue.\n";

    while (!gameEngine->getGameOverStatus()) {
        string inputCommand = "INVALID_COMMAND";
        cin >> inputCommand;
        cout << "You inputted " + inputCommand + "\n";
        TransitionCommand transitionCommand = mapStringToTransitionCommand(inputCommand);
        if (gameEngine->transitionToNextState(transitionCommand)) {
            if (mapEnumToString(gameEngine->getCurrentGameStateName()) == "WIN" && transitionCommand == TransitionCommand::END) {
                cout << "The game state is currently: WIN and END command invoked.\n\nTerminating Program";
                gameEngine->setGameOverStatus(true);
            } else {
                gameEngine->printCurrentStateCommands(gameEngine->getCurrentGameState()->getTransitionCommands(), mapEnumToString(gameEngine->getCurrentGameStateName()));
            }
        }
    }

    exit(EXIT_SUCCESS);
}