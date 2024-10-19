#ifndef MAP_H
#define MAP_H

#include <cctype> //for std::tolower
#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

const int MAX_NEIGHBORS = 10;
const int MAX_TERRITORIES = 255;

class Territory;
class Continent;
class Map;
class Player;

class Territory {
public:
    Territory(); // Default Constructor
    Territory(const std::string& n,
              int xCoord,
              int yCoord,
              Continent* c);                      // Constructor with arguments
    Territory(const Territory& other);            // Copy constructor
    Territory& operator=(const Territory& other); // Assignment operator
    ~Territory();                                 // Destructor

    // Accessor functions:

    int getArmies();
    int getX();
    int getY();
    std::string getName();
    Continent* getContinent() const;
    const std::vector<Territory*>* getNeighbors() const;
    Player* getOwner() const; // return pointer to Player that owns the territory, returns nullptr if no owner
    // Methods to modify armies and add neighbors:
    void addNeighbor(Territory* neighbor);
    void modifyArmies(int a);
    void setOwner(Player* newOwner);

    // Stream insertion operator:
    friend std::ostream& operator<<(std::ostream& os, const Territory& territory);

private:
    std::string* name;
    int* x;
    int* y;
    int* armies;
    Player* owner;
    Continent* continent;
    std::vector<Territory*>* neighbors;
};

class Continent {
public:
    // Constructors and Destructor:
    Continent();
    Continent(const std::string& n, int c);
    Continent(const Continent& other);            // Copy constructor
    Continent& operator=(const Continent& other); // Assignment operator
    ~Continent();                                 // Destructor

    // Accessor functions:
    std::string getName();
    int getControlValue();
    const std::vector<Territory*>* getTerritories() const;

    // Function to link territories to continents:
    void addTerritory(Territory* territory);

    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, const Continent& continent);

private:
    std::string* name;
    int* controlValue;
    std::vector<Territory*>* territories;
};

class Map { // Implements a map in a graph. Owns all territories and continents.
public:
    Map();
    Map(const std::string& authorName, const std::string& imageFile, bool scrollType, bool warn, bool wrap);
    Map(const Map& other);            // Copy Constructor
    Map& operator=(const Map& other); // Assignment Operator
    ~Map();                           // Deconstructor, deletes map and corresponding Territories and Continents.

    // Accessor functions:
    std::string getAuthor();
    std::string getImagePath();
    std::string getScroll();
    bool getWarn();
    bool getWrap();

    // Find a territory by name (case-insensitive)
    Territory* getTerritory(const std::string& name) const;

    Continent* getContinent(const std::string& name) const;

    // Add links to territories and continents in the map
    void addTerritory(Territory* territory);
    void addContinent(Continent* continent);
    bool validate(); // function to check that the Map object follows all the rules

    friend std::ostream& operator<<(std::ostream& os, const Map& map);

private:
    // Attributes from map file
    std::string* author;
    std::string* image; // path to map graphics
    bool* scroll;       // True for vertical, False for Horizontal
    bool* warn;         // warn users of errors in map
    bool* wrap;

    std::unordered_map<std::string, Territory*>* territories; // map of all territories
    std::unordered_map<std::string, Continent*>* continents;  // map of all continents
    // Helper functions:

    bool validateGraph();                                                                // validate that map is a graph
    bool validateContinents();                                                           // validate that continents are a connected subgraph through territories
    bool validateTerritories();                                                          // validate that territories are only connected to their proper continents.
    bool caseInsensitiveCompare(const std::string& str1, const std::string& str2) const; // compares std::strings ignoring of case
    void dfs(const Territory* territory, std::unordered_set<const Territory*>& visited);
};

// MapLoader Class: Responsible for loading a .map file and constructing a Map object.
class MapLoader {
public:
    // Constructor
    MapLoader() = default;
    // Load the .map file and create the corresponding Map object.
    Map* loadMap(const std::string& fileName);

private:
    // Helper Functions

    // Process the [Map] section of the file (first section)
    Map* createMap(std::ifstream& file);

    // Process the [Continents] section of the file (second section)
    void createContinents(std::ifstream& file, Map* map);

    // First pass of the [Territories] section: Create territories and link them to continents.
    void createTerritories(const std::string& line, Map* map);

    // Second pass of the [Territories] section: Connect territories to their neighbors.
    void processTerritoriesSectionSecondPass(const std::string& line, Map* map);
};

#endif