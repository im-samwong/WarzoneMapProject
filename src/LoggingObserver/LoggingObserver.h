#ifndef LOGGING_OBSERVER_H
#define LOGGING_OBSERVER_H

#include <string>
#include <list>
#include <fstream>

// ILoggable Interface
class ILoggable {
public:
    virtual std::string stringToLog() const = 0;
    virtual ~ILoggable() = default;
    virtual std::string nameI() const;
};

// Observer Interface
class Observer {
    public:
        virtual void update(ILoggable* loggable) = 0;
        virtual void closeFile() = 0;
        virtual ~Observer() = default;
};

// LogObserver Class
class LogObserver : public Observer {
    public:
        LogObserver(const std::string& filename);
        LogObserver(const LogObserver& other);  // Copy constructor
        LogObserver& operator=(const LogObserver& other);  // Assignment operator
        ~LogObserver() override;  // Destructor

        void update(ILoggable* loggable) override;
        void closeFile() override;

    private:
        std::string* fileName;
        std::ofstream* logfile;
};

// Subject Class
class Subject {
    public:
        Subject();
        Subject(const Subject& other);  // Copy constructor

        virtual std::string nameS() const;

        Subject& operator=(const Subject& other);  // Assignment operator
        virtual ~Subject();  // Destructor

        void attach(Observer* obs);
        void detach(Observer* obs);
        void notify(ILoggable* loggable);

    private:
        std::list<Observer*>* observers;
};

#endif // LOGGING_OBSERVER_H
