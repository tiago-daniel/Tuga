//
// Created by metta on 10/9/24.
//

#include "position.h"
#include "draw.h"


int main() {
    auto game = Position();
    auto draw = Draw();
    sf::Font font;
    if (!font.loadFromFile("../arial.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return -1; // Exit if font cannot be loaded
    }
    bool changes = true;
    std::vector<sf::Text> moveTexts;
    auto pieceTextures = draw.loadAllTextures();
    // Create a window with specified dimensions
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Chess Board with Moves", sf::Style::Close);

    // Example possible moves (you can modify this based on your game logic)
    auto moves = game.allMoves(game.getCurrentPlayer());
    auto selectedSquare = static_cast<Square>(64);
    auto playableMoves  = MoveList();
    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();  // Close the window if requested
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Get mouse position
                    int x = event.mouseButton.x;
                    int y = event.mouseButton.y;
                    if (x >= 80 and x <= BOARD_WIDTH + 80 and y >= 80 and y <= BOARD_WIDTH + 80) {
                        auto clickedSquare = draw.pixelToSquare(x, y);
                        for (auto move1 : playableMoves.getMoves()) {
                            if (move1.destination == clickedSquare) {
                                game.makeMove(move1);
                                moves = game.allMoves(game.getCurrentPlayer());
                                selectedSquare = static_cast<Square>(64);
                                changes = true;
                                break;
                            }
                        }
                        playableMoves.clear();
                        if (game.getColors()[game.getCurrentPlayer()].hasBit(clickedSquare) and !changes) {
                            selectedSquare = clickedSquare;
                            changes = true;
                            for (auto move2 : moves.getMoves()) {
                                if (move2.origin == selectedSquare) {
                                    playableMoves.push(move2);
                                }
                            }
                        }
                    }
                    else {
                        for (size_t i = 0; i < moveTexts.size(); ++i) {
                            if (moveTexts[i].getGlobalBounds().contains(x, y)) {
                                // Move was clicked, handle it
                                std::cout << "Clicked on move: " << i + 1 << std::endl;
                                game.makeMove(moves.getMoves()[i]);
                                moves = game.allMoves(game.getCurrentPlayer());
                                selectedSquare = static_cast<Square>(64);
                                changes = true;
                                // Handle the move here (e.g., make the move, update the board)
                            }
                        }
                    }
                }
            }
            if (changes) {
                window.clear();

                // Draw the chess board on the left side
                draw.drawChessBoard(window, selectedSquare);

                draw.drawPieces(window,game,pieceTextures,font);

                // Draw the list of possible moves on the right side
                draw.drawPossibleMoves(window, moves, moveTexts, font);

                window.display();  // Display the contents of the window
                changes = false;
            }
        }
    }
    return 0;
}