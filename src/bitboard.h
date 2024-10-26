//
// Created by metta on 10/6/24.
//

#ifndef BITBOARD_H
#define BITBOARD_H

#include <vector>
#include "utils.h"

class Bitboard {
    U64 positions = 0;
public:
    Bitboard();
    explicit Bitboard(U64 positions);
    [[nodiscard]] U64 getBitboard() const;
    void removeBit(int bit);
    void addBit(int bit);
    void print() const;
    [[nodiscard]] U64 countBits(Bitboard mask) const;
    void setBitboard(U64 positions);
    [[nodiscard]] bool hasBit(int bit) const;
};



#endif //BITBOARD_H
