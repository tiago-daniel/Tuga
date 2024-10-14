//
// Created by metta on 10/9/24.
//

#include "h/game.h"
#include "test.h"

int main() {
  Game game;
  int n = 0;
  while(true) {
    game.print();
    auto moves = game.allMoves();
    for (uint8_t i = 0; moves[i] != Move(); i++) {
      cout << int(i) << " : " << moves[i] << endl;
    }
    cout << game.getPassant();
    cin >> n;
    game.makeMove(moves[n]);
  }
  return 0;
}