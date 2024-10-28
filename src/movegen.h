//
// Created by metta on 10/6/24.
//

#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "bitboard.h"
#include "movelist.h"
#include "attacks.h"
#include <span>
#include <vector>


class MoveGen {
public:

    static void pawnMove(MoveList &moves, Square square, bool color, Bitboard allies, Bitboard enemies, Square passant);

    static void knightMove(MoveList &moves, bool player, Square square, U64 allies);

    static void bishopMove(MoveList &moves, bool player, Square square, U64 allies, U64 enemies);

    static void rookMove(MoveList &moves, bool player, Square square, U64 allies, U64 enemies);

    static void queenMove(MoveList &moves, bool player, Square square, U64 allies, U64 enemies);

    static void kingMove(MoveList &moves, bool player, Square square, U64 allies = 0);

    static void castleMove(MoveList &moves, bool player, Square square, int castlingRights,
                           Bitboard allies = Bitboard(), Bitboard enemies = Bitboard());

    static void moveBitboard(MoveList &moves, int sq, U64 b,bool player,U64 ally = 0);

};



#endif //MOVEGEN_H
