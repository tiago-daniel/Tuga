//
// Created by metta on 10/10/24.
//

#include "position.h"

#include <random>

Position::Position(const std::string &fen) {
    // Initialize colors, materials, boards, and pieces arrays to zero or empty states
    initAllAttackTables();
    for (int i = 0; i < 64; i++) {
        find_magic(i, 1);
    }
    for (int i = 0; i < 64; i++) {
        find_magic(i, 0);
    }
    std::array actualColors = {Bitboard(0), Bitboard(0)};
    this->colors = actualColors;
    this->materials = {0, 0};
    std::array actualBoards = {Bitboard(0), Bitboard(0), Bitboard(0), Bitboard(0), Bitboard(0), Bitboard(0)};
    this->boards = actualBoards;
    this->pieces = {
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
    };

    // Split the FEN string into its components
    std::istringstream fen_stream(fen);
    std::string piece_placement, active_color, castling_availability, en_passant_target, halfmove_clock, fullmove_number;
    fen_stream >> piece_placement >> active_color >> castling_availability >> en_passant_target >> halfmove_clock >> fullmove_number;

    // Start parsing the piece placement section
    int square = 56; // Start from 'a8' (rank 8, file a)
    for (char c : piece_placement) {
        if (c == '/') {
            square -= 16; // Move to the next rank
        } else if (isdigit(c)) {
            square += c - '0'; // Skip empty squares
        } else {
            // Determine the piece type and color
            Piece piece_type;
            int color;
            switch (c) {
                case 'P': piece_type = PAWN;   color = WHITE; break;
                case 'N': piece_type = KNIGHT; color = WHITE; break;
                case 'B': piece_type = BISHOP; color = WHITE; break;
                case 'R': piece_type = ROOK;   color = WHITE; break;
                case 'Q': piece_type = QUEEN;  color = WHITE; break;
                case 'K': piece_type = KING;   color = WHITE; break;
                case 'p': piece_type = PAWN;   color = BLACK; break;
                case 'n': piece_type = KNIGHT; color = BLACK; break;
                case 'b': piece_type = BISHOP; color = BLACK; break;
                case 'r': piece_type = ROOK;   color = BLACK; break;
                case 'q': piece_type = QUEEN;  color = BLACK; break;
                case 'k': piece_type = KING;   color = BLACK; break;
                default:
                    throw std::invalid_argument("Invalid FEN string: unrecognized piece type");
            }

            // Update the pieces array
            this->pieces[square] = piece_type;

            // Update the colors bitboards
            this->colors[color].addBit(square);

            // Update the piece type bitboards
            this->boards[piece_type].addBit(square);

            // Update the material counts (assuming you have predefined piece values)
            this->materials[color] += values[piece_type];

            square++;
        }
    }

    // Additional parsing can be done here for active color, castling rights, en passant target, etc.
    // For example, to set the side to move:
    this->current_player = (active_color == "w") ? WHITE : BLACK;

    // Handle castling rights, en passant target, halfmove clock, and fullmove number as needed
    Square passant = noSquare;
    if (en_passant_target.size() == 2) passant = static_cast<Square>(string_to_square(en_passant_target));
    int can_castle = 0;
    // If the castling availability is '-', no castling rights are available
    if (castling_availability != "-") {
        for (char c : castling_availability) {
            switch (c) {
                case 'K':
                    can_castle |= 0b1000; // White can castle kingside
                break;
                case 'Q':
                    can_castle |= 0b0100; // White can castle queenside
                break;
                case 'k':
                    can_castle |= 0b0010; // Black can castle kingside
                break;
                case 'q':
                    can_castle |= 0b0001; // Black can castle queenside
                break;
                default:
                    // Handle invalid characters if necessary
                        throw std::invalid_argument("Invalid castling character in FEN");
            }
        }
    }
    this->stack.push_back(StackType{passant, can_castle, EMPTY});
    if (!halfmove_clock.empty()) this->draw_count = std::stoi(halfmove_clock);
    if (!fullmove_number.empty()) this->fullmove_number = std::stoi(fullmove_number);
    initZobrist();
}

std::string Position::to_fen() const {
    std::string fen;

    // Generate piece placement
    for (int rank = 7; rank >= 0; --rank) {
        int empty_squares = 0;
        for (int file = 0; file < 8; ++file) {
            int square = rank * 8 + file;
            Piece piece_type = pieces[square];
            if (piece_type == EMPTY) {
                empty_squares++;
            } else {
                if (empty_squares > 0) {
                    fen += std::to_string(empty_squares);
                    empty_squares = 0;
                }

                char piece_char;
                switch (piece_type) {
                    case PAWN:   piece_char = 'P'; break;
                    case KNIGHT: piece_char = 'N'; break;
                    case BISHOP: piece_char = 'B'; break;
                    case ROOK:   piece_char = 'R'; break;
                    case QUEEN:  piece_char = 'Q'; break;
                    case KING:   piece_char = 'K'; break;
                    default:     piece_char = '?'; break; // Should not happen
                }

                // Determine the color
                if (colors[BLACK].hasBit(square)) {
                    piece_char = std::tolower(piece_char);
                }
                fen += piece_char;
            }
        }
        if (empty_squares > 0) {
            fen += std::to_string(empty_squares);
        }
        if (rank > 0) {
            fen += '/';
        }
    }

    // Side to move
    fen += ' ';
    fen += (current_player == WHITE) ? 'w' : 'b';

    // Castling availability
    fen += ' ';
    std::string castling;
    if (stack.back().castling_rights & 0b0100) castling += 'K'; // White kingside
    if (stack.back().castling_rights & 0b1000) castling += 'Q'; // White queenside
    if (stack.back().castling_rights & 0b0001) castling += 'k'; // Black kingside
    if (stack.back().castling_rights & 0b0010) castling += 'q'; // Black queenside
    if (castling.empty()) castling = "-";
    fen += castling;

    // En passant target square
    fen += ' ';
    if (stack.back().passant != a1) {
        fen += squareToString(stack.back().passant);
    } else {
        fen += '-';
    }

    // Halfmove clock
    fen += ' ';
    fen += std::to_string(draw_count);

    // Fullmove number (Assuming you have a fullmove_number attribute)
    fen += ' ';
    fen += std::to_string(fullmove_number);

    return fen;
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
    if (pieces[square] == EMPTY or square == noSquare) {return hash;}
    return hash^transpositionTable[square][pieces[square] + 6 * colors[BLACK].hasBit(square)];
}

int Position::getMaterials(bool color) const {
    return this->materials[color];
}

void Position::initZobrist() {
    for (auto & i : this->transpositionTable) {
        for (uint64_t& j : i) {
            j = randomU64();
        }
    }
    this->blackHash = randomU64();
    for (auto & i : this->castleHash) {
        i = randomU64();
    }
    for (auto & i : this->passantHash) {
        i = randomU64();
    }
    newHash(hash());
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
    return stack.back().passant;
}

int Position::getCastlingRights() const {
    return stack.back().castling_rights;
}

int Position::getDrawCount() const {
    return this->draw_count;
}

std::vector<StackType>* Position::getStack() {
    return &this->stack;
}

void Position::unmakeMove(const Move &move) {
    this->fullmove_number = (move.player == WHITE ) ? fullmove_number : fullmove_number - 1;
    Piece capturedPiece = this->stack.back().captured;
    this->hashHistory[hhSize] = 0;
    this->hhSize--;
    this->currentHash = this->hashHistory[hhSize-1];
    this->materials[move.player ^ 1] += values[capturedPiece];

    // Restore castling rights
    if (move.type == CASTLE) {
            if (move.destination > move.origin) {
                this->colors[move.player].removeBit(move.destination);
                this->colors[move.player].removeBit(move.destination - 1);
                this->colors[move.player].addBit(move.origin);
                this->colors[move.player].addBit(move.destination + 1);
                this->boards[KING].removeBit(move.destination);
                this->boards[KING].addBit(move.origin);
                this->boards[ROOK].removeBit(move.destination - 1);
                this->boards[ROOK].addBit(move.destination + 1);
                pieces[move.origin] = KING;
                pieces[move.destination + 1] = ROOK;
                pieces[move.destination] = EMPTY;
                pieces[move.destination-1] = EMPTY;
            }
            else {
                this->colors[move.player].removeBit(move.destination);
                this->colors[move.player].removeBit(move.destination + 1);
                this->colors[move.player].addBit(move.origin);
                this->colors[move.player].addBit(move.destination - 2);
                this->boards[KING].removeBit(move.destination);
                this->boards[KING].addBit(move.origin);
                this->boards[ROOK].removeBit(move.destination + 1);
                this->boards[ROOK].addBit(move.destination - 2);
                pieces[move.origin] = KING;
                pieces[move.destination - 2] = ROOK;
                pieces[move.destination] = EMPTY;
                pieces[move.destination-1] = EMPTY;
                pieces[move.destination + 1] = EMPTY;
            }
    }

    // Restore color bitboards and piece position
    else {
        this->colors[move.player].removeBit(move.destination);
        this->colors[move.player].addBit(move.origin);
        this->boards[pieceOn(move.destination)].removeBit(move.destination);
    }

    // If it's not a promotion, restore the piece's bitboard
    if (move.type != PROMOTION) {
        this->boards[pieceOn(move.destination)].addBit(move.origin);
    } else {
        this->boards[PAWN].addBit(move.origin);  // Handle promotion
        this->pieces[move.origin] = PAWN;
        this->pieces[move.destination] = capturedPiece;
    }

    // Handle en passant captures
    if (move.type == EN_PASSANT) {
        int capturedPawnSquare = (move.player == WHITE) ? move.destination - 8 : move.destination + 8;
        this->boards[PAWN].addBit(capturedPawnSquare);  // Restore the captured pawn
        this->pieces[capturedPawnSquare] = PAWN;
        this->colors[move.player ^ 1].addBit(capturedPawnSquare);
        capturedPiece = EMPTY;
    } else if (capturedPiece != EMPTY) {
        this->colors[move.player ^ 1].addBit(move.destination);  // Restore captured piece
    }

    // Update the draw count based on captured pieces
    this->draw_count = 0;
    size_t i = stack.size() - 1;
    while (stack[i].captured == EMPTY) {
        draw_count++;
        if (i-- == 0) break;
    }

    // Restore the piece's original position and flip the player
    if (move.type != CASTLE and move.type != PROMOTION) {
        this->pieces[move.origin] = pieceOn(move.destination);
        this->pieces[move.destination] = capturedPiece;
    }
    this->current_player ^= 1;
    this->endGame(2);
    this->stack.pop_back();
}

void Position::makeMove(const Move &move) {
    Piece captured;
    this->fullmove_number = (move.player == WHITE ) ? fullmove_number : fullmove_number + 1;
    if (move.type == CASTLE) {captured = EMPTY;}
    else if (move.type == EN_PASSANT) {captured = PAWN;}
    else {captured = pieceOn(move.destination);}
    uint64_t hashedBoard;
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
    assert(move.type == EN_PASSANT or pseudoAttacker(current_player, findKingSquare(current_player)) == noSquare);
    int castling_rights = stack.back().castling_rights;
    if (castling_rights & 0b0001) {
        if (pieceOn(e1) != KING or pieceOn(h1) != ROOK or colors[BLACK].hasBit(h1)) {
            hashedBoard ^= castleHash[0];
            castling_rights &= 0b1110;
        }
    }
    if (castling_rights & 0b0010) {
        if (pieceOn(e1) != KING or pieceOn(a1) != ROOK or colors[BLACK].hasBit(a1)) {
            hashedBoard ^= castleHash[1];
            castling_rights &= 0b1101;
        }
    }
    if (castling_rights & 0b0100) {
        if (pieceOn(e8) != KING or pieceOn(h8) != ROOK or colors[WHITE].hasBit(h8)) {
            hashedBoard ^= castleHash[2];
            castling_rights &= 0b1011;
        }
    }
    if (castling_rights & 0b1000){
        if (pieceOn(e8) != KING or pieceOn(a8) != ROOK or colors[WHITE].hasBit(a8)) {
            hashedBoard ^= castleHash[3];
            castling_rights &= 0b0111;
        }
    }
    auto passant = noSquare;
    if ((move.origin == move.destination + 16 or move.origin == move.destination - 16) and pieces[move.destination] == PAWN) {
        passant = move.destination;
    }
    stack.push_back(StackType{passant,castling_rights, captured});
    this->current_player ^= 1;

    hashedBoard ^= blackHash;
    if (passant != noSquare) hashedBoard ^= passantHash[passant % 8];
    newHash(hashedBoard);
    int count = 0;
    for (int i = 0; i < hhSize; i++) {
        if (hashHistory[i] == hashedBoard) {
            count+=1;
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
    this->materials[move.player] += values[move.promotion]- 1;
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
    hashedBoard = hashSquare(hashedBoard, stack.back().passant);
    // Clear the en passant bit on the board
    for (auto &board : this->boards) {
        board.removeBit(stack.back().passant);  // Clear the en passant square
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
    auto allies = this->colors[player].getBitboard();
    auto enemies = this->colors[player xor 1].getBitboard();
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            int n = row * 8 + col;
            if (allies & Bit(n)) {
                switch (this->pieces[n]) {
                    case PAWN:
                        MoveGen::pawnMove(moves, static_cast<Square>(n), player, Bitboard(allies), Bitboard(enemies), stack.back().passant);
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
                        MoveGen::castleMove(moves, player, static_cast<Square>(n), stack.back().castling_rights,Bitboard(allies), Bitboard(enemies));
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


        do {
            newRank += dRank;
            newFile += dFile;

            // If out of bounds, stop checking this direction
            if (newRank < 0 || newRank >= 8 || newFile < 0 || newFile >= 8) {
                break;
            }

            auto newSquare = squareIndex(newRank, newFile);
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
                            assert(newSquare <= h8);
                            return newSquare;
                        }
                    }
                }
                if (i < 4) {
                    if (attackingPiece == ROOK || attackingPiece == QUEEN) {
                        assert(newSquare >= a1);
                        assert(newSquare <= h8);
                        return newSquare;
                    }
                    break;
                }
                if (i < 8) {
                    if (attackingPiece == BISHOP || attackingPiece == QUEEN) {
                        assert(newSquare >= a1);
                        assert(newSquare <= h8);
                        return newSquare;
                    }
                    break;
                }
                if (attackingPiece == KNIGHT) {
                    assert(newSquare >= a1);
                    assert(newSquare <= h8);
                    return newSquare;  // Knight attacking
                }
                break;
            }

        } while (i < 8);  // Continue sliding only for rooks, bishops, queens
    }

    // Check for pawn attacks
    if (player == WHITE) {
        if (square < 55) {
            if ((colors[player ^ 1].hasBit(square + 9) && pieceOn(static_cast<Square>(square + 9)) == PAWN)) {
                assert(static_cast<Square>(square + 9) >= a1);
                assert(static_cast<Square>(square + 9) < h8);
                return static_cast<Square>(square + 9);
            }
        }
        if (square < 57){
            if (colors[player ^ 1].hasBit(square + 7) && pieceOn(static_cast<Square>(square + 7)) == PAWN) {
                assert(static_cast<Square>(square + 7) >= a1);
                assert(static_cast<Square>(square + 7) < h8);
                return static_cast<Square>(square + 7);
            }
        }
    } else {
        // BLACK pawns
        if (square >= 9) {
            if ((colors[player ^ 1].hasBit(square - 9) && pieceOn(static_cast<Square>(square - 9)) == PAWN)) {
                assert(static_cast<Square>(square - 9) >= a1);
                assert(static_cast<Square>(square - 9) < h8);
                return static_cast<Square>(square - 9);
            }
        }
        if (square >= 7) {
            if (colors[player ^ 1].hasBit(square - 7) && pieceOn(static_cast<Square>(square - 7)) == PAWN) {
                assert(static_cast<Square>(square - 7) >= a1);
                assert(static_cast<Square>(square - 7) < h8);
                return static_cast<Square>(square - 7);
            }
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
    if (this->boards[PAWN].getBitboard() != 0) {
        return false;
    }
    if (materials[WHITE] < 4 and materials[BLACK] < 4) {
        return true;
    }
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

    if (move.type == EN_PASSANT) {
        makeMove(move);
        if (!isPseudoAttacked(move.player, kingSquare)) {
            unmakeMove(move);
            return true;
        }
        unmakeMove(move);
        return false;
    }

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
