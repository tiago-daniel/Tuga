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
    enum enumSquare : uint8_t {
        a1 = 0,  b1 = 1,  c1 = 2,  d1 = 3,  e1 = 4,  f1 = 5,  g1 = 6,  h1 = 7,
        a2 = 8,  b2 = 9,  c2 = 10, d2 = 11, e2 = 12, f2 = 13, g2 = 14, h2 = 15,
        a3 = 16, b3 = 17, c3 = 18, d3 = 19, e3 = 20, f3 = 21, g3 = 22, h3 = 23,
        a4 = 24, b4 = 25, c4 = 26, d4 = 27, e4 = 28, f4 = 29, g4 = 30, h4 = 31,
        a5 = 32, b5 = 33, c5 = 34, d5 = 35, e5 = 36, f5 = 37, g5 = 38, h5 = 39,
        a6 = 40, b6 = 41, c6 = 42, d6 = 43, e6 = 44, f6 = 45, g6 = 46, h6 = 47,
        a7 = 48, b7 = 49, c7 = 50, d7 = 51, e7 = 52, f7 = 53, g7 = 54, h7 = 55,
        a8 = 56, b8 = 57, c8 = 58, d8 = 59, e8 = 60, f8 = 61, g8 = 62, h8 = 63
    };


    enum Color : bool{
        c_white = false,
        c_black = true
    };

    enum PieceType : uint8_t{
        pt_empty = 0,
        pt_pawn = 1,
        pt_knight = 2,
        pt_bishop = 3,
        pt_rook = 4,
        pt_queen = 5,
        pt_king = 6
    };

    enum PieceCode : uint8_t{
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
