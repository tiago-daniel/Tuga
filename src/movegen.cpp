//
// Created by metta on 10/6/24.
//

#include "movegen.h"

#include "magic.h"


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

void MoveGen::knightMove(MoveList &moves, bool player, Square square, U64 allies) {
    // Retrieve the attack bitboard for the king's position
    U64 attackBitboard = knightAttacks[square];

    // Exclude squares occupied by friendly pieces
    attackBitboard &= ~allies;
    moveBitboard(moves, square, attackBitboard,player, allies);
}


void MoveGen::bishopMove(MoveList &moves, bool player, Square square, U64 allies, U64 enemies) {
    U64 attackBitboard = bishopAttacks(allies|enemies, square);

    attackBitboard &= ~allies;
    moveBitboard(moves, square, attackBitboard,player, allies);
}

void MoveGen::rookMove(MoveList &moves, bool player, Square square, U64 allies, U64 enemies) {
    U64 attackBitboard = rookAttacks(allies|enemies, square);

    attackBitboard &= ~allies;
    moveBitboard(moves, square, attackBitboard,player, allies);
}

void MoveGen::queenMove(MoveList &moves, bool player, Square square, U64 allies, U64 enemies) {
    U64 attackBitboard = rookAttacks(allies|enemies, square);
    attackBitboard |= bishopAttacks(allies|enemies, square);

    attackBitboard &= ~allies;
    moveBitboard(moves, square, attackBitboard,player, allies);
}

void MoveGen::kingMove(MoveList &moves, bool player, Square square, U64 allies) {
    // Retrieve the attack bitboard for the king's position
    U64 attackBitboard = kingAttacks[square];

    // Exclude squares occupied by friendly pieces
    attackBitboard &= ~allies;
    moveBitboard(moves, square, attackBitboard,player, allies);
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

void MoveGen::moveBitboard(MoveList &moves, int sq, U64 b,bool player, U64 ally) {
    b &= ~ally;
    while (b) {
        moves.push(Move{static_cast<Square>(sq),static_cast<Square>(__builtin_ctzll(b)),player, NORMAL,});
        b &= b - 1;
    }
}
