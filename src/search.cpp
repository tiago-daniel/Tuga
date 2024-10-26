//
// Created by metta on 10/22/24.
//

#include "search.h"
#include <map>

inline int global = 0;

double Search::negaMax(Position& pos, int depth) {
    int count = 0;
    if (depth == 0) return Evaluation::evaluate(pos);

    double max = - 10000;

    auto moves = pos.allMoves(pos.getCurrentPlayer());
    for (int i = 0 ;i <  moves.getSize(); i++) {
        count++;
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
    int i;
    U64 nodes = 0;
    auto moves = pos.allMoves(pos.getCurrentPlayer());
    if (depth == 0) return 1ULL;
    for (i = 0 ;i <  moves.getSize(); i++) {
        pos.makeMove(moves.getMoves()[i]);
        nodes += preft(pos, depth - 1);
        pos.unmakeMove(moves.getMoves()[i]);
    }
    return nodes;
}

int Search::negaMaxTest(Position& pos, int depth, std::vector<int>& counts) {
    if (depth == 0) return Evaluation::evaluate(pos);

    int max = -1000;
    int currentDepth = counts.size() - depth;
    int count = 0; // Track moves explored at this depth

    for (int i = 0; i < pos.allMoves(pos.getCurrentPlayer()).getSize(); i++) {
        if (depth == counts.size()) {
            std::cout << pos.allMoves(pos.getCurrentPlayer()).getMoves()[i] << " : ";
        }
        count+=1;
        auto move = pos.allMoves(pos.getCurrentPlayer()).getMoves()[i];
        pos.makeMove(move);
        int score = -negaMaxTest(pos, depth - 1, counts); // Recursive call
        pos.unmakeMove(move);
        if (depth == counts.size()) {
            if (counts[counts.size()-1] == 0) std::cout <<  1 << std::endl;
            else if (counts[counts.size()-1] - global < 0) std::cout <<  counts[counts.size()-1] << std::endl;
            else std::cout << counts[counts.size()-1] - global << std::endl;
            global = counts[counts.size()-1];
        }
        if (score > max) max = score;
    }

    counts[currentDepth] += count; // Accumulate moves count for this depth
    return max;
}

void Search::runSearch(Position pos, int depth) {
    std::vector counts(depth, 0); // To store move counts for each depth
    negaMaxTest(pos, depth, counts);

    // After the search is complete, print the move counts for each depth
    for (int i = 0; i < depth; i++) {
        std::cout << "Depth " << i + 1 << " : " << counts[i] << " moves" << std::endl;
    }
}