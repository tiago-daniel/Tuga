//
// Created by metta on 10/9/24.
//

#include "h/Position.h"
#include "test.h"

int main() {
  Position game;
  int n = 0;
  while(true) {
    game.print();
    auto moves = game.allMoves(game.getCurrentPlayer());
    for (int i = 0; moves[i] != Move(); i++) {
      cout << i << " : " << moves[i] << endl;
    }
    cin >> n;
    game.makeMove(moves[n]);
  }
  return 0;
}