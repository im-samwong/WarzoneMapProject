#include "GameEngine.h"
#include <iostream>

std::string mapEnumToString(const TransitionCommand command) {
    std::string enumString;
    switch (command) {
    case TransitionCommand::LOAD_MAP:
        enumString = "loadmap";
        break;
    case TransitionCommand::VALIDATE_MAP:
        enumString = "validatemap";
        break;
    case TransitionCommand::ADD_PLAYER:
        enumString = "addplayer";
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

TransitionCommand mapStringToTransitionCommand(const std::string& transitionCommand) {
    TransitionCommand enumValue;
    if (transitionCommand == "loadmap") {
        enumValue = TransitionCommand::LOAD_MAP;
    } else if (transitionCommand == "validatemap") {
        enumValue = TransitionCommand::VALIDATE_MAP;
    } else if (transitionCommand == "addplayer") {
        enumValue = TransitionCommand::ADD_PLAYER;
    } else if (transitionCommand == "ASSIGN_COUNTRIES") {
        enumValue = TransitionCommand::ASSIGN_COUNTRIES;
    } else if (transitionCommand == "ISSUE_ORDER") {
        enumValue = TransitionCommand::ISSUE_ORDER;
    } else if (transitionCommand == "END_ISSUE_ORDERS") {
        enumValue = TransitionCommand::END_ISSUE_ORDERS;
    } else if (transitionCommand == "EXECUTE_ORDER") {
        enumValue = TransitionCommand::EXECUTE_ORDER;
    } else if (transitionCommand == "WIN_GAME") {
        enumValue = TransitionCommand::WIN_GAME;
    } else if (transitionCommand == "END_EXEC_ORDER") {
        enumValue = TransitionCommand::END_EXEC_ORDER;
    } else if (transitionCommand == "PLAY_AGAIN") {
        enumValue = TransitionCommand::PLAY_AGAIN;
    } else if (transitionCommand == "END") {
        enumValue = TransitionCommand::END;
    } else {
        enumValue = TransitionCommand::INVALID;
    }
    return enumValue;
}

std::string mapEnumToString(const GameStates stateName) {
    std::string enumString;
    switch (stateName) {
    case START:
        enumString = "start";
        break;
    case MAP_LOADED:
        enumString = "maploaded";
        break;
    case MAP_VALIDATED:
        enumString = "mapvalidated";
        break;
    case PLAYERS_ADDED:
        enumString = "playersadded";
        break;
    case ASSIGN_REINFORCEMENT:
        enumString = "assignreinforcement";
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

std::vector<std::string> getStringTransitionCommands() {
    std::vector<std::string> commands;
    for (int i = 0; i < 12; i++) {
        commands.push_back(mapEnumToString(static_cast<TransitionCommand>(i)));
    }
    return commands;
}

std::vector<std::string> getStringTransitionCommands(const std::vector<TransitionCommand>& gameStateCommands) {
    std::vector<std::string> commands;
    for (auto gameStateCommand : gameStateCommands) {
        commands.push_back(mapEnumToString(gameStateCommand));
    }
    return commands;
}

//Abstract Class implementation

GameState::GameState(const GameStateTypes &iStateType,const GameStates &iStateName,const std::vector<TransitionCommand> &iTransitionCommands,
    const std::vector<GameState *> &iNextStates) {
    stateType = new GameStateTypes(iStateType);
    stateName = new GameStates(iStateName);
    transitionCommands = new std::vector(iTransitionCommands);
    nextStates = new std::vector(iNextStates);
}

GameState::GameState(const GameStateTypes &iStateType, const GameStates &iStateName, const std::vector<TransitionCommand> &iTransitionCommands) {
    stateType = new GameStateTypes(iStateType);
    stateName = new GameStates(iStateName);
    transitionCommands = new std::vector(iTransitionCommands);
    nextStates = nullptr;
}

GameState::GameState(const GameState &otherGameState) {
    stateType = new GameStateTypes(*otherGameState.stateType);
    stateName = new GameStates(*otherGameState.stateName);
    transitionCommands = new std::vector(*otherGameState.transitionCommands);
    nextStates = new std::vector(*otherGameState.nextStates);
}

GameState& GameState::operator=(const GameState &otherGameState) {
    if (this != &otherGameState) {
        delete stateType;
        delete stateName;
        delete transitionCommands;
        delete nextStates;

        stateType = new GameStateTypes(*otherGameState.stateType);
        stateName = new GameStates(*otherGameState.stateName);
        transitionCommands = new std::vector(*otherGameState.transitionCommands);
        nextStates = new std::vector(*otherGameState.nextStates);
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const GameState& gameState) {
    os << "GameState: " << mapEnumToString(*gameState.stateName) << std::endl;
    os << "Available Commands:" << std::endl;

    for (const auto& transitionCommand : *gameState.transitionCommands) {
        os << "    " << mapEnumToString(transitionCommand) << std::endl;
    }

    os << "Next States:" << std::endl;
    for (const auto& nextState : *gameState.nextStates) {
        os << "    " << mapEnumToString(*nextState->stateName) << std::endl;
    }

    return os;
}

GameStateTypes GameState::getStateType() {
    return *this->stateType;
}

GameStates GameState::getStateName() {
    return *this->stateName;
}

std::vector<TransitionCommand> GameState::getTransitionCommands() {
    return *this->transitionCommands;
}

std::vector<GameState*> GameState::getNextStates() {
    return *this->nextStates;
}

void GameState::setNextStates(const std::vector<GameState*> &nextStates) {
    delete this->nextStates;
    this->nextStates = new std::vector(nextStates);
}

//Derived classes implementation

StartState::StartState(
    const GameStateTypes& stateType,
    const GameStates& stateName,
    const std::vector<TransitionCommand>& transitionCommands,
    const std::vector<GameState*>& nextStates) : GameState(stateType, stateName, transitionCommands, nextStates) {}

StartState::StartState(
    const GameStateTypes& stateType,
    const GameStates& stateName,
    const std::vector<TransitionCommand>& transitionCommands) : GameState(stateType, stateName, transitionCommands) {}

GameState* StartState::transitionToNextState(const TransitionCommand transitionCommand) {
    if (transitionCommand == TransitionCommand::LOAD_MAP) {
        return this->nextStates->at(0);
    }
    return nullptr;
}

MapLoadedState::MapLoadedState(
    const GameStateTypes& stateType,
    const GameStates& stateName,
    const std::vector<TransitionCommand>& transitionCommands,
    const std::vector<GameState*>& nextStates) : GameState(stateType, stateName, transitionCommands, nextStates) {}

MapLoadedState::MapLoadedState(
    const GameStateTypes& stateType,
    const GameStates& stateName,
    const std::vector<TransitionCommand>& transitionCommands) : GameState(stateType, stateName, transitionCommands) {}

GameState* MapLoadedState::transitionToNextState(const TransitionCommand transitionCommand) {
    bool isValidCommand = false;
    for (TransitionCommand command : *this->transitionCommands) {
        if (transitionCommand == command) {
            isValidCommand = true;
        }
    }

    if (isValidCommand) {
        if (transitionCommand == TransitionCommand::VALIDATE_MAP) {
            for (GameState* state : *this->nextStates) {
                if (state->getStateName() == MAP_VALIDATED) {
                    return state;
                }
            }
        }
    }
    return nullptr;
}

MapValidatedState::MapValidatedState(
    const GameStateTypes& stateType,
    const GameStates& stateName,
    const std::vector<TransitionCommand>& transitionCommands,
    const std::vector<GameState*>& nextStates) : GameState(stateType, stateName, transitionCommands, nextStates) {}

MapValidatedState::MapValidatedState(
    const GameStateTypes& stateType,
    const GameStates& stateName,
    const std::vector<TransitionCommand>& transitionCommands) : GameState(stateType, stateName, transitionCommands) {}

GameState* MapValidatedState::transitionToNextState(const TransitionCommand transitionCommand) {
    if (transitionCommand == TransitionCommand::ADD_PLAYER) {
        return this->nextStates->at(0);
    }
    return nullptr;
}

PlayersAddedState::PlayersAddedState(
    const GameStateTypes& stateType,
    const GameStates& stateName,
    const std::vector<TransitionCommand>& transitionCommands,
    const std::vector<GameState*>& nextStates) : GameState(stateType, stateName, transitionCommands, nextStates) {}

PlayersAddedState::PlayersAddedState(
    const GameStateTypes& stateType,
    const GameStates& stateName,
    const std::vector<TransitionCommand>& transitionCommands) : GameState(stateType, stateName, transitionCommands) {}

GameState* PlayersAddedState::transitionToNextState(const TransitionCommand transitionCommand) {
    bool isValidCommand = false;
    for (TransitionCommand command : *this->transitionCommands) {
        if (transitionCommand == command) {
            isValidCommand = true;
        }
    }

    if (isValidCommand) {
        if (transitionCommand == TransitionCommand::ASSIGN_COUNTRIES) {
            for (GameState* state : *this->nextStates) {
                if (state->getStateName() == ASSIGN_REINFORCEMENT) {
                    return state;
                }
            }
        }
    }
    return nullptr;
}

AssignReinforcementState::AssignReinforcementState(
    const GameStateTypes& stateType,
    const GameStates& stateName,
    const std::vector<TransitionCommand>& transitionCommands,
    const std::vector<GameState*>& nextStates) : GameState(stateType, stateName, transitionCommands, nextStates) {}

AssignReinforcementState::AssignReinforcementState(
    const GameStateTypes& stateType,
    const GameStates& stateName,
    const std::vector<TransitionCommand>& transitionCommands) : GameState(stateType, stateName, transitionCommands) {}

GameState* AssignReinforcementState::transitionToNextState(const TransitionCommand transitionCommand) {
    if (transitionCommand == TransitionCommand::ISSUE_ORDER) {
        return this->nextStates->at(0);
    }
    return nullptr;
}

IssueOrdersState::IssueOrdersState(
    const GameStateTypes& stateType,
    const GameStates& stateName,
    const std::vector<TransitionCommand>& transitionCommands,
    const std::vector<GameState*>& nextStates) : GameState(stateType, stateName, transitionCommands, nextStates) {}

IssueOrdersState::IssueOrdersState(
    const GameStateTypes& stateType,
    const GameStates& stateName,
    const std::vector<TransitionCommand>& transitionCommands) : GameState(stateType, stateName, transitionCommands) {}

GameState* IssueOrdersState::transitionToNextState(TransitionCommand transitionCommand) {
    bool isValidCommand = false;
    for (TransitionCommand command : *this->transitionCommands) {
        if (transitionCommand == command) {
            isValidCommand = true;
        }
    }

    if (isValidCommand) {
        if (transitionCommand == TransitionCommand::END_ISSUE_ORDERS) {
            for (GameState* state : *this->nextStates) {
                if (state->getStateName() == EXECUTE_ORDERS) {
                    return state;
                }
            }
        }
    }
    return nullptr;
}

ExecuteOrdersState::ExecuteOrdersState(
    const GameStateTypes& stateType,
    const GameStates& stateName,
    const std::vector<TransitionCommand>& transitionCommands,
    const std::vector<GameState*>& nextStates) : GameState(stateType, stateName, transitionCommands, nextStates) {}

ExecuteOrdersState::ExecuteOrdersState(
    const GameStateTypes& stateType,
    const GameStates& stateName,
    const std::vector<TransitionCommand>& transitionCommands) : GameState(stateType, stateName, transitionCommands) {}

GameState* ExecuteOrdersState::transitionToNextState(const TransitionCommand transitionCommand) {
    bool isValidCommand = false;
    for (TransitionCommand command : *this->transitionCommands) {
        if (transitionCommand == command) {
            isValidCommand = true;
        }
    }

    if (isValidCommand) {
        if (transitionCommand == TransitionCommand::END_EXEC_ORDER) {
            for (GameState* state : *this->nextStates) {
                if (state->getStateName() == ASSIGN_REINFORCEMENT) {
                    return state;
                }
            }
        }
        if (transitionCommand == TransitionCommand::WIN_GAME) {
            for (GameState* state : *this->nextStates) {
                if (state->getStateName() == WIN) {
                    return state;
                }
            }
        }
    }
    return nullptr;
}

WinState::WinState(
    const GameStateTypes& stateType,
    const GameStates& stateName,
    const std::vector<TransitionCommand>& transitionCommands,
    const std::vector<GameState*>& nextStates) : GameState(stateType, stateName, transitionCommands, nextStates) {}

WinState::WinState(
    const GameStateTypes& stateType,
    const GameStates& stateName,
    const std::vector<TransitionCommand>& transitionCommands) : GameState(stateType, stateName, transitionCommands) {}

GameState* WinState::transitionToNextState(const TransitionCommand transitionCommand) {
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
            for (GameState* state : *this->nextStates) {
                if (state->getStateName() == START) {
                    return state;
                }
            }
        }
    }
    return nullptr;
}

//Game Engine implementation code

GameEngine::~GameEngine() {
    delete gameOver;
    delete inputtedCommand;
    delete currentGameState;
}

GameStates GameEngine::getCurrentGameStateName() const {
    return this->currentGameState->getStateName();
}

GameState* GameEngine::getCurrentGameState() const {
    return this->currentGameState;
}

bool GameEngine::getGameOverStatus() const {
    return *this->gameOver;
}

std::string GameEngine::getInputtedCommand() const {
    return *this->inputtedCommand;
}

void GameEngine::setCurrentGameState(GameState* gameState) {
    delete currentGameState;
    currentGameState = gameState;
}

void GameEngine::setGameOverStatus(const bool gameOver) {
    delete this->gameOver;
    this->gameOver = new bool(gameOver);
}

void GameEngine::setInputtedCommand(const std::string &inputtedCommand) {
    delete this->inputtedCommand;
    this->inputtedCommand = new std::string(inputtedCommand);
}

bool GameEngine::transitionToNextState(TransitionCommand transitionCommand) {
    GameStates currentGameState = this->getCurrentGameState()->getStateName();
    GameState* nextState = this->getCurrentGameState()->transitionToNextState(transitionCommand);
    bool isTransitionCommandLoopBack = static_cast<int>(this->getCurrentGameStateName()) == static_cast<int>(transitionCommand);

    if (transitionCommand == TransitionCommand::END && nextState == nullptr && currentGameState == GameStates::WIN) {
        std::cout << "Issued command is END and current state is WIN. Game is over \n";
        return true;
    }

    if (isTransitionCommandLoopBack && nextState == nullptr) {
        // Do nothing because we stay in the same state for say executing another order
        std::cout << "Issued command is valid, staying in same state: " + mapEnumToString(this->getCurrentGameStateName()) << "\n\n";
        return false;
    }
    if (!isTransitionCommandLoopBack && nextState == nullptr) {
        std::cout << "Invalid Command. Please try again. State of Game has not changed \n\n";
        return false;
    }

    std::cout << "Issued command is valid, transitioning to next state of game\n";
    std::cout << std::endl;

    this->currentGameState = nextState;

    return true;
}

void GameEngine::printCurrentStateCommands(const std::vector<TransitionCommand>& commands, const std::string& gameStateName) {
    std::cout << "The available commands for the current state " + gameStateName + " are: \n";
    std::vector<std::string> stateCommands = getStringTransitionCommands(commands);
    for (const auto& command : stateCommands) {
        std::cout << command << "\n";
    }
}

GameEngine* GameEngine::getInstance() {
    if (!game_engine_instance) {
        game_engine_instance = new GameEngine();
    }
    return game_engine_instance;
}

void GameEngine::startupPhase() {
    std::cout << "Entering Startup Phase: \n" ;
}

GameEngine* GameEngine::game_engine_instance = nullptr;

std::ostream& operator<<(std::ostream& os, const GameEngine& gameEngine) {
    std::cout << "Current Game State: " << mapEnumToString(gameEngine.currentGameState->getStateName()) << std::endl;
    std::string gameOverString = *gameEngine.gameOver ? "True" : "False";
    std::cout << "Is Game Over: " << gameOverString << std::endl;
    std::cout << "Inputted Command: " << *gameEngine.inputtedCommand << std::endl;
    return os;
}
