#ifndef MOVE_H
#define MOVE_H

struct Move {
    int fromRow;
    int fromCol;
    int toRow;
    int toCol;
    bool isCapture;

    Move();
    Move(int fromRow, int fromCol, int toRow, int toCol, bool isCapture);
};

#endif