//
// Created by metta on 10/10/24.
//

#include "h/game.h"

#include <unordered_map>

game::game() {
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

array<reference_wrapper<Bitboard> , 8> game::getBitboards() {
    return this->boards;
}



void game::movePiece(Square_index initial_square, Square_index target_square, Bitboard &board) {
    board.removeBit(initial_square);
    board.addBit(target_square);
    pieces[target_square] = pieces[initial_square];
    pieces[initial_square] = pieces[i_empty];
    this->current_player ^= 1;
    this->draw_count++;
}

bool game::getCurrentPlayer() {
    return this->current_player;
}

void game::print() {
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
                    default:
                        cout << 'K' << " ";
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
                    default:
                        cout << 'k' << " ";
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

array<Move, 256> game::allMoves() {
    array<Move, 256> all_moves = {};
    uint8_t i = 0;
    Bitboard allies = this->getBitboards()[this->current_player];
    Bitboard enemies = this->getBitboards()[this->current_player xor 1];
    moveGen move_handler = moveGen();
    for (int row = 7; row >= 0; row--) {
        for (int col = 0; col < 8; col++) {
            uint8_t n = row * 8 + col;
            array<Move, 256> moves = {};
            if (allies.getBitboard() & Bit(n)) {
                switch (this->pieces[n]) {
                    case i_pawn:
                        moves = move_handler.pawnMove(static_cast<Square_index>(n), static_cast<Color>(this->current_player), allies, enemies, passant);
                    break;
                    case i_knight:
                        moves = move_handler.knightMove(static_cast<Square_index>(n), allies);
                    break;
                    case i_bishop:
                        moves = move_handler.bishopMove(static_cast<Square_index>(n), allies,enemies);
                    break;
                    case i_rook:
                        moves = move_handler.rookMove(static_cast<Square_index>(n), allies,enemies);
                    break;
                    case i_queen:
                        moves = move_handler.queenMove(static_cast<Square_index>(n), allies,enemies);
                    break;
                    default:
                        moves = move_handler.kingMove(static_cast<Square_index>(n), allies,enemies);
                    break;
                }
                for (Move move : moves) {
                    all_moves[i++] = move;
                }
            }
        }
    }
    return all_moves;
}