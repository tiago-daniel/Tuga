//
// Created by metta on 10/22/24.
//

#include "search.h"

int Search::negaMax(Position pos, int depth) {

    if (depth == 0) return Evaluation::evaluate(pos);

    int max = - 1000;

    for (int i = 0 ;i <  pos.allMoves(pos.getCurrentPlayer()).getSize(); i++) {
        auto move = pos.allMoves(pos.getCurrentPlayer()).getMoves()[i];
        pos.makeMove(move);
        int score = -negaMax(pos, depth - 1);
        pos.unmakeMove(move);
        if (score > max) max = score;
    }
    return max;
}