//
// Created by metta on 10/6/24.
//

#ifndef BITBOARD_H
#define BITBOARD_H

#include <vector>
#include "utils.h"

class Bitboard {
    unsigned int long long positions = 0;
public:
    Bitboard();
    explicit Bitboard(unsigned int long long positions);
    [[nodiscard]] unsigned int long long getBitboard() const;
    void removeBit(int bit);
    void addBit(int bit);
    void print() const;
    unsigned int long long countBits(Bitboard mask) const;
    void setBitboard(unsigned int long long positions);
    [[nodiscard]] bool hasBit(int bit) const;
};



#endif //BITBOARD_H
