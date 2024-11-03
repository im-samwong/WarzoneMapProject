#include "CommandProcessing.h"
#include <vector>
#include <string>
#include <iostream>

Command::Command(std::string& cmd) {
    command = new std::string(cmd);
    argument = nullptr;
    effect = nullptr;
}

// Command class definition
Command::Command(const std::string& cmd, const std::string& arg) {
    command = new std::string(cmd);
    argument = new std::string(arg);
    effect = nullptr;
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
    effect = new std::string(eff);
}

std::string Command::getArgument() const {
    if(argument==nullptr){
        return "";
    } else {
        return *argument;
    }
}

//CommandProcessor class definition
CommandProcessor::CommandProcessor() {}

CommandProcessor::~CommandProcessor() {
    for (Command* cmd : lc) {
        delete cmd;
    }
}

Command* CommandProcessor::getCommand() {

    readCommand(); //reads a command from user input and stores it in the list of commmands.
    return getLastCommand();
}

Command* CommandProcessor::getLastCommand() const {
    return lc.back(); //returns last command
}

bool CommandProcessor::validate(Command& command, std::string& currentGameState) {
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

    const auto& validCommandsForCurrentState = validCommands.find(currentGameState);
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
        command.saveEffect("For the current game state '" + currentGameState + "' the command '" + command.getCommand() + "' is invalid!");
    }

    return isValid;
}

void CommandProcessor::readCommand() {

    std::string* cmd = new std::string();
    std::cout << "Please enter command:\n";
    std::cin >> *cmd;
    std::string* command;
    std::string* argument;

    std::size_t space = cmd->find(' ');
    if (space != std::string::npos) {
        *command = cmd->substr(0, cmd->find(" "));
        *argument = cmd->substr(cmd->find(" ") + 1);
        saveCommand(command,argument);
    } else { // for commands that are not loadmap or addplayer
        command = cmd;
        saveCommand(command); 
    }
}

void CommandProcessor::saveCommand(std::string* command, std::string* argument) {

    if(argument == nullptr) {
        lc.push_back(new Command(*command));
    } else {
        lc.push_back(new Command(*command, *argument));
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

std::string FileLineReader::readLine() {

    std::string line;
    std::getline(*file, line);
    return line;
}


//FileCommandProcessorAdapter class definition
FileCommandProcessorAdapter::FileCommandProcessorAdapter(const std::string& fileName) {
    flr = new FileLineReader(fileName);
}

FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
    delete flr;
}

void FileCommandProcessorAdapter::readCommand() {
    
    std::cout << "Getting command from file\n";
    
    std::string* cmd = new std::string(flr->readLine());
    std::string* command;
    std::string* argument;

    std::size_t space = cmd->find(' ');
    if (space != std::string::npos) {
        *command = cmd->substr(0, cmd->find(" "));
        *argument = cmd->substr(cmd->find(" ") + 1);
        saveCommand(command,argument);
    } else { // for commands that are not loadmap or addplayer
        command = cmd;
        saveCommand(command); 
    }
}
