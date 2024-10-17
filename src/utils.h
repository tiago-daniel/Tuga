//
// Created by metta on 10/6/24.
//

#ifndef UTILS_H
#define UTILS_H

#include <iostream>

    inline unsigned int long long Bit(int n) {
        return 1ULL << n;
    }
    enum Square {
        a1,  b1,  c1,  d1,  e1,  f1,  g1,  h1,
        a2,  b2,  c2,  d2,  e2,  f2,  g2,  h2,
        a3,  b3,  c3,  d3,  e3,  f3,  g3,  h3,
        a4,  b4,  c4,  d4,  e4,  f4,  g4,  h4,
        a5,  b5,  c5,  d5,  e5,  f5,  g5,  h5,
        a6,  b6,  c6,  d6,  e6,  f6,  g6,  h6,
        a7,  b7,  c7,  d7,  e7,  f7,  g7,  h7,
        a8,  b8,  c8,  d8,  e8,  f8,  g8,  h8
    };

    enum Piece{
        EMPTY,
        PAWN,
        KNIGHT,
        BISHOP,
        ROOK,
        QUEEN,
        KING
    };

    enum Color {
        WHITE,
        BLACK
    };

    enum MoveType {
        NORMAL,
        CASTLE,
        EN_PASSANT,
        PROMOTION
    };

struct Move {
    Square  origin;
    Square  destination;
    MoveType type;
    Piece promotion = PAWN;

    bool operator==(const Move& other) const {
        return (this->origin == other.origin) &&
               (this->destination == other.destination) &&
               (this->type == other.type);
    };
};

inline std::string squareToString(Square square) {
    char file = 'a' + (square % 8);
    char rank = '1' + (square / 8);

    return std::string(1, file) + rank;
};

inline std::ostream& operator<<(std::ostream& os, const Square square) {
    os << squareToString(square);
    return os;
};

inline std::ostream& operator<<(std::ostream& os, const Move& i) {
    if (i.promotion == PAWN) {
        os << "(" << i.origin << ", " << i.destination << ", " << i.type << ")";
    }
    else {
        os << "(" << i.origin << ", " << i.destination << ", " << i.type << ", " << int(i.promotion) << ")";
    }
    return os;
};

#endif //UTILS_H
