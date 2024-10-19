//
// Created by metta on 10/18/24.
//

#include "draw.h"

sf::Texture Draw::loadPieceTexture(const std::string& filename) {
    sf::Texture texture;
    if (!texture.loadFromFile(filename)) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
    }
    return texture;
}
std::array<sf::Texture, 12> Draw::loadAllTextures(){
    std::array<sf::Texture, 12> pieceTextures;
    pieceTextures[0] = loadPieceTexture("../png/P.png");
    pieceTextures[1] = loadPieceTexture("../png/N.png");
    pieceTextures[2] = loadPieceTexture("../png/B.png");
    pieceTextures[3] = loadPieceTexture("../png/R.png");
    pieceTextures[4] = loadPieceTexture("../png/Q.png");
    pieceTextures[5] = loadPieceTexture("../png/K.png");
    pieceTextures[6] = loadPieceTexture("../png/p.png");
    pieceTextures[7] = loadPieceTexture("../png/n.png");
    pieceTextures[8] = loadPieceTexture("../png/b.png");
    pieceTextures[9] = loadPieceTexture("../png/r.png");
    pieceTextures[10] = loadPieceTexture("../png/q.png");
    pieceTextures[11] = loadPieceTexture("../png/k.png");
    return pieceTextures;
};

void Draw::drawChessBoard(sf::RenderWindow& window, const Square selectedSquare) {
    sf::Color lightSquareColor(240, 217, 181);
    sf::Color darkSquareColor(181, 136, 99);
    sf::Color highlighted(30, 120, 30, 255);
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            auto square = static_cast<Square>((56 - row * 8) + col);
            sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
            tile.setPosition(80 + col * TILE_SIZE, 80 + row * TILE_SIZE);

            if ((row + col) % 2 == 0) {
                tile.setFillColor(lightSquareColor);
            }
            else {
                tile.setFillColor(darkSquareColor);
            }
            if (square == selectedSquare) {
                tile.setFillColor(highlighted);
            }

            window.draw(tile);
        }
    }
}

Draw::Draw() = default;

// Function to convert a square enum value to its (x, y) position
sf::Vector2f Draw::squareToPosition(Square square) {
    int file = static_cast<int>(square) % BOARD_SIZE;
    int rank = static_cast<int>(square) / BOARD_SIZE;
    return sf::Vector2f(file * TILE_SIZE + 80, (BOARD_SIZE - 1 - rank) * TILE_SIZE + 80); // Invert rank for SFML
}

// Function to draw the pieces on the chessboard
void Draw::drawPieces(sf::RenderWindow& window, Position& game,
    const std::array<sf::Texture, 12>& pieceTextures, sf::Font& font) {
    sf::Text playerText;
    playerText.setFont(font);
    playerText.setCharacterSize(24);
    playerText.setFillColor(sf::Color::White);
    playerText.setPosition(600, 50);
    if (game.getCurrentPlayer()) {
        playerText.setString("Black to play.");
    }
    else {
        playerText.setString("White to play.");
    }
    window.draw(playerText);
    for (int rank = 0; rank < BOARD_SIZE; ++rank) {
        for (int file = 0; file < BOARD_SIZE; ++file) {
            Square square = static_cast<Square>(rank * BOARD_SIZE + file);
            Piece piece = game.pieceOn(square); // Get the piece at the current square

            // Skip empty squares
            if (piece == EMPTY) continue;

                sf::Sprite pieceSprite;

                // Determine the texture based on the piece type and color
            if (game.getColors()[WHITE].getBitboard() & Bit(square)) {
                if (piece == PAWN) {
                    pieceSprite.setTexture(pieceTextures[0]); // White pawn
                } else if (piece == KNIGHT) {
                    pieceSprite.setTexture(pieceTextures[1]); // White knight
                } else if (piece == BISHOP) {
                    pieceSprite.setTexture(pieceTextures[2]); // White bishop
                } else if (piece == ROOK) {
                    pieceSprite.setTexture(pieceTextures[3]); // White rook
                } else if (piece == QUEEN) {
                    pieceSprite.setTexture(pieceTextures[4]); // White queen
                } else if (piece == KING) {
                    pieceSprite.setTexture(pieceTextures[5]); // White king
                }
            }
        else if (game.getColors()[BLACK].getBitboard() & Bit(square)) {
            if (piece == PAWN) {
                pieceSprite.setTexture(pieceTextures[6]); // Black pawn
            } else if (piece == KNIGHT) {
                pieceSprite.setTexture(pieceTextures[7]); // Black knight
            } else if (piece == BISHOP) {
                pieceSprite.setTexture(pieceTextures[8]); // Black bishop
            } else if (piece == ROOK) {
                pieceSprite.setTexture(pieceTextures[9]); // Black rook
            } else if (piece == QUEEN) {
                pieceSprite.setTexture(pieceTextures[10]); // Black queen
            } else if (piece == KING) {
                pieceSprite.setTexture(pieceTextures[11]); // Black king
            }
        }
        // Set the position of the piece sprite
        pieceSprite.setPosition(squareToPosition(square));
        window.draw(pieceSprite); // Draw the piece on the window
    }
  }
}

void Draw::drawPossibleMoves(sf::RenderWindow& window, const MoveList& moves, std::vector<sf::Text>& moveTexts, sf::Font& font) {
    moveTexts.clear();

    for (size_t i = 0; i < moves.getSize(); ++i) {
        sf::Text moveText;
        moveText.setFont(font);
        moveText.setString("Move " + std::to_string(i + 1) + ": " + printMove(moves.getMoves()[i]));
        moveText.setCharacterSize(24);
        moveText.setFillColor(sf::Color::White);
        moveText.setPosition(900 + (i / 27) * 200, 80 + (i % 27) * 30); // Adjust y position for each move
        window.draw(moveText);
        moveTexts.push_back(moveText);
    }
}


Square Draw::pixelToSquare(float x, float y) {
    int col = static_cast<int>(x-80) / TILE_SIZE; // Column (file)
    int row = 7 - static_cast<int>(y-80) / TILE_SIZE; // Row (rank), inverted to match chessboard

    return static_cast<Square>(row * 8 + col);
}

void Draw::drawPlayableCircles(sf::RenderWindow& window, const MoveList & moves, Bitboard enemies) {
    for (int i = 0; i < moves.getSize(); ++i) {
        sf::CircleShape circle(15);
        int col = moves.getMoves()[i].destination % 8;
        int row = moves.getMoves()[i].destination / 8;
        circle.setPosition(115 +col * TILE_SIZE,  810 - (row * TILE_SIZE));
        circle.setFillColor(sf::Color(20,20,20,100));
        if (enemies.hasBit(moves.getMoves()[i].destination)) {
            circle.setFillColor(sf::Color(200,20,20,200));
        }

        window.draw(circle);
    }
}
