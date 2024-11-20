//
// Created by Binal Patel on 2024-10-15.
//
#ifndef COMMANDPROCESSING_H
#define COMMANDPROCESSING_H

# include "../LoggingObserver/LoggingObserver.h"
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

struct TournamentParameters {
    std::vector<std::string> mapFiles;
    std::vector<std::string> playerStrategies;
    int numberOfGames;
    int maxTurns;
};

struct TournamentResult {
    std::string mapName;
    std::vector<std::string> gameResults; // One result per game on this map
};


class Command: public Subject, public ILoggable {
public:
    //Constructors
    Command(const std::string& cmd);
    Command(const std::string& cmd, const std::string& arg);
    //Copy constructor
    Command(const Command& other);
    //Destructor
    ~Command();

    //Assignment operator
    Command& operator=(const Command& other);
    //Stream insertion operator
    friend std::ostream& operator<<(std::ostream& out, const Command& cmd);

    //Methods
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


class CommandProcessor: public Subject, public ILoggable {
public:
    //Constructor
    CommandProcessor();
    //Copy constructor
    CommandProcessor(const CommandProcessor& other);
    //Destructor
    ~CommandProcessor();

    //Assignment operator
    CommandProcessor& operator=(const CommandProcessor& other);
    //Stream insertion operator
    friend std::ostream& operator<<(std::ostream& out, const CommandProcessor& cmdProcessor);

    //Methods
    Command* getCommand();
    std::string stringToLog() const override;
    bool validate(Command& command, std::string& currentGameState); //checks if command is valid given current state of program
    Command* getLastCommand() const;
    void restoreConsoleInput();
    void setInputStream(std::istream* input); // Allow changing the input source dynamically
    bool validateTournamentCommand(const std::string& command, TournamentParameters& params);
    std::vector<std::vector<std::string>> generateTournamentCommands(const TournamentParameters& params);

protected:
    void saveCommand(const std::string& command, const std::string& argument="");

private:
    //Methods
    virtual void readCommand();
    //Attributes
    std::vector<Command*>* lc; //stores collection of command objects
    std::istream* inputStream; // Pointer to the current input source

};


class FileLineReader {
public:
    //Constructor
    FileLineReader(const std::string& fileName);
    //Copy constructor
    FileLineReader(const FileLineReader& other);
    //Destructor
    ~FileLineReader();

    //Assignment operator
    FileLineReader& operator=(const FileLineReader& other);
    //Stream insertion operator
    friend std::ostream& operator<<(std::ostream& out, const FileLineReader& fileLR);

    //Method
    std::string readLine();

private:
    std::ifstream* file;
    std::string* filename;
};


class FileCommandProcessorAdapter : public CommandProcessor {
public:
    //Constructor
    FileCommandProcessorAdapter(const std::string& fileName);
    //Copy constructor
    FileCommandProcessorAdapter(const FileCommandProcessorAdapter& other);
    //Destructor
    ~FileCommandProcessorAdapter();

    //Assignment operator
    FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter& other);
    //Stream insertion operator
    friend std::ostream& operator<<(std::ostream& out, const FileCommandProcessorAdapter& fileCmdPA);

    //Method
    void readCommand() override;

private:
    
    FileLineReader* flr;
};

#endif // COMMANDPROCESSING_H