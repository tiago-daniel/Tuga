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

void Draw::drawChessBoard(sf::RenderWindow& window) {
  sf::Color lightSquareColor(240, 217, 181);
  sf::Color darkSquareColor(181, 136, 99);
  for (int row = 0; row < BOARD_SIZE; ++row) {
    for (int col = 0; col < BOARD_SIZE; ++col) {
      sf::RectangleShape square(sf::Vector2f(TILE_SIZE, TILE_SIZE));
      square.setPosition(80 + col * TILE_SIZE, 80 + row * TILE_SIZE);

      if ((row + col) % 2 == 0) {
        square.setFillColor(lightSquareColor);
      } else {
        square.setFillColor(darkSquareColor);
      }

      window.draw(square);
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
void Draw::drawPieces(sf::RenderWindow& window, Position& game, const std::array<sf::Texture, 12>& pieceTextures) {
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
  int col = static_cast<int>(x) / TILE_SIZE; // Column (file)
  int row = 7 - static_cast<int>(y) / TILE_SIZE; // Row (rank), inverted to match chessboard

  // Ensure the column and row are within bounds
  if (col < 0 || col > 7 || row < 0 || row > 7) {
    throw std::out_of_range("Click is outside the board!");
  }

  // Calculate the enum value
  return static_cast<Square>(row * 8 + col);
}