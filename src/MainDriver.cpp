#include <iostream>
#include <string>
#include "./GameEngine/GameEngine.h"
#include "CommandProcessing/CommandProcessing.h"

void testOrderExecution();
void testCards();
void testLoadMaps();
void testPlayers();
void testGameStates();
void testCommandProcessor();
void testMainGameLoop();
void testStartupPhase();
void testLoggingObserver();
void testTournament();

// int main() {
//
//     // testCards();
//     // std::cout << std::endl;
//     //
//     // testPlayers();
//     // std::cout << std::endl;
//     //
//     //
//     // std::cout << std::endl;
//     //
//     // testLoadMaps();
//     //std::cout << std::endl;
//     //
//     // testGameStates();
//     // std::cout << std::endl;
//     // testOrderExecution();
//     // testCommandProcessor();
//     // testStartupPhase();
//     // testMainGameLoop();
//      testLoggingObserver();
//
//     return 0;
// }

int main(int argc, char* argv[]) {
    if (argc > 1 && std::string(argv[1]) == "tournament") {
        // Combine all command-line arguments into a single string
        std::string command;
        for (int i = 1; i < argc; ++i) {
            command += std::string(argv[i]) + " ";
        }
        command.pop_back(); // Remove the trailing space


        // Validate the tournament command
        TournamentParameters params;
        CommandProcessor cp{};
        if (cp.validateTournamentCommand(command, params)) {
            std::cout << "Tournament command is valid. Starting tournament...\n";

            // Execute the tournament
            GameEngine engine;
            engine.playTournament(params);
        } else {
            std::cout << "Invalid tournament command. Please check your input.\n";
        }
    } else if (argc > 1 && std::string(argv[1]) == "-test") {
        testTournament();
    } else {
        std::cout << "Please provide a valid tournament command or run the program with --test.\n";
    }

    return 0;
}
