//
// Created by metta on 10/10/24.
//

#include "position.h"

Position::Position() {
    this->colors = {Bitboard(65535),Bitboard(18446462598732840960)};
    this->boards = {Bitboard(71776119061282560),Bitboard(460763277876331008)
        ,Bitboard(288230376151711746),Bitboard(9367487224930631681),
        Bitboard(576460752303423496),Bitboard(1152921504606846992)};
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
    this->captured_pieces = {};
}

void Position::endGame(int score) {
    if (this->result == 2) {
        this->result = score;
    }
}

int Position::getResult() const {
    return this->result;
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

int Position::getCastlingRights() const {
    return this->can_castle;
}

std::stack<Piece> Position::getStack() {
    return this->captured_pieces;
}

void Position::unmakeMove(Move move) {
    Piece capturedPiece = this->captured_pieces.top();
    this->captured_pieces.pop();
    if (move.type == CASTLE) {
        if (move.origin == e1) {
            if (move.destination > move.origin) {
                this->can_castle |= 0b1;
            }
            else {
                this->can_castle &= 0b10;
            }
        }
        else {
            if (move.destination > move.origin) {
                this->can_castle |= 0b100;
            }
            else {
                this->can_castle &= 0b1000;
            }
        }
    }
    this->colors[move.player].removeBit(move.destination);
    this->colors[move.player].addBit(move.origin);
    this->boards[pieceOn(move.destination)].removeBit(move.destination);
    if (move.type != PROMOTION) {
        this->boards[pieceOn(move.destination)].addBit(move.origin);
    }
    else {
        this->boards[PAWN].addBit(move.origin);
    }
    if (capturedPiece != EMPTY) {
        this->colors[move.player ^ 1].addBit(move.destination);
        this->boards[capturedPiece].addBit(move.destination);
        auto capCopy = this->captured_pieces;
        this->draw_count = 0;
        while (!capCopy.empty() and capCopy.top() == EMPTY) {
            capCopy.pop();
            draw_count++;
        }
    }
    else {
        draw_count--;
    }
    this->pieces[move.origin] = pieceOn(move.destination);
    this->pieces[move.destination] = capturedPiece;
    this->current_player ^= 1;
}

void Position::makeMove(Move move) {
    this->draw_count++;
    this->captured_pieces.push(pieceOn(move.destination));
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
    if (this->can_castle & 0b0001) {
        if (pieceOn(e1) != KING or pieceOn(h1) != ROOK ) {
            this->can_castle &= 0b1110;
        }
    }
    if (this->can_castle & 0b0010) {
        if (pieceOn(e1) != KING or pieceOn(a1) != ROOK ) {
            this->can_castle &= 0b1101;
        }
    }
    if (this->can_castle & 0b0100) {
        if (pieceOn(e8) != KING or pieceOn(h8) != ROOK ) {
            this->can_castle &= 0b1011;
        }
    }
    if (this->can_castle & 0b1000){
        if (pieceOn(e8) != KING or pieceOn(a8) != ROOK ) {
            this->can_castle &= 0b0111;
        }
    }
    if (draw_count == 50) {
        endGame(0);
    }
}

void Position::promotionMove(Move move) {
    for (auto &board : this->boards) {
        if (board.getBitboard() & Bit(move.destination)) {
            this->draw_count = 0;
            board.removeBit(move.destination);
        }
        else if (board.getBitboard() & Bit(move.origin)) {
            board.removeBit(move.origin);
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
        for (auto &board : this->boards) {
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
        colors[current_player].removeBit(move.origin+3);
        colors[current_player].addBit(move.destination-1);
    }
    else {
        for (auto &board : this->boards) {
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
    for (auto &board : this->boards) {
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
    for (auto &board : this->boards) {
        if (board.hasBit(move.destination)) {
            this->draw_count = 0;
            board.removeBit(move.destination);
        }
        if (board.hasBit(move.origin)) {
            board.removeBit(move.origin);
            board.addBit(move.destination);
        }
    }

    pieces[move.destination] = pieces[move.origin];  // Copy piece from origin to destination
    pieces[move.origin] = EMPTY;  // Set origin square as empty

    this->passant = a1;
    if (this->boards[PAWN].hasBit(move.destination) and
    (move.destination == move.origin + 16 || move.destination == move.origin - 16)) {
        this->passant = move.destination; // Set en passant square
    }
    colors[current_player].removeBit(move.origin);
    colors[current_player ^ 1].removeBit(move.destination);
    colors[current_player].addBit(move.destination);
}

bool Position::getCurrentPlayer() const {
    return this->current_player;
}

void Position::print() const {
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
            else {
                std::cout << " " << " ";
            }
        }
        std::cout << std::endl;
    }
}

MoveList Position::pseudoLegal(bool player) const {
    auto moves = MoveList();
    Bitboard allies = this->colors[player];
    Bitboard enemies = this->colors[player xor 1];
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            int n = row * 8 + col;
            if (allies.getBitboard() & Bit(n)) {
                switch (this->pieces[n]) {
                    case PAWN:
                        MoveGen::pawnMove(moves, static_cast<Square>(n), player, allies, enemies, passant);
                    break;
                    case KNIGHT:
                        MoveGen::knightMove(moves, player, static_cast<Square>(n), allies);
                    break;
                    case BISHOP:
                        MoveGen::bishopMove(moves, player, static_cast<Square>(n), allies,enemies);
                    break;
                    case ROOK:
                        MoveGen::rookMove(moves, player, static_cast<Square>(n), allies,enemies);
                    break;
                    case QUEEN:
                        MoveGen::queenMove(moves, player, static_cast<Square>(n), allies,enemies);
                    break;
                    case KING:
                        MoveGen::kingMove(moves, player, static_cast<Square>(n), allies);
                        MoveGen::castleMove(moves, player, static_cast<Square>(n), this->can_castle,allies, enemies);
                        break;
                    default:
                        break;
                }
            }
        }
    }
    return moves;
}

MoveList Position::allMoves(bool player) {
    auto moves = pseudoLegal(player);
    auto result = MoveList();
    for (int i = 0; i < moves.getSize(); i++) {
        makeMove(moves.getMoves()[i]);
        if (isKingInCheck(player,findKingSquare(player)) < 64) {
            unmakeMove(moves.getMoves()[i]);
            continue;
        }
        unmakeMove(moves.getMoves()[i]);
        result.push(moves.getMoves()[i]);
    }
    if (result.getSize() == 0) {
        if (isKingInCheck(player,findKingSquare(player))) {
            endGame(1 - 2 * static_cast<int>(player));
        }
        else {
            endGame(0);
        }
    }
    return result;
}

Square Position::findKingSquare(bool player) const {
    auto kingSquare = Square(64);
    for (int i = 0; i < 64; i++) {
        if (pieces[i] == KING && colors[player].hasBit(i)) {
            kingSquare = static_cast<Square>(i);
            break;
        }
    }
    return kingSquare;
}

Square Position::isKingInCheck(bool player, Square kingSquare) const {
    MoveList enemyMoves = this->pseudoLegal(player ^ 1);
    for (const auto& move : enemyMoves.getMoves()) {
        if (move.destination == kingSquare) {
            return move.origin;
        }
    }
    return noSquare;
}

/*

bool Position::isPseudoAttacked(bool player, Square square) const {
    MoveList enemyMoves = this->pseudoLegal(player ^ 1);
    for(auto& move : enemyMoves.getMoves()) {
        if (move.destination == square) {
            return true;
        }
    }
    return false;
}

bool Position::isKingInDoubleCheck(bool player) const {
    Square kingSquare = a1;
    bool flag = false;
    for (int i = 0; i < 64; i++) {
        if (pieces[i] == KING && colors[player].hasBit(i)) {
            kingSquare = static_cast<Square>(i);
            break;
        }
    }
    MoveList enemyMoves = this->pseudoLegal(player ^ 1);
    for (const auto& move : enemyMoves.getMoves()) {
        if (move.destination == kingSquare) {
            if (flag == true) {
                return true;
            }
            flag = true;
        }
    }
    return false;
}

std::array<Square, 7> Position::isBetween(Square square1, Square square2) {
    std::array result = {noSquare, noSquare, noSquare, noSquare, noSquare, noSquare, noSquare};
    int i = 0;
    if (square1 % 8 == square2 % 8) {
        // Vertical move
        while (square1 != square2) {
            result[i++] = square2;
            square2 = static_cast<Square>(square2 > square1 ? square2 - 8 : square2 + 8);
        }
    }
    else if (square1 / 8 == square2 / 8) {
        // Horizontal move
        while (square1 != square2) {
            result[i++] = square2;
            square2 = static_cast<Square>(square2 > square1 ? square2 - 1 : square2 + 1);
        }
    }
    else if (std::abs((square1 / 8) - (square2 / 8)) == std::abs((square1 % 8) - (square2 % 8))) {
        // Diagonal move
        while (square1 != square2) {
            result[i++] = square2;
            square2 = static_cast<Square>(square2 > square1
                ? (square2 - 9 * (square1 % 8 < square2 % 8)) - 7 * (square1 % 8 > square2 % 8)
                : (square2 + 9 * (square1 % 8 < square2 % 8)) + 7 * (square1 % 8 > square2 % 8));
        }
    }
    return result;
}

bool Position::isSquareBetween(Square square1,Square square2,Square square3) {
    for(auto square : isBetween(square1,square2)) {
        if (square == square3) {
            return true;
        }
    }
    return false;
}

bool Position::isLegal(Move move) {
    if (move.type == CASTLE) {
        if (move.destination == move.origin + 2) {
            if (isPseudoAttacked(move.player, move.destination) or
                isPseudoAttacked(move.player, static_cast<Square>(move.destination - 1)) or
                isPseudoAttacked(move.player, move.origin)) {
                return false;
            }
            return true;
        }
        if (isPseudoAttacked(move.player, move.destination) or
            isPseudoAttacked(move.player, static_cast<Square>(move.destination + 1)) or
            isPseudoAttacked(move.player, move.origin)) {
            return false;
        }
        return true;
    }

    if (pieceOn(move.origin) == KING) {
        if (isPseudoAttacked(move.player, move.destination)){
            return false;
        }
        return true;
    }
    if (isKingInDoubleCheck(move.player)) {
        return false;
    }
    Square kingSquare = findKingSquare(move.player);
    Square kingThreat = isKingInCheck(move.player, kingSquare);
    if (kingThreat != noSquare) {
        if (not isSquareBetween(kingSquare, kingThreat,move.destination)){
            return false;
        }
    }
    return true;
}
*/