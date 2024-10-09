//
// Created by metta on 10/6/24.
//

#include "h/movegen.h"

#include <set>

#include "h/bitboard.h"

moveGen::moveGen() = default;

void moveGen::move(enumSquare initial_square, enumSquare target_square, Bitboard &board) {
    if (board.getBitboard() & Bit(initial_square)) {
        board.removeBit(initial_square);
        board.addBit(target_square);
    }
}

vector<enumSquare> moveGen::pawnMove(enumSquare square, Color color) {
    vector<enumSquare> squares = {};
    if (color == c_white) {
        squares.push_back(static_cast<enumSquare>(square+8));
        if (square<16 and square>=8) {
            squares.push_back(static_cast<enumSquare>(square + 16));
        }
    }
    else {
        squares.push_back(static_cast<enumSquare>(square-8));
        if(square<(64-8) and square>=(64-16)) {
            squares.push_back(static_cast<enumSquare>(square-16));
        }
    }
    return squares;
}

vector<enumSquare> moveGen::knightMove(enumSquare square) {
    vector<enumSquare>squares ={};
    // Get the current rank and file
    int rank = square / 8; // Rows (0 to 7)
    int file = square % 8; // Columns (0 to 7)

    vector<pair<int, int>> knightMoves = {
        {-2, -1}, {-2, 1},  // Two up, one left/right
        {-1, -2}, {-1, 2},  // One up, two left/right
        {1, -2}, {1, 2},    // One down, two left/right
        {2, -1}, {2, 1}     // Two down, one left/right
    };
    for (const auto& move : knightMoves) {
        int newRank = rank + move.first;
        int newFile = file + move.second;

        // Check if the new position is within bounds
        if (newRank >= 0 && newRank < 8 && newFile >= 0 && newFile < 8) {
            // Add the new square to possible moves
            squares.push_back(static_cast<enumSquare>(newRank * 8 + newFile));
        }
    }
    return squares;
}

vector<enumSquare> moveGen::bishopMove(enumSquare square) {
    vector<enumSquare>squares ={};

    // Get the current rank and file
    int rank = square / 8; // Rows (0 to 7)
    int file = square % 8; // Columns (0 to 7)

    vector<pair<int, int>> directions = {
        {-1, -1}, // Top-Left
        {-1, 1},  // Top-Right
        {1, -1},  // Bottom-Left
        {1, 1}    // Bottom-Right
    };

    for (const auto& direction : directions) {
        int dRank = direction.first;
        int dFile = direction.second;
        int newRank = rank;
        int newFile = file;

        while (true) {
            newRank += dRank;
            newFile += dFile;

            if (newRank < 0 || newRank >= 8 || newFile < 0 || newFile >= 8) {
                break;
            }

            squares.push_back(static_cast<enumSquare>(newRank * 8 + newFile));
        }
    }
    return squares;
}

vector<enumSquare> moveGen::rookMove(enumSquare square) {
    vector<enumSquare>squares ={};

    // Get the current rank and file
    int rank = square / 8; // Rows (0 to 7)
    int file = square % 8; // Columns (0 to 7)

    for (int i = 0; i < 8; i++) {
        if (i != file) {
            squares.push_back(static_cast<enumSquare>(rank * 8 + i)); // Add valid squares
        }
        if (i != rank) {
            squares.push_back(static_cast<enumSquare>(i * 8 + file));
        }
    }
    return squares;
}

vector<enumSquare> moveGen::queenMove(enumSquare square) {
    vector<enumSquare>squares =rookMove(square);
    vector<enumSquare> bishop = bishopMove(square);
    for (auto i : bishop) {
        squares.push_back(i);
    }
    return squares;
}

vector<enumSquare> moveGen::kingMove(enumSquare square) {
    vector<enumSquare>squares ={};

    // Get the current rank and file
    int rank = square / 8; // Rows (0 to 7)
    int file = square % 8; // Columns (0 to 7)
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

    for (const auto& direction : directions) {
        int dRank = direction.first;
        int dFile = direction.second;
        int newRank = rank;
        int newFile = file;

        newRank += dRank;
        newFile += dFile;
        if (newRank < 0 || newRank >= 8 || newFile < 0 || newFile >= 8) {
            continue;
        }
            squares.push_back(static_cast<enumSquare>(newRank * 8 + newFile));
        }
    return squares;
}