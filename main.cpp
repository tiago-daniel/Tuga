//
// Created by metta on 10/6/24.
//

#include "h/movelist.h"
#include "h/utils.h"

using namespace std;
using namespace utils;

int main() {
    auto board = Bitboard(Bit(33));
    board.removeBit(33);
    board.print();
    return 0;
}
