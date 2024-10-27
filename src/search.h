//
// Created by metta on 10/22/24.
//

#ifndef SEARCH_H
#define SEARCH_H

#include <map>


#include "evaluation.h"

class Search {
public:
    static double negaMax(Position &pos, int depth);
    static Move rootNegaMax(Position &pos, int depth);
    static U64 preft(Position &pos, int depth);
    static U64 preftBulk(Position &pos, int depth);
    static int negaMaxTest(Position &pos, int depth, std::vector<int> &counts);
    static void runSearch(Position pos, int depth);
};



#endif //SEARCH_H
