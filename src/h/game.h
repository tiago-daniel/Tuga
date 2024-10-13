//
// Created by metta on 10/10/24.
//

#ifndef GAME_H
#define GAME_H
#include "movegen.h"
#include <unordered_map>


class game {
private:
    Bitboard white,black,pawn,knight,bishop,rook,queen,king;
    array<reference_wrapper<Bitboard> , 8> boards = {white,black,pawn,knight,bishop,rook,queen,king};
    array<Piece_index, 64> pieces;
    moveGen move_handler = moveGen();
    uint8_t draw_count = 0;
    bool current_player = 0;
    Square_index passant = a1;
    uint8_t can_castle = 0b1111; // Most significant bits (3 & 4) - Black can castle | Left bit (can castle to left)
public:
    game();

    array<reference_wrapper<Bitboard> , 8> getBitboards();

    void movePiece(Square_index  initial_square, Square_index  target_square, Bitboard &board);

    bool getCurrentPlayer();

    void print();

    array<Move, 256> allMoves();
};



#endif //GAME_H