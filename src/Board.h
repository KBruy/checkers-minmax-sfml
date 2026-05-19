#ifndef BOARD_H
#define BOARD_H

#include <string>

class Board {
private:
    char fields[8][8];

    bool parsePosition(const std::string& position, int& row, int& col) const;
    bool isPlayerPiece(char piece, char player) const;
    bool isOpponentPiece(char piece, char player) const;

    void promoteIfNeeded(int row, int col);
    bool isKing(char piece) const;
    bool isPathClear(int fromRow, int fromCol, int toRow, int toCol) const;
    //bicie damką na dystans
    bool findCapturePieceOnPath(int fromRow, int fromCol, int toRow, int toCol, char player, int& capturedRow, int& capturedCol) const;


public:
    Board();

    void print() const;
    bool movePiece(const std::string& from, const std::string& to, char player);
};

#endif