cmake_minimum_required(VERSION 3.29)
project(tuga)

set(CMAKE_CXX_STANDARD 20)

#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS "-O3")

include_directories(src)

add_executable(Tuga
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
        src/evaluation.cpp
        src/evaluation.h
        src/search.cpp
        src/search.h
        src/magic.h
        src/attacks.h
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug mode detected - Adding sanitizers")

    # Add AddressSanitizer, UndefinedBehaviorSanitizer, etc.
    target_compile_options(Tuga PRIVATE
            -fsanitize=address,undefined
            -fno-omit-frame-pointer
            -g
    )
    target_link_options(Tuga PRIVATE
            -fsanitize=address,undefined
    )
endif()