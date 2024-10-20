//
// Created by metta on 10/10/24.
//

#ifndef GAME_H
#define GAME_H
#include <array>
#include <stack>
#include "movegen.h"

enum Result { WIN = 1, DRAW = 0, LOSS = -1 };

class Position {
    std::array<Bitboard, 2> colors;
    std::array<Bitboard, 6> boards;
    std::array<Piece, 64> pieces{};
    MoveGen move_handler = MoveGen();
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
    std::array<Bitboard, 6> getBitboards();
    std::array<Bitboard, 2> getColors();
    std::stack<Piece> getStack();
    void makeMove(Move move);
    void unmakeMove(Move move);
    Square findKingSquare(bool player) const;
    void promotionMove(Move move);
    void castleMove(Move move);
    Piece pieceOn(Square square);
    void passantMove(Move move);
    void normalMove(Move move);
    Square getPassant();
    [[nodiscard]] int getCastlingRights() const;
    [[nodiscard]] bool getCurrentPlayer() const;
    bool isKingInDoubleCheck(bool player) const;
    void print() const;
    [[nodiscard]] MoveList pseudoLegal(bool player) const;
    MoveList allMoves(bool player);
    static std::array<Square, 7> isBetween(Square square1, Square square2);
    bool isPseudoAttacked(bool player, Square square) const;
    [[nodiscard]] Square isKingInCheck(bool player, Square kingSquare) const;
    static bool isSquareBetween(Square square1,Square square2,Square square3);

    bool isLegal(Move move);
};



#endif //GAME_H