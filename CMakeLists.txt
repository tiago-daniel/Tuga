cmake_minimum_required(VERSION 3.29)
project(brian_chessbot)

set(CMAKE_CXX_STANDARD 20)

include_directories(src)

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
)
