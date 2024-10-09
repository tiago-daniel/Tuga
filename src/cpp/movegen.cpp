//
// Created by metta on 10/6/24.
//

#include "h/movegen.h"

#include <set>

#include "h/bitboard.h"

moveGen::moveGen() = default;

void moveGen::movePiece(enumSquare initial_square, enumSquare target_square, Bitboard &board) {
    board.removeBit(initial_square);
    board.addBit(target_square);
}

vector<enumSquare> moveGen::pawnMove(enumSquare square, Color color, Bitboard board) {
    vector<enumSquare> squares = {};
    if (color == c_white and not (board.getBitboard() & Bit(square+8))) {
        squares.push_back(static_cast<enumSquare>(square+8));
        if (square<16 and square>=8 and not (board.getBitboard() & Bit(square+16))) {
            squares.push_back(static_cast<enumSquare>(square + 16));
        }
    }
    else if (not (board.getBitboard() & Bit(square-8))) {
        squares.push_back(static_cast<enumSquare>(square-8));
        if(square<(64-8) and square>=(64-16) and not (board.getBitboard() & Bit(square-16))) {
            squares.push_back(static_cast<enumSquare>(square-16));
        }
    }
    return squares;
}

vector<enumSquare> moveGen::knightMove(enumSquare square, Bitboard allies) {
    vector<pair<int, int>> directions = {
        {-2, -1}, {-2, 1},  // Two up, one left/right
        {-1, -2}, {-1, 2},  // One up, two left/right
        {1, -2}, {1, 2},    // One down, two left/right
        {2, -1}, {2, 1}     // Two down, one left/right
    };
    return slideMove(square, directions, false, allies);
}

vector<enumSquare> moveGen::bishopMove(enumSquare square, Bitboard allies, Bitboard enemies) {
    vector<pair<int, int>> directions = {
        {-1, -1},
        {-1, 1},
        {1, -1},
        {1, 1},
    };

    return slideMove(square, directions, true, allies, enemies);
}

vector<enumSquare> moveGen::rookMove(enumSquare square, Bitboard allies, Bitboard enemies) {
    vector<pair<int, int>> directions = {
        {0, -1},
        {-1, 0},
        {1, 0},
        {0, 1},
    };

    return slideMove(square, directions, true, allies, enemies);
}

vector<enumSquare> moveGen::queenMove(enumSquare square, Bitboard allies, Bitboard enemies) {
    vector<pair<int, int>> directions = {
        {-1, -1},
        {-1, 1},
        {1, -1},
        {1, 1},
        {0, -1},
        {-1, 0},
        {1, 0},
        {0, 1},
    };

    return slideMove(square, directions, true, allies, enemies);
}

vector<enumSquare> moveGen::kingMove(enumSquare square, Bitboard allies, Bitboard enemies) {
    vector<pair<int, int>> directions = {
        {-1, -1},
        {-1, 1},
        {1, -1},
        {1, 1},
        {0, -1},
        {-1, 0},
        {1, 0},
        {0, 1},
    };

    return slideMove(square, directions, false, allies);
}

vector<enumSquare> moveGen::slideMove(enumSquare square, vector<pair<int, int>> directions,
    bool slide, Bitboard allies, Bitboard enemies) {
    vector<enumSquare>squares ={};

    // Get the current rank and file
    int rank = square / 8; // Rows (0 to 7)
    int file = square % 8; // Columns (0 to 7)

    for (const auto& direction : directions) {
        int dRank = direction.first;
        int dFile = direction.second;
        int newRank = rank;
        int newFile = file;

        do {
            newRank += dRank;
            newFile += dFile;

            if (newRank < 0 || newRank >= 8 || newFile < 0 || newFile >= 8) {
                break;
            }
            if ( allies.getBitboard() & Bit((newRank * 8 + newFile))) {
                break;
            }
            squares.push_back(static_cast<enumSquare>(newRank * 8 + newFile));
            if (enemies.getBitboard() & Bit((newRank * 8 + newFile))) {
                break;
            }
        } while (slide);
    }
    return squares;
}