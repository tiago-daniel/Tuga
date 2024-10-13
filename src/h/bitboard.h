//
// Created by metta on 10/6/24.
//

#ifndef BITBOARD_H
#define BITBOARD_H

#include <array>
#include <vector>
#include "utils.h"

using namespace utils;
using namespace std;

class Bitboard {
private:
    uint64_t positions = 0;
public:
    Bitboard();
    explicit Bitboard(uint64_t positions);
    explicit Bitboard(vector<Square_index> squares);
    uint64_t getBitboard() const;
    void removeBit(uint8_t bit);
    void addBit(uint8_t bit);
    void print();
    void setBitboard(uint64_t positions);
};



#endif //BITBOARD_H
