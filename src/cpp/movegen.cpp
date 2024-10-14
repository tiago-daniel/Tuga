//
// Created by metta on 10/6/24.
//

#include "h/movegen.h"

#include <set>

#include "h/bitboard.h"

moveGen::moveGen() = default;

void moveGen::pawnMove(array<Move, 256> &moves, Square_index square, Color color, Bitboard allies, Bitboard enemies, Square_index passant) {
    uint8_t i = 0;
    array<Piece_index, 4> pieces = {i_queen, i_rook, i_bishop, i_knight};
    while (moves[i] != Move()) {i++;}
    Bitboard board = Bitboard(allies.getBitboard() | enemies.getBitboard());
    Move_type type = NORMAL;

    if (color == c_white and not (board.getBitboard() & Bit(square+8))) {
        if(square<(64-8) and square>=(64-16)) {
            type = PROMOTION;
            for (Piece_index piece: pieces) {
                moves[i++] = (Move{
                    square,                                             // origin
                    static_cast<Square_index >(square+8),               // destination
                    PROMOTION,                                           // type
                    piece
                });
            }
        }
        else {
            moves[i++] = (Move{
                square,                                             // origin
                static_cast<Square_index >(square+8),               // destination
                NORMAL                                                // type
            });
        }
        // Single move

        // Double move
        if (square<16 and square>=8 and not (board.getBitboard() & Bit(square+16))) {
            moves[i++] = (Move{
                square,                                             // origin
                static_cast<Square_index >(square+16),              // destination
                NORMAL                                              // type
            });
        }

        // Capture
        if(enemies.getBitboard() & Bit(square+7) and square % 8 != 0) {
            if (type == PROMOTION) {
                for (Piece_index piece: pieces) {
                    moves[i++] = (Move{
                        square,                                             // origin
                        static_cast<Square_index >(square+7),               // destination
                        PROMOTION,                                           // type
                        piece
                    });
                }
            }
            else {
                moves[i++] = (Move{
                    square,                                             // origin
                    static_cast<Square_index >(square+7),               // destination
                    NORMAL                                                // type
                });
            }
        }
        if(enemies.getBitboard() & Bit(square+9) and square % 8 != 7) {
            if (type == PROMOTION) {
                for (Piece_index piece: pieces) {
                    moves[i++] = (Move{
                        square,                                             // origin
                        static_cast<Square_index >(square+9),               // destination
                        PROMOTION,                                           // type
                        piece
                    });
                }
            }
            else {
                moves[i++] = (Move{
                    square,                                             // origin
                    static_cast<Square_index >(square+9),               // destination
                    NORMAL                                                // type
                });
            }
        }

        // En passant
        if ((square == passant+1 and square % 8 != 7) or (square == passant-1 and square % 8 != 0)) {
            moves[i++] = (Move{
                    square,                                             // origin
                    static_cast<Square_index >(passant+8),              // destination
                    EN_PASSANT                                          // type
            });
        }
    }
    // black pieces
    else if (not (board.getBitboard() & Bit(square-8))) {
            // Promotion
            if(square<(16) and square>=(8)) {
                type = PROMOTION;
                for (Piece_index piece: pieces) {
                    moves[i++] = (Move{
                        square,                                             // origin
                        static_cast<Square_index >(square+8),               // destination
                        PROMOTION,                                           // type
                        piece
                    });
                }
            }
            else {
                //Single Move
                moves[i++] = (Move{
                    square,                                             // origin
                    static_cast<Square_index >(square-8),               // destination
                    NORMAL                                                // type
                });
            }
            // Double move
            if(square<(64-8) and square>=(64-16) and not (board.getBitboard() & Bit(square-16))) {
                moves[i++] = (Move{
                    square,                                             // origin
                    static_cast<Square_index >(square-16),              // destination
                    NORMAL                                              // type
                });
            }

            // Capture
            if(enemies.getBitboard() & Bit(square-7) and (square % 8 != 7)) {
                if (type == PROMOTION) {
                    for (Piece_index piece: pieces) {
                        moves[i++] = (Move{
                            square,                                             // origin
                            static_cast<Square_index >(square-7),               // destination
                            PROMOTION,                                           // type
                            piece
                        });
                    }
                }
                else {
                    moves[i++] = (Move{
                        square,                                             // origin
                        static_cast<Square_index >(square-7),               // destination
                        NORMAL                                                // type
                    });
                }
            }
            if(enemies.getBitboard() & Bit(square-9) and (square % 8 != 0)) {
                if (type == PROMOTION) {
                    for (Piece_index piece: pieces) {
                        moves[i++] = (Move{
                            square,                                             // origin
                            static_cast<Square_index >(square-9),               // destination
                            PROMOTION,                                           // type
                            piece
                        });
                    }
                }
                else {
                    moves[i++] = (Move{
                        square,                                             // origin
                        static_cast<Square_index >(square-9),               // destination
                        NORMAL                                                // type
                    });
                }
            }

            // En passant
            if ((square == passant+1 and square % 8 != 7) or (square == passant-1 and square % 8 != 0)) {
                moves[i++] = (Move{
                        square,                                             // origin
                        static_cast<Square_index >(passant-8),              // destination
                        EN_PASSANT                                          // type
                });
            }
        }
}

void moveGen::knightMove(array<Move, 256> &moves, Square_index square, Bitboard allies){
    vector<pair<int, int>> directions = {
        {-2, -1}, {-2, 1},  // Two up, one left/right
        {-1, -2}, {-1, 2},  // One up, two left/right
        {1, -2}, {1, 2},    // One down, two left/right
        {2, -1}, {2, 1}     // Two down, one left/right
    };
    pieceMove(moves, square, directions, false, allies);
}

void moveGen::bishopMove(array<Move, 256> &moves, Square_index square, Bitboard allies, Bitboard enemies) {
    vector<pair<int, int>> directions = {
        {-1, -1},
        {-1, 1},
        {1, -1},
        {1, 1},
    };

    pieceMove(moves,square, directions, true, allies, enemies);
}

void moveGen::rookMove(array<Move, 256> &moves, Square_index square, Bitboard allies, Bitboard enemies) {
    vector<pair<int, int>> directions = {
        {0, -1},
        {-1, 0},
        {1, 0},
        {0, 1},
    };

    pieceMove(moves,square, directions, true, allies, enemies);
}

void moveGen::queenMove(array<Move, 256> &moves, Square_index square, Bitboard allies, Bitboard enemies) {
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

    pieceMove(moves,square, directions, true, allies, enemies);
}

void moveGen::kingMove(array<Move, 256> &moves, Square_index square, Bitboard allies, Bitboard enemies) {
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

    pieceMove(moves,square, directions, false, allies);
}

void moveGen::CastleMove(array<Move, 256> &moves, Square_index square, uint8_t castlingRights) {
    uint8_t i = 0;
    while (moves[i] != Move()) {i++;}
    // Queen side White Castle
    if(castlingRights & 0b0010) {
        moves[i++] = (Move{
                square,                                             // origin
                static_cast<Square_index >(square+2),               // destination
                CASTLE                                              // type
        });
    }
    //King side White Castle
    if(castlingRights & 0b0001) {
        moves[i++] = (Move{
                square,                                             // origin
                static_cast<Square_index >(square-2),               // destination
                CASTLE                                              // type
        });
    }

    // Queen side Black Castle
    if(castlingRights & 0b1000) {
        moves[i++] = (Move{
                square,                                             // origin
                static_cast<Square_index >(square-2),               // destination
                CASTLE                                              // type
        });
    }

    //King side Black Castle
    if(castlingRights & 0b0100) {
        moves[i++] = (Move{
                square,                                             // origin
                static_cast<Square_index >(square+2),               // destination
                CASTLE                                              // type
        });
    }
}


void moveGen::pieceMove(array<Move, 256> &moves, Square_index square, vector<pair<int, int>> directions,
    bool slide, Bitboard allies, Bitboard enemies) {
    uint8_t i = 0;
    while (moves[i] != Move()) {i++;}

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
            moves[i++] = (Move{
                square,                                             // origin
                static_cast<Square_index>(newRank * 8 + newFile),     // destination
                NORMAL                                              // type
            });
            if (enemies.getBitboard() & Bit((newRank * 8 + newFile))) {
                break;
            }
        } while (slide);
    }
}
