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

    enum class GameStateTypes {
        STARTUP,
        PLAY
    };

    class GameState {
        protected:
            shared_ptr<GameStateTypes> stateType;
            shared_ptr<GameStates> stateName;
            shared_ptr<vector<TransitionCommand>> transitionCommands;
            shared_ptr<vector<shared_ptr<GameState>>> nextStates;

        public:
            virtual ~GameState() = default;

            GameStateTypes getStateType();

            GameStates getStateName();

            vector<shared_ptr<GameState>> getNextStates();

            vector<TransitionCommand> getTransitionCommands();

            virtual shared_ptr<GameState> transitionToNextState(TransitionCommand transitionCommand) = 0;

            void setNextStates(const shared_ptr<vector<shared_ptr<GameState>>> &nextStates);
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

    class GameEngine {
        shared_ptr<bool> gameOver;
        shared_ptr<GameState> currentGameState;
        shared_ptr<string> inputtedCommand;

        public:
            explicit GameEngine(const shared_ptr<GameState> &initialGameState, const shared_ptr<bool> &gameOver);

            shared_ptr<GameState> getCurrentGameState();

            GameStates getCurrentGameStateName();

            bool transitionToNextState(TransitionCommand transitionCommand);

            void printCurrentStateCommands(const vector<TransitionCommand> &commands, const string &gameStateName);

            void testGameStates();
    };

    string mapEnumToString(TransitionCommand command);

    TransitionCommand mapStringToTransitionCommand(const string &transitionCommand);

    vector<string> getStringTransitionCommands();

    vector<string> getStringTransitionCommands(const vector<TransitionCommand> &gameStateCommands);

    string mapEnumToString(GameStates stateName);
}

#endif //GAMEENGINE_H
