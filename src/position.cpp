//
// Created by metta on 10/10/24.
//

#include "position.h"

#include <random>

Position::Position() {
    this->colors = {Bitboard(0xFFFF),Bitboard(0xFFFF000000000000)};
    this->materials = {39, 39};
    this->boards = {Bitboard(0x00FF00000000FF00),Bitboard(0x4200000000000042)
        ,Bitboard(0x2400000000000024),Bitboard(0x8100000000000081),
        Bitboard(0x0800000000000008),Bitboard(0x1000000000000010)};
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
    initZobrist();
}

uint64_t Position::randomU64() {
    constexpr int rand = 0x1333317;
    static std::mt19937_64 mt(rand);
    std::uniform_int_distribution<uint64_t> dist{};
    return dist(mt);
}

uint64_t Position::hash() const {
    uint64_t hash = 0;
    if (current_player == BLACK) {hash ^= blackHash;}
    for (int i = 0; i < 64;i++) {
        if (pieceOn(static_cast<Square>(i)) != EMPTY) {
            hash ^= hashSquare(hash, static_cast<Square>(i));
        }
    }
    return hash;
}

uint64_t Position::hashSquare(uint64_t hash, Square square) const {
    if (pieces[square] == EMPTY) {return hash;}
    return hash^transpositionTable[square][pieces[square] + 6 * colors[BLACK].hasBit(square)];
}

void Position::initZobrist() {
    for (auto & i : this->transpositionTable) {
        for (uint64_t& j : i) {
            j = randomU64();
        }
    }
    this->blackHash = randomU64();
    for (auto & i : this->castleHash) {
        this->castleHash[i] = randomU64();
    }
    this->currentHash = hash();
}

void Position::newHash(uint64_t hash) {
    this->currentHash = hash;
    this->hashHistory[this->hhSize] = hash;
    this->hhSize++;
}

void Position::endGame(int score) {
    this->result = score;
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

int Position::getDrawCount() const {
    return this->draw_count;
}

std::stack<Piece> Position::getStack() {
    return this->captured_pieces;
}

void Position::unmakeMove(const Move &move) {
    Piece capturedPiece = this->captured_pieces.top();
    this->hashHistory[hhSize] = 0;
    this->hhSize--;
    this->currentHash = this->hashHistory[hhSize-1];
    this->materials[move.player ^ 1] += values[capturedPiece];
    this->captured_pieces.pop();

    // Restore castling rights
    if (move.type == CASTLE) {
        if (move.origin == e1) {
            if (move.destination > move.origin) {
                this->can_castle |= 0b1;  // Restore castling right for kingside
            } else {
                this->can_castle &= 0b10;  // Restore castling right for queenside
            }
        } else {
            if (move.destination > move.origin) {
                this->can_castle |= 0b100;
            } else {
                this->can_castle &= 0b1000;
            }
        }
    }

    // Restore color bitboards and piece position
    this->colors[move.player].removeBit(move.destination);
    this->colors[move.player].addBit(move.origin);
    this->boards[pieceOn(move.destination)].removeBit(move.destination);

    // If it's not a promotion, restore the piece's bitboard
    if (move.type != PROMOTION) {
        this->boards[pieceOn(move.destination)].addBit(move.origin);
    } else {
        this->boards[PAWN].addBit(move.origin);  // Handle promotion separately
    }

    // Handle en passant captures
    if (move.type == EN_PASSANT) {
        int capturedPawnSquare = (move.player == WHITE) ? move.destination - 8 : move.destination + 8;
        this->boards[PAWN].addBit(capturedPawnSquare);  // Restore the captured pawn
        this->pieces[capturedPawnSquare] = PAWN;
        this->colors[move.player ^ 1].addBit(capturedPawnSquare);
    } else if (capturedPiece != EMPTY) {
        this->colors[move.player ^ 1].addBit(move.destination);  // Restore captured piece
    }

    // Update the draw count based on captured pieces
    auto capCopy = this->captured_pieces;
    this->draw_count = 0;
    while (!capCopy.empty() && capCopy.top() == EMPTY) {
        capCopy.pop();
        draw_count++;
    }

    // Restore the piece's original position and flip the player
    this->pieces[move.origin] = pieceOn(move.destination);
    this->current_player ^= 1;
    this->endGame(2);
}

void Position::makeMove(const Move &move) {
    auto captured = pieceOn(move.destination);
    uint64_t hashedBoard;
    this->captured_pieces.push(captured);
    if (captured == EMPTY) {
        this->draw_count++;
    }
    else {
        this->draw_count = 0;
        this->materials[move.player ^ 1] -= values[captured];
    }
    switch (move.type) {
        case EN_PASSANT:
            hashedBoard = passantMove(move);
            break;
        case NORMAL:
            hashedBoard = normalMove(move);
            break;
        case CASTLE:
            hashedBoard = castleMove(move);
            break;
        default:
            hashedBoard = promotionMove(move);
            break;
    }
    assert(move.origin < 64);
    assert(move.origin >= 0);
    assert(move.type == EN_PASSANT or (pseudoAttacker(current_player, findKingSquare(current_player)) == noSquare));
    this->current_player ^= 1;
    if (this->can_castle & 0b0001) {
        if (pieceOn(e1) != KING or pieceOn(h1) != ROOK ) {
            hashedBoard ^= castleHash[0];
            this->can_castle &= 0b1110;
        }
    }
    if (this->can_castle & 0b0010) {
        if (pieceOn(e1) != KING or pieceOn(a1) != ROOK ) {
            hashedBoard ^= castleHash[1];
            this->can_castle &= 0b1101;
        }
    }
    if (this->can_castle & 0b0100) {
        if (pieceOn(e8) != KING or pieceOn(h8) != ROOK ) {
            hashedBoard ^= castleHash[2];
            this->can_castle &= 0b1011;
        }
    }
    if (this->can_castle & 0b1000){
        if (pieceOn(e8) != KING or pieceOn(a8) != ROOK ) {
            hashedBoard ^= castleHash[3];
            this->can_castle &= 0b0111;
        }
    }
    hashedBoard ^= blackHash;;
    newHash(hashedBoard);
    int count = 0;
    for (int i = 0; i < hhSize; i++) {
        std::cout << hashHistory[i]<< std::endl;
        if (hashHistory[i] == hashedBoard) {
            count++;
        }
        if (count == 3) {
            endGame(0);
        }
    }
    if (draw_count == 50) {
        endGame(0);
    }
    if (insufficientMaterial()) {
        endGame(0);
    }
}

uint64_t Position::promotionMove(const Move &move) {
    auto hashedBoard = hash();
    hashedBoard = hashSquare(hashedBoard, move.origin);
    hashedBoard = hashSquare(hashedBoard, move.destination);
    for (auto &board : this->boards) {
        if (board.getBitboard() & Bit(move.destination)) {
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
    hashedBoard = hashSquare(hashedBoard, move.destination);
    return hashedBoard;
}

uint64_t Position::castleMove(const Move &move) {
    auto hashedBoard = this->currentHash;
    if (move.origin + 2 == move.destination) {
        hashedBoard = hashSquare(hashedBoard, move.origin);
        hashedBoard = hashSquare(hashedBoard, static_cast<Square>(move.origin + 2));
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
        hashedBoard = hashSquare(hashedBoard, move.destination);
        hashedBoard = hashSquare(hashedBoard, static_cast<Square>(move.destination - 1));
    }
    else {
        hashedBoard = hashSquare(hashedBoard, move.origin);
        hashedBoard = hashSquare(hashedBoard, static_cast<Square>(move.origin - 2));
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
        hashedBoard = hashSquare(hashedBoard, move.destination);
        hashedBoard = hashSquare(hashedBoard, static_cast<Square>(move.destination + 1));
    }
    return hashedBoard;
}

Piece Position::pieceOn(const Square square) const {
    return this->pieces[square];
}

uint64_t Position::passantMove(const Move &move) {
    auto hashedBoard = this->currentHash;
    hashedBoard = hashSquare(hashedBoard, move.origin);
    hashedBoard = hashSquare(hashedBoard, this->passant);
    // Clear the en passant bit on the board
    for (auto &board : this->boards) {
        board.removeBit(this->passant);  // Clear the en passant square
        if (board.getBitboard() & Bit(move.origin)) {
            board.removeBit(move.origin);  // Remove the moving piece from origin
            board.addBit(move.destination);  // Add the moving piece to the destination
        }
    }

    // Handle the en passant capture
    if (move.type == EN_PASSANT) {
        int capturedPawnSquare = (move.player == WHITE) ? move.destination - 8 : move.destination + 8;
        this->boards[PAWN].removeBit(capturedPawnSquare);  // Remove captured pawn
        this->pieces[capturedPawnSquare] = EMPTY;
        this->colors[move.player ^ 1].removeBit(capturedPawnSquare);  // Remove pawn from opponent's color set
    }

    // Update the piece positions
    pieces[move.destination] = pieces[move.origin];
    pieces[move.origin] = EMPTY;
    this->passant = a1;  // Clear en passant target
    this->draw_count = 0;

    // Update color bitboards
    colors[current_player].removeBit(move.origin);
    colors[current_player].addBit(move.destination);
    hashedBoard = hashSquare(hashedBoard, move.destination);
    return hashedBoard;
}

uint64_t Position::normalMove(const Move &move) {
    auto hashedBoard = this->currentHash;
    hashedBoard = hashSquare(hashedBoard, move.origin);
    hashedBoard = hashSquare(hashedBoard, move.destination);
    for (auto &board : this->boards) {
        if (board.hasBit(move.destination)) {
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
    hashedBoard = hashSquare(hashedBoard, move.destination);
    return hashedBoard;
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
        if (isPseudoAttacked(player,findKingSquare(player))) {
            endGame(-1 + 2 * static_cast<int>(player));
        }
        else {
            endGame(0);
        }
    }
    return result;
}

Square Position::findKingSquare(bool player) const {
    auto kingSquare = noSquare;
    for (int i = 0; i < 64; i++) {
        if (pieces[i] == KING && colors[player].hasBit(i)) {
            kingSquare = static_cast<Square>(i);
            break;
        }
    }
    assert(kingSquare != noSquare);
    return kingSquare;
}

Square Position::pseudoAttacker(bool player, Square square) const {
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

            auto newSquare = static_cast<Square>(newRank * 8 + newFile);
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
                            assert(newSquare >= a1);
                            assert(newSquare < h8);
                            return newSquare;
                        }
                    }
                }
                if (i < 4) {
                    if (attackingPiece == ROOK || attackingPiece == QUEEN) {
                        assert(newSquare >= a1);
                        assert(newSquare < h8);
                        return newSquare;
                    }
                    break;
                }
                if (i < 8) {
                    if (attackingPiece == BISHOP || attackingPiece == QUEEN) {
                        assert(newSquare >= a1);
                        assert(newSquare < h8);
                        return newSquare;
                    }
                    break;
                }
                if (attackingPiece == KNIGHT) {
                    assert(newSquare >= a1);
                    assert(newSquare < h8);
                    return newSquare;  // Knight attacking
                }
                break;
            }

        } while (isSlidingPiece);  // Continue sliding only for rooks, bishops, queens
    }

    // Check for pawn attacks
    if (player == WHITE) {
        if ((colors[player ^ 1].hasBit(square + 9) && pieceOn(static_cast<Square>(square + 9)) == PAWN)) {
            assert(static_cast<Square>(square + 9) >= a1);
            assert(static_cast<Square>(square + 9) < h8);
            return static_cast<Square>(square + 9);
        }
        if (colors[player ^ 1].hasBit(square + 7) && pieceOn(static_cast<Square>(square + 7)) == PAWN) {
            assert(static_cast<Square>(square + 7) >= a1);
            assert(static_cast<Square>(square + 7) < h8);
            return static_cast<Square>(square + 7);
        }
    } else {  // BLACK pawns
        if ((colors[player ^ 1].hasBit(square - 9) && pieceOn(static_cast<Square>(square - 9)) == PAWN)) {
            assert(static_cast<Square>(square - 9) >= a1);
            assert(static_cast<Square>(square - 9) < h8);
            return static_cast<Square>(square - 9);
        }
        if (colors[player ^ 1].hasBit(square - 7) && pieceOn(static_cast<Square>(square - 7)) == PAWN) {
            assert(static_cast<Square>(square - 7) >= a1);
            assert(static_cast<Square>(square - 7) < h8);
            return static_cast<Square>(square - 7);
        }
    }

    // No attack found
    return noSquare;
}

bool Position::isPseudoAttacked(bool player, Square square) const {
    return pseudoAttacker(player, square) != noSquare;
}

bool Position::isKingInDoubleCheck(bool player) const {
    Square kingSquare = findKingSquare(player);
    Square flag = noSquare;
    MoveList enemyMoves = this->pseudoLegal(player ^ 1);
    for (int i = 0; i < enemyMoves.getSize(); i++) {
        if (enemyMoves.getMoves()[i].destination == kingSquare) {
            if (flag != noSquare and flag != enemyMoves.getMoves()[i].origin) {
                return true;
            }
            flag = enemyMoves.getMoves()[i].origin;
        }
    }
    return false;
}
bool Position::insufficientMaterial() const {
    if (this->boards[PAWN].getBitboard() != 0) {return false;}
    if (materials[WHITE] < 4 and materials[BLACK] < 4) {return true;}
    return false;
}

std::array<Square, 8> Position::isBetween(Square square1, Square square2) {
    std::array result = {square2, square1, noSquare, noSquare, noSquare, noSquare, noSquare, noSquare};
    int i = 2;
    auto dir = getDirection(square1, square2);

    if (dir == VERTICAL) {
        // Vertical move (same file)
        int step = (square2 > square1) ? 8 : -8;  // Move up or down the file
        for (int sq = square1 + step; sq != square2; sq += step) {
            result[i++] = static_cast<Square>(sq);
        }
    }
    else if (dir == HORIZONTAL) {
        // Horizontal move (same rank)
        int step = (square2 > square1) ? 1 : -1;  // Move right or left on the rank
        for (int sq = square1 + step; sq != square2; sq += step) {
            result[i++] = static_cast<Square>(sq);
        }
    }
    else if (dir == EVEN) {
        // Diagonal move down-left to up-right (step is 9)
        int step = ((square2 / 8 > square1 / 8) && (square2 % 8 > square1 % 8)) ? 9 : -9;
        for (int sq = square1 + step; sq != square2; sq += step) {
            result[i++] = static_cast<Square>(sq);
        }
    }
    else if (dir == ODD) {
        // Diagonal move up-left to down-right (step is 7)
        int step = ((square2 / 8 > square1 / 8) && (square2 % 8 < square1 % 8)) ? 7 : -7;
        for (int sq = square1 + step; sq != square2; sq += step) {
            result[i++] = static_cast<Square>(sq);
        }
    }

    return result;
}

bool Position::isLegal(const Move &move) {
    Square kingSquare = findKingSquare(move.player);
    assert(isValid(move.origin));


    // Castling handling
    if (move.type == CASTLE) {
        if (move.destination == move.origin + 2) {
            if (isPseudoAttacked(move.player, move.destination) ||
                isPseudoAttacked(move.player, static_cast<Square>(move.destination - 1)) ||
                isPseudoAttacked(move.player, move.origin)) {
                return false;
            }
            return true;
        }
        if (isPseudoAttacked(move.player, move.destination) ||
            isPseudoAttacked(move.player, static_cast<Square>(move.destination + 1)) ||
            isPseudoAttacked(move.player, move.origin)) {
            return false;
        }
        return true;
    }

    // Moving the king
    if (move.origin == kingSquare) {
        if (isPseudoAttacked(move.player, move.destination)) {
            return false;
        }
        return true;
    }

    // Check if the king is in double check
    if (isKingInDoubleCheck(move.player)) {
        return false;
    }

    // Check if the piece is pinned and restrict its movement to the pin direction
    auto dir = directionPinned(move.origin);
    if (dir != NO_DIRECTION) {
        if (getDirection(move.origin, move.destination) != dir) {
            return false;  // A pinned piece can only move along the pin direction
        }
    }
    // Check if the king is in check and the move must block or capture the attacker
    if (isPseudoAttacked(move.player, kingSquare)) {
        Square kingThreat = pseudoAttacker(move.player, kingSquare);

        // Allow capture of the attacking piece
        if (move.destination == kingThreat) {
            return true;
        }

        // Block the attacker by placing a piece in between the king and the attacker
        if (!isSquareBetween(kingSquare, kingThreat, move.destination)) {
            return false;
        }
    }

    return true;  // If no conditions fail, the move is legal
}


bool Position::isSquareBetween(Square square1,Square square2,Square square3) {
    assert(isValid(square1) && isValid(square2) && isValid(square3));
    for(auto square : isBetween(square1,square2)) {
        if (square == square3) {
            return true;
        }
    }
    return false;
}

Direction Position::getDirection(Square sq1, Square sq2) {
    int rank1 = sq1 / 8;
    int file1 = sq1 % 8;
    int rank2 = sq2 / 8;
    int file2 = sq2 % 8;

    if (rank1 == rank2) {
        return HORIZONTAL;
    }
    if (file1 == file2) {
        return VERTICAL;
    }

    // EVEN: down-left to up-right (rank - file remains constant)
    if ((rank1 - file1) == (rank2 - file2)) {
        return EVEN;
    }

    // ODD: up-left to down-right (rank + file remains constant)
    if ((rank1 + file1) == (rank2 + file2)) {
        return ODD;
    }

    return NO_DIRECTION;
}

Direction Position::directionPinned(Square square) const {
    assert(isValid(square));
    bool color;

    // Determine the color of the piece
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
    if (isPinned(square, color, 0, 1) || isPinned(square, color, 0, -1)) {  // Vertical
        return VERTICAL;
    }
    if (isPinned(square, color, 1, 1) || isPinned(square, color, -1, -1)) {  // Diagonal (down-left to up-right)
        return EVEN;
    }
    if (isPinned(square, color, -1, 1) || isPinned(square, color, 1, -1)) {  // Diagonal (up-left to down-right)
        return ODD;
    }

    return NO_DIRECTION;
}


// Helper function to check for pin in any direction
bool Position::isPinned(Square square, bool color, int horizontalInc, const int verticalInc) const {
    assert(isValid(square));

    int rank = square / 8;
    int file = square % 8;

    // Step 1: Check in the direction towards the attacker
    int newRank = rank + verticalInc;
    int newFile = file + horizontalInc;

    // Traverse towards the direction of the potential attacker
    while (newRank >= 0 && newRank < 8 && newFile >= 0 && newFile < 8) {
        auto newSquare = static_cast<Square>(newRank * 8 + newFile);
        auto piece = pieceOn(newSquare);

        if (piece == EMPTY) {
            // Continue searching in the same direction
            newRank += verticalInc;
            newFile += horizontalInc;
            continue;
        }

        // Check if the found piece is an opponent's Rook/Queen (for horizontal/vertical)
        // or Bishop/Queen (for diagonal)
        if ((horizontalInc == 0 || verticalInc == 0) && (piece == ROOK || piece == QUEEN) && this->colors[color ^ 1].hasBit(newSquare)) {
            // Found a potential pinning attacker
            break;
        }
        if ((std::abs(horizontalInc) == std::abs(verticalInc)) && (piece == BISHOP || piece == QUEEN) && this->colors[color ^ 1].hasBit(newSquare)) {
            // Found a potential pinning attacker
            break;
        }

        // If another piece blocks the path, we cannot have a pin
        return false;
    }

    // If we didn't find an attacker, return false
    if (!(newRank >= 0 && newRank < 8 && newFile >= 0 && newFile < 8)) {
        return false;
    }

    // Step 2: Reset the square and check in the direction towards the king
    newRank = rank - verticalInc;
    newFile = file - horizontalInc;

    while (newRank >= 0 && newRank < 8 && newFile >= 0 && newFile < 8) {
        auto newSquare = static_cast<Square>(newRank * 8 + newFile);
        auto piece = pieceOn(newSquare);

        if (piece == EMPTY) {
            // Continue searching in the opposite direction
            newRank -= verticalInc;
            newFile -= horizontalInc;
            continue;
        }

        // Check if the found piece is the player's king
        if (piece == KING && this->colors[color].hasBit(newSquare)) {
            return true;  // Piece is pinned between the king and the attacker
        }

        // If another piece blocks the path to the king, it is not a pin
        return false;
    }

    return false;  // No valid pin found
}






// Helper function to validate board boundaries
bool Position::isValid(Square square) {
    return (square >= 0 && square < 64);  // Ensure the square is within the board's limits
}
