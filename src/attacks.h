//
// Created by metta on 10/27/24.
//

#ifndef ATTACKS_H
#define ATTACKS_H

#include "magic.h"

inline U64 bishopAttacks(U64 occ, Square sq) {
    U64* aptr = mBishopTbl[sq].ptr;
    occ      &= mBishopTbl[sq].mask;
    occ      *= mBishopTbl[sq].magic;
    occ     >>= mBishopTbl[sq].shift;
    return aptr[occ];
}

inline U64 rookAttacks(U64 occ, Square sq) {
    U64* aptr = mRookTbl[sq].ptr;
    occ      &= mRookTbl[sq].mask;
    occ      *= mRookTbl[sq].magic;
    occ     >>= mRookTbl[sq].shift;
    return aptr[occ];
}

inline U64 knightAttacks[64];
inline U64 kingAttacks[64];

inline void initKnightAttacks() {
    constexpr int knightMoves[8][2] = {
        {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
        {1, -2},  {1, 2},  {2, -1},  {2, 1}
    };

    for (int square = 0; square < 64; ++square) {
        U64 attacks = 0ULL;
        int rank = getRank(square);
        int file = getFile(square);

        for (const auto& move : knightMoves) {
            int newRank = rank + move[0];
            int newFile = file + move[1];

            if (isValidSquare(newRank, newFile)) {
                int targetSquare = newRank * 8 + newFile;
                attacks |= Bit(targetSquare);
            }
        }

        knightAttacks[square] = attacks;
    }
}

inline void initKingAttacks() {
    for (int square = 0; square < 64; ++square) {
        U64 attacks = 0ULL;
        int rank = square / 8;
        int file = square % 8;

        // King moves: iterate over all possible directions
        for (int dr = -1; dr <= 1; ++dr) {
            for (int df = -1; df <= 1; ++df) {
                if (dr == 0 && df == 0) {
                    continue; // Skip the current square
                }

                int newRank = rank + dr;
                int newFile = file + df;

                // Check if new position is within the board boundaries
                if (newRank >= 0 && newRank < 8 && newFile >= 0 && newFile < 8) {
                    int targetSquare = newRank * 8 + newFile;
                    attacks |= 1ULL << targetSquare;
                }
            }
        }

        kingAttacks[square] = attacks;
    }
}


inline void initAllAttackTables() {
    initKingAttacks();
    initKnightAttacks();
}

#endif //ATTACKS_H
