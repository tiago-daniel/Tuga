//
// Created by metta on 10/22/24.
//

#include "evaluation.h"


double Evaluation::evaluate(const Position& game) {
    bool color = game.getCurrentPlayer();
    double myMaterial = game.getMaterials(color);
    double enemyMaterial = game.getMaterials(!color);
    double materialDifference = myMaterial - enemyMaterial;

    if (materialDifference > 0) {
        return materialDifference * 1.5 - 0.5 * enemyMaterial;
    }
    // If we're behind, discourage trading by slightly boosting our material value
    return materialDifference + 0.5 * myMaterial;
}