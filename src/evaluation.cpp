//
// Created by metta on 10/22/24.
//

#include "evaluation.h"


int Evaluation::evaluate(const Position& game) {
    bool color = game.getCurrentPlayer();
    if (game.getResult() != 2) {
        return game.getResult() * 1000 - 1;
    }
    return game.getMaterials(color) - game.getMaterials(color^1);
}