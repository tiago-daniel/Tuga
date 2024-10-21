//
// Created by metta on 10/10/24.
//

#ifndef GAME_H
#define GAME_H
#include <array>
#include <stack>
#include <cassert>
#include "movegen.h"

enum Result { WIN = 1, DRAW = 0, LOSS = -1 };

class Position {
    std::array<Bitboard, 2> colors;
    std::array<Bitboard, 6> boards;
    std::array<Piece, 64> pieces{};
    int draw_count = 0;
    bool current_player = WHITE;
    Square passant = a1;
    int can_castle = 0b1111; // Most significant bits (3 & 4) - Black can castle | Left bit (can castle to left)
    std::stack<Piece> captured_pieces{};
    int result = 2;
public:
    Position();
    void endGame(int score);
    int getResult() const;
    int getDrawCount() const;
    std::array<Bitboard, 6> getBitboards() const;
    std::array<Bitboard, 2> getColors() const;
    std::stack<Piece> getStack();
    void makeMove(Move move);
    void unmakeMove(Move move);
    Square findKingSquare(bool player) const;
    void promotionMove(Move move);
    void castleMove(Move move);
    Piece pieceOn(Square square);
    void passantMove(Move move);
    void normalMove(Move move);
    Square getPassant() const;
    int getCastlingRights() const;
    bool getCurrentPlayer() const;
    bool isKingInDoubleCheck(bool player);
    void print() const;
    MoveList pseudoLegal(bool player) const;
    MoveList allMoves(bool player);
    static std::array<Square, 8> isBetween(Square square1, Square square2);
    Square pseudoAttacker(bool player, Square square);
    bool isPseudoAttacked(bool player, Square square);
    static bool isSquareBetween(Square square1,Square square2,Square square3);
    bool isLegal(Move move);
    static Direction getDirection(Square sq1, Square sq2);
    Direction directionPinned(Square square);
    bool isPinned(Square square, bool color, int horizontalInc, int verticalInc);
    static bool isValid(Square square);
};



#endif //GAME_H