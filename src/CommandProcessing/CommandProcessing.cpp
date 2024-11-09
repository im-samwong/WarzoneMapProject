#include "CommandProcessing.h"
#include <vector>
#include <string>
#include <iostream>

Command::Command(const std::string& cmd) {
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

//copy constructor
Command::Command(const Command& other) {
    command = new std::string(*other.command);
    argument = other.argument ? new std::string(*other.argument) : nullptr;
    effect = other.effect ? new std::string(*other.effect) : nullptr;
}

//destructor
Command::~Command() {
    delete command;
    delete argument;
    delete effect;
}

//assignment operator
Command& Command::operator=(const Command& other) {
    if (this == &other) {
        return *this;
    }

    delete command;
    delete argument;
    delete effect;
    command = new std::string(*other.command);
    argument = other.argument ? new std::string(*other.argument) : nullptr;
    effect = other.effect ? new std::string(*other.effect) : nullptr;

    return *this;
}

//stream insertion operator
std::ostream& operator<<(std::ostream& out, const Command& cmd) {
    out << "Command: " << *cmd.command << "\n Its argument: " << *cmd.argument << "\n Its effect: " << *cmd.effect << std::endl;
    return out;
}

void Command::saveEffect(const std::string& eff) {
    effect = new std::string(eff);

    if (logObserver != nullptr) {
        notify(this);
    }
}

std::string Command::stringToLog() const {
    return this-> getEffect();
}


std::string Command::getEffect() const {
    if(effect==nullptr){
        return "This command has no effect.";
    } else {
        return *effect;
    }
}

std::string Command::getArgument() const {
    if(argument==nullptr){
        return "";
    } else {
        return *argument;
    }
}

//CommandProcessor class definition
CommandProcessor::CommandProcessor() {
    lc = new std::vector<Command*>();
}

//copy constructor
CommandProcessor::CommandProcessor(const CommandProcessor& other) {
    lc = new std::vector<Command*>(); //empty vector
    for (Command* cmd : *other.lc) {
        lc->push_back(new Command(*cmd));
    }
}

//destructor
CommandProcessor::~CommandProcessor() {
    for (Command* cmd : *lc) {
        delete cmd;
    }
    delete lc;
}

//assignment operator
CommandProcessor& CommandProcessor::operator=(const CommandProcessor& other) {
    if (this == &other) {
        return *this;
    }

    for (Command* cmd : *lc) {
        delete cmd;
    }
    lc->clear(); //making it an empty vector
    for (Command* cmd : *other.lc) {
        lc->push_back(new Command(*cmd));
    }

    return *this;
}

//stream insertion operator
std::ostream& operator<<(std::ostream& out, const CommandProcessor& cmdProcessor) {
    out << "CommandProcessor has the following commands:\n";
    for (const Command* cmd : *cmdProcessor.lc) {
        out << *cmd << std::endl;
    }
    return out;
}

Command* CommandProcessor::getCommand() {
    readCommand(); //reads a command from user input and stores it in the list of commmands
    return getLastCommand();
}

Command* CommandProcessor::getLastCommand() const {
    return lc->back(); //returns last command
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
                    std::cout << "The command '"+ validCommand + "' requires an argument" << std::endl;
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
    std::string cmd;
    std::cout << "Please enter command:\n";
    std::getline(std::cin, cmd);
    std::string command;
    std::string argument;

    std::size_t space = cmd.find(' ');
    if (space != std::string::npos) {
        command = cmd.substr(0, cmd.find(" "));
        argument = cmd.substr(cmd.find(" ") + 1);
        saveCommand(command,argument);
    } else { // for commands that are not loadmap or addplayer
        saveCommand(cmd);
    }
}

void CommandProcessor::saveCommand(const std::string& command, const std::string& argument) {
    if(argument.empty()) {
        lc->push_back(new Command(command));
    } else {
        lc->push_back(new Command(command, argument));
    }

    if (logObserver != nullptr) {
        notify(this);
    }
}

std::string CommandProcessor::stringToLog() const {
    Command* toLog = this->getLastCommand();
    if (toLog != nullptr) {
        std::string log = "Current Command: " + toLog->getCommand() + "-----Argument: "
                          + toLog->getArgument() + "-----Effect: " + toLog->getEffect() + ".";
        return log;
    }
    return "No Command has been saved.";
}

//FileLineReader class definition
FileLineReader::FileLineReader(const std::string& fileName) {
    file = new std::ifstream(fileName);
    if (!file->is_open()) {
        throw std::runtime_error("Error! The file '" + fileName + "' could not be opened");
    }
}

//copy constructor
FileLineReader::FileLineReader(const FileLineReader& other) {
    filename = new std::string(*other.filename);
    file = new std::ifstream(*filename);
    if (!file->is_open()) {
        throw std::runtime_error("Error! The file '" + *filename + "' could not be opened in the copy constructor");
    }
}

//destructor
FileLineReader::~FileLineReader() {
    if (file->is_open()) {
        file->close();
    }
    delete file;
}

//assignment operator
FileLineReader& FileLineReader::operator=(const FileLineReader& other) {
    if (this == &other) {
        return *this;
    }

    if (file->is_open()) {
        file->close();
    }
    delete file;

    filename = new std::string(*other.filename);
    file = new std::ifstream(*filename);
    if (!file->is_open()) {
        throw std::runtime_error("Error! The file '" + *filename + "' could not be opened in the assignment operator");
    }

    return *this;
}

//stream insertion operator
std::ostream& operator<<(std::ostream& out, const FileLineReader& fileLR) {
    out << "The file is " << (fileLR.file->is_open() ? "open!" : "closed!") << std::endl;
    return out;
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

//copy constructor
FileCommandProcessorAdapter::FileCommandProcessorAdapter(const FileCommandProcessorAdapter& other)
    : CommandProcessor(other) {
    flr = new FileLineReader(*other.flr);
}

//destructor
FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
    delete flr;
}

//assignment operator
FileCommandProcessorAdapter& FileCommandProcessorAdapter::operator=(const FileCommandProcessorAdapter& other) {
    if (this == &other) {
        return *this;
    }

    CommandProcessor::operator=(other);
    delete flr;
    flr = new FileLineReader(*other.flr);

    return *this;
}

//stream insertion operator
std::ostream& operator<<(std::ostream& out, const FileCommandProcessorAdapter& fileCmdPA) {
    out << "FileLineReader: " << *fileCmdPA.flr << std::endl;
    return out;
}


void FileCommandProcessorAdapter::readCommand() {
    std::cout << "Getting command from file\n";
    std::string cmd = flr->readLine();
    std::string command;
    std::string argument;

    std::size_t space = cmd.find(' ');
    if (space != std::string::npos) {
        command = cmd.substr(0, cmd.find(" "));
        argument = cmd.substr(cmd.find(" ") + 1);
        saveCommand(command,argument);
    } else { //for commands that are not loadmap or addplayer
        command = cmd;
        saveCommand(command); 
    }
}
