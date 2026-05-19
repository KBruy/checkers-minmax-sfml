#include "Board.h"

#include <iostream>

Board::Board() {

    for (int row = 0; row < 8; row++) {

        for (int col = 0; col < 8; col++) {

            fields[row][col] = '.';
        }

    }

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 8; col++) {
            if ((row + col) % 2 == 1) {
                fields[row][col] = 'b';
            }
        }
    }

    for (int row = 5; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if ((row + col) % 2 == 1) {
                fields[row][col] = 'w';
            }
        }
    }
}

void Board::print() const {
    std::cout << "   a b c d e f g h\n";

    for (int row = 0; row < 8; row++) {
        std::cout << 8 - row << "  ";

        for (int col = 0; col < 8; col++) {
            std::cout << fields[row][col] << ' ';
        }

        std::cout << " " << 8 - row << '\n';
    }

    
}

bool Board::parsePosition(const std::string& position, int& row, int& col) const {
    if (position.length() != 2) {
        return false;
    }

    char file = position[0];
    char rank = position[1];

    if (file < 'a' || file > 'h') {
        return false;
    }

    if (rank < '1' || rank > '8') {
        return false;
    }

    col = file - 'a';
    row = 8 - (rank - '0');

    return true;
}

bool Board::movePiece(const std::string& from, const std::string& to, char player) {
    int fromRow;
    int fromCol;
    int toRow;
    int toCol;

    if (!parsePosition(from, fromRow, fromCol)) {
        return false;
    }

    if (!parsePosition(to, toRow, toCol)) {
        return false;
    }

    char piece = fields[fromRow][fromCol];

    if (piece == '.') {
        return false;
    }

    if (!isPlayerPiece(piece, player)) {
        return false;
    }

    if (fields[toRow][toCol] != '.') {
        return false;
    }

    int rowDiff = toRow - fromRow;
    int colDiff = toCol - fromCol;

   //Zwykly ruch o jedno pole
    if ((rowDiff == 1 || rowDiff == -1) && (colDiff == 1 || colDiff == -1)) {
        if (player == 'w' && rowDiff != -1) {
            return false;
        }

        if (player == 'b' && rowDiff != 1)  {
            return false;
        }

        if (player == 'b' && rowDiff != 1) {
            return false;
        }

        fields[toRow][toCol] = piece;
        fields[fromRow][fromCol] = '.';

        return true;
    }

    // Bicie o dwa pola, do przodu albo do tyłu
    if ((rowDiff == 2 || rowDiff == -2) && (colDiff == 2 || colDiff == -2)) {
        int middleRow = (fromRow + toRow) / 2;
        int middleCol = (fromCol + toCol) /2;

        char middlePiece = fields[middleRow][middleCol];

        if (!isOpponentPiece(middlePiece, player)) {
            return false;
        }

        fields[toRow][toCol] = piece;
        fields[fromRow][fromCol] = '.';
        fields[middleRow][middleCol] = '.';

        return true;
    }

    return false;
}

bool Board::isPlayerPiece(char piece, char player) const {
    if (player == 'w') {
        return piece == 'w';
    }

    if (player == 'b') {
        return piece == 'b';
    }

    return false;
}

bool Board::isOpponentPiece(char piece, char player) const{
    if (player == 'w') {
        return piece == 'b';
    }

    if (player == 'b') {
        return piece == 'w';
    }

    return false;

}