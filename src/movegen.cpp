//
// Created by metta on 10/6/24.
//

#include "movegen.h"



void MoveGen::pawnMove(MoveList &moves, Square square, bool color, Bitboard allies, Bitboard enemies, Square passant) {
    std::array pieces = {QUEEN, ROOK, BISHOP, KNIGHT};
    auto board = Bitboard(allies.getBitboard() | enemies.getBitboard());
    MoveType type = NORMAL;

    if (color == WHITE) {
        if(square<(64-8) and square>=(64-16)) {
            type = PROMOTION;
            if (not (board.getBitboard() & Bit(square+8))) {
                for (Piece piece: pieces) {
                    moves.push(Move{
                        square,                                             // origin
                        static_cast<Square >(square+8),               // destination
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
                            static_cast<Square >(square+8),               // destination
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

void MoveGen::knightMove(MoveList &moves, bool player, Square square, Bitboard allies){
    std::array directions = {
        std::make_pair(-2, -1), std::make_pair(-2, 1),
        std::make_pair(-1, -2), std::make_pair(-1, 2),
        std::make_pair(1, -2), std::make_pair(1, 2),
        std::make_pair(2, -1), std::make_pair(2, 1)
    };

    pieceMove(moves,player, square, directions, false, allies);
}

void MoveGen::bishopMove(MoveList &moves, bool player, Square square, Bitboard allies, Bitboard enemies) {
    std::array directions = {
        std::make_pair(-1, -1), // Up-Left (Northwest)
        std::make_pair(-1, 1),  // Up-Right (Northeast)
        std::make_pair(1, -1),  // Down-Left (Southwest)
        std::make_pair(1, 1)    // Down-Right (Southeast)
    };

    pieceMove(moves,player,square, directions, true, allies, enemies);
}

void MoveGen::rookMove(MoveList &moves, bool player, Square square, Bitboard allies, Bitboard enemies) {
    std::array directions = {
        std::make_pair(-1, 0),  // Up (North)
        std::make_pair(0, -1),  // Left (West)
        std::make_pair(0, 1),   // Right (East)
        std::make_pair(1, 0),   // Down (South)
    };

    pieceMove(moves,player,square, directions, true, allies, enemies);
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
    pieceMove(moves,player,square, directions, true, allies, enemies);
}

void MoveGen::kingMove(MoveList &moves, bool player, Square square, Bitboard allies) {
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

    pieceMove(moves,player,square, directions, false, allies);
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


void MoveGen::pieceMove(MoveList &moves, bool player, Square square, std::span<std::pair<int, int>> directions,
    bool slide, Bitboard allies, Bitboard enemies) {

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
            moves.push(Move{
                square,                                             // origin
                static_cast<Square>(newRank * 8 + newFile),         // destination
                player,
                NORMAL                                              // type
            });
            if (enemies.getBitboard() & Bit((newRank * 8 + newFile))) {
                break;
            }
        } while (slide);
    }
}
