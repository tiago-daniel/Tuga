//
// Created by metta on 10/6/24.
//

#ifndef BITBOARD_H
#define BITBOARD_H

#include <bits/stl_vector.h>
#include <vector>
#include "utils.h"

using namespace utils;

class Bitboard {
private:
    uint64_t positions = 0;
public:
    Bitboard();
    Bitboard(uint64_t positions);
    Bitboard(vector<enumSquare> squares);
    uint64_t getBitboard();
    void removeBit(uint8_t bit);
    void addBit(uint8_t bit);
    void print();
    void setBitboard(uint64_t positions);
};



#endif //BITBOARD_H
