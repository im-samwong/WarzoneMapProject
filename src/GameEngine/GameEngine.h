//
// Created by Daniel on 2024-09-29.
//

#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <memory>
#include <string>
#include <vector>

using namespace std;

namespace GameEngine {
    /*
      Enum representing the possible game states of the game i.e states on the transition diagram
     */
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

    //Enum representing the list of possible transition commands for each state
    enum class TransitionCommand {
        LOAD_MAP = 1,
        VALIDATE_MAP = 2,
        ADD_PLAYER = 3,
        ASSIGN_COUNTRIES = 4,
        ISSUE_ORDER = 5,
        END_ISSUE_ORDERS = 7,
        EXECUTE_ORDER = 6,
        END_EXEC_ORDER = 8,
        WIN_GAME = 9,
        PLAY_AGAIN = 10,
        END = 11,
        INVALID = 12
    };

    //Extra enum for the 2 types of states
    enum class GameStateTypes {
        STARTUP,
        PLAY
    };

    //Helper method for mapping a TransitionCommand enum value to a String
    string mapEnumToString(TransitionCommand command);

    //Helper method from mapping a String to a corresponding Transition Command
    TransitionCommand mapStringToTransitionCommand(const string &transitionCommand);

    //Method that fetches all TransitionCommands as Strings
    vector<string> getStringTransitionCommands();

    //Method that fetches TransitionCommands as Strings but based on some input vector
    vector<string> getStringTransitionCommands(const vector<TransitionCommand> &gameStateCommands);

    //Helper function for mapping any GameStates enum value to a string
    string mapEnumToString(GameStates stateName);

    /*
        Abstract class that represents all possible states in the game. Consists entirely of pointer members
        Each possible state of the game is derived from this class with each specific state implementing their own
        transition logic
     */
    class GameState {
        protected:
            shared_ptr<GameStateTypes> stateType;
            shared_ptr<GameStates> stateName;
            shared_ptr<vector<TransitionCommand>> transitionCommands;
            shared_ptr<vector<shared_ptr<GameState>>> nextStates;

        public:
            virtual ~GameState() = default;

            //Default Constructor given all necessary members
            GameState(const GameStateTypes &stateType, const GameStates &stateName, const vector<TransitionCommand> &transitionCommands, const vector<shared_ptr<GameState>> &nextStates);

            //Constructor used in case we don't want to declare the next states immediately
            GameState(const GameStateTypes &stateType, const GameStates &stateName, const vector<TransitionCommand> &transitionCommands);

            //Copy Constructor for Base class which will be used by derived classes as well
            GameState(const GameState &otherGameState);

            friend ostream &operator<<(ostream &os, const GameState &gameState);

            //Getter for getting the state type i.e a value of the enum GameStateTypes
            GameStateTypes getStateType();

            //Getter that obtains this GameState's state name i.e a value from the enum GameStates
            GameStates getStateName();

            //Getter for obtaining the nextStates available for this GameState
            vector<shared_ptr<GameState>> getNextStates();

            //Getter for obtaining the list of TransitionCommands
            vector<TransitionCommand> getTransitionCommands();

            //Method for determining if GameState will transition to next state.
            //Returns a pointer to the next state if transition is successful or nullptr if a transition is not to take place
            virtual shared_ptr<GameState> transitionToNextState(TransitionCommand transitionCommand) = 0;

            //Setter method for setting nextStates, used for initialization
            void setNextStates(const shared_ptr<vector<shared_ptr<GameState>>> &nextStates);

            GameState& operator=(const GameState &otherGameState);
    };

    class StartState: public GameState {
        public:
            StartState(
                const GameStateTypes &stateType,
                const GameStates &stateName,
                const vector<TransitionCommand> &transitionCommands,
                const vector<shared_ptr<GameState>> &nextStates
            );

            StartState(
                const GameStateTypes &stateType,
                const GameStates &stateName,
                const vector<TransitionCommand> &transitionCommands
            );

            shared_ptr<GameState> transitionToNextState(TransitionCommand transitionCommand) override;
    };

    class MapLoadedState: public GameState {
        public:
            MapLoadedState(
                const GameStateTypes &stateType,
                const GameStates &stateName,
                const vector<TransitionCommand> &transitionCommands,
                const vector<shared_ptr<GameState>> &nextStates
            );

            MapLoadedState(
                const GameStateTypes &stateType,
                const GameStates &stateName,
                const vector<TransitionCommand> &transitionCommands
            );

            shared_ptr<GameState> transitionToNextState(TransitionCommand transitionCommand) override;
    };

    class MapValidatedState: public GameState {
        public:
            MapValidatedState(
                const GameStateTypes &stateType,
                const GameStates &stateName,
                const vector<TransitionCommand> &transitionCommands,
                const vector<shared_ptr<GameState>> &nextStates
            );

            MapValidatedState(
                const GameStateTypes &stateType,
                const GameStates &stateName,
                const vector<TransitionCommand> &transitionCommands
            );
            shared_ptr<GameState> transitionToNextState(TransitionCommand transitionCommand) override;
    };

    class PlayersAddedState: public GameState {
        public:
            PlayersAddedState(
                const GameStateTypes &stateType,
                const GameStates &stateName,
                const vector<TransitionCommand> &transitionCommands,
                const vector<shared_ptr<GameState>> &nextStates
            );

            PlayersAddedState(
                const GameStateTypes &stateType,
                const GameStates &stateName,
                const vector<TransitionCommand> &transitionCommands
            );
            shared_ptr<GameState> transitionToNextState(TransitionCommand transitionCommand) override;
    };

    class AssignReinforcementState: public GameState {
        public:
            AssignReinforcementState(
                const GameStateTypes &stateType,
                const GameStates &stateName,
                const vector<TransitionCommand> &transitionCommands,
                const vector<shared_ptr<GameState>> &nextStates
            );

        AssignReinforcementState(
            const GameStateTypes &stateType,
            const GameStates &stateName,
            const vector<TransitionCommand> &transitionCommands
        );

            shared_ptr<GameState> transitionToNextState(TransitionCommand transitionCommand) override;
    };

    class IssueOrdersState: public GameState {
        public:
            IssueOrdersState(
                const GameStateTypes &stateType,
                const GameStates &stateName,
                const vector<TransitionCommand> &transitionCommands,
                const vector<shared_ptr<GameState>> &nextStates
            );

            IssueOrdersState(
                const GameStateTypes &stateType,
                const GameStates &stateName,
                const vector<TransitionCommand> &transitionCommands
            );
            shared_ptr<GameState> transitionToNextState(TransitionCommand transitionCommand) override;
    };

    class ExecuteOrdersState: public GameState {
        public:
            ExecuteOrdersState(
                const GameStateTypes &stateType,
                const GameStates &stateName,
                const vector<TransitionCommand> &transitionCommands,
                const vector<shared_ptr<GameState>> &nextStates
            );

            ExecuteOrdersState(
                const GameStateTypes &stateType,
                const GameStates &stateName,
                const vector<TransitionCommand> &transitionCommands
            );

            shared_ptr<GameState> transitionToNextState(TransitionCommand transitionCommand) override;
    };

    class WinState: public GameState {
        public:
            WinState(
                const GameStateTypes &stateType,
                const GameStates &stateName,
                const vector<TransitionCommand> &transitionCommands,
                const vector<shared_ptr<GameState>> &nextStates
            );

            WinState(
                const GameStateTypes &stateType,
                const GameStates &stateName,
                const vector<TransitionCommand> &transitionCommands
            );
            shared_ptr<GameState> transitionToNextState(TransitionCommand transitionCommand) override;
    };

    /*
        Handles navigating through the states of the game will also handling command inputs, triggering transitions and ending the game
        Warning: This class should ever only be instantiated once. Logically it makes sense to only have 1 Game Engine at a time
        Avoid trying to create multiple GameEngines unless strictly necessary
     */
    class GameEngine {
        shared_ptr<bool> gameOver;
        shared_ptr<GameState> currentGameState;
        shared_ptr<string> inputtedCommand;

        public:
            explicit GameEngine(const shared_ptr<GameState> &initialGameState, const shared_ptr<bool> &gameOver);

            //Getter for obtaining the current GameState object
            shared_ptr<GameState> getCurrentGameState();

            //Gets the enum value representing the name of the current game state
            GameStates getCurrentGameStateName();

            //Handles transitioning to next state. Returns true if successfully transitioning or false otherwise
            bool transitionToNextState(TransitionCommand transitionCommand);

            //Helper function for printing out the possible commands of the current state
            void printCurrentStateCommands(const vector<TransitionCommand> &commands, const string &gameStateName);

            //Free function for testing the GameEngine class
            void testGameStates();

            bool getGameOverStatus() const;

            string getInputtedCommand() const;

            friend ostream& operator<<(ostream& os, const GameEngine &gameEngine);

            GameEngine& operator=(const GameEngine &otherGameEngine);

    };
}

#endif //GAMEENGINE_H
