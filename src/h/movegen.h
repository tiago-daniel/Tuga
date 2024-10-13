//
// Created by metta on 10/6/24.
//

#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "bitboard.h"
#include <array>

class moveGen {
public:
    moveGen();

    vector<Move> pawnMove(Square_index square, Color color, Bitboard board = Bitboard(), Square_index  passant=a1);

    vector<Move> knightMove(Square_index  square,Bitboard allies = Bitboard());

    vector<Move> bishopMove(Square_index  square,Bitboard allies = Bitboard(), Bitboard enemies = Bitboard());

    vector<Move> rookMove(Square_index  square,Bitboard allies = Bitboard(), Bitboard enemies = Bitboard());

    vector<Move> queenMove(Square_index  square,Bitboard allies = Bitboard(), Bitboard enemies = Bitboard());

    vector<Move> kingMove(Square_index  square, Bitboard allies = Bitboard(),Bitboard enemies = Bitboard());

    vector<Move> pieceMove(Square_index  square, vector<pair<int, int>> directions,
        bool slide, Bitboard allies = Bitboard(), Bitboard enemies = Bitboard());

    vector<Move> allMoves(array<reference_wrapper<Bitboard>, 8> board, Color color);
};



#endif //MOVEGEN_H
