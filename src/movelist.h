//
// Created by metta on 10/17/24.
//

#ifndef MOVELIST_H
#define MOVELIST_H

#include "utils.h"
#include <array>


class MoveList {
std::array<Move,256> moves;
int size = 0;

public:
    MoveList();
    void push(Move move);
    [[nodiscard]] int getSize() const;
    [[nodiscard]] bool isEmpty() const;
    void remove(Move move);
    [[nodiscard]] std::array<Move, 256>& getMoves();
    void clear();
    [[nodiscard]] bool has(Move move) const;
    Move findMove(Square origin, Square destination, Piece promotion = PAWN) const;
};



#endif //MOVELIST_H
