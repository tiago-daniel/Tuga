//
// Created by metta on 10/6/24.
//

#include "h/movegen.h"
#include "h/utils.h"

using namespace std;
using namespace utils;

int main() {
    Bitboard board = Bitboard(Bit(a1)|Bit(a2)|Bit(a3)|Bit(a4)|Bit(a5));
    auto move_handler = moveGen();
    move_handler.move(a2,c2,board);
    board.print();
    return 0;
}
