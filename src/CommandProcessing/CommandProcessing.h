//
// Created by Binal Patel on 2024-10-15.
//
#ifndef COMMANDPROCESSING_H
#define COMMANDPROCESSING_H

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>


class Command {
public:
    //Constructors
    Command(const std::string& cmd, const std::string& arg);
    //Methods
    void saveEffect(const std::string& effect);
    std::string getCommand() const {return command;}
    std::string getEffect() const {return effect;}
    std::string getArgument() const {return argument;}

private:
    //Attributes
    std::string command;
    std::string effect;
    std::string argument;
};


class CommandProcessor {
public:
    //Constructor
    CommandProcessor() : currentGameState("start") {}
    //Methods
    void getCommand(const std::string& cmd);
    bool validate(Command& command);
    Command getLastCommand() const;

private:
    //Methods
    void readCommand(const std::string& cmd);
    void saveCommand(const Command& command);
    void updateGameState(const std::string& command);
    //Attributes
    std::string currentGameState;
    std::vector<std::unique_ptr<Command>> lc; //stores collection of command objects
};


class FileLineReader {
public:
    //Constructor
    FileLineReader(const std::string& fileName);
    //Method
    bool readLineFromFile(std::string& line);

//private:
    std::ifstream file;
};


class FileCommandProcessorAdapter : public CommandProcessor {
public:
    //Constructor
    FileCommandProcessorAdapter(const std::string& fileName);
    //Method
    void readCommand();

private:
    FileLineReader flr;
};

#endif // COMMANDPROCESSING_H