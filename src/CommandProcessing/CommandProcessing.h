//
// Created by Binal Patel on 2024-10-15.
//
#ifndef COMMANDPROCESSING_H
#define COMMANDPROCESSING_H

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "../LoggingObserver/LoggingObserver.h"

class Command : public Subject, public ILoggable{
public:
    //Constructors
    Command(const std::string& cmd);
    Command(const std::string& cmd, const std::string& arg);
    Command(const Command& other);
    //Destructor
    ~Command();
    // Methods
    void saveEffect(const std::string& effect);
    std::string stringToLog() const override;
    std::string getCommand() const {return *command;}
    std::string getEffect() const;
    std::string getArgument() const;

private:
    //Attributes
    std::string* command;
    std::string* effect;
    std::string* argument;
};


class CommandProcessor : public Subject, public ILoggable{
public:
    //Constructor
    CommandProcessor();
    //Destructor
    ~CommandProcessor();
    //Methods
    Command* getCommand();
    std::string stringToLog() const override;
    
    //check if command is valid given the current state of the program
    bool validate(Command& command, std::string& currentGameState);
    
    Command* getLastCommand() const;

protected:
    void saveCommand(const std::string& command, const std::string& argument="");

private:
    //Methods
    virtual void readCommand();
    //Attributes
    std::vector<Command*>* lc; //stores collection of command objects
};


class FileLineReader {
public:
    //Constructor
    FileLineReader(const std::string& fileName);
    //Destructor
    ~FileLineReader();
    //Method
    std::string readLine();

private:
    std::ifstream* file;
};


class FileCommandProcessorAdapter : public CommandProcessor {
public:
    //Constructor
    FileCommandProcessorAdapter(const std::string& fileName);
    //Destructor
    ~FileCommandProcessorAdapter();
    //Method
    void readCommand() override;

private:
    
    FileLineReader* flr;
};

#endif // COMMANDPROCESSING_H