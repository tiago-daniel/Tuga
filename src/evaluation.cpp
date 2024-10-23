//
// Created by metta on 10/22/24.
//

#include "evaluation.h"


int Evaluation::evaluate(const Position& game) {
    if (game.getCurrentPlayer() == WHITE) return game.getMaterials(WHITE) - game.getMaterials(BLACK);
    return game.getMaterials(BLACK) - game.getMaterials(WHITE);
}