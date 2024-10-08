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
};



#endif //MOVEGEN_H
