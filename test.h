//
// Created by metta on 10/6/24.
//

#ifndef TEST_H
#define TEST_H

#include <cassert>
#include "h/movegen.h"
#include "h/utils.h"

using namespace std;
using namespace utils;
namespace test {
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
        move_handler.movePiece(a2,b2,b);
        move_handler.movePiece(b2,c2,b);
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

    void testBishopMove() {
        auto move_handler = moveGen();
        auto squares = move_handler.bishopMove(e4);
        auto b = Bitboard(squares);
        assert(b.getBitboard()==108724279602332802);
        cout << "BishopMove passed!" << endl;
    }

    void testKnightMove() {
        auto move_handler = moveGen();
        auto squares = move_handler.knightMove(e4);
        auto b = Bitboard(squares);
        assert(b.getBitboard()==44272527353856);
        cout << "KnightMove passed!" << endl;
    }

    void testRookMove() {
        auto move_handler = moveGen();
        auto squares = move_handler.rookMove(e4);
        auto b = Bitboard(squares);
        assert(b.getBitboard()==1157442769150545936);
        cout << "RookMove passed!" << endl;
    }

    void testQueenMove() {
        auto move_handler = moveGen();
        auto squares = move_handler.queenMove(e4);
        auto b = Bitboard(squares);
        assert(b.getBitboard()==1266167048752878738);
        cout << "QueenMove passed!" << endl;
    }

    void testKingMove() {
        auto move_handler = moveGen();
        auto squares = move_handler.kingMove(e4);
        auto b = Bitboard(squares);
        assert(b.getBitboard()==241192927232);
        cout << "KingMove passed!" << endl;
    }



    int test() {
        testEmptyBitboard();
        testPositionsBitboard();
        testComplexBitboard();
        testSetBitboard();
        testMove();
        testPawnMove();
        testBishopMove();
        testKnightMove();
        testRookMove();
        testQueenMove();
        testKingMove();
        cout << "All tests passed!" << endl;
        return 0;
    }
}

#endif //TEST_H