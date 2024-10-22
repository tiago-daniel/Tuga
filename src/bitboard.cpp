//
// Created by metta on 10/6/24.
//

#include "bitboard.h"

Bitboard::Bitboard() = default;

Bitboard::Bitboard(const unsigned int long long positions=0) {
    this->positions = positions;
}

void Bitboard::setBitboard(const unsigned int long long positions) {
    this->positions = positions;
}

bool Bitboard::hasBit(int bit) const {
    return positions & Bit(bit);
}

unsigned int long long Bitboard::getBitboard() const {
    return this->positions;
}

void Bitboard::removeBit(int bit) {
    this->positions &= ~Bit(bit);
}

void Bitboard::addBit(int bit) {
    this->positions |= Bit(bit);
}

void Bitboard::print() const {
    for (int row = 7; row >= 0; row--) {
        for (int col = 0; col < 8; col++) {
            int n = row * 8 + col;
            std::cout << (this->hasBit(n) ? "1" : "0") << " ";
        }
        std::cout << std::endl;
    }
}
