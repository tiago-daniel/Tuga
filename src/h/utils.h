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
    enum Square_index  : uint8_t {
        a1 = 0,  b1 = 1,  c1 = 2,  d1 = 3,  e1 = 4,  f1 = 5,  g1 = 6,  h1 = 7,
        a2 = 8,  b2 = 9,  c2 = 10, d2 = 11, e2 = 12, f2 = 13, g2 = 14, h2 = 15,
        a3 = 16, b3 = 17, c3 = 18, d3 = 19, e3 = 20, f3 = 21, g3 = 22, h3 = 23,
        a4 = 24, b4 = 25, c4 = 26, d4 = 27, e4 = 28, f4 = 29, g4 = 30, h4 = 31,
        a5 = 32, b5 = 33, c5 = 34, d5 = 35, e5 = 36, f5 = 37, g5 = 38, h5 = 39,
        a6 = 40, b6 = 41, c6 = 42, d6 = 43, e6 = 44, f6 = 45, g6 = 46, h6 = 47,
        a7 = 48, b7 = 49, c7 = 50, d7 = 51, e7 = 52, f7 = 53, g7 = 54, h7 = 55,
        a8 = 56, b8 = 57, c8 = 58, d8 = 59, e8 = 60, f8 = 61, g8 = 62, h8 = 63
    };

    enum Piece_index : uint8_t{
        i_white = 0,
        i_black = 1,
        i_pawn = 2,
        i_knight = 3,
        i_bishop = 4,
        i_rook = 5,
        i_queen = 6,
        i_king = 7,
        i_empty = 8
    };

    enum Color : bool{
        c_white = 0,
        c_black = 1,
    };

    enum Move_type {
        NORMAL = 0,
        CASTLE = 1,
        EN_PASSANT = 2,
        PROMOTION = 3,
    };
};

struct Move {
    utils::Square_index  origin;
    utils::Square_index  destination;
    utils::Move_type type;
    utils::Piece_index promotion = utils::i_pawn;

    bool operator==(const Move& other) const {
        return (this->origin == other.origin) &&
               (this->destination == other.destination) &&
               (this->type == other.type);
    };
};

inline string squareToString(utils::Square_index square) {
    char file = 'a' + (square % 8);
    char rank = '1' + (square / 8);

    return string(1, file) + rank;
};

inline ostream& operator<<(ostream& os, const utils::Square_index square) {
    os << squareToString(square);
    return os;
};

inline ostream& operator<<(ostream& os, const Move& i) {
    if (i.promotion == utils::i_pawn) {
        os << "(" << i.origin << ", " << i.destination << ", " << i.type << ")";
    }
    else {
        os << "(" << i.origin << ", " << i.destination << ", " << i.type << ", " << int(i.promotion) << ")";
    }
    return os;
};

#endif //UTILS_H
