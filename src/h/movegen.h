//
// Created by metta on 10/6/24.
//

#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "bitboard.h"

class moveGen {
public:
    moveGen();
    void move(enumSquare initial_square, enumSquare target_square, Bitboard &board);
    vector<enumSquare> pawnMove(enumSquare square, Color color);
    vector<enumSquare> knightMove(enumSquare square);
    vector<enumSquare> bishopMove(enumSquare square);
    vector<enumSquare> rookMove(enumSquare square);
    vector<enumSquare> queenMove(enumSquare square);
    vector<enumSquare> kingMove(enumSquare square);
};



#endif //MOVEGEN_H
