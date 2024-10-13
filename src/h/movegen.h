//
// Created by metta on 10/6/24.
//

#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "bitboard.h"
#include <vector>


class moveGen {
public:
    moveGen();

    array<Move, 256> pawnMove(Square_index square, Color color, Bitboard allies, Bitboard enemies, Square_index passant);

    array<Move, 256> knightMove(Square_index square,Bitboard allies = Bitboard());

    array<Move, 256> bishopMove(Square_index square,Bitboard allies = Bitboard(), Bitboard enemies = Bitboard());

    array<Move, 256> rookMove(Square_index square,Bitboard allies = Bitboard(), Bitboard enemies = Bitboard());

    array<Move, 256> queenMove(Square_index square,Bitboard allies = Bitboard(), Bitboard enemies = Bitboard());

    array<Move, 256> kingMove(Square_index square, Bitboard allies = Bitboard(),Bitboard enemies = Bitboard());

    array<Move, 256> CastleMove(Square_index square, uint8_t castlingRights);

    array<Move, 256> pieceMove(Square_index  square, vector<pair<int, int>> directions,
        bool slide, Bitboard allies = Bitboard(), Bitboard enemies = Bitboard());
};



#endif //MOVEGEN_H
