//
// Created by metta on 10/9/24.
//

#include "search.h"
// #include "draw.h"
#include <chrono>
#include <thread>


int main() {
    std::string fen = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1";
    auto game = Position(fen);

    // Example possible moves (you can modify this based on your game logic)
    // Main loop
    while (game.getResult() == 2) {
        auto start = std::chrono::high_resolution_clock::now();
        Search::runSearch(game,5);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);

        // Output the time in milliseconds
        std::cout << "Elapsed time: " << duration.count() << " seconds" << std::endl;
        /*
        std::string input;
        std::cin >> input;
        std::cout << std::endl;
        Square origin = string_to_square(input.substr(0,2));
        Square destination = string_to_square(input.substr(2,2));
        Piece piece;
        if (input.length() == 5) piece = stringToPiece(input[4]);
        game.makeMove(game.allMoves(game.getCurrentPlayer()).findMove(origin, destination, piece));
        */
        game.endGame(0);
    }
    std::this_thread::sleep_for(std::chrono::seconds(5));
    return game.getResult();
}