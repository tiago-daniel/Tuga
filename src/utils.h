//
// Created by metta on 10/6/24.
//

#ifndef UTILS_H
#define UTILS_H

#include <array>
#include <cctype>
#include <cstdint>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>

typedef uint64_t U64;
typedef uint8_t U8;

inline U64 Bit(int n) {
    return 1ULL << n;
}

// Check if a square is on the board
constexpr bool isValidSquare(int rank, int file) {
        return rank >= 0 && rank < 8 && file >= 0 && file < 8;
    }

inline uint64_t randomU64() {
    constexpr int rand = 0x1333317;
    static std::mt19937_64 mt(rand);
    std::uniform_int_distribution<uint64_t> dist{};
    return dist(mt);
}

inline uint64_t randomFewBits() {
    return randomU64() & randomU64() & randomU64();
}

// Get the rank from a square index
constexpr int getRank(int square) { return square / 8; }

// Get the file from a square index
constexpr int getFile(int square) {
        return square % 8;
    }

enum Square {
    a1,  b1,  c1,  d1,  e1,  f1,  g1,  h1,
    a2,  b2,  c2,  d2,  e2,  f2,  g2,  h2,
    a3,  b3,  c3,  d3,  e3,  f3,  g3,  h3,
    a4,  b4,  c4,  d4,  e4,  f4,  g4,  h4,
    a5,  b5,  c5,  d5,  e5,  f5,  g5,  h5,
    a6,  b6,  c6,  d6,  e6,  f6,  g6,  h6,
    a7,  b7,  c7,  d7,  e7,  f7,  g7,  h7,
    a8,  b8,  c8,  d8,  e8,  f8,  g8,  h8,
    noSquare = -1
};

inline Square squareIndex(int rank, int file) {
    return static_cast<Square>((rank << 3) | file); // Equivalent to rank * 8 + file
}

enum Direction {
    HORIZONTAL,
    VERTICAL,
    EVEN,
    ODD,
    NO_DIRECTION
};

enum Piece{
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    EMPTY
};

enum Color : bool {
    WHITE = false,
    BLACK = true
};

enum PieceCode {
    WHITE_PAWN,
    WHITE_KNIGHT,
    WHITE_BISHOP,
    WHITE_ROOK,
    WHITE_QUEEN,
    WHITE_KING,
    BLACK_PAWN,
    BLACK_KNIGHT,
    BLACK_BISHOP,
    BLACK_ROOK,
    BLACK_QUEEN,
    BLACK_KING
};

enum MoveType {
    NORMAL,
    CASTLE,
    EN_PASSANT,
    PROMOTION
};

struct Move {
    Square  origin;
    Square  destination;
    bool player;
    MoveType type;
    Piece promotion = PAWN;

    bool operator==(const Move& other) const {
        return (this->origin == other.origin) &&
               (this->destination == other.destination) &&
               (this->promotion == other.promotion);
    }
};

struct StackType {
    Square passant = noSquare;
    int castling_rights = 0b1111;
    Piece captured = EMPTY;
};

inline std::string squareToString(Square square) {
    char file = static_cast<char>('a' + (square % 8));
    char rank = static_cast<char>('1' + (square / 8));

    return std::string(1, file) + rank;
};

inline std::string pieceToString(Piece piece) {
    switch (piece) {
        case PAWN:
            return "Pawn";
        case KNIGHT:
            return "Knight";
        case BISHOP:
            return "Bishop";
        case ROOK:
            return "Rook";
        case QUEEN:
            return "Queen";
        case KING:
            return "King";
        default:
            return "Unknown Piece";
    }
}

inline std::array values{1, 3, 3, 5, 9, 100, 0};

inline Square string_to_square(const std::string& square_str) {
    if (square_str.length() != 2) {
        throw std::invalid_argument("Invalid square string: must be two characters long.");
    }

    // Convert file character to lowercase to handle uppercase inputs
    char file_char = std::tolower(square_str[0]);
    char rank_char = square_str[1];

    // Validate file character ('a' to 'h')
    if (file_char < 'a' || file_char > 'h') {
        throw std::invalid_argument("Invalid file character in square string.");
    }
    int file = file_char - 'a';


    // Validate rank character ('1' to '8')
    if (rank_char < '1' || rank_char > '8') {
        throw std::invalid_argument("Invalid rank character in square string.");
    }
    int rank = rank_char - '1';

    // Calculate square index (0 to 63)
    int square = rank * 8 + file;

    return static_cast<Square>(square);
}

inline Piece stringToPiece(const char& pieceStr) {
    switch (pieceStr) {
        case 'n':
            return KNIGHT;
        case 'b':
            return BISHOP;
        case 'r':
            return ROOK;
        case 'q':
            return QUEEN;
        default:
            return PAWN;
    }
}

inline std::ostream& operator<<(std::ostream& os, const Square square) {
    os << squareToString(square);
    return os;
};

inline std::ostream& operator<<(std::ostream& os, const Move& i) {
    if (i.promotion == PAWN) {
        os << i.origin << i.destination;
    }
    else {
        switch (i.promotion) {
            case KNIGHT:
                os << i.origin << i.destination << "n";
                break;
            case BISHOP:
                os << i.origin << i.destination << "b";
                break;
            case ROOK:
                os << i.origin << i.destination << "r";
                break;
            case QUEEN:
                os << i.origin << i.destination << "q";
                break;
            default:
                break;
        }
    }
    return os;
};

inline std::string printMove(const Move &i) {
    if (i.promotion == PAWN) {
        return squareToString(i.origin) + " " + squareToString(i.destination);
    }
    return squareToString(i.origin) + " " + squareToString(i.destination) + " " + pieceToString(i.promotion);
}

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

constexpr uint64_t BISHOP_MAGICS[64] = {
    9227928559077564936ULL, 1162109040952623138ULL, 1157584534511026466ULL, 2452245731778494500ULL,
    4611985360619569440ULL, 10523225691750662144ULL, 172267170032914432ULL, 565329382350848ULL,
    576465219103496452ULL, 4983807033859ULL, 4483954638848ULL, 1152940213492777472ULL,
    1169313236993ULL, 4609771496275968ULL, 9225626070118843480ULL, 18015516313523200ULL,
    7512004219826869248ULL, 11553806833745984ULL, 4688247246457280544ULL, 1125943947165712ULL,
    309903984255565825ULL, 869335882191126528ULL, 1153489557268809729ULL, 144398046036304912ULL,
    9296485162592768512ULL, 36595050070943744ULL, 2253999978193408ULL, 291371252465920ULL,
    567348167737864ULL, 2311473917514031106ULL, 13984248915485069312ULL, 10376371624239958532ULL,
    865852521973940752ULL, 180293930993582617ULL, 82335846989037632ULL, 1299887177115107840ULL,
    9385503831053338752ULL, 576759849002927104ULL, 36469726951573508ULL, 9234640111189655688ULL,
    1164189310776838211ULL, 7080222131652396032ULL, 288516524623335426ULL, 10376862272575258752ULL,
    10522167965458106624ULL, 9332057180787966722ULL, 4611353981487360ULL, 40677534397913224ULL,
    2816193447591956ULL, 218464241824352257ULL, 9223443522844360968ULL, 39587261448256ULL,
    82261139286786048ULL, 4467336546624ULL, 4701760493499582596ULL, 1189575382643262720ULL,
    4611714608145895424ULL, 576531400292835908ULL, 578149606525830144ULL, 74340609691240448ULL,
    23643966830609408ULL, 2251834441943556ULL, 2338564490054996482ULL, 770125500647604292ULL
};

constexpr uint64_t ROOK_MAGICS[64] = {
    180145635706552322ULL, 9241386572804259904ULL, 4683753508340498496ULL, 144119621036818944ULL,
    144125152469189120ULL, 11565246044262433792ULL, 1765413253229322496ULL, 72057809893754112ULL,
    1153062242640478208ULL, 18366310954115072ULL, 2882444636452818945ULL, 281543698350088ULL,
    18155685820762112ULL, 9223653550486324224ULL, 289356344812717096ULL, 6485746552970806340ULL,
    9259401109288534016ULL, 5044315257401524736ULL, 1155604863271374848ULL, 282574690713633ULL,
    9223654611477333008ULL, 72198881315652608ULL, 364795968274702608ULL, 9223374236164325444ULL,
    4791865739797610496ULL, 18014950414876736ULL, 11276593404052608ULL, 281625301094400ULL,
    436857962095706241ULL, 2201179128832ULL, 3459063666883184418ULL, 864701049832751236ULL,
    70371034267684ULL, 225215234464284736ULL, 140806216228865ULL, 4620702015998136320ULL,
    146371388091926528ULL, 2306124501387051528ULL, 282608915189954ULL, 72066944819265732ULL,
    4612319339272634402ULL, 216454534149570592ULL, 9007269064802336ULL, 2450244104941010976ULL,
    2251816994111616ULL, 288793532331589640ULL, 562954382737412ULL, 613052933210243076ULL,
    37385559626240ULL, 633037497804357888ULL, 9804337832431485312ULL, 563019747172864ULL,
    2253998904066112ULL, 1153484489197126144ULL, 1152965765587141632ULL, 9011598383448576ULL,
    576496795954348033ULL, 4616259987069501699ULL, 2355945624861941890ULL, 4521191897303073ULL,
    54324739258712069ULL, 649081313743601922ULL, 14148060430825752580ULL, 2324425310180999202ULL
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

inline U64 attack_table[128][840];

struct SMagic {
    U64 *ptr; // pointer to attack_table for each particular square
    U64 mask; // to mask relevant squares of both lines (no outer squares)
    U64 magic; // magic 64-bit factor
    int shift; // shift right
};

inline SMagic mBishopTbl[64];
inline SMagic mRookTbl[64];

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

#endif //UTILS_H
