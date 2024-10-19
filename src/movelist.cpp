//
// Created by metta on 10/17/24.
//

#include "movelist.h"


MoveList::MoveList() = default;

void MoveList::push(Move move){
    this->moves[size] = move;
    this->size++;
};

int MoveList::getSize() const{
    return this->size;
};

bool MoveList::isEmpty() const{
    return this->size == 0;
};

void MoveList::remove(Move move){
    for(int i = 0; i < this->size; i++){
      if(this->moves[i] == move){
          for(int j = i; j < this->size; j++) {
              this->moves[j] = this->moves[j + 1];
          }
      }
    }
    this->size--;
};

std::array<Move, 256> MoveList::getMoves() const {
    return this->moves;
};

void MoveList::clear() {
    std::array<Move,256> emptyMoves;
    this->moves = emptyMoves;
    this->size = 0;
};

bool MoveList::has(Move move) const {
    for (int i = 0; i < this->size; i++) {
        if (this->moves[i] == move) {
            return true;
        }
    }
    return false;
};