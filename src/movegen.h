//
// Created by metta on 10/6/24.
//

#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "bitboard.h"
#include "movelist.h"
#include <array>
#include <span>
#include <vector>


class MoveGen {
public:
    MoveGen();

    void pawnMove(MoveList &moves, Square square, bool color, Bitboard allies, Bitboard enemies, Square passant);

    void knightMove(MoveList &moves, bool player, Square square,Bitboard allies = Bitboard());

    void bishopMove(MoveList &moves, bool player, Square square,Bitboard allies = Bitboard(), Bitboard enemies = Bitboard());

    void rookMove(MoveList &moves, bool player, Square square,Bitboard allies = Bitboard(), Bitboard enemies = Bitboard());

    void queenMove(MoveList &moves, bool player, Square square,Bitboard allies = Bitboard(), Bitboard enemies = Bitboard());

    void kingMove(MoveList &moves, bool player, Square square, Bitboard allies = Bitboard());

    void castleMove(MoveList &moves, bool player, Square square,int castlingRights, Bitboard allies = Bitboard(),Bitboard enemies = Bitboard());

    void pieceMove(MoveList &moves, bool player, Square  square, std::span<std::pair<int, int>> directions,
        bool slide, Bitboard allies = Bitboard(), Bitboard enemies = Bitboard());
};



#endif //MOVEGEN_H
