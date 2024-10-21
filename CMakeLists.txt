cmake_minimum_required(VERSION 3.29)
project(brian_chessbot)

set(CMAKE_CXX_STANDARD 20)

include_directories(src)

# Find SFML package
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(brian_chessbot
        src/bitboard.cpp
        src/bitboard.h
        src/movegen.cpp
        src/movegen.h
        src/utils.h
        src/main.cpp
        src/position.h
        src/position.cpp
        src/movelist.cpp
        src/movelist.h
        src/draw.cpp
)

# Link SFML libraries
target_link_libraries(brian_chessbot sfml-graphics sfml-window sfml-system)
