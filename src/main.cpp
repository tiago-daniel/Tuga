//
// Created by metta on 10/9/24.
//

#include "search.h"
// #include "draw.h"
#include <chrono>


int main() {
    std::string fen = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1";
    auto game = Position(fen);
    // Search::runSearch(game, depth);

    // Example possible moves (you can modify this based on your game logic)
    // Main loop
    while (game.getResult() == 2) {
        auto start = std::chrono::high_resolution_clock::now();
        Search::runSearch(game,5);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);

        // Output the time in milliseconds
        std::cout << "Elapsed time: " << duration.count() << " seconds" << std::endl;
        std::string i;
        std::cin >> i;
    }
    return game.getResult();
}