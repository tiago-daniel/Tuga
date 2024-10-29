//
// Created by metta on 10/9/24.
//

#include "search.h"
#include <chrono>
#include <thread>


int main() {
    std::string fen = "r3k2r/p1ppqpb1/1n2pnp1/3PN3/1p2P3/2N2Q1p/PPPBbPPP/1R2K2R w Kkq - 0 2";
    auto game = Position(fen);
    Search::runSearch(game, 2);
    return 0;
    // Example possible moves (you can modify this based on your game logic)
    // Main loop
    auto start = std::chrono::high_resolution_clock::now();
    auto nodes = Search::preft(game, 5);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Number of nodes : " << nodes << std::endl;
    std::cout << "Elapsed time: " << duration.count() / 1000 << " milliseconds" << std::endl;
    std::cout << "Number of nodes per seconds: " <<  nodes *1000000/ duration.count()<< std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    return game.getResult();
}