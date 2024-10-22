//
// Created by metta on 10/10/24.
//

#ifndef GAME_H
#define GAME_H
#include <stack>
#include <cassert>
#include "movegen.h"

enum Result { WIN = 1, DRAW = 0, LOSS = -1 };

class Position {
    std::array<Bitboard, 2> colors;
    std::array<Bitboard, 6> boards;
    std::array<int, 2> materials;
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
    [[nodiscard]] int getResult() const;
    [[nodiscard]] int getDrawCount() const;
    [[nodiscard]] std::array<Bitboard, 6> getBitboards() const;
    [[nodiscard]] std::array<Bitboard, 2> getColors() const;
    std::stack<Piece> getStack();
    void makeMove(const Move &move);
    void unmakeMove(const Move &move);
    [[nodiscard]] Square findKingSquare(bool player) const;
    void promotionMove(const Move &move);
    void castleMove(const Move &move);
    [[nodiscard]] Piece pieceOn(Square square) const;
    void passantMove(const Move &move);
    void normalMove(const Move &move);
    [[nodiscard]] Square getPassant() const;
    [[nodiscard]] int getCastlingRights() const;
    [[nodiscard]] bool getCurrentPlayer() const;
    [[nodiscard]] bool isKingInDoubleCheck(bool player) const;
    bool insufficientMaterial() const;
    void print() const;
    [[nodiscard]] MoveList pseudoLegal(bool player) const;
    MoveList allMoves(bool player);
    static std::array<Square, 8> isBetween(Square square1, Square square2);
    [[nodiscard]] Square pseudoAttacker(bool player, Square square) const;
    [[nodiscard]] bool isPseudoAttacked(bool player, Square square) const;
    static bool isSquareBetween(Square square1,Square square2,Square square3);
    bool isLegal(const Move &move);
    static Direction getDirection(Square sq1, Square sq2);
    [[nodiscard]] Direction directionPinned(Square square) const;
    [[nodiscard]] bool isPinned(Square square, bool color, int horizontalInc, int verticalInc) const;
    static bool isValid(Square square);
};



#endif //GAME_H