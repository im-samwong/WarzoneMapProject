//
// Created by samue on 2024-09-12.
//

#include "Map.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Territory class implementation:

// Default constructor, sets all pointers to nullptr:
Territory::Territory() {
    name = nullptr;
    x = nullptr;
    y = nullptr;
    armies = nullptr;
    owner = nullptr;
    neighbors= nullptr;
    continent = nullptr;
}

// Constructor with name and x,y coords. This one will be actually used in the
// game logic.
Territory::Territory(const string &n, int xCoord, int yCoord, Continent *c) {
    name = new string(n);
    x = new int(xCoord);
    y = new int(yCoord);
    armies = new int(0); // territories are created with no armies on them;
    continent = c;
    neighbors = new vector<Territory *>;
}
// Copy Constructor
Territory::Territory(const Territory &other) {
    name = new string(*other.name);
    x = new int(*other.x);
    y = new int(*other.y);
    armies = new int(*other.armies);
    neighbors = new vector<Territory *>(*other.neighbors);
}
Territory &Territory::operator=(const Territory &other) {
    if (this != &other) {
        // Delete existing resources
        delete name;
        delete x;
        delete y;
        delete neighbors;

        // Deep copy the new values
        name = new string(*other.name);
        x = new int(*other.x);
        y = new int(*other.y);
        continent = other.continent; // Just copy the pointer, as we don't own the continent

        neighbors = new vector<Territory *>(*other.neighbors);
    }
    return *this;
}
// Destructor:
Territory::~Territory() {
    delete name;
    delete x;
    delete y;
    delete armies;
    delete neighbors;
    // we don't delete the pointer to the continet, as a continent is owned by the map
}

// Accessor functions

int Territory::getArmies() {
    return *armies;
}

int Territory::getX() {
    return *x;
}

int Territory::getY() {
    return *y;
}

string Territory::getName() {
    return *name;
}

Continent* Territory::getContinent() const{
    return continent;
}

const vector<Territory *> *Territory::getNeighbors() const {
    return neighbors;
}

// Functions to modify a territory:

void Territory::addNeighbor(Territory *territory) {
    if (neighbors->size() < MAX_NEIGHBORS) {
        if (territory == nullptr) {
            return; // Ignore null pointers.
        }

        // Check if the territory already exists in the territories vector
        for (const auto &existingNeighbor : *neighbors) {
            if (existingNeighbor->getName() == territory->getName()) {
                return; // If a territory with the same name exists, do nothing
            }
        }

        // If not found, add the territory to the vector
        neighbors->push_back(territory);
    } else {
        cout << "Max neighbors reached for territory: " << *name << endl;
    }
}

void Territory::modifyArmies(int a) {
    *armies = a + *armies;
}

// Stream insertion operator:
ostream &operator<<(ostream &os, const Territory &territory) {
    os << "Territory: " << *territory.name << " (" << *territory.x << ", " << *territory.y << "), Continent: " << territory.continent->getName() << "\n";
    os << "Neighbors: ";
    const vector<Territory *> *neighbors = territory.getNeighbors();

    if (!neighbors->empty()) { // In a game, a territory should ALWAYS have neighbors, but this check prevents segmentation faults when testing objects without neighbors
        for (size_t i = 0; i < neighbors->size(); ++i) {
            if (neighbors->at(i) != nullptr) {
                os << (neighbors->at(i)->getName());
                if (i < neighbors->size() - 1) {
                    os << ", "; // Add a comma between neighbors
                }
            }
            
        }
    } else {
        os << "None";
    }
    os << "\n";
    return os;
}

// Continent Class Implementation:

// Default constructor to be nice
Continent::Continent() {
    name = nullptr;
    controlValue = nullptr;
    territories = nullptr;
}

// Constructor with arguments
Continent::Continent(const string &n, int c) {
    name = new string(n);
    controlValue = new int(c);
    territories = new vector<Territory *>;
}

// Copy Constructor
Continent::Continent(const Continent &other) {
    name = new string(*other.name);
    controlValue = new int(*other.controlValue);
    territories = new vector<Territory *>(*other.territories);
}

// Assignment Operator
Continent &Continent::operator=(const Continent &other) {
    if (this != &other) {
        // Delete existing resources
        delete name;
        delete controlValue;

        // Deep copy the new values
        name = new string(*other.name);
        controlValue = new int(*other.controlValue);
    }
    return *this;
}

Continent::~Continent() {
    delete name;
    delete controlValue;
    delete territories;
}

string Continent::getName() {
    return *name;
}

int Continent::getControlValue() {
    return *controlValue;
}

const vector<Territory *> *Continent::getTerritories() const {
    return territories;
}

void Continent::addTerritory(Territory *territory) {
    if (territory == nullptr) {
        return; // Ignore null pointers.
    }

    // Check if the territory already exists in the territories vector
    for (const auto& existingTerritory : *territories) {
        if (existingTerritory->getName() == territory->getName()) {
            return; // If a territory with the same name exists, do nothing
        }
    }

    // If not found, add the territory to the vector
    territories->push_back(territory);
}

ostream &operator<<(ostream &os, const Continent &continent) {
    os << "Continent: " << *continent.name << ", Control Value: " << *continent.controlValue << "\n";
    os << "Territories within Continent: ";
    const vector<Territory *> *territories = continent.getTerritories();

    if (!territories->empty()) { // In a game, a territory should ALWAYS have neighbors, but this check prevents segmentation faults when testing objects without neighbors
        for (size_t i = 0; i < territories->size(); ++i) {
            if (territories->at(i) != nullptr) {
                os << (territories->at(i)->getName());
                if (i < territories->size() - 1) {
                    os << ", "; // Add a comma between neighbors
                }
            }
        }
    } else {
        os << "None";
    }
    os << "\n";
    return os;
}



// Map class implementation

//Default constructor:
Map::Map() {
    author = nullptr;
    image = nullptr;
    scroll = nullptr;
    warn = nullptr;
    wrap = nullptr;
    territories = nullptr;
    continents = nullptr;
}

//Constructor with Class Member arguments:
Map::Map(const string &authorName, const string &imageFile, bool scrollType, bool warn, bool wrap) {

    author = new string(authorName);
    image = new string(imageFile);
    scroll = new bool(scrollType);
    this->warn = new bool(warn);
    this->wrap = new bool(wrap);
    territories = new unordered_map<string, Territory*>();
    continents = new unordered_map<string, Continent*>();
}

Map::Map(const Map &other) {

    //Deep copy class members:
    author = new string(*other.author);
    image = new string(*other.image);
    scroll = new bool(*other.scroll);
    warn = new bool(*other.warn);
    wrap = new bool(*other.wrap);

    //create new vectors to deep copy the continents and territories:
    continents = new unordered_map<string, Continent *>();
    territories = new unordered_map<string, Territory *>();
    // Deep copy continents
    for (const auto &pair : *(other.continents)) {
        continents->emplace(pair.first, new Continent(*(pair.second))); // Deep copy each Continent
    }

    // Deep copy territories
    for (const auto &pair : *(other.territories)) {
        territories->emplace(pair.first, new Territory(*(pair.second))); // Deep copy each Territory
    }
}

Map &Map::operator=(const Map &other) { //Assignment operator
    if (this != &other) { //no need to do all the below on self assignment :)

        // Delete existing dynamic memory
        delete this->author;
        delete this->image;
        delete this->scroll;
        delete this->warn;
        delete this->wrap;

        //make sure all territories and continents are also deleted:

        for (auto &pair : *this->territories) {
            delete pair.second;
        }
        delete this->territories;

        for (auto &pair : *this->continents) {
            delete pair.second;
        }
        delete this->continents;

        // Deep copy of primitive data members
        this->author = new string(*(other.author));
        this->image = new string(*(other.image));
        this->scroll = new bool(*(other.scroll));
        this->warn = new bool(*(other.warn));
        this->wrap = new bool(*(other.wrap));

        // Deep copy of territories
        this->territories = new unordered_map<string, Territory *>();
        for (const auto &pair : *(other.territories)) {
            this->territories->emplace(pair.first, new Territory(*(pair.second)));
        }

        // Deep copy of continents
        this->continents = new unordered_map<string, Continent *>();
        for (const auto &pair : *(other.continents)) {
            this->continents->emplace(pair.first, new Continent(*(pair.second)));
        }
    }
    // finally return a pointer to the created object
    return *this;
}

Map::~Map() {
    // As the map object is the one that will own all territories and continets
    // the destructor should loop through all the territories and Continents and delete them
    // This will avoid memory leaks.
    // For continents:
    if (continents != nullptr) { //to prevent segfault that would happen if we try to acccess an index on a nullptr
        for (auto &pair : *continents) {
            delete pair.second;
        }
    delete continents; // Delete the container itself. Calling this on a nullptr doesn't matter
    }
    //Same thing for territories:
    if (territories != nullptr) {
        for (auto &pair : *territories) {
            delete pair.second;
        }
    delete territories;
    }
    //delete the rest of the class members:
    delete author;
    delete image;
    delete scroll;
    delete warn;
    delete wrap;
}

string Map::getAuthor() {
    return *author;
}

string Map::getImagePath() {
    return *image;
}

string Map::getScroll() {
    string v = "vertical";
    string h = "horizontal";
    if(scroll){
        return v;
    }
    return h;
}

bool Map::getWarn() {
    return warn;
}

bool Map::getWrap() {
    return wrap;
}

// Find a territory by name (case-insensitive)
Territory* Map::getTerritory(const string& name) const {
    // Iterate over the territories map
    for (const auto& pair : *territories) {
        if (caseInsensitiveCompare(pair.first, name)) {
            return pair.second; // Return the territory if name matches
        }
    }

    // If no match is found, return nullptr
    return nullptr;
}

// Find a continent by name (case-insensitive)
Continent* Map::getContinent(const string& name) const {
    // Iterate over the continents map
    for (const auto& pair : *continents) {
        if (caseInsensitiveCompare(pair.first, name)) {
            return pair.second; // Return the continent if name matches
        }
    }

    // If no match is found, return nullptr
    return nullptr;
}

// Find a territory by name (case-insensitive)

// Adds a Territory to the map's territories if it doesn't already exist.
void Map::addTerritory(Territory* territory) {
    if (territory == nullptr) {
        return; // Ignore null pointers.
    }

    string territoryName = territory->getName();
    
    // Check if the territory already exists in the map.
    if (territories->find(territoryName) == territories->end()) {
        // If not found, add the territory to the map.
        (*territories)[territoryName] = territory;
    }
    // If the territory already exists, do nothing.
}

// Adds a Continent to the map's continents if it doesn't already exist.
void Map::addContinent(Continent* continent) {
    if (continent == nullptr) {
        return; // Ignore null pointers.
    }

    string continentName = continent->getName();

    // Check if the continent already exists in the map.
    if (continents->find(continentName) == continents->end()) {
        // If not found, add the continent to the map.
        (*continents)[continentName] = continent;
    }
    // If the continent already exists, do nothing.
}

bool Map::caseInsensitiveCompare(const string &str1, const string &str2) const {

    if (str1.size() != str2.size()) {
        return false; // If sizes don't match, the strings are not equal
    }

    for (size_t i = 0; i < str1.size(); ++i) {
        if (tolower(str1[i]) != tolower(str2[i])) {
            return false; // If characters differ (case-insensitive), return false
        }
    }

    return true; // All characters match in a case-insensitive comparison
}

void Map::dfs(const Territory *territory, unordered_set<const Territory *> &visited) { 
    visited.insert(territory);
    for (const auto& neighbor : *(territory->getNeighbors())) {
        if (visited.find(neighbor) == visited.end()) {
            dfs(neighbor, visited);
        }
    }
}

bool Map::validateGraph() {
    if (territories->empty()) return false; // An empty map is not valid

    unordered_set<const Territory*> visited;
    auto it = territories->begin();
    dfs(it->second, visited); // Start DFS from the first territory

    return visited.size() == territories->size(); // Check if all territories were visited
}

// Validate if each continent is a connected subgraph
bool Map::validateContinents() {
    unordered_set<Continent*> visitedContinents;
    unordered_set<const Territory*> visitedTerritories;

    // Start from the first territory in the territories map
    if (territories->empty()) return false; // No territories

    auto it = territories->begin();
    dfs(it->second, visitedTerritories); // DFS to mark territories as visited

    // Use the visited territories to explore and mark connected continents
    for (const auto& territory : visitedTerritories) {
        Continent* currentContinent = territory->getContinent();
        if (currentContinent != nullptr) {
            visitedContinents.insert(currentContinent);
        }
    }

    // Check if all continents are connected
    return visitedContinents.size() == continents->size();
}

// Validate that each territory is linked only to its correct continent
bool Map::validateTerritories() {
    for (const auto& pair : *territories) {
        Territory* territory = pair.second;

        // Get the continent of the current territory
        Continent* continent = territory->getContinent();
        
        // If the territory has no continent, it's invalid
        if (continent == nullptr) {
            return false; // Territory is not assigned to any continent
        }

        // Check if the territory is part of the continent's territories vector
        const auto& continentTerritories = *(continent->getTerritories());
        bool isInCurrentContinent = false;

        // Check if the territory is linked to its continent
        for (const auto& contTerritory : continentTerritories) {
            if (contTerritory == territory) {
                isInCurrentContinent = true;
                break;
            }
        }

        if (!isInCurrentContinent) {
            cout << "Territory" << territory->getName() << " is not connected to its continent " << continent->getName();
            return false; // Territory is not linked to its continent
        }

        // Check all other continents to ensure this territory is not found elsewhere
        for (const auto& otherPair : *continents) {
            Continent* otherContinent = otherPair.second;

            if (otherContinent != continent) { // Avoid checking the same continent
                const auto& otherTerritories = *(otherContinent->getTerritories());
                for (const auto& otherTerritory : otherTerritories) {
                    if (otherTerritory == territory) {
                        cout << "Territory" << territory->getName() << " is can't be in more than one continent.";
                        return false; // Territory is found in another continent
                    }
                }
            }
        }
    }

    return true; // All territories are valid
}

// Implementation of the validate method
bool Map::validate() {
    cout << "Validating map...\n";
    return validateGraph() && validateContinents() && validateTerritories();
}

ostream &operator<<(ostream &os, const Map &map) {
    
    // Output the author's name and image path
    os << "INFO:\n";
    os << "Map Author: " << *(map.author) << "\n";
    os << "Image Path: " << *(map.image) << "\n";
    os << "Scroll Type: " << (*(map.scroll) ? "Vertical" : "Horizontal") << "\n";
    os << "Warn Users: " << (*(map.warn) ? "Yes" : "No") << "\n";
    os << "Wrap: " << (*(map.wrap) ? "Yes" : "No") << "\n";
    os << "------------------\n";
    // Output all territories
    os << "TERRITORIES:\n";
    for (const auto& pair : *(map.territories)) {
        os << *(pair.second) << "\n";
         // Use Territory's stream insertion operator
    }
    os << "------------------\n";

    // Output all continents
    os << "CONTINENTS:\n";
    for (const auto& pair : *(map.continents)) {
        os << *(pair.second) << "\n"; // Use Continent's stream insertion operator
    }

    return os; // Return the stream for chaining
}

// Map Loader class implementation:
Map *MapLoader::loadMap(const string &fileName) {
        ifstream file(fileName); // Open the specified .map file
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << fileName << endl;
        return nullptr; // Return nullptr if the file could not be opened
    }

    cout << "Loading Map...\n";
    Map* map = nullptr; // Initialize map pointer
    string line;

    // Read through the file and process each section
    while (getline(file, line)) {
        // Trim trailing whitespace and newline characters directly
        line.erase(line.find_last_not_of("\r") + 1); // Removes trailing \r
        if (line.empty()) continue; // Skip empty lines

        if (line == "[Map]") {
            map = createMap(file); // Process the Map section and create the Map object
        } else if (line == "[Continents]") {
            createContinents(file, map); // Process the Continents section
        } else if (line == "[Territories]") {
            while (getline(file, line)) {
                line.erase(line.find_last_not_of("\r") + 1); // Removes trailing \r
                createTerritories(line, map); // First pass: Create territories
            }
            file.clear(); // Clear any EOF flags
            file.seekg(0); // Rewind the file stream to the beginning
            while (getline(file, line)) {
                line.erase(line.find_last_not_of("\r") + 1); // Removes trailing \r 
                if (line == "[Territories]") {
                    break; // Move to the second pass
                }
            }
            while (getline(file, line)) {
                line.erase(line.find_last_not_of("\r") + 1); // Removes trailing \r 
                processTerritoriesSectionSecondPass(line, map); // Second pass: Connect neighbors
            }
        }
    }

    file.close(); // Close the file
    return map; // Return the created Map object
}

// Process the [Map] section of the file
Map* MapLoader::createMap(ifstream& file) {

    string line;
    string author;
    string imagePath;
    bool wrap = false;
    string scroll;
    bool warn = false;

    while (getline(file, line)) {
        line.erase(line.find_last_not_of("\r") + 1);
        if (line.empty() || line == "[Continents]") break; // Stop on empty line or next section
        istringstream iss(line);
        string key, value;
        getline(iss, key, '=');
        getline(iss, value);
        
        // Set attributes of the map based on the keys
        if (key == "image") {
            imagePath = value; // Save image path
        } else if (key == "wrap") {
            wrap = (value == "yes");
        } else if (key == "scroll") {
            scroll = value;
        } else if (key == "author") {
            author = value;
        } else if (key == "warn") {
            warn = (value == "yes");
        }
    }
    
    // Create the map directly with the parsed parameters
    return new Map(author, imagePath, scroll == "vertical", warn, wrap);
}

// Process the [Continents] section of the file
void MapLoader::createContinents(ifstream& file, Map* map) {
    string line;
    while (getline(file, line)) {
        line.erase(line.find_last_not_of("\r") + 1);
        if (line.empty() || line == "[Territories]") break; // Stop on empty line or next section
        istringstream iss(line);
        string continentName;
        int controlValue;
        getline(iss, continentName, '=');
        iss >> controlValue;
        
        // Create and add the continent to the map
        Continent* continent = new Continent(continentName, controlValue);
        map->addContinent(continent);
    }
}

// First pass of the [Territories] section: Create territories and link them to continents
void MapLoader::createTerritories(const string& line, Map* map) {
    istringstream iss(line);
    string name, continentName;

    // Extract territory name and skip x, y, and continent values
    if (getline(iss, name, ',')) {
        string xStr, yStr;
        getline(iss, xStr, ','); // Read x
        getline(iss, yStr, ','); // Read y
        getline(iss, continentName, ','); // Read continent name

        int x = stoi(xStr);
        int y = stoi(yStr);

        // Create territory and add it to the map
        Continent* continent = map->getContinent(continentName); // Get continent by name
        Territory* territory = new Territory(name, x, y, continent); // x and y are not needed yet
        map->addTerritory(territory); // Add territory to the map
        if (continent) {
            continent->addTerritory(territory); // Link territory to its continent
        }
    }
}

// Second pass of the [Territories] section: Connect territories to their neighbors
void MapLoader::processTerritoriesSectionSecondPass(const string& line, Map* map) {
    istringstream iss(line);
    string name, neighbor;

    // Extract territory name and skip x, y, and continent values
    if (getline(iss, name, ',')) {
        string xStr, yStr, continentName;
        getline(iss, xStr, ',');  // Skip x coordinate
        getline(iss, yStr, ',');  // Skip y coordinate
        getline(iss, continentName, ','); // Skip continent name
        
        // Now, read the neighbors
        Territory* territory = map->getTerritory(name); // Assume this function exists
        while (getline(iss, neighbor, ',')) {
            Territory* neighborTerritory = map->getTerritory(neighbor); // Assume this function exists
            if (territory && neighborTerritory) {
                territory->addNeighbor(neighborTerritory); // Connect neighbors
            }
        }
    }
}