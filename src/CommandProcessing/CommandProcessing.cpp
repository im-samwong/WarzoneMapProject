//
// Created by Binal Patel on 2024-10-15.
//
#include "CommandProcessing.h"
#include <vector>
#include <string>
#include <iostream>

//Command class definition
Command::Command(const std::string& cmd, const std::string& arg) {
    command = new std::string(cmd);
    argument = new std::string(arg);
    effect = new std::string();
}

Command::Command(const Command& other) {
    command = new std::string(*other.command);
    argument = new std::string(*other.argument);
    effect = new std::string(*other.effect);
}

Command::~Command() {
    delete command;
    delete argument;
    delete effect;
}

void Command::saveEffect(const std::string& eff) {
    *effect = eff;
}


//CommandProcessor class definition
CommandProcessor::CommandProcessor() {
    currentGameState = new std::string("start");
}

CommandProcessor::~CommandProcessor() {
    delete currentGameState;
    for (Command* cmd : lc) {
        delete cmd;
    }
}

void CommandProcessor::getCommand(const std::string& cmd) {
    std::string commandName = cmd.substr(0, cmd.find(" "));
    std::string argument = (cmd.find(" ") != std::string::npos) ? cmd.substr(cmd.find(" ") + 1) : "";

    Command* command = new Command(commandName, argument);

    if (validate(*command)) {
        command->saveEffect("The command '" + commandName + "' is valid!");
        std::cout << "Command's effect: " + command->getEffect() << std::endl;
        saveCommand(*command);

        updateGameState(command->getCommand());
        std::cout << "Current game state: " + *currentGameState + "\n" << std::endl;
    } else {
        command->saveEffect("ERROR! The command '" + commandName + "' is invalid!\n Current game state: " + *currentGameState);
        saveCommand(*command);
    }
}

Command CommandProcessor::getLastCommand() const {
    return *lc.back(); //returns last command
}

bool CommandProcessor::validate(Command& command) {
    static const std::unordered_map<std::string, std::vector<std::string>> validCommands = {
        {"start", {"loadmap"}},
        {"maploaded", {"loadmap", "validatemap"}},
        {"mapvalidated", {"addplayer"}},
        {"playersadded", {"addplayer", "gamestart"}},
        {"assignreinforcement", {"issueorder"}},
        {"issueorders", {"issueorder", "issueordersend"}},
        {"executeorders", {"execorder", "endexecorders", "win"}},
        {"win", {"replay", "quit"}}
    };

    const auto& validCommandsForCurrentState = validCommands.find(*currentGameState);
    bool isValid = false;

    if (validCommandsForCurrentState != validCommands.end()) {
        for (const std::string& validCommand : validCommandsForCurrentState->second) {
            if (command.getCommand() == validCommand) {
                if ((validCommand == "loadmap" || validCommand == "addplayer") && command.getArgument().empty()) {
                    if (validCommand == "loadmap") {
                        std::cout << "The command '"+ validCommand + "' requires a file argument (i.e. loadmap fileName.map)" << std::endl;
                    } else {
                        std::cout << "The command '"+ validCommand + "' requires a name argument (i.e. addplayer playerName)" << std::endl;
                    }

                    isValid = false;
                    break;
                } else {
                    isValid = true;
                    break;
                }
            }
        }
    }

    if (!isValid) {
        command.saveEffect("For the current game state '" + *currentGameState + "' the command '" + command.getCommand() + "' is invalid!");
    }

    return isValid;
}

void CommandProcessor::readCommand(const std::string& cmd) {
    std::string command;
    std::string argument;

    size_t space = cmd.find(' ');
    if (space != std::string::npos) {
        command = cmd.substr(0, cmd.find(" "));
        argument = cmd.substr(cmd.find(" ") + 1);
    } else {
        command = cmd; // for commands that are not loadmap or addplayer
    }

    lc.push_back(new Command(command, argument));
    updateGameState(command);
}

void CommandProcessor::saveCommand(const Command& command) {
    lc.push_back(new Command(command));
}

void CommandProcessor::updateGameState(const std::string& command) {
    if (command.find("loadmap") == 0) {
        *currentGameState = "maploaded";
    }
    else if (command == "validatemap") {
        *currentGameState = "mapvalidated";
    }
    else if (command.find("addplayer") == 0) {
        *currentGameState = "playersadded";
    }
    else if (command == "gamestart") {
        *currentGameState = "assignreinforcement";
    }
    else if (command == "issueorder") {
        *currentGameState = "issueorders";
    }
    else if (command == "issueordersend") {
        *currentGameState = "executeorders";
    }
    else if (command == "execorder") {
        *currentGameState = "executeorders";
    }
    else if (command == "endexecorders") {
        *currentGameState = "assignreinforcement";
    }
    else if (command == "win") {
        *currentGameState = "win";
    }
    else if (command == "replay") {
        *currentGameState = "start";
    }
    else if (command == "quit") {
        *currentGameState = "exit";
    }
}


//FileLineReader class definition
FileLineReader::FileLineReader(const std::string& fileName) {
    file = new std::ifstream(fileName);
    if (!file->is_open()) {
        throw std::runtime_error("Error! The file '" + fileName + "' could not be opened");
    }
}

FileLineReader::~FileLineReader() {
    if (file->is_open()) {
        file->close();
    }
    delete file;
}

bool FileLineReader::readLineFromFile(std::string& line) {
    return std::getline(*file, line) ? true : false;
}


//FileCommandProcessorAdapter class definition
FileCommandProcessorAdapter::FileCommandProcessorAdapter(const std::string& fileName) {
    flr = new FileLineReader(fileName);
}

FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
    delete flr;
}

void FileCommandProcessorAdapter::readCommand() {
    std::string line;
    while (flr->readLineFromFile(line)) {
        getCommand(line);
    }
}
