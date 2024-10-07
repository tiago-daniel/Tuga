//
// Created by metta on 10/6/24.
//

#include "h/bitboard.h"

Bitboard::Bitboard() {
    this->positions = 0;
}

Bitboard::Bitboard(const uint64_t positions=0) {
    this->positions = positions;
}

void Bitboard::setBitboard(const uint64_t positions) {
    this->positions = positions;
}

uint64_t Bitboard::getBitboard() {
    return this->positions;
}

void Bitboard::removeBit(uint8_t bit) {
    this->positions &= (uint64_t)-1-Bit(bit);
}

void Bitboard::addBit(uint8_t bit) {
    this->positions |= Bit(bit);
}

void Bitboard::print() {
    for (int row = 7; row >= 0; row--) {
        for (int col = 0; col < 8; col++) {
            uint8_t n = row * 8 + col;
            cout << bool(this->getBitboard() & Bit(n)) << " ";
        }
        cout << endl;
    }
}
