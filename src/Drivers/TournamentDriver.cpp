//
// Created by samue on 2024-11-20.
//
#include "../CommandProcessing/CommandProcessing.h"
#include "../GameEngine/GameEngine.h"

void testTournament() {
    std::string command = "tournament -M World.map -P Aggressive,Neutral -G 1 -D 30";
    TournamentParameters params;

    CommandProcessor cmd{};

    if (cmd.validateTournamentCommand(command, params)) {
        GameEngine engine;
        engine.playTournament(params);
    } else {
        std::cout << "Tournament command validation failed.\n";
    }
}
