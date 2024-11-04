#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "../CommandProcessing/CommandProcessing.h"
#include "../Map/Map.h"
#include "../Player/Player.h"
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <random>

//Enum representing the possible game states of the game i.e. states on the transition diagram
enum GameStates {
    START = 0,
    MAP_LOADED = 1,
    MAP_VALIDATED = 2,
    PLAYERS_ADDED = 3,
    ASSIGN_REINFORCEMENT = 4,
    ISSUE_ORDERS = 5,
    EXECUTE_ORDERS = 6,
    WIN = 7
};

// Enum representing the list of possible transition commands for each state
enum class TransitionCommand {
    LOAD_MAP = 0,
    VALIDATE_MAP = 1,
    ADD_PLAYER = 2,
    ASSIGN_COUNTRIES = 3,
    ISSUE_ORDER = 4,
    END_ISSUE_ORDERS = 5,
    EXECUTE_ORDER = 6,
    END_EXEC_ORDER = 7,
    WIN_GAME = 8,
    PLAY_AGAIN = 9,
    END = 10,
    INVALID = 11
};

// Extra enum for the 2 types of states
enum class GameStateTypes {
    STARTUP,
    PLAY
};

const int MAX_PLAYERS = 6;
const int MIN_PLAYERS = 2;

/*********************** Utility Functions start here ***********************/

// Helper method for mapping a TransitionCommand enum value to a String
std::string mapEnumToString(TransitionCommand command);

// Helper method from mapping a String to a corresponding Transition Command
TransitionCommand mapStringToTransitionCommand(const std::string& transitionCommand);

// Method that fetches all TransitionCommands as Strings
std::vector<std::string> getStringTransitionCommands();

// Method that fetches TransitionCommands as Strings but based on some input std::vector
std::vector<std::string> getStringTransitionCommands(const std::vector<TransitionCommand>& gameStateCommands);

// Helper function for mapping any GameStates enum value to a std::string
std::string mapEnumToString(GameStates stateName);

/*
    Abstract class that represents all possible states in the game. Consists entirely of pointer members
    Each possible state of the game is derived from this class with each specific state implementing their own
    transition logic
 */
class GameState {
protected:
    GameStateTypes* stateType;
    GameStates* stateName;
    std::vector<TransitionCommand>* transitionCommands;
    std::vector<GameState*>* nextStates{};

public:
    virtual ~GameState() = default;

    // Default Constructor given all necessary members
    GameState(const GameStateTypes& iStateType, const GameStates& iStateName, const std::vector<TransitionCommand>& iTransitionCommands, const std::vector<GameState*> &iNextStates);

    // Constructor used in case we don't want to declare the next states immediately
    GameState(const GameStateTypes& iStateType, const GameStates& iStateName, const std::vector<TransitionCommand>& iTransitionCommands);

    // Copy Constructor for Base class which will be used by derived classes as well
    GameState(const GameState& otherGameState);

    friend std::ostream& operator<<(std::ostream& os, const GameState& gameState);

    // Getter for getting the state type i.e a value of the enum GameStateTypes
    GameStateTypes getStateType();

    // Getter that obtains this GameState's state name i.e a value from the enum GameStates
    GameStates getStateName();

    // Getter for obtaining the nextStates available for this GameState
    std::vector<GameState*> getNextStates();

    // Getter for obtaining the list of TransitionCommands
    std::vector<TransitionCommand> getTransitionCommands();

    // Method for determining if GameState will transition to next state.
    // Returns a pointer to the next state if transition is successful or nullptr if a transition is not to take place
    virtual GameState* transitionToNextState(TransitionCommand transitionCommand) = 0;

    // Setter method for setting nextStates, used for initialization
    void setNextStates(const std::vector<GameState*> &nextStates);

    GameState& operator=(const GameState& otherGameState);
};

class StartState : public GameState {
public:
    StartState(
        const GameStateTypes& stateType,
        const GameStates& stateName,
        const std::vector<TransitionCommand>& transitionCommands,
        const std::vector<GameState*>& nextStates);

    StartState(
        const GameStateTypes& stateType,
        const GameStates& stateName,
        const std::vector<TransitionCommand>& transitionCommands);

    GameState* transitionToNextState(TransitionCommand transitionCommand) override;
};

class MapLoadedState : public GameState {
public:
    MapLoadedState(
        const GameStateTypes& stateType,
        const GameStates& stateName,
        const std::vector<TransitionCommand>& transitionCommands,
        const std::vector<GameState*>& nextStates);

    MapLoadedState(
        const GameStateTypes& stateType,
        const GameStates& stateName,
        const std::vector<TransitionCommand>& transitionCommands);

    GameState* transitionToNextState(TransitionCommand transitionCommand) override;
};

class MapValidatedState : public GameState {
public:
    MapValidatedState(
        const GameStateTypes& stateType,
        const GameStates& stateName,
        const std::vector<TransitionCommand>& transitionCommands,
        const std::vector<GameState*>& nextStates);

    MapValidatedState(
        const GameStateTypes& stateType,
        const GameStates& stateName,
        const std::vector<TransitionCommand>& transitionCommands);

    GameState* transitionToNextState(TransitionCommand transitionCommand) override;
};

class PlayersAddedState : public GameState {
public:
    PlayersAddedState(
        const GameStateTypes& stateType,
        const GameStates& stateName,
        const std::vector<TransitionCommand>& transitionCommands,
        const std::vector<GameState*>& nextStates);

    PlayersAddedState(
        const GameStateTypes& stateType,
        const GameStates& stateName,
        const std::vector<TransitionCommand>& transitionCommands);

    GameState* transitionToNextState(TransitionCommand transitionCommand) override;
};

class AssignReinforcementState : public GameState {
public:
    AssignReinforcementState(
        const GameStateTypes& stateType,
        const GameStates& stateName,
        const std::vector<TransitionCommand>& transitionCommands,
        const std::vector<GameState*>& nextStates);

    AssignReinforcementState(
        const GameStateTypes& stateType,
        const GameStates& stateName,
        const std::vector<TransitionCommand>& transitionCommands);

    GameState* transitionToNextState(TransitionCommand transitionCommand) override;
};

class IssueOrdersState : public GameState {
public:
    IssueOrdersState(
        const GameStateTypes& stateType,
        const GameStates& stateName,
        const std::vector<TransitionCommand>& transitionCommands,
        const std::vector<GameState*>& nextStates);

    IssueOrdersState(
        const GameStateTypes& stateType,
        const GameStates& stateName,
        const std::vector<TransitionCommand>& transitionCommands);

    GameState* transitionToNextState(TransitionCommand transitionCommand) override;
};

class ExecuteOrdersState : public GameState {
public:
    ExecuteOrdersState(
        const GameStateTypes& stateType,
        const GameStates& stateName,
        const std::vector<TransitionCommand>& transitionCommands,
        const std::vector<GameState*>& nextStates);

    ExecuteOrdersState(
        const GameStateTypes& stateType,
        const GameStates& stateName,
        const std::vector<TransitionCommand>& transitionCommands);

    GameState* transitionToNextState(TransitionCommand transitionCommand) override;
};

class WinState : public GameState {
public:
    WinState(
        const GameStateTypes& stateType,
        const GameStates& stateName,
        const std::vector<TransitionCommand>& transitionCommands,
        const std::vector<GameState*>& nextStates);

    WinState(
        const GameStateTypes& stateType,
        const GameStates& stateName,
        const std::vector<TransitionCommand>& transitionCommands);

    GameState* transitionToNextState(TransitionCommand transitionCommand) override;
};

//Game Engine declarations

/*
    GameEngine class implemented following the Singleton design pattern since logically it makes sense to only have 1 GameEngine object
    Provides several accessors to get its internal state as well as mutators to update its state and a method to obtain(create as well if non-existent) an instance of this class.
 */
class GameEngine {

    static GameEngine* game_engine_instance;
    bool* gameOver;
    GameState* currentGameState;
    std::string* inputtedCommand;
    std::vector<Player*>* players; //vector containing pointers to all player object initialized for the game.
    Map* map; //stores the map for the game
    CommandProcessor* cp; //to get commands from the command line or a file.
    Deck* deck; //The main deck which players draw from

    GameEngine() { //Consturctor
        gameOver = nullptr;
        currentGameState = nullptr;
        inputtedCommand = nullptr;
        players = new std::vector<Player*>();
        deck = new Deck();
        cp = new CommandProcessor();
    };

public:
    ~GameEngine();

    // Gets the enum value representing the name of the current game state
    GameStates getCurrentGameStateName() const;

    // Getter for obtaining the current GameState object
    GameState* getCurrentGameState() const;

    //Gets the current game over status of the game as a boolean
    bool getGameOverStatus() const;

    //Gets the inputted command as a std::string
    std::string getInputtedCommand() const;

    //Mutator for setting the currentGameState
    void setCurrentGameState(GameState* gameState);

    //Mutator for setting the gameOverStatus
    void setGameOverStatus(bool gameOver);

    //Mutator for setting the inputtedCommand
    void setInputtedCommand(const std::string &inputtedCommand);

    // Handles transitioning to next state. Returns true if successfully transitioning or false otherwise
    bool transitionToNextState(TransitionCommand transitionCommand);

    // Helper function for printing out the possible commands of the current state
    void printCurrentStateCommands(const std::vector<TransitionCommand>& commands, const std::string& gameStateName);

    static GameEngine* getInstance();

    //helper method to add player to game
    void addPlayer(const std::string& playerName);

    //tells the GameEngine to take commands from a file instead
    void readInputFromFile(const std:: string& filename); 

    bool startupPhase();

    //helper method to distribute territories to players at the start of the game
    void distrubuteTerritories(); 

    //helper method to randomly determine play order by shuffling the players vector.
    void shufflePlayers(); 

    friend std::ostream& operator<<(std::ostream& os, const GameEngine& gameEngine);
};
#endif //GAMEENGINE_H