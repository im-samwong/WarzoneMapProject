//
// Created by Binal Patel on 2024-10-15.
//
#include "../CommandProcessing/CommandProcessing.h"
#include <iostream>
#include <string>

void testCommandProcessor() {
    CommandProcessor consoleProcessor;

    std::string currentGameState = "start";

    std::cout << "Welcome to the Warzone!" << std::endl;
    std::cout << "If you would like to play using the command line, type 0. (i.e. 0)" << std::endl;
    std::cout << "If you would like to play using a file of commands, type 1. (i.e. 1)" << std::endl;

    std::string inputType;
    std::string command;
    std::getline(std::cin, inputType);

    if (inputType == "0") { //reading from command line
        std::cout << "INPUT 0 CHOSEN: Reading from command line " << "\n" << std::endl;
        std::cout << "Enter a command (i.e. loadmap fileName.map): " << std::endl;
        while (true) {
            Command* cmd = consoleProcessor.getCommand();
            if (cmd->getCommand() == "quit") {
                break;
            }
            std::cout << "Current game state: " << currentGameState << "\n" << std::endl;
            //validating the command
            if (consoleProcessor.validate(*cmd, currentGameState)) {
                cmd->saveEffect("The command '" + cmd->getCommand() + "' is valid!");
                std::cout << "Effect: " << cmd->getEffect() << std::endl;

                //update state based on command
                if (cmd->getCommand() == "loadmap") {
                    currentGameState = "maploaded";
                } else if (cmd->getCommand() == "validatemap") {
                    currentGameState = "mapvalidated";
                } else if (cmd->getCommand() == "addplayer") {
                    currentGameState = "playersadded";
                } else if (cmd->getCommand() == "gamestart") {
                    currentGameState = "assignreinforcement";
                } else if (cmd->getCommand() == "issueorder") {
                    currentGameState = "issueorders";
                } else if (cmd->getCommand() == "issueordersend") {
                    currentGameState = "executeorders";
                } else if (cmd->getCommand() == "execorder") {
                    currentGameState = "executeorders";
                } else if (cmd->getCommand() == "endexecorders") {
                    currentGameState = "assignreinforcement";
                } else if (cmd->getCommand() == "win") {
                    currentGameState = "win";
                } else if (cmd->getCommand() == "replay") {
                    currentGameState = "start";
                }

                std::cout << "Current game state: " << currentGameState << "\n" << std::endl;
            } else {
                std::cout << "The command '"+ cmd->getCommand() +"' is invalid for the game state '" << currentGameState << "'\n" << std::endl;
            }
        }
    } else if (inputType == "1") { //reading from file
        std::cout << "INPUT 1 CHOSEN: Reading from file" << "\n" << std::endl;

            FileCommandProcessorAdapter fileProcessor("../commands.txt");
            currentGameState = "start";
            //reading commands from file
            while (true) {
                Command* cmd = fileProcessor.getCommand();
                if (cmd->getCommand() == "") {
                    break;  //reached the end of the file
                }

                //validating commands
                if (fileProcessor.validate(*cmd, currentGameState)) { // issue here
                    cmd->saveEffect("The command '" + cmd->getCommand() + "' is valid!");
                    std::cout << "Effect: " << cmd->getEffect() << std::endl;

                    //update state based on command
                    if (cmd->getCommand() == "loadmap") {
                        currentGameState = "maploaded";
                    } else if (cmd->getCommand() == "validatemap") {
                        currentGameState = "mapvalidated";
                    } else if (cmd->getCommand() == "addplayer") {
                        currentGameState = "playersadded";
                    } else if (cmd->getCommand() == "gamestart") {
                        currentGameState = "assignreinforcement";
                    } else if (cmd->getCommand() == "issueorder") {
                        currentGameState = "issueorders";
                    } else if (cmd->getCommand() == "issueordersend") {
                        currentGameState = "executeorders";
                    } else if (cmd->getCommand() == "execorder") {
                        currentGameState = "executeorders";
                    } else if (cmd->getCommand() == "endexecorders") {
                        currentGameState = "assignreinforcement";
                    } else if (cmd->getCommand() == "win") {
                        currentGameState = "win";
                    } else if (cmd->getCommand() == "replay") {
                        currentGameState = "start";
                    }

                    std::cout << "Current game state: " << currentGameState << "\n" << std::endl;
                } else {
                    std::cout << "Command: " << cmd->getCommand() << std::endl; //debug
                    std::cout << "Invalid command for the game state: '" << currentGameState << "'\n" << std::endl;
                }
            }

    } else {
        std::cout << "Invalid input!" << std::endl;
    }
}