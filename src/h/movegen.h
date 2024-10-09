//
// Created by metta on 10/6/24.
//

#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "bitboard.h"

class moveGen {
public:
    moveGen();
    void movePiece(enumSquare initial_square, enumSquare target_square, Bitboard &board);

    vector<enumSquare> pawnMove(enumSquare square, Color color, Bitboard board = Bitboard());

    vector<enumSquare> knightMove(enumSquare square,Bitboard allies = Bitboard());

    vector<enumSquare> bishopMove(enumSquare square,Bitboard allies = Bitboard(), Bitboard enemies = Bitboard());

    vector<enumSquare> rookMove(enumSquare square,Bitboard allies = Bitboard(), Bitboard enemies = Bitboard());

    vector<enumSquare> queenMove(enumSquare square,Bitboard allies = Bitboard(), Bitboard enemies = Bitboard());

    vector<enumSquare> kingMove(enumSquare square, Bitboard allies = Bitboard(),Bitboard enemies = Bitboard());

    vector<enumSquare> slideMove(enumSquare square, vector<pair<int, int>> directions,
        bool slide, Bitboard allies = Bitboard(), Bitboard enemies = Bitboard());
};



#endif //MOVEGEN_H
