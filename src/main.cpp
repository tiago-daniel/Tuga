//
// Created by metta on 10/9/24.
//

#include "search.h"
// #include "draw.h"
#include <chrono>
#include <thread>


int main() {
    std::string fen = "8/2p5/3p4/KP5r/1R3P2/6k1/6P1/8 b - - 0 2";
    auto game = Position(fen);
    // Example possible moves (you can modify this based on your game logic)
    // Main loop
    auto start = std::chrono::high_resolution_clock::now();
    U64 nodes = Search::preft(game, 6);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Number of nodes : " << nodes << std::endl;
    std::cout << "Elapsed time: " << duration.count() / 1000 << " milliseconds" << std::endl;
    std::cout << "Number of nodes per seconds: " <<  nodes *1000000/ duration.count()<< std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    return game.getResult();
}