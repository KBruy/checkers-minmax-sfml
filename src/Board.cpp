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

bool Board::movePiece(const std::string& from, const std::string& to) {
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

    if (fields[toRow][toCol] != '.') {
        return false;
    }

    int rowDiff = toRow - fromRow;
    int colDiff = toCol - fromCol;

    if (rowDiff < -1 || rowDiff > 1 || colDiff < -1 || colDiff > 1) {
        return false;
    }

    if (rowDiff == 0 || colDiff == 0) {
        return false;
    }

    fields[toRow][toCol] = piece;
    fields[fromRow][fromCol] = '.';

    return true;
}