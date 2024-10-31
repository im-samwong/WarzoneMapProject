//
// Created by Binal Patel on 2024-10-15.
//
#include "../CommandProcessing/CommandProcessing.h"
#include <iostream>
#include <string>

void testCommandProcessor() {
    CommandProcessor consoleProcessor;

    std::string command;
    std::cout << "Welcome to Warzone!" << std::endl;
    std::cout << "If you would like to play using the command line, type 0. (i.e. 0)" << std::endl;
    std::cout << "If you would like to play using a file of commands, type 1. (i.e. 1)" << std::endl;
    std::string playMethod;
    std::getline(std::cin, playMethod);
    if (playMethod == "0")
    {
        std::cout << "Enter a command (i.e. loadmap fileName.map): " << std::endl;
        while (std::getline(std::cin, command)) {
            if (command == "quit") {
                break;
            }
            consoleProcessor.getCommand(command);
            std::cout << "Effect of the command: " << consoleProcessor.getLastCommand().getEffect() << std::endl;
            std::cout << "Enter a command (or type 'quit' to end the game): " << std::endl;
        }
    }
    else {
        // Reading commands from a file
        FileCommandProcessorAdapter fileProcessor("commands.txt");
        fileProcessor.readCommand();
        std::cout << "Commands successfully read from file." << std::endl;
    }
}