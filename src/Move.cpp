#include "Move.h"

Move::Move() {
    fromRow = 0;
    fromCol = 0;
    toRow = 0;
    toCol = 0;
    isCapture = false;
}

Move::Move(int fromRow, int fromCol, int toRow, int toCol, bool isCapture) {
    this->fromRow = fromRow;
    this->fromCol = fromCol;
    this->toRow = toRow;
    this->toCol = toCol;
    this->isCapture = isCapture;
}