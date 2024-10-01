#include <iostream>
#include <vector>
#include <filesystem> // Include filesystem for directory operations
#include "../Map/Map.h" // Include your MapLoader header file

using namespace std;
namespace fs = std::filesystem; // Create an alias for the filesystem namespace

// Function to test loading multiple map files from a directory
void testLoadMaps() {
    MapLoader loader; // Create an instance of MapLoader
    string directoryPath;
    cout << "Enter path of directory with map files";
    cin >> directoryPath; // Get directory path from user (try ../../maps/)
    // Iterate through each file in the specified directory
    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".map") { // Check for .map files
            string fileName = entry.path().string(); // Get the full file path
            cout << "*************\nFile: " << fileName << endl;
            Map* map = loader.loadMap(fileName); // Attempt to load the map

            if (map->validate()) { // Check if the map was successfully loaded
                cout << "Successfully loaded the map: " << fileName << "\n" << *map << endl;
                delete map; // Clean up the created map object after use
            } else {
                cout << "Failed to load or validate the map: " << fileName << endl;
            }
        }
    }
}
