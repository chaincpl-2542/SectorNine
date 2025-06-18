cmake_minimum_required(VERSION 3.15)
project(SectorNine)

set(CMAKE_CXX_STANDARD 17)

# Add include directory
include_directories(include)

# Add executable with all source files
add_executable(SectorNine
        main.cpp
        Map.cpp
        Player.cpp
        Entity.cpp
        Enemy.cpp
        GameEngine.cpp
)
