//
// Created by metta on 10/10/24.
//

#ifndef GAME_H
#define GAME_H
#include "movegen.h"
#include <array>
#include <unordered_map>


class Position {
    std::array<Bitboard, 2> colors;
    std::array<Bitboard, 6> boards;
    std::array<Piece, 64> pieces;
    MoveGen move_handler = MoveGen();
    int draw_count = 0;
    bool current_player = false;
    Square passant = a1;
    int can_castle = 0b1111; // Most significant bits (3 & 4) - Black can castle | Left bit (can castle to left)
public:
    Position();

    std::array<Bitboard, 6> getBitboards();

    std::array<Bitboard, 2> getColors();

    void makeMove(Move move);

    void promotionMove(Move move);

    void castleMove(Move move);

    Piece pieceOn(Square square);

    void passantMove(Move move);

    void normalMove(Move move);

    Square getPassant();

    bool getCurrentPlayer();

    void print();

    MoveList allMoves(bool player);

    bool isKingInCheck(bool player);
};



#endif //GAME_H