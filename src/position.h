//
// Created by metta on 10/10/24.
//

#ifndef GAME_H
#define GAME_H

#include <cassert>
#include <cstdint>
#include <string>
#include <sstream>
#include <string>
#include <cctype>

#include "movegen.h"

enum Result { WIN = 1, DRAW = 0, LOSS = -1 };

class Position {
    std::array<Bitboard, 2> colors;
    std::array<Bitboard, 6> boards;
    std::array<int, 2> materials{};
    std::array<Piece, 64> pieces{};
    uint64_t transpositionTable[64][12]{};
    uint64_t blackHash = 0;
    uint64_t castleHash[4]{};
    uint64_t passantHash[8]{};
    uint64_t currentHash = 0;
    std::array<uint64_t,1024> hashHistory{};
    int hhSize = 0;
    int draw_count = 0;
    bool current_player = WHITE;
    std::vector<StackType> stack = {}; // Most significant bits (3 & 4) - Black can castle | Left bit (can castle to left)
    int fullmove_number = 0;
    int result = 2;
public:
    explicit Position(const std::string &fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    [[nodiscard]] std::string to_fen() const;
    static uint_fast64_t randomU64();
    [[nodiscard]] uint64_t hash() const;
    [[nodiscard]] uint64_t hashSquare(uint64_t hash, Square square) const;
    [[nodiscard]] int getMaterials(bool color) const;
    void initZobrist();
    void newHash(uint64_t hash);
    void endGame(int score);
    [[nodiscard]] int getResult() const;
    [[nodiscard]] int countAttackers(bool attackingPlayer, Square square) const;
    [[nodiscard]] int getDrawCount() const;
    [[nodiscard]] std::array<Bitboard, 6> getBitboards() const;
    [[nodiscard]] std::array<Bitboard, 2> getColors() const;
    std::vector<StackType>* getStack();
    void makeMove(const Move &move);
    void unmakeMove(const Move &move);
    [[nodiscard]] Square findKingSquare(bool player) const;
    [[nodiscard]] Piece pieceOn(Square square) const;
    uint64_t promotionMove(const Move &move);
    uint64_t castleMove(const Move &move);
    uint64_t passantMove(const Move &move);
    uint64_t normalMove(const Move &move);
    [[nodiscard]] Square getPassant() const;
    [[nodiscard]] int getCastlingRights() const;
    [[nodiscard]] bool getCurrentPlayer() const;
    [[nodiscard]] bool isKingInDoubleCheck(bool player) const;
    [[nodiscard]] bool insufficientMaterial() const;
    void print() const;
    [[nodiscard]] MoveList pseudoLegal(bool player) const;
    MoveList allMoves(bool player);
    static std::array<Square, 8> isBetween(Square square1, Square square2);
    bool isLegal(const Move &move);
    [[nodiscard]] Square pseudoAttacker(bool player, Square square) const;
    [[nodiscard]] bool isPseudoAttacked(bool player, Square square) const;
    static bool isSquareBetween(Square square1,Square square2,Square square3);
    static Direction getDirection(Square sq1, Square sq2);
    [[nodiscard]] Direction directionPinned(Square square) const;
    [[nodiscard]] bool isPinned(Square square, bool color, int horizontalInc, int verticalInc) const;
    static bool isValid(Square square);
};



#endif //GAME_H