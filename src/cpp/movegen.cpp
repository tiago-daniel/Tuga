//
// Created by metta on 10/6/24.
//

#include "h/movegen.h"

#include <set>

#include "h/bitboard.h"

MoveGen::MoveGen() = default;

void MoveGen::pawnMove(std::array<Move, 256> &moves, Square square, Color color, Bitboard allies, Bitboard enemies, Square passant) {
    int i = 0;
    std::array pieces = {QUEEN, ROOK, BISHOP, KNIGHT};
    while (moves[i] != Move()) {i++;}
    Bitboard board = Bitboard(allies.getBitboard() | enemies.getBitboard());
    MoveType type = NORMAL;

    if (color == WHITE) {
        if(square<(64-8) and square>=(64-16)) {
            type = PROMOTION;
            if (not (board.getBitboard() & Bit(square+8))) {
                for (Piece piece: pieces) {
                    moves[i++] = (Move{
                        square,                                             // origin
                        static_cast<Square >(square+8),               // destination
                        PROMOTION,                                           // type
                        piece
                    });
                }
            }
        }
        else if (not (board.getBitboard() & Bit(square+8))) {
            moves[i++] = (Move{
                square,                                             // origin
                static_cast<Square >(square+8),               // destination
                NORMAL                                                // type
            });
        }
        // Single move

        // Double move
        if (square<16 and square>=8 and not (board.getBitboard() & Bit(square+16)) and not (board.getBitboard() & Bit(square+8))) {
            moves[i++] = (Move{
                square,                                             // origin
                static_cast<Square >(square+16),              // destination
                NORMAL                                              // type
            });
        }

        // Capture
        if(enemies.getBitboard() & Bit(square+7) and square % 8 != 0) {
            if (type == PROMOTION) {
                for (Piece piece: pieces) {
                    moves[i++] = (Move{
                        square,                                             // origin
                        static_cast<Square >(square+7),               // destination
                        PROMOTION,                                           // type
                        piece
                    });
                }
            }
            else {
                moves[i++] = (Move{
                    square,                                             // origin
                    static_cast<Square >(square+7),               // destination
                    NORMAL                                                // type
                });
            }
        }
        if(enemies.getBitboard() & Bit(square+9) and square % 8 != 7) {
            if (type == PROMOTION) {
                for (Piece piece: pieces) {
                    moves[i++] = (Move{
                        square,                                             // origin
                        static_cast<Square >(square+9),               // destination
                        PROMOTION,                                           // type
                        piece
                    });
                }
            }
            else {
                moves[i++] = (Move{
                    square,                                             // origin
                    static_cast<Square >(square+9),               // destination
                    NORMAL                                                // type
                });
            }
        }

        // En passant
        if ((square == passant+1 and square % 8 != 0) or (square == passant-1 and square % 8 != 7)) {
            moves[i++] = (Move{
                    square,                                             // origin
                    static_cast<Square >(passant+8),              // destination
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
                        moves[i++] = (Move{
                            square,                                             // origin
                            static_cast<Square >(square+8),               // destination
                            PROMOTION,                                           // type
                            piece
                        });
                    }
                }
            }
            else if (not (board.getBitboard() & Bit(square-8))){
                //Single Move
                moves[i++] = (Move{
                    square,                                             // origin
                    static_cast<Square >(square-8),               // destination
                    NORMAL                                                // type
                });
            }
            // Double move
            if(square<(64-8) and square>=(64-16) and not (board.getBitboard() & Bit(square-16)) and not (board.getBitboard() & Bit(square-8))) {
                moves[i++] = (Move{
                    square,                                             // origin
                    static_cast<Square >(square-16),              // destination
                    NORMAL                                              // type
                });
            }

            // Capture
            if(enemies.getBitboard() & Bit(square-7) and (square % 8 != 7)) {
                if (type == PROMOTION) {
                    for (Piece piece: pieces) {
                        moves[i++] = (Move{
                            square,                                             // origin
                            static_cast<Square >(square-7),               // destination
                            PROMOTION,                                           // type
                            piece
                        });
                    }
                }
                else {
                    moves[i++] = (Move{
                        square,                                             // origin
                        static_cast<Square >(square-7),               // destination
                        NORMAL                                                // type
                    });
                }
            }
            if(enemies.getBitboard() & Bit(square-9) and (square % 8 != 0)) {
                if (type == PROMOTION) {
                    for (Piece piece: pieces) {
                        moves[i++] = (Move{
                            square,                                             // origin
                            static_cast<Square >(square-9),               // destination
                            PROMOTION,                                           // type
                            piece
                        });
                    }
                }
                else {
                    moves[i++] = (Move{
                        square,                                             // origin
                        static_cast<Square >(square-9),               // destination
                        NORMAL                                                // type
                    });
                }
            }

            // En passant
            if ((square == passant+1 and square % 8 != 0) or (square == passant-1 and square % 8 != 7)) {
                moves[i++] = (Move{
                        square,                                             // origin
                        static_cast<Square >(passant-8),              // destination
                        EN_PASSANT                                          // type
                });
            }
        }
}

void MoveGen::knightMove(std::array<Move, 256> &moves, Square square, Bitboard allies){
    std::vector<std::pair<int, int>> directions = {
        {-2, -1}, {-2, 1},  // Two up, one left/right
        {-1, -2}, {-1, 2},  // One up, two left/right
        {1, -2}, {1, 2},    // One down, two left/right
        {2, -1}, {2, 1}     // Two down, one left/right
    };
    pieceMove(moves, square, directions, false, allies);
}

void MoveGen::bishopMove(std::array<Move, 256> &moves, Square square, Bitboard allies, Bitboard enemies) {
    std::vector<std::pair<int, int>> directions = {
        {-1, -1},
        {-1, 1},
        {1, -1},
        {1, 1},
    };

    pieceMove(moves,square, directions, true, allies, enemies);
}

void MoveGen::rookMove(std::array<Move, 256> &moves, Square square, Bitboard allies, Bitboard enemies) {
    std::vector<std::pair<int, int>> directions = {
        {0, -1},
        {-1, 0},
        {1, 0},
        {0, 1},
    };

    pieceMove(moves,square, directions, true, allies, enemies);
}

void MoveGen::queenMove(std::array<Move, 256> &moves, Square square, Bitboard allies, Bitboard enemies) {
    std::vector<std::pair<int, int>> directions = {
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

void MoveGen::kingMove(std::array<Move, 256> &moves, Square square, Bitboard allies) {
    std::vector<std::pair<int, int>> directions = {
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

void MoveGen::castleMove(std::array<Move, 256> &moves, Square square, int castlingRights, Bitboard allies, Bitboard enemies) {
    int i = 0;
    auto board = Bitboard(allies.getBitboard() | enemies.getBitboard());
    while (moves[i] != Move()) {i++;}
    // Queen side White Castle
    if(((castlingRights & 0b0010 and square == e1) or (castlingRights & 0b1000 and square == e8))
        and not (board.getBitboard() & Bit(square - 1)) and not (board.getBitboard() & Bit(square - 2)) and  not (board.getBitboard() & Bit(square - 3))) {
        moves[i++] = (Move{
                square,                                             // origin
                static_cast<Square >(square-2),               // destination
                CASTLE                                              // type
        });
    }
    //King side White Castle
    if(((castlingRights & 0b0001 and square == e1) or (castlingRights & 0b0100 and square == e8)) and  not (board.getBitboard()
        & Bit(square + 1)) and not (board.getBitboard() & Bit(square + 2))) {
        moves[i++] = (Move{
                square,                                             // origin
                static_cast<Square >(square+2),               // destination
                CASTLE                                              // type
        });
    }
}


void MoveGen::pieceMove(std::array<Move, 256> &moves, Square square, std::vector<std::pair<int, int>> directions,
    bool slide, Bitboard allies, Bitboard enemies) {
    int i = 0;
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
                static_cast<Square>(newRank * 8 + newFile),     // destination
                NORMAL                                              // type
            });
            if (enemies.getBitboard() & Bit((newRank * 8 + newFile))) {
                break;
            }
        } while (slide);
    }
}
