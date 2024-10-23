//
// Created by metta on 10/22/24.
//

#include "search.h"
#include <map>

inline int global = 0;

int Search::negaMax(Position pos, int depth) {
    int count = 0;
    if (depth == 0) return Evaluation::evaluate(pos);

    int max = - 1000;

    for (int i = 0 ;i <  pos.allMoves(pos.getCurrentPlayer()).getSize(); i++) {
        count++;
        auto move = pos.allMoves(pos.getCurrentPlayer()).getMoves()[i];
        pos.makeMove(move);
        int score = -negaMax(pos, depth - 1);
        pos.unmakeMove(move);
        if (score > max) max = score;
    }
    std::cout << "Depth " << 4 - depth << " : " << count << std::endl;
    return max;
}

int Search::negaMaxTest(Position pos, int depth, std::vector<int>& counts) {
    if (depth == 0) return Evaluation::evaluate(pos);

    int max = -1000;
    int currentDepth = counts.size() - depth; // Depth level we're currently in
    int count = 0; // Track moves explored at this depth

    for (int i = 0; i < pos.allMoves(pos.getCurrentPlayer()).getSize(); i++) {
        if (depth == counts.size()) {
            std::cout << pos.allMoves(pos.getCurrentPlayer()).getMoves()[i] << " : ";
        }
        count+=1;
        auto move = pos.allMoves(pos.getCurrentPlayer()).getMoves()[i];
        // pos.makeMove(move);
        int score = -negaMaxTest(pos, depth - 1, counts); // Recursive call
        // pos.unmakeMove(move);
        if (depth == counts.size()) {
            if (counts[counts.size()-1] == 0) std::cout <<  1 << std::endl;
            else std::cout << counts[counts.size()-1] - global << std::endl;
            global += counts[counts.size()-1];
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