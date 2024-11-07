#include "LoggingObserver.h"
#include <iostream>

// Function to show inheritance
std::string ILoggable::nameI() const {
    return "Derived from ILoggable.";
}

// Constructor to open the log file
LogObserver::LogObserver(const std::string& filename): fileName(new std::string(filename)) {
    logfile = new std::ofstream(filename, std::ios::out | std::ios::trunc);
}

// Copy constructor (deep copy)
LogObserver::LogObserver(const LogObserver& other): fileName(new std::string(*other.fileName)) {
    if (other.logfile && other.logfile->is_open()) {
        logfile = new std::ofstream(*other.fileName, std::ios::out | std::ios::trunc);
    } else {
        logfile = nullptr;
    }
}

// Assignment operator (deep copy)
LogObserver& LogObserver::operator=(const LogObserver& other) {
    if (this != &other) {
        delete logfile;  // Free existing memory

        if (other.logfile && other.logfile->is_open()) {
            logfile = new std::ofstream(*other.fileName, std::ios::out | std::ios::trunc);
        } else {
            logfile = nullptr;
        }
    }
    return *this;
}

// Destructor to close and clean up the log file
LogObserver::~LogObserver() {
    if (logfile) {
        if (logfile->is_open()) {
            logfile->close();
        }
        delete logfile;
        logfile = nullptr;
        delete fileName;
        fileName = nullptr;
    }
}

// Write to log file when notified
void LogObserver::update(ILoggable* loggable) {
    if (logfile && logfile->is_open()) {
        *logfile << loggable->stringToLog() << std::endl;
    }
}

void LogObserver::closeFile() {
    this->logfile->close();
}

// Subject constructor
Subject::Subject() {
    observers = new std::vector<Observer*>();
}

// Copy constructor (deep copy)
Subject::Subject(const Subject& other) {
    observers = new std::vector<Observer*>();
    for (Observer* obs : *other.observers) {
        observers->push_back(obs);  // Copy pointers (shallow copy)
    }
}

// Function to show inheritance
std::string Subject::nameS() const {
    return "Derived from Subject.";
}


// Assignment operator (deep copy)
Subject& Subject::operator=(const Subject& other) {
    if (this != &other) {
        delete observers;  // Free existing memory

        observers = new std::vector<Observer*>();
        for (Observer* obs : *other.observers) {
            observers->push_back(obs);  // Copy pointers (shallow copy)
        }
    }
    return *this;
}

// Destructor to clean up observers
Subject::~Subject() {
    delete observers;  // Only delete the vector, not the observers themselves
}

// Attach an observer
void Subject::attach(Observer* obs) {
    observers->push_back(obs);
}

// Detach an observer
void Subject::detach(Observer* obs) {
    for (auto it = observers->begin(); it != observers->end(); ++it) {
        if (*it == obs) {
            observers->erase(it);
            break;
        }
    }
}

// Notify all observers with a loggable event
void Subject::notify(ILoggable* loggable) {
    std::cout << "---------------Calling notify------------------" << std::endl;
    for (Observer* obs : *observers) {
        obs->update(loggable);
    }
}
