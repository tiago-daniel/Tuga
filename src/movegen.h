//
// Created by metta on 10/6/24.
//

#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "bitboard.h"
#include "movelist.h"
#include <span>
#include <vector>


class MoveGen {
    static U64 knightAttacks[64];
    static U64 kingAttacks[64];
public:
    static void initKnightAttacks();
    static void initKingAttacks();
    static void initAllAttackTables();

    static void pawnMove(MoveList &moves, Square square, bool color, Bitboard allies, Bitboard enemies, Square passant);

    static void knightMove(MoveList &moves, bool player, Square square,Bitboard allies = Bitboard());

    static void bishopMove(MoveList &moves, bool player, Square square,Bitboard allies = Bitboard(), Bitboard enemies = Bitboard());

    static void rookMove(MoveList &moves, bool player, Square square,Bitboard allies = Bitboard(), Bitboard enemies = Bitboard());

    static void queenMove(MoveList &moves, bool player, Square square,Bitboard allies = Bitboard(), Bitboard enemies = Bitboard());

    static void kingMove(MoveList &moves, bool player, Square square, Bitboard allies = Bitboard());

    static void castleMove(MoveList &moves, bool player, Square square,int castlingRights, Bitboard allies = Bitboard(),Bitboard enemies = Bitboard());

    static void pieceMove(MoveList &moves, bool player, Square  square, std::span<std::pair<int, int>> directions,
        bool slide, Bitboard allies = Bitboard(), Bitboard enemies = Bitboard());
};



#endif //MOVEGEN_H
