cmake_minimum_required(VERSION 3.16)

project(PAACheckersMinMax)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_executable(PAACheckersMinMax
    src/main.cpp
    src/Board.cpp
    src/Move.cpp
)

target_link_libraries(PAACheckersMinMax
    SFML::Graphics
    SFML::Window
    SFML::System
)