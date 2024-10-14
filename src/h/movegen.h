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

    void pawnMove(array<Move, 256> &moves, Square_index square, Color color, Bitboard allies, Bitboard enemies, Square_index passant);

    void knightMove(array<Move, 256> &moves, Square_index square,Bitboard allies = Bitboard());

    void bishopMove(array<Move, 256> &moves, Square_index square,Bitboard allies = Bitboard(), Bitboard enemies = Bitboard());

    void rookMove(array<Move, 256> &moves, Square_index square,Bitboard allies = Bitboard(), Bitboard enemies = Bitboard());

    void queenMove(array<Move, 256> &moves, Square_index square,Bitboard allies = Bitboard(), Bitboard enemies = Bitboard());

    void kingMove(array<Move, 256> &moves, Square_index square, Bitboard allies = Bitboard(),Bitboard enemies = Bitboard());

    void castleMove(array<Move, 256> &moves, Square_index square,uint8_t castlingRights, Bitboard allies = Bitboard(),Bitboard enemies = Bitboard());

    void pieceMove(array<Move, 256> &moves, Square_index  square, vector<pair<int, int>> directions,
        bool slide, Bitboard allies = Bitboard(), Bitboard enemies = Bitboard());
};



#endif //MOVEGEN_H
