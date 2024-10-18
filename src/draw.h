//
// Created by metta on 10/18/24.
//

#ifndef DRAW_H
#define DRAW_H

const int BOARD_SIZE = 8;
const int TILE_SIZE = 100;
const int BOARD_WIDTH = 800;
const int WINDOW_WIDTH = 1360;
const int WINDOW_HEIGHT = 960;

#include <SFML/Graphics.hpp>
#include "position.h"

class Draw {
public:
    Draw();
    sf::Texture loadPieceTexture(const std::string& filename);
    std::array<sf::Texture, 12> loadAllTextures ();
    void drawChessBoard(sf::RenderWindow& window);
    sf::Vector2f squareToPosition(Square square);
    void drawPieces(sf::RenderWindow& window, Position& game, const std::array<sf::Texture, 12>& pieceTextures);
    void drawPossibleMoves(sf::RenderWindow &window, const MoveList &moves, std::vector<sf::Text> &moveTexts, sf::Font &font);
    Square pixelToSquare(float x, float y);
};



#endif //DRAW_H
