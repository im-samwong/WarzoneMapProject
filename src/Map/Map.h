#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector> 
#include <cctype> //for std::tolower
#include <unordered_set>
#include <stack>
using namespace std;

const int MAX_NEIGHBORS = 10;
const int MAX_TERRITORIES = 255;

class Territory;
class Continent;
class Map;

class Territory {
public:
    Territory(); // Default Constructor
    Territory(const string &n,
              int xCoord,
              int yCoord,
              Continent* c);             // Constructor with arguments
    Territory(const Territory &other); // Copy constructor
    Territory& operator=(const Territory& other);  // Assignment operator
    ~Territory();                      // Destructor

    //Accessor functions:

    int getArmies();
    int getX();
    int getY();
    string getName();
    Continent* getContinent() const;
    const vector<Territory*>* getNeighbors() const;
    //Methods to modify armies and add neighbors:
    void addNeighbor(Territory *neighbor);
    void modifyArmies(int a);

    //Stream insertion operator:
    friend ostream& operator<<(ostream& os, const Territory& territory);
private:
    string *name;
    int *x;
    int *y;
    int *armies;
    string* owner;
    Continent* continent;
    vector<Territory*>* neighbors;
};

class Continent {
public:

    // Constructors and Destructor:
    Continent();
    Continent(const string &n, int c);
    Continent(const Continent &other);            // Copy constructor
    Continent& operator=(const Continent& other);  // Assignment operator
    ~Continent();                                 // Destructor

    //Accessor functions:
    string getName();
    int getControlValue();
    const vector<Territory*>* getTerritories() const;

    //Function to link territories to continents:
    void addTerritory(Territory* territory); 

    // Stream insertion operator
    friend ostream &operator<<(ostream &os, const Continent &continent);

private:
    string* name;
    int *controlValue;
    vector<Territory*>* territories;

};

class Map { //Implements a map in a graph. Owns all territories and continents.
public:

    
    Map();
    Map(const string& authorName, const string& imageFile, bool scrollType, bool warn, bool wrap);
    Map(const Map &other); // Copy Constructor
    Map &operator=(const Map &other); // Assignment Operator
    ~Map(); //Deconstructor, deletes map and corresponding Territories and Continents.

    //Accessor functions:
    string getAuthor();
    string getImagePath();
    string getScroll();
    bool getWarn();
    bool getWrap();

    // Find a territory by name (case-insensitive)
    Territory* getTerritory(const string& name) const;

    Continent* getContinent(const string& name) const;

    // Add links to territories and continents in the map
    void addTerritory(Territory* territory);
    void addContinent(Continent* continent);
    bool validate(); // function to check that the Map object follows all the rules

    friend ostream& operator<<(ostream& os, const Map& map);

private:
    // Attributes from map file
    string *author; 
    string *image; //path to map graphics
    bool* scroll; //True for vertical, False for Horizontal
    bool *warn; //warn users of errors in map
    bool *wrap; 

    unordered_map<string, Territory*>* territories; //map of all territories
    unordered_map<string, Continent*>* continents; //map of all continents
    //Helper functions:

    bool validateGraph(); //validate that map is a graph
    bool validateContinents(); //validate that continents are a connected subgraph through territories
    bool validateTerritories(); //validate that territories are only connected to their proper continents.
    bool caseInsensitiveCompare(const string& str1, const string& str2) const; //compares strings ignoring of case 
    void dfs(const Territory* territory, unordered_set<const Territory*>& visited);
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