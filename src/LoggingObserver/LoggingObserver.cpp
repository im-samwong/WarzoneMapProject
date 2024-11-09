//
// Created by USER on 2024-11-08.
//

#include "LoggingObserver.h"
#include <iostream>

LogObserver* logObserver = nullptr;

// Function to show inheritance
std::string ILoggable::nameI() const {
    return "Derived from ILoggable.";
}

// Constructor to open the log file
LogObserver::LogObserver(const std::string& filename): fileName(new std::string(filename)) {
    logfile = new std::ofstream(filename, std::ios::out | std::ios::trunc);
    if (!logfile->is_open()) {
        std::cerr << "Unable to open file " << filename << std::endl;
    }
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
    std::cout << "Loggable updated" << std::endl;
    if (logfile && logfile->is_open()) {
        std::cout << "Writing to log file: " << loggable->stringToLog() << std::endl; // Debug statement
        *logfile << loggable->stringToLog() << std::endl;
    } else {
        std::cerr << "Log file is not open!" << std::endl; // Debug statement
    }
}

void LogObserver::closeFile() {
    this->logfile->close();
    std::cout << std::endl << "Loggable closed" << std::endl;
}

// Subject constructor
Subject::Subject() {
    observers = new std::list<Observer*>();
}

// Copy constructor (deep copy)
Subject::Subject(const Subject& other) {
    observers = new std::list<Observer*>();
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

        observers = new std::list<Observer*>();
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
    std::cout << "Attaching to Subject: " << typeid(*obs).name()  << std::endl;
    observers->push_back(obs);
}

// Detach an observer
void Subject::detach(Observer* obs) {
    std::cout << "Detaching from Subject: " << typeid(*obs).name() << std::endl;
    observers->remove(obs);
}

// Notify all observers with a loggable event
void Subject::notify(ILoggable* loggable) {
    std::cout << "---------------Calling notify------------------" << std::endl;
    std::list<Observer*>::iterator i = observers->begin();
    for (; i != observers->end(); ++i)
        (*i)->update(loggable);
}