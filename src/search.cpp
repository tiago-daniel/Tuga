//
// Created by metta on 10/22/24.
//

#include "search.h"
#include <map>

inline int global = 0;

double Search::negaMax(Position& pos, int depth) {
    if (pos.getResult() != 2) {
        if (pos.getCurrentPlayer() == WHITE) {
            return pos.getResult() * 1000.0;
        }
        return  - pos.getResult() * 1000.0;
    }
    if (depth == 0) return Evaluation::evaluate(pos);
    double max = - 10000;

    auto moves = pos.allMoves(pos.getCurrentPlayer());
    for (int i = 0 ;i <  moves.getSize(); i++) {
        auto move = moves.getMoves()[i];
        pos.makeMove(move);
        double score = -negaMax(pos, depth - 1);
        pos.unmakeMove(move);
        if (score > max) max = score;
    }
    return max;
}

Move Search::rootNegaMax(Position& pos, int depth) {
    Move bestMove;
    int count = 0;

    double max = - 10000;

    for (int i = 0 ;i <  pos.allMoves(pos.getCurrentPlayer()).getSize(); i++) {
        count++;
        auto move = pos.allMoves(pos.getCurrentPlayer()).getMoves()[i];
        pos.makeMove(move);
        double score = -negaMax(pos, depth - 1);
        pos.unmakeMove(move);
        if (score > max) {
            max = score;
            bestMove = pos.allMoves((pos.getCurrentPlayer())).getMoves()[i];
        }
    }
    assert(bestMove != Move());
    return bestMove;
}

U64 Search::preft(Position& pos, int depth) {
    U64 nodes = 0;
    if (depth == 0) return 1ULL;

    auto moves = pos.allMoves(pos.getCurrentPlayer());
    for (int i = 0 ;i <  moves.getSize(); i++) {
        pos.makeMove(moves.getMoves()[i]);
        nodes += preft(pos, depth - 1);
        pos.unmakeMove(moves.getMoves()[i]);
    }
    return nodes;
}

void Search::runSearch(Position pos, int depth) {

    U64 nodes = 0;
    U64 curr;

    auto moves = pos.allMoves(pos.getCurrentPlayer());
    for (int i = 0 ;i <  moves.getSize(); i++) {
        curr = 0;
        auto move = moves.getMoves()[i];
        pos.makeMove(move);
        curr =  preft(pos, depth - 1);
        pos.unmakeMove(move);
        std::cout << move << " - " <<  curr << std::endl;
        nodes += curr;
    }
    std::cout << "Nodes " << " : " << nodes << std::endl;
}