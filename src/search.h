//
// Created by metta on 10/22/24.
//

#ifndef SEARCH_H
#define SEARCH_H

#include <map>


#include "evaluation.h"

class Search {
public:
    static int negaMax(Position pos, int depth);
    static int negaMaxTest(Position pos, int depth, std::vector<int> &counts);
    static void runSearch(Position pos, int depth);
};



#endif //SEARCH_H
