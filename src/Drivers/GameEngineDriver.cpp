// Created by Daniel on 2024-09-29.

#include "../GameEngine/GameEngine.h"
#include <iostream>

namespace GameEngine {
    void testGameStates() {
        cout << "Testing GameEngine.GameStates" << endl;

        vector<TransitionCommand> startStateTransitionCommands = { TransitionCommand::LOAD_MAP };
        shared_ptr<StartState> startState = make_shared<StartState>(
            GameStateTypes::STARTUP,
            GameStates::START,
            startStateTransitionCommands
        );

        vector<TransitionCommand> winTransitionCommands = { TransitionCommand::PLAY_AGAIN, TransitionCommand::END };
        shared_ptr<WinState> winState = make_shared<WinState>(
            GameStateTypes::PLAY,
            GameStates::WIN,
            winTransitionCommands
        );

        vector<TransitionCommand> executeOrdersTransitionCommands = { TransitionCommand::EXECUTE_ORDER, TransitionCommand::END_EXEC_ORDER, TransitionCommand::WIN_GAME };
        shared_ptr<ExecuteOrdersState> executeOrdersState = make_shared<ExecuteOrdersState>(
            GameStateTypes::PLAY,
            GameStates::EXECUTE_ORDERS,
            executeOrdersTransitionCommands
        );

        vector<TransitionCommand> issueOrdersTransitionCommands = { TransitionCommand::ISSUE_ORDER, TransitionCommand::END_ISSUE_ORDERS };
        shared_ptr<IssueOrdersState> issueOrdersState = make_shared<IssueOrdersState>(
            GameStateTypes::PLAY,
            GameStates::ISSUE_ORDERS,
            issueOrdersTransitionCommands
        );

        vector<TransitionCommand> assignReinforcementTransitionCommands = { TransitionCommand::ISSUE_ORDER };
        shared_ptr<AssignReinforcementState> assignReinforcementState = make_shared<AssignReinforcementState>(
            GameStateTypes::PLAY,
            GameStates::ASSIGN_REINFORCEMENT,
            assignReinforcementTransitionCommands
        );

        vector<TransitionCommand> playersAddedTransitionCommands = { TransitionCommand::ADD_PLAYER, TransitionCommand::ASSIGN_COUNTRIES };
        shared_ptr<PlayersAddedState> playersAddedState = make_shared<PlayersAddedState>(
            GameStateTypes::STARTUP,
            GameStates::PLAYERS_ADDED,
            playersAddedTransitionCommands
        );

        vector<TransitionCommand> mapValidatedTransitionCommands = { TransitionCommand::ADD_PLAYER };
        shared_ptr<MapValidatedState> mapValidatedState = make_shared<MapValidatedState>(
            GameStateTypes::STARTUP,
            GameStates::MAP_VALIDATED,
            mapValidatedTransitionCommands
        );

        vector<TransitionCommand> mapLoadedTransitionCommands = { TransitionCommand::LOAD_MAP, TransitionCommand::VALIDATE_MAP };
        shared_ptr<MapLoadedState> mapLoadedState = make_shared<MapLoadedState>(
            GameStateTypes::STARTUP,
            GameStates::MAP_LOADED,
            mapLoadedTransitionCommands
        );

        // Set next states for each state
        startState->setNextStates(make_shared<vector<shared_ptr<GameState>>>(vector<shared_ptr<GameState>>{ mapLoadedState }));
        mapLoadedState->setNextStates(make_shared<vector<shared_ptr<GameState>>>(vector<shared_ptr<GameState>>{ mapValidatedState }));
        mapValidatedState->setNextStates(make_shared<vector<shared_ptr<GameState>>>(vector<shared_ptr<GameState>>{ playersAddedState }));
        playersAddedState->setNextStates(make_shared<vector<shared_ptr<GameState>>>(vector<shared_ptr<GameState>>{ assignReinforcementState }));
        assignReinforcementState->setNextStates(make_shared<vector<shared_ptr<GameState>>>(vector<shared_ptr<GameState>>{ issueOrdersState }));
        issueOrdersState->setNextStates(make_shared<vector<shared_ptr<GameState>>>(vector<shared_ptr<GameState>>{ executeOrdersState }));
        executeOrdersState->setNextStates(make_shared<vector<shared_ptr<GameState>>>(vector<shared_ptr<GameState>>{ assignReinforcementState, winState }));
        winState->setNextStates(make_shared<vector<shared_ptr<GameState>>>(vector<shared_ptr<GameState>>{ startState }));

        // Create game engine with current game state set to StartState
        shared_ptr<bool> gameOver = make_shared<bool>(false);
        shared_ptr<GameState> currentGameState = startState;
        GameEngine gameEngine(currentGameState, gameOver);

        gameEngine.testGameStates();
    }
}