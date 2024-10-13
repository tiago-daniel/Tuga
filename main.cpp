//
// Created by metta on 10/9/24.
//

#include "h/game.h"
#include "test.h"

int main() {
  game game;
  game.print();
  for (Move move : game.allMoves()) {
    cout << move << endl;
  }
  return 0;
}