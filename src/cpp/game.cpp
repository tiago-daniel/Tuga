//
// Created by metta on 10/10/24.
//

#include "h/game.h"

#include <unordered_map>

Game::Game() {
    this->white = Bitboard({a1,b1,c1,d1,e1,f1,g1,h1,a2,b2,c2,d2,e2,f2,g2,h2}); // i_white = 0
    this->black = Bitboard({a7,b7,c7,d7,e7,f7,g7,h7,a8,b8,c8,d8,e8,f8,g8,h8}); // i_black = 1
    this->pawn = Bitboard({a2,b2,c2,d2,e2,f2,g2,h2,a7,b7,c7,d7,e7,f7,g7,h7}); // i_pawn = 2
    this->knight = Bitboard({b1,g1,b8,g8}); // i_knight = 3
    this->bishop = Bitboard({c1,f1,c8,f8}); // i_bishop = 4
    this->rook = Bitboard({a1,h1,a8,h8}); // i_rook = 5
    this->queen = Bitboard({d1,d8}); // i_queen = 6
    this->king = Bitboard({e1,e8}); // i_king = 7
    this->pieces = {
        i_rook, i_knight, i_bishop, i_queen, i_king, i_bishop, i_knight, i_rook,
        i_pawn, i_pawn, i_pawn, i_pawn, i_pawn, i_pawn, i_pawn, i_pawn,
        i_empty, i_empty, i_empty, i_empty, i_empty, i_empty, i_empty, i_empty,
        i_empty, i_empty, i_empty, i_empty, i_empty, i_empty, i_empty, i_empty,
        i_empty, i_empty, i_empty, i_empty, i_empty, i_empty, i_empty, i_empty,
        i_empty, i_empty, i_empty, i_empty, i_empty, i_empty, i_empty, i_empty,
        i_pawn, i_pawn, i_pawn, i_pawn, i_pawn, i_pawn, i_pawn, i_pawn,
        i_rook, i_knight, i_bishop, i_queen, i_king, i_bishop, i_knight, i_rook,
    };
}

array<reference_wrapper<Bitboard> , 8> Game::getBitboards() {
    return this->boards;
}

Square_index Game::getPassant() {
    return this->passant;
}

void Game::makeMove(Move move) {
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

void Game::promotionMove(Move move) {
    for (auto board : this->boards) {
        if (board.get().getBitboard() & Bit(move.destination)) {
            this->draw_count = 0;
            board.get().removeBit(move.destination);
        }
        else if (board.get().getBitboard() & Bit(move.origin)) {
            board.get().removeBit(move.origin);
            if (board.get().getBitboard() != this->boards[i_pawn].get().getBitboard()) {
                board.get().addBit(move.destination);
            }
        }
    }
    this->boards[move.promotion].get().addBit(move.destination);
    pieces[move.origin] = i_empty;
    pieces[move.destination] = move.promotion;
}

void Game::castleMove(Move move) {
    if (move.origin + 2 == move.destination) {
        for (auto board : this->boards) {
            if (board.get().getBitboard() & Bit(move.origin)) {
                board.get().removeBit(move.origin);
                board.get().addBit(move.destination);
            }
            if (board.get().getBitboard() & Bit(move.origin+3)) {
                board.get().removeBit(move.origin+3);
                board.get().addBit(move.destination-1);
            }
        }
        pieces[move.destination] = i_king;
        pieces[move.origin] = i_empty;
        pieces[move.destination+1] = i_empty;
        pieces[move.destination-1] = i_rook;
    }
    else {
        for (auto board : this->boards) {
            if (board.get().getBitboard() & Bit(move.origin)) {
                board.get().removeBit(move.origin);
                board.get().addBit(move.destination);
            }
            if (board.get().getBitboard() & Bit(move.destination-2)) {
                board.get().removeBit(move.destination-2);
                board.get().addBit(move.destination+1);
            }
        }
        pieces[move.destination] = i_king;
        pieces[move.origin] = i_empty;
        pieces[move.destination+1] = i_rook;
        pieces[move.destination-2] = i_empty;
    }
}

void Game::passantMove(Move move) {
    for (auto board : this->boards) {
        board.get().removeBit(this->passant);
        if (board.get().getBitboard() & Bit(move.origin)) {
            board.get().removeBit(move.origin);
            board.get().addBit(move.destination);
        }
    }
    pieces[move.destination] = pieces[move.origin];
    pieces[move.origin] = i_empty;
    pieces[this->passant] = i_empty;
    passant = a1;
    this->draw_count = 0;
}

void Game::normalMove(Move move) {
    for (auto board : this->boards) {
        if (board.get().getBitboard() & Bit(move.destination)) {
            this->draw_count = 0;
            board.get().removeBit(move.destination);
        }
        if (board.get().getBitboard() & Bit(move.origin)) {
            board.get().removeBit(move.origin);
            board.get().addBit(move.destination);
        }
    }

        pieces[move.destination] = pieces[move.origin];  // Copy piece from origin to destination
        pieces[move.origin] = i_empty;  // Set origin square as empty

        passant = a1;
        if (pieces[move.destination] == i_pawn and (move.destination == move.origin + 16 or move.destination == move.origin - 16)) {
            passant = move.destination;                     // make en passant square
    }
}

bool Game::getCurrentPlayer() {
    return this->current_player;
}

void Game::print() {
    uint64_t white = this->getBitboards()[i_white].get().getBitboard();
    uint64_t black = this->getBitboards()[i_black].get().getBitboard();
    for (int row = 7; row >= 0; row--) {
        for (int col = 0; col < 8; col++) {
            uint8_t n = row * 8 + col;
            if (white & Bit(n)) {
                switch (pieces[n]) {
                    case i_pawn:
                        cout << 'P' << " ";
                        break;
                    case i_knight:
                        cout << 'N' << " ";
                        break;
                    case i_bishop:
                        cout << 'B' << " ";
                        break;
                    case i_rook:
                        cout << 'R' << " ";
                        break;
                    case i_queen:
                        cout << 'Q' << " ";
                        break;
                    case i_king:
                        cout << 'K' << " ";
                        break;
                    default:
                        cout << 'X' << " ";
                        break;
                }
            }
            else if (black & Bit(n)) {
                switch (pieces[n]) {
                    case i_pawn:
                        cout << 'p' << " ";
                        break;
                    case i_knight:
                        cout << 'n' << " ";
                        break;
                    case i_bishop:
                        cout << 'b' << " ";
                        break;
                    case i_rook:
                        cout << 'r' << " ";
                        break;
                    case i_queen:
                        cout << 'q' << " ";
                        break;
                    case i_king:
                        cout << 'k' << " ";
                        break;
                    default:
                        cout << 'x' << " ";
                        break;
                }
            }
            else if ((row + col) % 2 == 0){
                cout << 'o' << " ";
            }
            else {
                cout << 'O' << " ";
            }
        }
        cout << endl;
    }
}

array<Move, 256> Game::allMoves() {
    array<Move, 256> moves = {};
    Bitboard allies = this->getBitboards()[this->current_player];
    Bitboard enemies = this->getBitboards()[this->current_player xor 1];
    moveGen move_handler = moveGen();
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            uint8_t n = row * 8 + col;
            if (allies.getBitboard() & Bit(n)) {
                switch (this->pieces[n]) {
                    case i_pawn:
                        move_handler.pawnMove(moves, static_cast<Square_index>(n), static_cast<Color>(this->current_player), allies, enemies, passant);
                    break;
                    case i_knight:
                        move_handler.knightMove(moves, static_cast<Square_index>(n), allies);
                    break;
                    case i_bishop:
                        move_handler.bishopMove(moves, static_cast<Square_index>(n), allies,enemies);
                    break;
                    case i_rook:
                        move_handler.rookMove(moves, static_cast<Square_index>(n), allies,enemies);
                    break;
                    case i_queen:
                        move_handler.queenMove(moves, static_cast<Square_index>(n), allies,enemies);
                    break;
                    default:
                        move_handler.kingMove(moves, static_cast<Square_index>(n), allies,enemies);
                        move_handler.castleMove(moves, static_cast<Square_index>(n), this->can_castle,allies, enemies);
                        break;
                }
            }
        }
    }
    return moves;
}