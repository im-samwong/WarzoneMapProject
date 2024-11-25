#include "GameEngine.h"
#include "../PlayerStrategies/PlayerStrategies.h"
#include <iostream>
#include <map>

// Initialize static members
std::set<std::string> GameState::negotiations;
std::unordered_map<Player*, bool> GameState::conqueredTerritories;

// Negotiation methods
void GameState::addNegotiation(Player* player1, Player* player2) {
    negotiations.insert(makeClause(player1, player2));
}

bool GameState::hasNegotiation(Player* player1, Player* player2) {
    return negotiations.find(makeClause(player1, player2)) != negotiations.end();
}

void GameState::resetNegotiations() {
    negotiations.clear();
}

// Helper for negotiation clause creation
std::string GameState::makeClause(Player* player1, Player* player2) {
    return player1->getName() < player2->getName() ?
           player1->getName() + "_" + player2->getName() :
           player2->getName() + "_" + player1->getName();
}

// Conquered territory methods
void GameState::setConqueredTerritory(Player* player, bool status) {
    conqueredTerritories[player] = status;
}

bool GameState::hasConqueredTerritory(Player* player) {
    auto it = conqueredTerritories.find(player);
    return it != conqueredTerritories.end() && it->second;
}

void GameState::resetConqueredTerritories() {
    conqueredTerritories.clear();
}

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

// Abstract Class implementation

GameState::GameState(const GameStateTypes& iStateType, const GameStates& iStateName, const std::vector<TransitionCommand>& iTransitionCommands,
                     const std::vector<GameState*>& iNextStates) {
    stateType = new GameStateTypes(iStateType);
    stateName = new GameStates(iStateName);
    transitionCommands = new std::vector(iTransitionCommands);
    nextStates = new std::vector(iNextStates);

    if (logObserver != nullptr) {
        this->attach(logObserver);
        notify(this);
    }
}

GameState::GameState(const GameStateTypes& iStateType, const GameStates& iStateName, const std::vector<TransitionCommand>& iTransitionCommands) {
    stateType = new GameStateTypes(iStateType);
    stateName = new GameStates(iStateName);
    transitionCommands = new std::vector(iTransitionCommands);
    nextStates = nullptr;

    if (logObserver != nullptr) {
        this->attach(logObserver);
        notify(this);
    }
}

GameState::GameState(const GameState& otherGameState) {
    stateType = new GameStateTypes(*otherGameState.stateType);
    stateName = new GameStates(*otherGameState.stateName);
    transitionCommands = new std::vector(*otherGameState.transitionCommands);
    nextStates = new std::vector(*otherGameState.nextStates);

    if (logObserver != nullptr) {
        this->attach(logObserver);
        notify(this);
    }
}

std::string GameState::stringToLog() const {
    std::string state = mapEnumToString(*this->stateName);
    return "Current state: " + state;
}

GameState& GameState::operator=(const GameState& otherGameState) {
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

void GameState::setNextStates(const std::vector<GameState*>& nextStates) {
    delete this->nextStates;
    this->nextStates = new std::vector(nextStates);
}

// Derived classes implementation

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

// Game Engine implementation code

GameEngine::~GameEngine() {

    delete gameOver;
    delete inputtedCommand;
    delete currentGameState;
    for (Player* player : *players) {
        delete player; // Free the memory for each Player object
    }
    delete players;
    delete map;
    delete cp;
    delete deck;
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

void GameEngine::setInputtedCommand(const std::string& inputtedCommand) {
    delete this->inputtedCommand;
    this->inputtedCommand = new std::string(inputtedCommand);
}

bool GameEngine::transitionToNextState(TransitionCommand transitionCommand) {
    GameStates currentGameState = this->getCurrentGameState()->getStateName();
    GameState* nextState = this->getCurrentGameState()->transitionToNextState(transitionCommand);
    bool isTransitionCommandLoopBack = static_cast<int>(this->getCurrentGameStateName()) == static_cast<int>(transitionCommand);

    if (transitionCommand == TransitionCommand::END && nextState == nullptr && currentGameState == GameStates::WIN) {
        std::cout << "Issued command is END and current state is WIN. Game is over \n";

        if (logObserver != nullptr){
            notify(this);
        }

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


    if (logObserver != nullptr){
        notify(this);
    }

    return true;
}

std::string GameEngine::stringToLog() const {
    GameStates currentGameState = this->getCurrentGameState()->getStateName();
    return "Current state: " + mapEnumToString(currentGameState);
}

void GameEngine::printCurrentStateCommands(const std::vector<TransitionCommand>& commands, const std::string& gameStateName) {
    std::cout << "The available commands for the current state " + gameStateName + " are: \n";
    std::vector<std::string> stateCommands = getStringTransitionCommands(commands);
    for (const auto& command : stateCommands) {
        std::cout << command << "\n";
    }
}

void GameEngine::addPlayer(const std::string& playerName) {
    Player* p = new Player(playerName);
    if(playerName == "Aggressive") {
        p->setPlayerStrategy(new AggressivePlayer(p));
    } else if(playerName == "Neutral") {
        p->setPlayerStrategy(new NeutralPlayer(p));
    } else {
        p->setPlayerStrategy(new HumanPlayer(p));
    }
    players->push_back(p);
}

void GameEngine::readInputFromFile(const std::string& filename) {
    delete cp;
    cp = new FileCommandProcessorAdapter(filename);
}

void GameEngine::playTournament(const TournamentParameters& params) {
    std::cout << "Starting Tournament Mode...\n";

    // Generate tournament commands grouped by map
    std::vector<std::vector<std::string>> commandsByMap = cp->generateTournamentCommands(params);

    // Store tournament results
    std::vector<TournamentResult> tournamentResults;

    for (size_t mapIndex = 0; mapIndex < commandsByMap.size(); ++mapIndex) {
        TournamentResult result; // Create a result object for this map
        result.mapName = params.mapFiles[mapIndex];

        // For each map, run the specified number of games
        for (size_t gameIndex = 0; gameIndex < params.numberOfGames; ++gameIndex) {
            if (!(mapIndex == 0 && gameIndex == 0)) {
                resetGame();
            }

            const auto& commands = commandsByMap[mapIndex]; // Commands for the current map
            std::cout << "Starting tournament for map " << result.mapName << ", Game " << gameIndex + 1 << "\n";

            // Prepare command stream for the current game
            std::stringstream commandStream;
            for (const auto& command : commands) {
                commandStream << command << "\n";
            }
            cp->setInputStream(&commandStream); // Redirect input to automated commands

            // Execute startupPhase with automated input
            if (!startupPhase()) {
                std::cout << "Failed to initialize game for map: " << result.mapName << "\n";
                result.gameResults.push_back("Initialization Failed");
                continue; // Skip to the next game if initialization fails
            }

            cp->restoreConsoleInput(); // Restore console input for manual player decisions

            // Run main game loop and record the result
            std::string winner = mainGameLoop(); // Assume this returns the winner or outcome
            result.gameResults.push_back(winner);
        }

        // Add the result for this map to the tournament results
        tournamentResults.push_back(result);
    }

    std::cout << "Tournament finished.\n";

    // Print the tournament results
    printTournamentResults(tournamentResults, params.numberOfGames);
}


bool GameEngine::startupPhase() {

    std::cout << "== Entering Startup Phase == \n";

    // set the state to start
    currentGameState = new StartState(
        GameStateTypes::STARTUP,
        GameStates::START,
        {TransitionCommand::LOAD_MAP});
    std::string currentStateName = mapEnumToString(currentGameState->getStateName());
    std::cout << "Currently in 'Start' state, waiting for 'loadmap <filename>' command\n";

    if (logObserver != nullptr) {
        cp->attach(logObserver);
    }

    Command* cmd = cp->getCommand(); // get command from user

    if (!(cp->validate(*cmd, currentStateName))) { // validate command (only loadmap should work here)
        std::cout << "Unable to start game, received unexpected command:" << cmd->getCommand() << std::endl;
        return false;
    }

    if (logObserver != nullptr) {
        cmd->attach(logObserver);
    }

    // load the map file
    std::cout << "Loading map file: " << cmd->getArgument() << '\n';
    MapLoader* loader = new MapLoader();
    map = loader->loadMap(cmd->getArgument());
    std::cout << "Map loaded \n";
    cmd->saveEffect("Map file loaded successfully.\n");

    // Transition to maploaded state
    setCurrentGameState(new MapLoadedState(
        GameStateTypes::STARTUP,
        GameStates::MAP_LOADED,
        {TransitionCommand::LOAD_MAP, TransitionCommand::VALIDATE_MAP}));
    currentStateName = mapEnumToString(currentGameState->getStateName());
    std::cout << "Currently in maploaded state, waiting for command...\n";

    // while loop to allow loading another map file or moving to validate
    while (currentGameState->getStateName() == GameStates::MAP_LOADED) {
        cmd = cp->getCommand();
        if (!(cp->validate(*cmd, currentStateName))) { // check if command is valid
            std::cout << "Unable to start game, received unexpected command:" << cmd->getCommand() << std::endl;
            return false;
        }

        if (logObserver != nullptr) {
            cmd->attach(logObserver);
        }

        if (cmd->getCommand() == "validatemap") {
            std::cout << "Validating Map...\n";
            if (map->validate()) { // validate the loaded map file using the Map object built in method.
                std::cout << "Map is valid.\n";
                cmd->saveEffect("Map is validated");
                delete loader; // no longer needed as the map has been loaded.

                // change the state
                setCurrentGameState(new MapValidatedState(GameStateTypes::STARTUP, GameStates::MAP_VALIDATED, {TransitionCommand::ADD_PLAYER})); // if map is valid, move to mapvalidated
                currentStateName = mapEnumToString(currentGameState->getStateName());

            } else { // ask user to try another map if not valid
                std::cout << "Map is not valid, please try loading another map file with loadmap <filename>\n";
                cmd->saveEffect("Unable to validate map file."); // otherwise ask the user to load another file.
            }
        } else if (cmd->getCommand() == "loadmap") {
            map = new Map(*(loader->loadMap(cmd->getArgument())));
            std::cout << "Map loaded \n";
            cmd->saveEffect("Map file loaded successfully.\n");
        }
    }

    std::cout << "Currently in mapvalidated state, you can add players with addplayer <player>\n";
    cmd = cp->getCommand();

    if (!(cp->validate(*cmd, currentStateName))) {
        std::cout << "Unable to start game, received unexpected command:" << cmd->getCommand() << std::endl;
        return false;
    }

    if (logObserver != nullptr) {
        cmd->attach(logObserver);
    }

    std::cout << "Adding player " << cmd->getArgument() << "\n";
    addPlayer(cmd->getArgument());
    cmd->saveEffect("Player added");
    std::cout << "Player added successfully.";

    // change game state to playersadded
    setCurrentGameState(new MapValidatedState(GameStateTypes::STARTUP, GameStates::PLAYERS_ADDED, {TransitionCommand::ADD_PLAYER, TransitionCommand::ASSIGN_COUNTRIES}));
    currentStateName = mapEnumToString(currentGameState->getStateName());

    // Players added phase loop:
    while (currentGameState->getStateName() == GameStates::PLAYERS_ADDED) {
        std::cout << "Currently in playersadded state, you can add players with addplayer <player> or start the game with gamestart (minimum 2 players needed)\n";
        cmd = cp->getCommand();

        if (!(cp->validate(*cmd, currentStateName))) { // check if command is valid
            std::cout << "Unable to start game, received unexpected command:" << cmd->getCommand() << std::endl;
            return false;
        }

        if (logObserver != nullptr) {
            cmd->attach(logObserver);
        }

        if (cmd->getCommand() == "addplayer") {
            if (players->size() < MAX_PLAYERS) {
                addPlayer(cmd->getArgument());
                cmd->saveEffect("Player added");
            } else {
                cmd->saveEffect("Couldn't add player, max player count reached");
            }
        } else if (cmd->getCommand() == "gamestart") {
            if (players->size() < MIN_PLAYERS) {
                std::cout << "Need at least 2 players to start the game.\n";
                cmd->saveEffect("Couldn't start the game with only 1 player");
            } else {
                std::cout << "Starting game\n";
                cmd->saveEffect("Triggered game start");
                break; // break out of the loop to start carry on with the startup phase
            }
        }
    }

    std::cout << "Randomly distributing territories to all players...\n";
    distrubuteTerritories();
    std::cout << "Randomly determining play order...\n";
    shufflePlayers(); // shuffle players in the vector to determine playing order.
    std::cout << "Giving every player 50 reinforcements and drawing 2 cards for them\n";
    std::cout << "Play order:\n";

    // Print out player's name by play order
    for (std::size_t i = 0; i < players->size(); i++) {
        (*players)[i]->changeReinforcements(50); // add 50 reinforcements to each player

        // let each player draw 2 cards:
        (*players)[i]->getHand().addCard(deck->draw());
        (*players)[i]->getHand().addCard(deck->draw());
        std::cout << (*players)[i]->getName() << '\n';
    }

    std::cout << "Startup phase done. Moving game state to assignreinforcements\n";

    setCurrentGameState(new AssignReinforcementState(
        GameStateTypes::PLAY,
        GameStates::ASSIGN_REINFORCEMENT,
        {TransitionCommand::ISSUE_ORDER}));

    return true;
}

void GameEngine::distrubuteTerritories() {

    std::vector<Territory*>* territories = new std::vector<Territory*>(map->getTerritories());

    // Shuffle the list of territories to ensure random distribution
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(territories->begin(), territories->end(), gen);

    // Assign territories to players in round-robin fashion
    std::size_t playerCount = players->size();
    for (std::size_t i = 0; i < territories->size(); ++i) {
        Player* currentPlayer = (*players)[i % playerCount];
        currentPlayer->addTerritory((*territories)[i]);
        (*territories)[i]->setOwner(currentPlayer);
    }
}

void GameEngine::shufflePlayers() {
    // Obtain a random seed based on the system clock
    std::random_device rd;
    std::mt19937 gen(rd());

    // Shuffle the vector with the random generator
    std::shuffle(players->begin(), players->end(), gen);
}

std::ostream& operator<<(std::ostream& os, const GameEngine& gameEngine) {
    std::cout << "Current Game State: " << mapEnumToString(gameEngine.currentGameState->getStateName()) << std::endl;
    std::string gameOverString = *gameEngine.gameOver ? "True" : "False";
    std::cout << "Is Game Over: " << gameOverString << std::endl;
    std::cout << "Inputted Command: " << *gameEngine.inputtedCommand << std::endl;
    return os;
}

void GameEngine::reinforcementPhase() {
    std::cout << "\nIn Reinforcement Phase, adding army units to each player. No input needed from users please wait" << std::endl;

    for (Player* player : *players) {
        player->changeReinforcements(3);
        const std::size_t player_territory_count = player->toDefend().size();
        const int owned_territory_bonus_reinforcements = static_cast<int>(player_territory_count / 3);
        player->changeReinforcements(owned_territory_bonus_reinforcements);

        const int continent_reinforcement_bonus = map->getPlayerContinentBonuses(player);
        player->changeReinforcements(continent_reinforcement_bonus);
    }

    std::cout << "Added army units to players, moving to next state issueorders" << std::endl;
    setCurrentGameState(new IssueOrdersState(
        GameStateTypes::PLAY,
        GameStates::ISSUE_ORDERS,
        {TransitionCommand::ISSUE_ORDER, TransitionCommand::END_ISSUE_ORDERS})
    );
}

void GameEngine::issueOrdersPhase() {
    std::cout << "\nIn Issue Orders Phase!\n" << std::endl;

    for (Player* player : *players) {
        if (logObserver != nullptr) {
            player->getOrdersList()->attach(logObserver);
        }
        std::cout << "Player " << player->getName() << " it is your turn.\n" << std::endl;
        player->issueOrder(players);
        std::cout << std::endl;
    }

    std::cout << "Issue Orders Phase done, moving to next state" << std::endl;

    setCurrentGameState(new ExecuteOrdersState(
        GameStateTypes::PLAY,
        GameStates::EXECUTE_ORDERS,
        {TransitionCommand::EXECUTE_ORDER, TransitionCommand::END_EXEC_ORDER, TransitionCommand::WIN_GAME})
    );
}

void GameEngine::executeOrdersPhase() {
    std::cout << "\nIn Execute Orders Phase!\n" << std::endl;
    std::cout << "Will first execute all deploy/reinforcement orders for each player" << std::endl;

    //Execute all deploy/reinforcement orders
    for(Player* player : *players) {
        OrderList* playerOrderList = player->getOrdersList();
        const std::vector<std::unique_ptr<Order>>& playerOrders = playerOrderList->getOrders();
        std::vector<int> ordersToRemove;
        for(int i = 0; i < playerOrders.size(); ++i) {
            if (auto castedPtr = dynamic_cast<DeployOrder*>(playerOrders[i].get())) {
                if (logObserver != nullptr) {
                    castedPtr->attach(logObserver);
                }
                castedPtr->execute();
                ordersToRemove.push_back(i);
            }
        }

        std::sort(ordersToRemove.rbegin(), ordersToRemove.rend());
        for(const int index: ordersToRemove) {
            playerOrderList->removeOrder(index);//Should remove them
        }
    }

    std::cout << "\nAll Reinforcement/Deploy orders done. Will now execute remaining orders\n" << std::endl;

    for(Player* player : *players) {
        OrderList* playerOrderList = player->getOrdersList();
        const std::vector<std::unique_ptr<Order>>& playerOrders = playerOrderList->getOrders();
        std::vector<int> ordersToRemove;
        if (playerOrders.empty()) {
            std::cout << "No orders to execute for player " << player->getName() << std::endl;
        } else {
            for(int i = 0; i < playerOrders.size(); ++i) {
                std::cout << "Player " << player->getName() << ", your next order is: " << *playerOrders[i] << std::endl;
                if (logObserver != nullptr) {
                    playerOrders[i].get()->attach(logObserver);
                }
                playerOrders[i].get()->execute();
                ordersToRemove.push_back(i);
                std::cout << std::endl;
            }

            std::sort(ordersToRemove.rbegin(), ordersToRemove.rend());
            for(const int index: ordersToRemove) {
                playerOrderList->removeOrder(index);//Should remove them
            }
        }
    }
    std::cout << std::endl;
}

void GameEngine::removeEliminatedPlayers() {
    std::vector<int> playersToRemove;
    for(int i = 0; i < players->size(); ++i) {
        if (players->at(i)->toDefend().empty()) {//No territories to defend = no more owned territories
            std::cout << "Player " << players->at(i)->getName() << " has been ELIMINATED and will be removed." << std::endl;
            playersToRemove.push_back(i);
        }
    }

    std::sort(playersToRemove.rbegin(), playersToRemove.rend());
    for(const int index: playersToRemove) {
        delete (*players)[index];
        players->erase(players->begin() + index);
    }
}

bool GameEngine::hasGameEnded() {
    if(players->size() == 1) {
        std::cout << "Only 1 Player remains, the Game has now ended." << std::endl;
        std::cout << players->at(0)->getName() << " has won!" << std::endl;
        std::cout << "Game Over, moving to Win State" << std::endl;
        setCurrentGameState(new WinState(
            GameStateTypes::PLAY,
            GameStates::WIN,
            {TransitionCommand::PLAY_AGAIN, TransitionCommand::END}
        ));
        return true;
    }
    return false;
}

void GameEngine::resetGame() {

    std::cout << "Resetting game parameters." << std::endl;

    // Reset the game over flag
    *gameOver = false;

    std::cout << "Error after gameover." << std::endl;

    // Clear players
    for (auto player : *players) {
        delete player; // Clean up player objects
    }
    players->clear();

    std::cout << "Error after players." << std::endl;

    // Clear map
    if (map != nullptr) {
        delete map; // Clean up map object
        map = nullptr;
    }

    std::cout << "Error after map." << std::endl;

    // Reset the deck
    if (deck != nullptr) {
        delete deck; // Clean up the old deck
    }
    deck = new Deck();

    std::cout << "Error after deck." << std::endl;

    // Reset the current game state
    if (currentGameState != nullptr) {
        delete currentGameState; // Clean up the old state
    }
    currentGameState = nullptr;

    std::cout << "GameEngine has been reset for the next game.\n";
}

void GameEngine::refreshPlayerConstraints() {
    std::cout << "Refreshing player constraints." << std::endl;
    for(Player* player : *players) {
        // Check for card reward
        if (GameState::hasConqueredTerritory(player)) {
            player->getHand().addCard(deck->draw());
            std::cout << player->getName() << " receives a card for conquering a territory this turn.\n";
        }
    }
    GameState::resetConqueredTerritories();  // Reset the conquest status for the next turn
    GameState::resetNegotiations();          // Reset the negotiation status for the next turn
}

std::string GameEngine::mainGameLoop() {
    while (!*gameOver) {
        reinforcementPhase();
        issueOrdersPhase();
        executeOrdersPhase();

        // Card bonus check and reset player constraint status
        refreshPlayerConstraints();

        // endGame();//Will make player 1 win automatically ONLY FOR DEMO PURPOSES
        removeEliminatedPlayers();
        setGameOverStatus(hasGameEnded());
    }

    if (players->size() == 1) {
        return players->front()->getName(); // Return the winner's name
    }

    return "Draw"; // If no players remain or game ends in a tie

    //exit(0);
}

void GameEngine::printTournamentResults(const std::vector<TournamentResult>& results, int numberOfGames) {
    std::cout << "\nTournament Results:\n";

    // Dynamically generate the header based on numberOfGames
    std::cout << " Map                ";
    for (int i = 1; i <= numberOfGames; ++i) {
        std::cout << "| Game " << i << "   ";
    }
    std::cout << "\n";

    // Print separator
    std::cout << "--------------------";
    for (int i = 0; i < numberOfGames; ++i) {
        std::cout << "|----------";
    }
    std::cout << "\n";

    // Print results for each map
    for (const auto& result : results) {
        // Print map name
        std::cout << result.mapName;
        for (size_t i = result.mapName.size(); i < 20; ++i) std::cout << " "; // Align map names

        // Print game results
        for (size_t i = 0; i < result.gameResults.size(); ++i) {
            std::cout << "| " << result.gameResults[i];
            for (size_t j = result.gameResults[i].size(); j < 9; ++j) std::cout << " "; // Align results
        }

        // Fill empty slots for unplayed games
        for (size_t i = result.gameResults.size(); i < numberOfGames; ++i) {
            std::cout << "|          ";
        }

        std::cout << "\n";
    }
}

void GameEngine::endGame() {
    std::cout << "\n\nFORCING END OF GAME FOR TESTING PURPOSES\n\n" << std::endl;
    for (int i = 1; i < players->size(); ++i) {
        for(Territory* territory : (*players)[i]->toDefend()) {
            territory->setOwner((*players)[0]);
            (*players)[0]->addTerritory(territory);
        }

        (*players)[i]->emptyToDefend();
    }
}
