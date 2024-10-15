//
// Created by metta on 10/6/24.
//

#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "bitboard.h"
#include <array>


class MoveGen {
public:
    MoveGen();

    void pawnMove(std::array<Move, 256> &moves, Square square, Color color, Bitboard allies, Bitboard enemies, Square passant);

    void knightMove(std::array<Move, 256> &moves, Square square,Bitboard allies = Bitboard());

    void bishopMove(std::array<Move, 256> &moves, Square square,Bitboard allies = Bitboard(), Bitboard enemies = Bitboard());

    void rookMove(std::array<Move, 256> &moves, Square square,Bitboard allies = Bitboard(), Bitboard enemies = Bitboard());

    void queenMove(std::array<Move, 256> &moves, Square square,Bitboard allies = Bitboard(), Bitboard enemies = Bitboard());

    void kingMove(std::array<Move, 256> &moves, Square square, Bitboard allies = Bitboard());

    void castleMove(std::array<Move, 256> &moves, Square square,int castlingRights, Bitboard allies = Bitboard(),Bitboard enemies = Bitboard());

    void pieceMove(std::array<Move, 256> &moves, Square  square, std::vector<std::pair<int, int>> directions,
        bool slide, Bitboard allies = Bitboard(), Bitboard enemies = Bitboard());
};



#endif //MOVEGEN_H
