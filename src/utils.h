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

#endif //UTILS_H
