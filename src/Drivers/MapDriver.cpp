#include "../Map/Map.h" // Include your MapLoader header file
#include <filesystem>   // Include filesystem for directory operations
#include <iostream>
#include <vector>

// Function to test loading multiple map files from a directory

void testLoadMaps() {
    MapLoader loader; // Create an instance of MapLoader
    std::string directoryPath;
    std::cout << "Enter path of directory with map files";
    std::cin >> directoryPath; // Get directory path from user (try ../../maps/)
    // Iterate through each file in the specified directory
    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".map") { // Check for .map files
            std::string fileName = entry.path().string();                    // Get the full file path
            std::cout << "*************\nFile: " << fileName << std::endl;
            Map* map = loader.loadMap(fileName); // Attempt to load the map

            if (map->validate()) { // Check if the map was successfully loaded
                std::cout << "Successfully loaded the map: " << fileName << "\n"
                          << *map << std::endl;
                delete map; // Clean up the created map object after use
            } else {
                std::cout << "Failed to load or validate the map: " << fileName << std::endl;
                delete map;
            }
        }
    }
}