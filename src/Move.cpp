#include "Move.h"

Move::Move() {
    this->fromRow = 0;
    this->fromCol = 0;
    this->toRow = 0;
    this->toCol = 0;
}

Move::Move(int fromRow, int fromCol, int toRow, int toCol) {
    this->fromRow = fromRow;
    this->fromCol = fromCol;
    this->toRow = toRow;
    this->toCol = toCol;
}