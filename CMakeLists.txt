cmake_minimum_required(VERSION 3.29)
project(WarzoneMapProject)

set(CMAKE_CXX_STANDARD 20)

add_executable(MainDriver src/MainDriver.cpp
        src/Map/Map.cpp
        src/Map/Map.h
        src/Player/Player.cpp
        src/Player/Player.h
        src/Orders/Orders.cpp
        src/Orders/Orders.h
        src/Cards/Cards.cpp
        src/Cards/Cards.h
        src/GameEngine/GameEngine.cpp
        src/GameEngine/GameEngine.h
        src/Drivers/MapDriver.cpp
        src/Drivers/PlayerDriver.cpp
        src/Drivers/OrdersDriver.cpp
        src/Drivers/CardsDriver.cpp
        src/Drivers/GameEngineDriver.cpp
)