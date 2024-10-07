//
// Created by metta on 10/6/24.
//

#ifndef BITBOARD_H
#define BITBOARD_H

#include "utils.h"

using namespace utils;

enum enumSquare {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8
  };

enum Color {
    c_white = 0,
    c_black = 1
};

enum PieceType {
    pt_empty = 0,
    pt_pawn = 1,
    pt_knight = 2,
    pt_bishop = 3,
    pt_rook = 4,
    pt_queen = 5,
    pt_king = 6
};

enum PieceCode {
    pc_empty = pt_empty,
    pc_wpawn = pt_pawn,
    pc_wknight = pt_knight,
    pc_wbishop = pt_bishop,
    pc_wrook = pt_rook,
    pc_wqueen = pt_queen,
    pc_wking = pt_king,

    pc_black = 8,
    pc_bpawn = pc_black + pt_pawn,
    pc_bknight = pc_black + pt_knight,
    pc_bbishop = pc_black + pt_bishop,
    pc_brook = pc_black + pt_rook,
    pc_bqueen = pc_black + pt_queen,
    pc_bking = pc_black + pt_king
};

class Bitboard {
private:
    uint64_t positions = 0;
public:
    Bitboard();
    Bitboard(uint64_t positions);
    uint64_t getBitboard();
    void removeBit(uint8_t bit);

    void addBit(uint8_t bit);

    void print();
    void setBitboard(uint64_t positions);
};



#endif //BITBOARD_H
