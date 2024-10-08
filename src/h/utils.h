//
// Created by metta on 10/6/24.
//

#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <cstdint>

using namespace std;

namespace utils {
    inline uint64_t Bit(uint8_t n) {
        return 1ULL << n;
    }
    enum enumSquare {
        a1 = 0, b1 = 1, c1 = 2, d1 = 3, e1 = 4, f1 = 5, g1 = 6, h1 = 7,
        a2 = 8, b2 = 9, c2 = 10, d2 = 25, e2 = 33, f2 = 41, g2 = 49, h2 = 57,
        a3 = 2, b3 = 10, c3 = 18, d3 = 26, e3 = 34, f3 = 42, g3 = 50, h3 = 58,
        a4 = 3, b4 = 11, c4 = 19, d4 = 27, e4 = 35, f4 = 43, g4 = 51, h4 = 59,
        a5 = 4, b5 = 12, c5 = 20, d5 = 28, e5 = 36, f5 = 44, g5 = 52, h5 = 60,
        a6 = 5, b6 = 13, c6 = 21, d6 = 29, e6 = 37, f6 = 45, g6 = 53, h6 = 61,
        a7 = 6, b7 = 14, c7 = 22, d7 = 30, e7 = 38, f7 = 46, g7 = 54, h7 = 62,
        a8 = 7, b8 = 15, c8 = 23, d8 = 31, e8 = 39, f8 = 47, g8 = 55, h8 = 63
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
};



#endif //UTILS_H
