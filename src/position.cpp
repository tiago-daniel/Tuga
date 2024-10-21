//
// Created by metta on 10/10/24.
//

#include "position.h"

#include <bits/ranges_algobase.h>

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

std::array<Bitboard, 6> Position::getBitboards() const {
    return this->boards;
}

std::array<Bitboard, 2> Position::getColors() const {
    return this->colors;
}

Square Position::getPassant() const {
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
    for (int i = 0; i < moves.getSize(); i++) {;
        if (isLegal(moves.getMoves()[i])) {
            result.push(moves.getMoves()[i]);
        }
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
    for (int i = 0; i < enemyMoves.getSize(); i++) {
        if (enemyMoves.getMoves()[i].destination == kingSquare) {
            return enemyMoves.getMoves()[i].origin;
        }
    }
    return noSquare;
}

bool Position::isPseudoAttacked(bool player, Square square) {
    // Directions: first 4 are orthogonal (rook, queen), next 4 are diagonal (bishop, queen), last 8 are knight moves.
    std::array directions = {
        std::make_pair(-1, 0), std::make_pair(0, -1), std::make_pair(0, 1), std::make_pair(1, 0),
        std::make_pair(-1, -1), std::make_pair(-1, 1), std::make_pair(1, -1), std::make_pair(1, 1),
        std::make_pair(-2, -1), std::make_pair(-2, 1), std::make_pair(-1, -2), std::make_pair(-1, 2),
        std::make_pair(1, -2), std::make_pair(1, 2), std::make_pair(2, -1), std::make_pair(2, 1)
    };

    int rank = square / 8;
    int file = square % 8;

    // Check all directions
    for (int i = 0; i < directions.size(); i++) {
        int dRank = directions[i].first;
        int dFile = directions[i].second;
        int newRank = rank;
        int newFile = file;

        // Handle sliding pieces (rook, bishop, queen)
        bool isSlidingPiece = (i < 8);  // First 8 are sliding directions

        do {
            newRank += dRank;
            newFile += dFile;

            // If out of bounds, stop checking this direction
            if (newRank < 0 || newRank >= 8 || newFile < 0 || newFile >= 8) {
                break;
            }

            auto newSquare = Square(newRank * 8 + newFile);

            // If we encounter a friendly piece, stop
            if (colors[player].hasBit(newSquare) and pieceOn(newSquare) != KING) {
                break;
            }

            // Check if an opponent piece is attacking this square
            if (colors[player ^ 1].hasBit(newSquare)) {
                Piece attackingPiece = pieceOn(newSquare);
                if (i < 8) {
                    if (newRank == rank + dRank and newFile == file + dFile) {
                        if (attackingPiece == KING) {
                            return true;
                        }
                    }
                }
                if (i < 4) {
                    if (attackingPiece == ROOK || attackingPiece == QUEEN) {
                        return true;
                    }
                    break;
                }
                if (i < 8) {
                    if (attackingPiece == BISHOP || attackingPiece == QUEEN) {
                        return true;
                    }
                    break;
                }
                if (attackingPiece == KNIGHT) {
                    return true;  // Knight attacking
                }
                break;
            }

        } while (isSlidingPiece);  // Continue sliding only for rooks, bishops, queens
    }

    // Check for pawn attacks
    if (player == WHITE) {
        if ((colors[player ^ 1].hasBit(square + 9) && pieceOn(static_cast<Square>(square + 9)) == PAWN) ||
            (colors[player ^ 1].hasBit(square + 7) && pieceOn(static_cast<Square>(square + 7)) == PAWN)) {
            return true;
        }
    } else {  // BLACK pawns
        if ((colors[player ^ 1].hasBit(square - 9) && pieceOn(static_cast<Square>(square - 9)) == PAWN) ||
            (colors[player ^ 1].hasBit(square - 7) && pieceOn(static_cast<Square>(square - 7)) == PAWN)) {
            return true;
        }
    }

    // No attack found
    return false;
}

bool Position::isKingInDoubleCheck(bool player) {
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

std::array<Square, 8> Position::isBetween(Square square1, Square square2) {
    std::array result = {square2, square1, noSquare, noSquare, noSquare, noSquare, noSquare, noSquare};
    int i = 2;
    auto dir = getDirection(square1,square2);
    if (dir == VERTICAL) {
        // Vertical move (same file)
        int step = (square2 > square1) ? 8 : -8;  // Move up or down the file
        for (int sq = square1+step; sq != square2; sq += step) {
            result[i++] = static_cast<Square>(sq);
        }
    }
    else if (dir == HORIZONTAL) {
        // Horizontal move (same rank)
        int step = (square2 > square1) ? 1 : -1;  // Move right or left on the rank
        for (int sq = square1+step; sq != square2; sq += step) {
            result[i++] = static_cast<Square>(sq);
        }
    }
    else if (dir == DIAGONAL) {
        // Diagonal move
        int step = (square2 > square1)
            ? (square1 % 8 < square2 % 8 ? 9 : 7)  // Moving up-right or up-left
            : (square1 % 8 < square2 % 8 ? -7 : -9);  // Moving down-right or down-left

        for (int sq = square1+step; sq != square2; sq += step) {
            result[i++] = static_cast<Square>(sq);
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
    Square kingSquare = findKingSquare(move.player);
    if (move.type == EN_PASSANT) {
        makeMove(move);
        if (isPseudoAttacked(move.player, kingSquare)) {
            unmakeMove(move);
            return false;
        }
        unmakeMove(move);
        return true;
    }
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

    if (move.origin == kingSquare) {
        if (isPseudoAttacked(move.player, move.destination)){
            return false;
        }
        return true;
    }
    if (isKingInDoubleCheck(move.player)) {
        return false;
    }
    Square kingThreat = isKingInCheck(move.player, kingSquare);
    if (kingThreat != noSquare) {
        if (not isSquareBetween(kingSquare, kingThreat,move.destination)){
            return false;
        }
    }
    auto dir = directionPinned(move.origin);
    if (dir != NO_DIRECTION) {
        if (getDirection(move.origin, move.destination) == dir) {
            return true;
        }
        return false;
    }
    return true;
}

Direction Position::getDirection(Square sq1, Square sq2) {
    if (sq1 / 8 == sq2 / 8) {
        return HORIZONTAL;
    }
    if (sq1 % 8 == sq2 % 8) {
        return VERTICAL;
    }
    if (std::abs((sq1 / 8) - (sq2 / 8)) == std::abs((sq1 % 8) - (sq2 % 8))) {
        return DIAGONAL;
    }
    return NO_DIRECTION;
}

Direction Position::directionPinned(Square square) {
    bool color;

    // Determine color of the piece
    if (this->colors[WHITE].hasBit(square)) {
        color = WHITE;
    } else if (this->colors[BLACK].hasBit(square)) {
        color = BLACK;
    } else {
        return NO_DIRECTION;
    }

    // Check for horizontal, vertical, and diagonal pins
    if (isPinned(square, color, 1, 0) || isPinned(square, color, -1, 0)) {  // Horizontal
        return HORIZONTAL;
    }
    if (isPinned(square, color, 0, 8) || isPinned(square, color, 0, -8)) {  // Vertical
        return VERTICAL;
    }
    if (isPinned(square, color, 9, 0) || isPinned(square, color, -9, 0)) {  // Diagonal (right-down, left-up)
        return DIAGONAL;
    }
    if (isPinned(square, color, 7, 0) || isPinned(square, color, -7, 0)) {  // Diagonal (left-down, right-up)
        return DIAGONAL;
    }

    return NO_DIRECTION;
}

// Helper function to check for pin in any direction
bool Position::isPinned(Square square, bool color, int horizontalInc, int verticalInc) {
    int flag = 0b00;
    auto newSquare = square + horizontalInc + verticalInc;

    // Move in the direction to see if there's a pinning piece
    while (isValid(Square(newSquare))) {
        auto piece = pieceOn(static_cast<Square>(newSquare));
        if (piece == noSquare) {
            newSquare += horizontalInc + verticalInc;
            continue;
        }
        if (piece == KING && this->colors[color].hasBit(newSquare)) {
            flag += 0b01;
            break;
        }
        if ((piece == ROOK || piece == QUEEN || piece == BISHOP) && this->colors[color ^ 1].hasBit(newSquare)) {
            flag += 0b10;
            break;
        }
        break;
    }

    // Move in the opposite direction to confirm pin
    newSquare = square - horizontalInc - verticalInc;
    while (flag > 0b00 && isValid(Square(newSquare))) {
        auto piece = pieceOn(static_cast<Square>(newSquare));
        if (piece == noSquare) {
            newSquare -= horizontalInc + verticalInc;
            continue;
        }
        if (piece == KING && this->colors[color].hasBit(newSquare)) {
            return true;  // Confirm the piece is pinned
        }
        if ((piece == ROOK || piece == QUEEN || piece == BISHOP) && this->colors[color ^ 1].hasBit(newSquare) && flag == 0b01) {
            return true;  // Confirm the piece is pinned
        }
        break;
    }

    return false;
}

// Helper function to validate board boundaries
bool Position::isValid(Square square) {
    return (square >= 0 && square < 64);  // Ensure the square is within the board's limits
}
