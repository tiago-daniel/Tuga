//
// Created by metta on 10/6/24.
//

#include "h/movegen.h"
#include "h/bitboard.h"

moveGen::moveGen() = default;

void moveGen::move(enumSquare initial_square, enumSquare target_square, Bitboard &board) {
    if (board.getBitboard() & Bit(initial_square)) {
        board.removeBit(initial_square);
        board.addBit(target_square);
    }
}

vector<enumSquare> moveGen::pawnMove(enumSquare square, Color color) {
    vector<enumSquare> squares = {};
    if (color == c_white) {
        squares.push_back(enumSquare(square+8));
        if (square<16 and square>=8) {
            squares.push_back(enumSquare(square + 16));
        }
    }
    else {
        squares.push_back(enumSquare(square-8));
        if(square<(64-8) and square>=(64-16)) {
            squares.push_back(enumSquare(square-16));
        }
    }
    return squares;
}
