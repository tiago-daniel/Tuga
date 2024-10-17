//
// Created by metta on 10/10/24.
//

#include "position.h"

Position::Position() {
    this->colors = {Bitboard({a1,b1,c1,d1,e1,f1,g1,h1,a2,b2,c2,d2,e2,f2,g2,h2}),Bitboard({a7,b7,c7,d7,e7,f7,g7,h7,a8,b8,c8,d8,e8,f8,g8,h8})};
    this->boards = {Bitboard({a2,b2,c2,d2,e2,f2,g2,h2,a7,b7,c7,d7,e7,f7,g7,h7}),Bitboard({b1,g1,b8,g8})
        ,Bitboard({c1,f1,c8,f8}),Bitboard({a1,h1,a8,h8}),Bitboard({d1,d8}),Bitboard({e1,e8})};
    this->pieces = {
        ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK,
        PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN,
        ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK,
    };
}

std::array<Bitboard, 6> Position::getBitboards() {
    return this->boards;
}

std::array<Bitboard, 2> Position::getColors() {
    return this->colors;
}

Square Position::getPassant() {
    return this->passant;
}

void Position::makeMove(Move move) {
    switch (move.type) {
        case EN_PASSANT:
            passantMove(move);
            break;
        case NORMAL:
            normalMove(move);
            break;
        case CASTLE:
            castleMove(move);
            break;
        default:
            promotionMove(move);
            break;
    }
    this->current_player ^= 1;
    this->draw_count++;

}

void Position::promotionMove(Move move) {
    for (auto board : this->boards) {
        if (board.getBitboard() & Bit(move.destination)) {
            this->draw_count = 0;
            board.removeBit(move.destination);
        }
        else if (board.getBitboard() & Bit(move.origin)) {
            board.removeBit(move.origin);
            if (board.getBitboard() != boards[PAWN].getBitboard()) {
                board.addBit(move.destination);
            }
        }
        colors[current_player].removeBit(move.origin);
        colors[current_player ^ 1].removeBit(move.destination);
        colors[current_player].addBit(move.destination);
    }
    this->boards[move.promotion].addBit(move.destination);
    pieces[move.origin] = EMPTY;
    pieces[move.destination] = move.promotion;
}

void Position::castleMove(Move move) {
    if (move.origin + 2 == move.destination) {
        for (auto board : this->boards) {
            if (board.getBitboard() & Bit(move.origin)) {
                board.removeBit(move.origin);
                board.addBit(move.destination);
            }
            if (board.getBitboard() & Bit(move.origin+3)) {
                board.removeBit(move.origin+3);
                board.addBit(move.destination-1);
            }
        }
        pieces[move.destination] = KING;
        pieces[move.origin] = EMPTY;
        pieces[move.destination+1] = EMPTY;
        pieces[move.destination-1] = ROOK;
        colors[current_player].removeBit(move.origin);
        colors[current_player].addBit(move.destination);
        colors[current_player].removeBit(move.destination+3);
        colors[current_player].addBit(move.destination-1);
    }
    else {
        for (auto board : this->boards) {
            if (board.getBitboard() & Bit(move.origin)) {
                board.removeBit(move.origin);
                board.addBit(move.destination);
            }
            if (board.getBitboard() & Bit(move.destination-2)) {
                board.removeBit(move.destination-2);
                board.addBit(move.destination+1);
            }
        }
        pieces[move.destination] = KING;
        pieces[move.origin] = EMPTY;
        pieces[move.destination+1] = ROOK;
        pieces[move.destination-2] = EMPTY;
        colors[current_player].removeBit(move.origin);
        colors[current_player].addBit(move.destination);
        colors[current_player].removeBit(move.destination-2);
        colors[current_player].addBit(move.destination+1);
    }
}

Piece Position::pieceOn(Square square) {
    return this->pieces[square];
}

void Position::passantMove(Move move) {
    for (auto board : this->boards) {
        board.removeBit(this->passant);
        if (board.getBitboard() & Bit(move.origin)) {
            board.removeBit(move.origin);
            board.addBit(move.destination);
        }
    }
    pieces[move.destination] = pieces[move.origin];
    pieces[move.origin] = EMPTY;
    pieces[this->passant] = EMPTY;
    passant = a1;
    this->draw_count = 0;
    colors[current_player].removeBit(move.origin);
    colors[current_player ^ 1].removeBit(this->passant);
    colors[current_player].addBit(move.destination);
}

void Position::normalMove(Move move) {
    for (auto board : this->boards) {
        if (board.getBitboard() & Bit(move.destination)) {
            this->draw_count = 0;
            board.removeBit(move.destination);
        }
        if (board.getBitboard() & Bit(move.origin)) {
            board.removeBit(move.origin);
            board.addBit(move.destination);
        }
    }

        pieces[move.destination] = pieces[move.origin];  // Copy piece from origin to destination
        pieces[move.origin] = EMPTY;  // Set origin square as empty

        passant = a1;
        if (this->boards[PAWN].getBitboard() & Bit(move.destination) and (move.destination == move.origin + 16 or move.destination == move.origin - 16)) {
            passant = move.destination;                     // make en passant square
    }
    colors[current_player].removeBit(move.origin);
    colors[current_player ^ 1].removeBit(move.destination);
    colors[current_player].addBit(move.destination);
}

bool Position::getCurrentPlayer() {
    return this->current_player;
}

void Position::print() {
    for (int row = 7; row >= 0; row--) {
        for (int col = 0; col < 8; col++) {
            int n = row * 8 + col;
            if (colors[WHITE].hasBit(n)) {
                switch (pieces[n]) {
                    case PAWN:
                        std::cout << 'P' << " ";
                        break;
                    case KNIGHT:
                        std::cout << 'N' << " ";
                        break;
                    case BISHOP:
                        std::cout << 'B' << " ";
                        break;
                    case ROOK:
                        std::cout << 'R' << " ";
                        break;
                    case QUEEN:
                        std::cout << 'Q' << " ";
                        break;
                    case KING:
                        std::cout << 'K' << " ";
                        break;
                    default:
                        std::cout << 'X' << " ";
                        break;
                }
            }
            else if (colors[BLACK].hasBit(n)) {
                switch (pieces[n]) {
                    case PAWN:
                        std::cout << 'p' << " ";
                        break;
                    case KNIGHT:
                        std::cout << 'n' << " ";
                        break;
                    case BISHOP:
                        std::cout << 'b' << " ";
                        break;
                    case ROOK:
                        std::cout << 'r' << " ";
                        break;
                    case QUEEN:
                        std::cout << 'q' << " ";
                        break;
                    case KING:
                        std::cout << 'k' << " ";
                        break;
                    default:
                        std::cout << 'x' << " ";
                        break;
                }
            }
            else if ((row + col) % 2 == 0){
                std::cout << 'o' << " ";
            }
            else {
                std::cout << 'O' << " ";
            }
        }
        std::cout << std::endl;
    }
}

MoveList Position::allMoves(bool player) {
    auto moves = MoveList();
    Bitboard allies = this->colors[player];
    Bitboard enemies = this->colors[player xor 1];
    MoveGen move_handler = MoveGen();
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            int n = row * 8 + col;
            if (allies.getBitboard() & Bit(n)) {
                switch (this->pieces[n]) {
                    case PAWN:
                        move_handler.pawnMove(moves, static_cast<Square>(n), static_cast<Color>(this->current_player), allies, enemies, passant);
                    break;
                    case KNIGHT:
                        move_handler.knightMove(moves, static_cast<Square>(n), allies);
                    break;
                    case BISHOP:
                        move_handler.bishopMove(moves, static_cast<Square>(n), allies,enemies);
                    break;
                    case ROOK:
                        move_handler.rookMove(moves, static_cast<Square>(n), allies,enemies);
                    break;
                    case QUEEN:
                        move_handler.queenMove(moves, static_cast<Square>(n), allies,enemies);
                    break;
                    case KING:
                        move_handler.kingMove(moves, static_cast<Square>(n), allies);
                        move_handler.castleMove(moves, static_cast<Square>(n), this->can_castle,allies, enemies);
                        break;
                    default:
                        break;
                }
            }
        }
    }
    return moves;
}

bool Position::isKingInCheck(bool player) {
    Square kingSquare = a1;
    for (int i = 0; i < 64; i++) {
        if (pieces[i] == KING && colors[player].hasBit(i)) {
            kingSquare = static_cast<Square>(i);
            break;
        }
    }

    MoveList enemyMoves = this->allMoves(player ^ 1);
    for (const auto& move : enemyMoves.getMoves()) {
        if (move.destination == kingSquare) {
            return true;
        }
    }
    return false;
}