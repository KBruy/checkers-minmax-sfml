#ifndef MOVE_H
#define MOVE_H

#include <vector>

struct Move {
    std::vector<int> rows;
    std::vector<int> cols;
    bool isCapture;

    Move();
    Move(int fromRow, int fromCol, int toRow, int toCol, bool isCapture);

    int getFromRow() const;
    int getFromCol() const;
    int getToRow() const;
    int getToCol() const;

    void addStep(int row, int col);
};

#endif