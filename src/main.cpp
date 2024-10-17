//
// Created by metta on 10/9/24.
//

#include "position.h"

int main() {
  Position game;
  game.getBitboards()[0].print();
  int n = 0;
  while(true) {
    game.print();
    auto moves = game.allMoves(game.getCurrentPlayer());
    for (int i = 0; i < moves.getSize(); i++) {
      std::cout << i << " : " << moves.getMoves()[i] << std::endl;
    }
    std::cin >> n;
    game.makeMove(moves.getMoves()[n]);
  }
  return 0;
}