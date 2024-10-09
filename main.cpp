//
// Created by metta on 10/6/24.
//

#include <cassert>
#include "h/movegen.h"
#include "h/utils.h"

using namespace std;
using namespace utils;

void testEmptyBitboard() {
    Bitboard b;
    assert(b.getBitboard()==0);
    cout << "EmptyBitboard passed!" << endl;
}

void testPositionsBitboard() {
    auto b = Bitboard(1605);
    assert(b.getBitboard()==1605);
    cout << "PositionsBitboard passed!" << endl;
}

void testComplexBitboard() {
    vector squares = {a2,b2,b3,c4,d5,b6,b7,b8,f3};
    auto b = Bitboard(squares);
    assert(b.getBitboard()==144680371481608960);
    cout << "ComplexBitboard passed!" << endl;
}
void testSetBitboard() {
    Bitboard b;
    assert(b.getBitboard()==0);
    b.setBitboard(10);
    assert(b.getBitboard()==10);
    b.setBitboard(1805);
    assert(b.getBitboard()==1805);
    cout << "SetBitboard passed!" << endl;
}

void testMove() {
    auto b = Bitboard(vector{a2});
    auto move_handler = moveGen();
    assert(b.getBitboard()==Bit(a2));
    move_handler.move(a2,b2,b);
    move_handler.move(b2,c2,b);
    move_handler.move(a2,d2,b);
    assert(b.getBitboard()==Bit(c2));
    cout << "Move passed!" << endl;
}

void testPawnMove() {
    auto move_handler = moveGen();
    vector<enumSquare> squares = move_handler.pawnMove(b2,c_white);
    auto b = Bitboard(squares);
    assert(b.getBitboard()==Bit(b3)|Bit(b4));
    squares = move_handler.pawnMove(b2,c_black);
    b = Bitboard(squares);
    assert(b.getBitboard()==Bit(b1));
    cout << "PawnMove passed!" << endl;
}

int main() {
    testEmptyBitboard();
    testPositionsBitboard();
    testComplexBitboard();
    testSetBitboard();
    testMove();
    testPawnMove();
    return 0;
}