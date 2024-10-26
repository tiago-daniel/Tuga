//
// Created by metta on 10/22/24.
//

#include "evaluation.h"


double Evaluation::evaluate(const Position& game) {
    bool color = game.getCurrentPlayer();
    if (game.getResult() != 2) {
        return game.getResult() * 1000 - 1;
    }
    if (game.getMaterials(color) > game.getMaterials(color^1)) {
        return game.getMaterials(color) - game.getMaterials(color^1) * 2;
    }
    return game.getMaterials(color) ^ 2 - game.getMaterials(color^1) ^ 2;
}