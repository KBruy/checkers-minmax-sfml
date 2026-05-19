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

    bool canManCaptureFrom(int row, int col, char player) const;
    bool canKingCaptureFrom(int row, int col, char player) const;


public:
    Board();

    void print() const;
    bool movePiece(const std::string& from, const std::string& to, char player);
    bool hasAnyCapture(char player) const; //monitoring czy pojawia sie bicie
    bool isCaptureMove(const std::string& from, const std::string& to, char player) const; // zwraca czy obecnie jest wymuszenie bicia
    bool canCaptureFromPosition(const std::string& position, char player) const; //wielokrotne bicie

    bool hasPieces(char player) const; //monitor czy gra sie skonczyla (gracz ma pionki)
    
};

#endif