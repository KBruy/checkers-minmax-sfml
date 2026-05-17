#ifndef BOARD_H
#define BOARD_H

#include <string>

class Board {
private:
    char fields[8][8];

    bool parsePosition(const std::string& position, int& row, int& col) const;

    bool isPlayerPiece(char piece, char player) const;

public:
    Board();

    void print() const;
    bool movePiece(const std::string& from, const std::string& to, char player);
};

#endif