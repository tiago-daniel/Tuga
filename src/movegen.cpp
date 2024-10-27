//
// Created by metta on 10/6/24.
//

#include "movegen.h"

U64 MoveGen::knightAttacks[64];
U64 MoveGen::kingAttacks[64];

void MoveGen::initKnightAttacks() {
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

void MoveGen::initKingAttacks() {
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


void MoveGen::initAllAttackTables() {
    initKingAttacks();
    initKnightAttacks();
}


void MoveGen::pawnMove(MoveList &moves, Square square, bool color, Bitboard allies, Bitboard enemies, Square passant) {
    std::array pieces = {QUEEN, ROOK, BISHOP, KNIGHT};
    auto board = Bitboard(allies.getBitboard() | enemies.getBitboard());
    MoveType type = NORMAL;

    if (color == WHITE) {
        if(square<(64-8) and square>=(64-16)) {
            type = PROMOTION;
            if (not (board.getBitboard() & Bit(square + 8))) {
                for (Piece piece: pieces) {
                    moves.push(Move{
                        square,                                             // origin
                        static_cast<Square >(square + 8),               // destination
                        color,
                        PROMOTION,                                           // type
                        piece
                    });
                }
            }
        }
        else if (not (board.getBitboard() & Bit(square+8))) {
            moves.push(Move{
                square,                                             // origin
                static_cast<Square >(square+8),               // destination
                color,
                NORMAL                                                // type
            });
        }
        // Single move

        // Double move
        if (square<16 and square>=8 and not (board.getBitboard() & Bit(square+16)) and not (board.getBitboard() & Bit(square+8))) {
            moves.push(Move{
                square,                                             // origin
                static_cast<Square >(square+16),              // destination
                color,
                NORMAL                                              // type
            });
        }

        // Capture
        if(enemies.getBitboard() & Bit(square+7) and square % 8 != 0) {
            if (type == PROMOTION) {
                for (Piece piece: pieces) {
                    moves.push(Move{
                        square,                                             // origin
                        static_cast<Square >(square+7),               // destination
                        color,
                        PROMOTION,                                           // type
                        piece
                    });
                }
            }
            else {
                moves.push(Move{
                    square,                                             // origin
                    static_cast<Square >(square+7),               // destination
                    color,
                    NORMAL                                                // type
                });
            }
        }
        if(enemies.getBitboard() & Bit(square+9) and square % 8 != 7) {
            if (type == PROMOTION) {
                for (Piece piece: pieces) {
                    moves.push(Move{
                        square,                                             // origin
                        static_cast<Square >(square+9),               // destination
                        color,
                        PROMOTION,                                           // type
                        piece
                    });
                }
            }
            else {
                moves.push(Move{
                    square,                                             // origin
                    static_cast<Square >(square+9),               // destination
                    color,
                    NORMAL                                                // type
                });
            }
        }

        // En passant
        if ((square == passant+1 and square % 8 != 0) or (square == passant-1 and square % 8 != 7)) {
            moves.push(Move{
                    square,                                             // origin
                    static_cast<Square >(passant+8),              // destination
                    color,
                    EN_PASSANT                                          // type
            });
        }
    }
    // black pieces
    else if (color == BLACK) {
            // Promotion
            if(square<(16) and square>=(8)) {
                type = PROMOTION;
                if (not (board.getBitboard() & Bit(square-8))) {
                    for (Piece piece: pieces) {
                        moves.push(Move{
                            square,                                             // origin
                            static_cast<Square >(square-8),               // destination
                            color,
                            PROMOTION,                                           // type
                            piece
                        });
                    }
                }
            }
            else if (not (board.getBitboard() & Bit(square-8))){
                //Single Move
                moves.push(Move{
                    square,                                             // origin
                    static_cast<Square >(square-8),               // destination
                    color,
                    NORMAL                                                // type
                });
            }
            // Double move
            if(square<(64-8) and square>=(64-16) and not (board.getBitboard() & Bit(square-16)) and not (board.getBitboard() & Bit(square-8))) {
                moves.push(Move{
                    square,                                             // origin
                    static_cast<Square >(square-16),              // destination
                    color,
                    NORMAL                                              // type
                });
            }

            // Capture
            if(enemies.getBitboard() & Bit(square-7) and (square % 8 != 7)) {
                if (type == PROMOTION) {
                    for (Piece piece: pieces) {
                        moves.push(Move{
                            square,                                             // origin
                            static_cast<Square >(square-7),               // destination
                            color,
                            PROMOTION,                                           // type
                            piece
                        });
                    }
                }
                else {
                    moves.push(Move{
                        square,                                             // origin
                        static_cast<Square >(square-7),               // destination
                        color,
                        NORMAL                                                // type
                    });
                }
            }
            if(enemies.getBitboard() & Bit(square-9) and (square % 8 != 0)) {
                if (type == PROMOTION) {
                    for (Piece piece: pieces) {
                        moves.push(Move{
                            square,                                             // origin
                            static_cast<Square >(square-9),               // destination
                            color,
                            PROMOTION,                                           // type
                            piece
                        });
                    }
                }
                else {
                    moves.push(Move{
                        square,                                             // origin
                        static_cast<Square >(square-9),               // destination
                        color,
                        NORMAL                                                // type
                    });
                }
            }

            // En passant
            if ((square == passant+1 and square % 8 != 0) or (square == passant-1 and square % 8 != 7)) {
                moves.push(Move{
                        square,                                             // origin
                        static_cast<Square >(passant-8),              // destination
                        color,
                        EN_PASSANT                                          // type
                });
            }
        }
}

void MoveGen::knightMove(MoveList &moves, bool player, Square square, Bitboard allies) {
    // Retrieve the attack bitboard for the knight's position
    U64 attackBitboard = knightAttacks[square];

    // Exclude squares occupied by friendly pieces
    attackBitboard &= ~allies.getBitboard();

    // Iterate over each possible move
    while (attackBitboard) {
        // Get the index of the least significant set bit (the target square)
        int targetSquare = __builtin_ctzll(attackBitboard);

        // Clear the least significant set bit
        attackBitboard &= attackBitboard - 1;

        // Determine the move type

        // Add the move to the move list
        moves.push(Move{
            square,                            // origin
            static_cast<Square>(targetSquare), // destination
            player,
            NORMAL                           // type
        });
    }
}


void MoveGen::bishopMove(MoveList &moves, bool player, Square square, Bitboard allies, Bitboard enemies) {
    std::array directions = {
        std::make_pair(-1, -1), // Up-Left (Northwest)
        std::make_pair(-1, 1),  // Up-Right (Northeast)
        std::make_pair(1, -1),  // Down-Left (Southwest)
        std::make_pair(1, 1)    // Down-Right (Southeast)
    };

    pieceMove(moves,player,square, directions, allies, enemies);
}

void MoveGen::rookMove(MoveList &moves, bool player, Square square, Bitboard allies, Bitboard enemies) {
    std::array directions = {
        std::make_pair(-1, 0),  // Up (North)
        std::make_pair(0, -1),  // Left (West)
        std::make_pair(0, 1),   // Right (East)
        std::make_pair(1, 0),   // Down (South)
    };

    pieceMove(moves,player,square, directions, allies, enemies);
}

void MoveGen::queenMove(MoveList &moves, bool player, Square square, Bitboard allies, Bitboard enemies) {
    std::array directions = {
        std::make_pair(-1, -1), // Up-Left (Northwest)
        std::make_pair(-1, 0),  // Up (North)
        std::make_pair(-1, 1),  // Up-Right (Northeast)
        std::make_pair(0, -1),  // Left (West)
        std::make_pair(0, 1),   // Right (East)
        std::make_pair(1, -1),  // Down-Left (Southwest)
        std::make_pair(1, 0),   // Down (South)
        std::make_pair(1, 1)    // Down-Right (Southeast)
    };
    pieceMove(moves,player,square, directions, allies, enemies);
}

void MoveGen::kingMove(MoveList &moves, bool player, Square square, Bitboard allies) {
    // Retrieve the attack bitboard for the king's position
    U64 attackBitboard = kingAttacks[square];

    // Exclude squares occupied by friendly pieces
    attackBitboard &= ~allies.getBitboard();

    // Iterate over each possible move
    while (attackBitboard) {
        // Get the index of the least significant set bit (the target square)
        int targetSquare = __builtin_ctzll(attackBitboard);

        // Clear the least significant set bit
        attackBitboard &= attackBitboard - 1;

        // Ensure targetSquare is within valid range (0 to 63)
        if (targetSquare >= 0 && targetSquare < 64) {

            // Add the move to the move list
            moves.push(Move{
                square,                            // origin
                static_cast<Square>(targetSquare), // destination
                player,
                NORMAL                           // type
            });
        }
    }
}

void MoveGen::castleMove(MoveList &moves, bool player, Square square, int castlingRights, Bitboard allies, Bitboard enemies) {
    auto board = Bitboard(allies.getBitboard() | enemies.getBitboard());
    // Queen side Castle
    if(((castlingRights & 0b0010 and square == e1) or (castlingRights & 0b1000 and square == e8))
        and not (board.getBitboard() & Bit(square - 1)) and not (board.getBitboard() & Bit(square - 2)) and  not (board.getBitboard() & Bit(square - 3))) {
        moves.push(Move{
                square,                                             // origin
                static_cast<Square >(square-2),               // destination
                player,
                CASTLE                                              // type
        });
    }
    //King side Castle
    if(((castlingRights & 0b0001 and square == e1) or (castlingRights & 0b0100 and square == e8)) and  not (board.getBitboard()
        & Bit(square + 1)) and not (board.getBitboard() & Bit(square + 2))) {
        moves.push(Move{
                square,                                             // origin
                static_cast<Square >(square+2),               // destination
                player,
                CASTLE                                              // type
        });
    }
}

U64 MoveGen::rookAttack(int square, U64 block) {
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

U64 MoveGen::bishopAttack(int square, U64 block) {
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

U64 MoveGen::find_magic(int sq, int bishop) {
    U64 mask, b[4096], a[4096], used[4096], magic;
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
        if(!fail) return magic;
    }
    return 0ULL;
}

void MoveGen::pieceMove(MoveList &moves, bool player, Square square, std::span<std::pair<int, int>> directions,
    Bitboard allies, Bitboard enemies) {

    // Get the current rank and file
    int rank = square / 8; // Rows (0 to 7)
    int file = square % 8; // Columns (0 to 7)

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
            if ( allies.getBitboard() & Bit((newRank * 8 + newFile))) {
                break;
            }
            moves.push(Move{
                square,                                             // origin
                static_cast<Square>(newRank * 8 + newFile),         // destination
                player,
                NORMAL                                              // type
            });
            if (enemies.getBitboard() & Bit((newRank * 8 + newFile))) {
                break;
            }
        };
    }
}
