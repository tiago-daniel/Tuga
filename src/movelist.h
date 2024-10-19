//
// Created by metta on 10/17/24.
//

#ifndef MOVELIST_H
#define MOVELIST_H

#include "utils.h"
#include <array>
#include <span>


class MoveList {
std::array<Move,256> moves;
int size = 0;

public:
    MoveList();
    void push(Move move);
    int getSize() const;
    bool isEmpty() const;
    void remove(Move move);
    std::array<Move, 256> getMoves() const;
    void clear();
    bool has(Move move) const;
};



#endif //MOVELIST_H
