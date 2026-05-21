#include "Move.h"

Move::Move() {
    isCapture = false;
}

Move::Move(int fromRow, int fromCol, int toRow, int toCol, bool isCapture) {
    rows.push_back(fromRow);
    cols.push_back(fromCol);

    rows.push_back(toRow);
    cols.push_back(toCol);

    this->isCapture = isCapture;
}

int Move::getFromRow() const {
    return rows[0];
}

int Move::getFromCol() const {
    return cols[0];
}

int Move::getToRow() const {
    return rows[rows.size() - 1];
}

int Move::getToCol() const {
    return cols[cols.size() - 1];
}

void Move::addStep(int row, int col) {
    rows.push_back(row);
    cols.push_back(col);
}