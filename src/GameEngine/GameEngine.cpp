//
// Created by Daniel on 2024-09-29.
//

#include "GameEngine.h"
#include <iostream>

namespace GameEngine {
    string mapEnumToString(const TransitionCommand command) {
        string enumString;
        switch(command) {
            case TransitionCommand::LOAD_MAP:
                enumString = "LOAD_MAP";
                break;
            case TransitionCommand::VALIDATE_MAP:
                enumString = "VALIDATE_MAP";
                break;
            case TransitionCommand::ADD_PLAYER:
                enumString = "ADD_PLAYER";
                break;
            case TransitionCommand::ASSIGN_COUNTRIES:
                enumString = "ASSIGN_COUNTRIES";
                break;
            case TransitionCommand::ISSUE_ORDER:
                enumString = "ISSUE_ORDER";
                break;
            case TransitionCommand::END_ISSUE_ORDERS:
                enumString = "END_ISSUE_ORDERS";
                break;
            case TransitionCommand::EXECUTE_ORDER:
                enumString = "EXECUTE_ORDER";
                break;
            case TransitionCommand::WIN_GAME:
                enumString = "WIN_GAME";
                break;
            case TransitionCommand::END_EXEC_ORDER:
                enumString = "END_EXEC_ORDER";
                break;
            case TransitionCommand::PLAY_AGAIN:
                enumString = "PLAY_AGAIN";
                break;
            case TransitionCommand::END:
                enumString = "END";
                break;
            default:
                enumString = "INVALID_COMMAND";
        }
        return enumString;
    }

    TransitionCommand mapStringToTransitionCommand(const string& transitionCommand) {
        TransitionCommand enumValue;
        if (transitionCommand == "LOAD_MAP") {
            enumValue = TransitionCommand::LOAD_MAP;
        } else if (transitionCommand == "VALIDATE_MAP"){
            enumValue = TransitionCommand::VALIDATE_MAP;
        } else if (transitionCommand == "ADD_PLAYER"){
            enumValue = TransitionCommand::ADD_PLAYER;
        } else if (transitionCommand == "ASSIGN_COUNTRIES"){
            enumValue = TransitionCommand::ASSIGN_COUNTRIES;
        } else if (transitionCommand == "ISSUE_ORDER"){
            enumValue = TransitionCommand::ISSUE_ORDER;
        } else if (transitionCommand == "END_ISSUE_ORDERS"){
            enumValue = TransitionCommand::END_ISSUE_ORDERS;
        } else if (transitionCommand == "EXECUTE_ORDER"){
            enumValue = TransitionCommand::EXECUTE_ORDER;
        } else if (transitionCommand == "WIN_GAME"){
            enumValue = TransitionCommand::WIN_GAME;
        } else if (transitionCommand == "END_EXEC_ORDER"){
            enumValue = TransitionCommand::END_EXEC_ORDER;
        } else if (transitionCommand == "PLAY_AGAIN"){
            enumValue = TransitionCommand::PLAY_AGAIN;
        } else if (transitionCommand == "END"){
            enumValue = TransitionCommand::END;
        } else {
            enumValue = TransitionCommand::INVALID;
        }
        return enumValue;
    }


    string mapEnumToString(const GameStates stateName) {
        string enumString;
        switch(stateName) {
            case START:
                enumString = "START";
                break;
            case MAP_LOADED:
                enumString = "MAP_LOADED";
                break;
            case MAP_VALIDATED:
                enumString = "MAP_VALIDATED";
                break;
            case PLAYERS_ADDED:
                enumString = "PLAYERS_ADDED";
                break;
            case ASSIGN_REINFORCEMENT:
                enumString = "ASSIGN_REINFORCEMENT";
                break;
            case ISSUE_ORDERS:
                enumString = "ISSUE_ORDERS";
                break;
            case EXECUTE_ORDERS:
                enumString = "EXECUTE_ORDERS";
                break;
            case WIN:
                enumString = "WIN";
                break;
            default:
                enumString = "INVALID_STATE";
                break;
        }
        return enumString;
    }

    void GameState::setNextStates(const shared_ptr<vector<shared_ptr<GameState>>> &nextStates) {
        this->nextStates = nextStates;
    }

    GameState::GameState(
        const GameStateTypes &stateType,
        const GameStates &stateName,
        const vector<TransitionCommand> &transitionCommands,
        const vector<shared_ptr<GameState>> &nextStates
    ) : stateType(make_shared<GameStateTypes>(stateType)),
        stateName(make_shared<GameStates>(stateName)),
        transitionCommands(make_shared<vector<TransitionCommand>>(transitionCommands)),
        nextStates(make_shared<vector<shared_ptr<GameState>>>(nextStates)) {}

    GameState::GameState(
        const GameStateTypes &stateType,
        const GameStates &stateName,
        const vector<TransitionCommand> &transitionCommands
    ) : stateType(make_shared<GameStateTypes>(stateType)),
        stateName(make_shared<GameStates>(stateName)),
        transitionCommands(make_shared<vector<TransitionCommand>>(transitionCommands)) {}

    GameState::GameState(const GameState &otherGameState) {
        this->nextStates = make_shared<vector<shared_ptr<GameState>>>(*otherGameState.nextStates);
        this->stateName = make_shared<GameStates>(*otherGameState.stateName);
        this->stateType = make_shared<GameStateTypes>(*otherGameState.stateType);
        this->transitionCommands = make_shared<vector<TransitionCommand>>(*otherGameState.transitionCommands);
    }

    GameState& GameState::operator=(const GameState &otherGameState) {
        if (this != &otherGameState) {
            stateType = make_shared<GameStateTypes>(*otherGameState.stateType);
            stateName = make_shared<GameStates>(*otherGameState.stateName);
            transitionCommands = make_shared<vector<TransitionCommand>>(*otherGameState.transitionCommands);
            nextStates = make_shared<vector<shared_ptr<GameState>>>(*otherGameState.nextStates);
        }
        return *this;
    }


    ostream & operator<<(ostream &os, const GameState &gameState) {
        os << "GameState: " << mapEnumToString(*gameState.stateName) << endl;
        os << "Available Commands:" << endl;

        for (const auto &transitionCommand : *gameState.transitionCommands) {
            os << "    " << mapEnumToString(transitionCommand) << endl;
        }

        os << "Next States:" << endl;
        for (const auto &nextState : *gameState.nextStates) {
            os << "    " << mapEnumToString(*nextState->stateName) << endl;
        }

        return os;
    }

    GameStateTypes GameState::getStateType() {
        return *this->stateType;
    }

    GameStates GameState::getStateName(){
        return *this->stateName;
    }

    vector<TransitionCommand> GameState::getTransitionCommands() {
        return *this->transitionCommands;
    }

    vector<shared_ptr<GameState>> GameState::getNextStates() {
        return *this->nextStates;
    }

    StartState::StartState(
        const GameStateTypes &stateType,
        const GameStates &stateName,
        const vector<TransitionCommand> &transitionCommands,
        const vector<shared_ptr<GameState>> &nextStates
    ) : GameState(stateType, stateName, transitionCommands, nextStates) {}

    StartState::StartState(
        const GameStateTypes &stateType,
        const GameStates &stateName,
        const vector<TransitionCommand> &transitionCommands
        ) : GameState(stateType, stateName, transitionCommands) {}

    shared_ptr<GameState> StartState::transitionToNextState(const TransitionCommand transitionCommand) {
        if (transitionCommand == TransitionCommand::LOAD_MAP) {
            return this->nextStates->at(0);
        }
        return nullptr;
    }

    MapLoadedState::MapLoadedState(
        const GameStateTypes &stateType,
        const GameStates &stateName,
        const vector<TransitionCommand> &transitionCommands,
        const vector<shared_ptr<GameState>> &nextStates
    ) : GameState(stateType, stateName, transitionCommands, nextStates) {}

    MapLoadedState::MapLoadedState(
        const GameStateTypes &stateType,
        const GameStates &stateName,
        const vector<TransitionCommand> &transitionCommands
    ) : GameState(stateType, stateName, transitionCommands) {}

    shared_ptr<GameState> MapLoadedState::transitionToNextState(const TransitionCommand transitionCommand) {
        bool isValidCommand = false;
        for (TransitionCommand command : *this->transitionCommands) {
            if (transitionCommand == command) {
                isValidCommand = true;
            }
        }

        if (isValidCommand) {
            if (transitionCommand == TransitionCommand::VALIDATE_MAP) {
                for (shared_ptr<GameState> state : *this->nextStates) {
                    if (state->getStateName() == MAP_VALIDATED) {
                        return state;
                    }
                }
            }
        }
        return nullptr;
    }

    MapValidatedState::MapValidatedState(
        const GameStateTypes &stateType,
        const GameStates &stateName,
        const vector<TransitionCommand> &transitionCommands,
        const vector<shared_ptr<GameState>> &nextStates
    ) : GameState(stateType, stateName, transitionCommands, nextStates) {}

    MapValidatedState::MapValidatedState(
        const GameStateTypes &stateType,
        const GameStates &stateName,
        const vector<TransitionCommand> &transitionCommands
    ) : GameState(stateType, stateName, transitionCommands) {}

    shared_ptr<GameState> MapValidatedState::transitionToNextState(const TransitionCommand transitionCommand) {
        if (transitionCommand == TransitionCommand::ADD_PLAYER) {
            return this->nextStates->at(0);
        }
        return nullptr;
    }

    PlayersAddedState::PlayersAddedState(
        const GameStateTypes &stateType,
        const GameStates &stateName,
        const vector<TransitionCommand> &transitionCommands,
        const vector<shared_ptr<GameState>> &nextStates
    ) : GameState(stateType, stateName, transitionCommands, nextStates) {}

    PlayersAddedState::PlayersAddedState(
        const GameStateTypes &stateType,
        const GameStates &stateName,
        const vector<TransitionCommand> &transitionCommands
    ) : GameState(stateType, stateName, transitionCommands) {}

    shared_ptr<GameState> PlayersAddedState::transitionToNextState(const TransitionCommand transitionCommand) {
        bool isValidCommand = false;
        for (TransitionCommand command : *this->transitionCommands) {
            if (transitionCommand == command) {
                isValidCommand = true;
            }
        }

        if (isValidCommand) {
            if (transitionCommand == TransitionCommand::ASSIGN_COUNTRIES) {
                for (shared_ptr<GameState> state : *this->nextStates) {
                    if (state->getStateName() == ASSIGN_REINFORCEMENT) {
                        return state;
                    }
                }
            }
        }
        return nullptr;
    }

    AssignReinforcementState::AssignReinforcementState(
        const GameStateTypes &stateType,
        const GameStates &stateName,
        const vector<TransitionCommand> &transitionCommands,
        const vector<shared_ptr<GameState>> &nextStates
    ) : GameState(stateType, stateName, transitionCommands, nextStates) {}

    AssignReinforcementState::AssignReinforcementState(
        const GameStateTypes &stateType,
        const GameStates &stateName,
        const vector<TransitionCommand> &transitionCommands
    ) : GameState(stateType, stateName, transitionCommands) {}

    shared_ptr<GameState> AssignReinforcementState::transitionToNextState(const TransitionCommand transitionCommand) {
        if (transitionCommand == TransitionCommand::ISSUE_ORDER) {
            return this->nextStates->at(0);
        }
        return nullptr;
    }

    IssueOrdersState::IssueOrdersState(
        const GameStateTypes &stateType,
        const GameStates &stateName,
        const vector<TransitionCommand> &transitionCommands,
        const vector<shared_ptr<GameState>> &nextStates
    ) : GameState(stateType, stateName, transitionCommands, nextStates) {}

    IssueOrdersState::IssueOrdersState(
        const GameStateTypes &stateType,
        const GameStates &stateName,
        const vector<TransitionCommand> &transitionCommands
    ) : GameState(stateType, stateName, transitionCommands) {}

    shared_ptr<GameState> IssueOrdersState::transitionToNextState(TransitionCommand transitionCommand) {
        bool isValidCommand = false;
        for (TransitionCommand command : *this->transitionCommands) {
            if (transitionCommand == command) {
                isValidCommand = true;
            }
        }

        if (isValidCommand) {
            if (transitionCommand == TransitionCommand::END_ISSUE_ORDERS) {
                for (shared_ptr<GameState> state : *this->nextStates) {
                    if (state->getStateName() == EXECUTE_ORDERS) {
                        return state;
                    }
                }
            }
        }
        return nullptr;
    }

    ExecuteOrdersState::ExecuteOrdersState(
        const GameStateTypes &stateType,
        const GameStates &stateName,
        const vector<TransitionCommand> &transitionCommands,
        const vector<shared_ptr<GameState>> &nextStates
    ) : GameState(stateType, stateName, transitionCommands, nextStates) {}

    ExecuteOrdersState::ExecuteOrdersState(
        const GameStateTypes &stateType,
        const GameStates &stateName,
        const vector<TransitionCommand> &transitionCommands
    ) : GameState(stateType, stateName, transitionCommands) {}

    shared_ptr<GameState> ExecuteOrdersState::transitionToNextState(const TransitionCommand transitionCommand) {
        bool isValidCommand = false;
        for (TransitionCommand command : *this->transitionCommands) {
            if (transitionCommand == command) {
                isValidCommand = true;
            }
        }

        if (isValidCommand) {
            if (transitionCommand == TransitionCommand::END_EXEC_ORDER) {
                for (shared_ptr<GameState> state : *this->nextStates) {
                    if (state->getStateName() == ASSIGN_REINFORCEMENT) {
                        return state;
                    }
                }
            }
            if (transitionCommand == TransitionCommand::WIN_GAME) {
                for (shared_ptr<GameState> state : *this->nextStates) {
                    if (state->getStateName() == WIN) {
                        return state;
                    }
                }
            }
        }
        return nullptr;
    }

    WinState::WinState(
        const GameStateTypes &stateType,
        const GameStates &stateName,
        const vector<TransitionCommand> &transitionCommands,
        const vector<shared_ptr<GameState>> &nextStates
    ) : GameState(stateType, stateName, transitionCommands, nextStates) {}

    WinState::WinState(
        const GameStateTypes &stateType,
        const GameStates &stateName,
        const vector<TransitionCommand> &transitionCommands
    ) : GameState(stateType, stateName, transitionCommands) {}

    shared_ptr<GameState> WinState::transitionToNextState(const TransitionCommand transitionCommand) {
        bool isValidCommand = false;
        for (TransitionCommand command : *this->transitionCommands) {
            if (transitionCommand == command) {
                isValidCommand = true;
            }
        }

        if (transitionCommand == TransitionCommand::END) {
            return nullptr;
        }

        if (isValidCommand) {
            if (transitionCommand == TransitionCommand::PLAY_AGAIN) {
                for (shared_ptr<GameState> state : *this->nextStates) {
                    if (state->getStateName() == START) {
                        return state;
                    }
                }
            }
        }
        return nullptr;
    }

    vector<string> getStringTransitionCommands() {
        vector<string> commands;
        for (int i = 0; i < 12; i++) {
            commands.push_back(mapEnumToString(static_cast<TransitionCommand>(i)));
        }
        return commands;
    }

    vector<string> getStringTransitionCommands(const vector<TransitionCommand> &gameStateCommands) {
        vector<string> commands;
        for (auto gameStateCommand : gameStateCommands) {
            commands.push_back(mapEnumToString(gameStateCommand));
        }
        return commands;
    }

    void GameEngine::printCurrentStateCommands(const vector<TransitionCommand> &commands, const string &gameStateName) {
        cout << "The available commands for the current state " + gameStateName + " are: \n";
        vector<string> stateCommands = getStringTransitionCommands(commands);
        for (const auto & command : stateCommands) {
            cout << command << "\n";
        }
    }

    shared_ptr<GameState> GameEngine::getCurrentGameState() {
        return this->currentGameState;
    }

    GameStates GameEngine::getCurrentGameStateName() {
        return this->currentGameState->getStateName();
    }

    GameEngine::GameEngine(const shared_ptr<GameState> &initialGameState, const shared_ptr<bool> &gameOver) {
        this->gameOver = gameOver;
        this->currentGameState = initialGameState;
        this->inputtedCommand = make_shared<string>("N/A");
    }

    bool GameEngine::transitionToNextState(TransitionCommand transitionCommand) {
        GameStates currentGameState = this->getCurrentGameState()->getStateName();
        shared_ptr<GameState> nextState = this->getCurrentGameState()->transitionToNextState(transitionCommand);
        bool isTransitionCommandLoopBack = static_cast<int>(this->getCurrentGameStateName()) == static_cast<int>(transitionCommand);

        if (transitionCommand == TransitionCommand::END && nextState == nullptr && currentGameState == GameStates::WIN) {
            cout << "Issued command is END and current state is WIN. Game is over \n";
            return true;
        }

        if (isTransitionCommandLoopBack && nextState == nullptr) {
            //Do nothing because we stay in the same state for say executing another order
            cout << "Issued command is valid, staying in same state: " + mapEnumToString(this->getCurrentGameStateName()) << "\n\n";
            return false;
        }
        if (!isTransitionCommandLoopBack && nextState == nullptr) {
            cout << "Invalid Command. Please try again. State of Game has not changed \n\n";
            return false;
        }

        cout << "Issued command is valid, transitioning to next state of game\n";
        cout << endl;
        this->currentGameState = nextState;
        return true;
    }

    void GameEngine::testGameStates() {
        cout << "Game State is currently: " + mapEnumToString(GameStates::START) << endl;

        this->printCurrentStateCommands(this->currentGameState->getTransitionCommands(), mapEnumToString(this->getCurrentGameStateName()));

        cout << "\nPlease input one of the previous commands to continue.\n";

        while(!*gameOver) {
            string inputCommand = "INVALID_COMMAND";
            cin >> inputCommand;
            cout << "You inputted " + inputCommand + "\n";
            TransitionCommand transitionCommand = mapStringToTransitionCommand(inputCommand);
            if (this->transitionToNextState(transitionCommand)) {
                if (mapEnumToString(this->getCurrentGameStateName()) == "WIN" && transitionCommand == TransitionCommand::END) {
                    cout << "The game state is currently: WIN and END command invoked.\n\nTerminating Program";
                    *this->gameOver = true;
                } else {
                    this->printCurrentStateCommands(this->currentGameState->getTransitionCommands(), mapEnumToString(this->getCurrentGameStateName()));
                }
            }
        }

        exit(EXIT_SUCCESS);
    }

    string GameEngine::getInputtedCommand() const {
        return *this->inputtedCommand;
    }

    bool GameEngine::getGameOverStatus() const {
        return *this->gameOver;
    }

    GameEngine& GameEngine::operator=(const GameEngine &otherGameEngine) {
        if (this != &otherGameEngine) {
            currentGameState = make_shared<StartState>(*dynamic_cast<StartState*>(otherGameEngine.currentGameState.get()));
            gameOver = make_shared<bool>(*otherGameEngine.gameOver);
            inputtedCommand = make_shared<string>(*otherGameEngine.inputtedCommand);
        }

        return *this;
    }

    ostream& operator<<(ostream &os, const GameEngine &gameEngine) {
        cout << "Current Game State: " << mapEnumToString(gameEngine.currentGameState->getStateName()) << endl;
        string gameOverString = *gameEngine.gameOver ? "True" : "False";
        cout << "Is Game Over: " << gameOverString << endl;
        cout << "Inputted Command: " << *gameEngine.inputtedCommand << endl;
        return os;
    }

}

