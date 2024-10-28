//
// Created by metta on 10/27/24.
//

#ifndef MAGIC_H
#define MAGIC_H
#include "utils.h"

constexpr int BISHOP_BITS[64] = {
    58, 59, 59, 59, 59, 59, 59, 58,
    59, 59, 59, 59, 59, 59, 59, 59,
    59, 59, 57, 57, 57, 57, 59, 59,
    59, 59, 57, 55, 55, 57, 59, 59,
    59, 59, 57, 55, 55, 57, 59, 59,
    59, 59, 57, 57, 57, 57, 59, 59,
    59, 59, 59, 59, 59, 59, 59, 59,
    58, 59, 59, 59, 59, 59, 59, 58
};

constexpr int ROOK_BITS[64] = {
    52, 53, 53, 53, 53, 53, 53, 52,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    52, 53, 53, 53, 53, 53, 53, 52
};


inline U64 rookMask(int square) {
    uint64_t mask = 0ULL;
    int rank = square / 8;
    int file = square % 8;
    for (int i = 1; i < 7; i++) {
        if (i != file) {
            mask |= (1ULL << (rank * 8 + i));
        }
    }
    for (int i = 1; i < 7; i++) {
        if (i != rank) {
            mask |= (1ULL << (i * 8 + file));
        }
    }
    return mask;
}

inline int countBits(U64 b) {
    int r;
    for(r = 0; b; r++, b &= b - 1);
    return r;
}


inline int pop_1st_bit(U64 *b) {
    if (*b == 0) {
        return -1;                      // No bits set, return an invalid index
    }
    int index = __builtin_ctzll(*b);    // Count trailing zeros to find the index of LSB set to 1
    *b &= (*b - 1);                     // Clear the least significant bit
    return index;
}

inline U64 index_to_uint64(int index, int bits, U64 m) {
    int i, j;
    U64 result = 0ULL;
    for(i = 0; i < bits; i++) {
        j = pop_1st_bit(&m);
        if(index & (1 << i)) result |= (1ULL << j);
    }
    return result;
}


inline U64 bishopMask(int square) {
    U64 result = 0ULL;
    int rank = square/8, file = square % 8, r, f;
    for(r=rank+1, f=file+1; r<=6 && f<=6; r++, f++) result |= (1ULL << (f + r*8));
    for(r=rank+1, f=file-1; r<=6 && f>=1; r++, f--) result |= (1ULL << (f + r*8));
    for(r=rank-1, f=file+1; r>=1 && f<=6; r--, f++) result |= (1ULL << (f + r*8));
    for(r=rank-1, f=file-1; r>=1 && f>=1; r--, f--) result |= (1ULL << (f + r*8));
    return result;
}

struct SMagic {
    U64 *ptr; // pointer to attack_table for each particular square
    U64 mask; // to mask relevant squares of both lines (no outer squares)
    U64 magic; // magic 64-bit factor
    int shift; // shift right
};

inline U64 rookAttack(int square, U64 block) {
    U64 result = 0ULL;
    int rank = square / 8, file = square % 8, r, f;
    for(r = rank+1; r <= 7; r++) {
        result |= (1ULL << (file + r*8));
        if(block & (1ULL << (file + r*8))) break;
    }
    for(r = rank-1; r >= 0; r--) {
        result |= (1ULL << (file + r*8));
        if(block & (1ULL << (file + r*8))) break;
    }
    for(f = file+1; f <= 7; f++) {
        result |= (1ULL << (f + rank*8));
        if(block & (1ULL << (f + rank*8))) break;
    }
    for(f = file-1; f >= 0; f--) {
        result |= (1ULL << (f + rank*8));
        if(block & (1ULL << (f + rank*8))) break;
    }
    return result;
}

inline U64 bishopAttack(int square, U64 block) {
    U64 result = 0ULL;
    int rank = square / 8, file = square % 8, r, f;
    for(r = rank+1, f = file + 1; r <= 7 && f <= 7; r++, f++) {
        result |= (1ULL << (f + r * 8));
        if(block & (1ULL << (f + r * 8))) break;
    }
    for(r = rank+1, f = file - 1; r <= 7 && f >= 0; r++, f--) {
        result |= (1ULL << (f + r * 8));
        if(block & (1ULL << (f + r * 8))) break;
    }
    for(r = rank-1, f = file + 1; r >= 0 && f <= 7; r--, f++) {
        result |= (1ULL << (f + r * 8));
        if(block & (1ULL << (f + r * 8))) break;
    }
    for(r = rank-1, f = file - 1; r >= 0 && f >= 0; r--, f--) {
        result |= (1ULL << (f + r * 8));
        if(block & (1ULL << (f + r * 8))) break;
    }
    return result;
}

inline SMagic mBishopTbl[64];
inline SMagic mRookTbl[64];

inline U64 rAttOcc[64][4096];
inline U64 bAttOcc[64][4096];

inline U64 find_magic(int sq, int bishop) {
    U64 mask, b[4096], a[4096], magic;
    U64* used = bishop ? bAttOcc[sq] : rAttOcc[sq];
    int i, j, k, n, fail;
    int m = bishop ? BISHOP_BITS[sq] : ROOK_BITS[sq];
    mask = bishop ? bishopMask(sq) : rookMask(sq);
    n = countBits(mask);

    for(i = 0; i < (1 << n); i++) {
        b[i] = index_to_uint64(i, n, mask);
        a[i] = bishop ?  bishopAttack(sq, b[i]) : rookAttack(sq, b[i]);
    }
    for(k = 0; k < 100000000; k++) {
        magic = randomFewBits();
        if(countBits((mask * magic) & 0xFF00000000000000ULL) < 6) continue;
        for(i = 0; i < 4096; i++) used[i] = 0ULL;
        for(i = 0, fail = 0; !fail && i < (1 << n); i++) {
            j = static_cast<int>((b[i] * magic) >> m);
            if(used[j] == 0ULL) used[j] = a[i];
            else if(used[j] != a[i]) fail = 1;
        }
        if(!fail) {
            if (bishop) {
                mBishopTbl[sq] = SMagic{bAttOcc[sq], mask, magic, m};
            }
            else {
                mRookTbl[sq] = SMagic{rAttOcc[sq], mask, magic, m};
            }
            return magic;
        }
    }
    return 0ULL;
}

#endif //MAGIC_H
