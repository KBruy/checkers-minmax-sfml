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

    int absRowDiff = rowDiff;
    int absColDiff = colDiff;

    if (absRowDiff < 0) {
        absRowDiff = -absRowDiff;
    }

    if (absColDiff < 0) {
        absColDiff = -absColDiff;
    }

    // Ruch albo bicie damki po przekatnej
    if (isKing(piece)) {
        if (absRowDiff != absColDiff || absRowDiff == 0) {
            return false;
        }

        // Zwykly ruch damki po pustej przekatnej
        if (isPathClear(fromRow, fromCol, toRow, toCol)) {
            fields[toRow][toCol] = piece;
            fields[fromRow][fromCol] = '.';

            return true;
        }

        // Bicie damki z odleglosci
        int capturedRow = -1;
        int capturedCol = -1;

        if (findCapturePieceOnPath(fromRow, fromCol, toRow, toCol, player, capturedRow, capturedCol)) {
            fields[toRow][toCol] = piece;
            fields[fromRow][fromCol] = '.';
            fields[capturedRow][capturedCol] = '.';

            return true;
        }

        return false;
    }

    // Zwykly ruch pionka o jedno pole
    if ((rowDiff == 1 || rowDiff == -1) && (colDiff == 1 || colDiff == -1)) {
        if (player == 'w' && rowDiff != -1) {
            return false;
        }

        if (player == 'b' && rowDiff != 1) {
            return false;
        }

        fields[toRow][toCol] = piece;
        fields[fromRow][fromCol] = '.';

        promoteIfNeeded(toRow, toCol);

        return true;
    }

    // Bicie pionka o dwa pola, do przodu albo do tylu
    if ((rowDiff == 2 || rowDiff == -2) && (colDiff == 2 || colDiff == -2)) {
        int middleRow = (fromRow + toRow) / 2;
        int middleCol = (fromCol + toCol) / 2;

        char middlePiece = fields[middleRow][middleCol];

        if (!isOpponentPiece(middlePiece, player)) {
            return false;
        }

        fields[toRow][toCol] = piece;
        fields[fromRow][fromCol] = '.';
        fields[middleRow][middleCol] = '.';

        promoteIfNeeded(toRow, toCol);

        return true;
    }

    return false;
}

bool Board::isPlayerPiece(char piece, char player) const {
    if (player == 'w') {
        return piece == 'w' || piece == 'W';
    }

    if (player == 'b') {
        return piece == 'b' || piece == 'B';
    }

    return false;
}

bool Board::isOpponentPiece(char piece, char player) const {
    if (player == 'w') {
        return piece == 'b' || piece == 'B';
    }

    if (player == 'b') {
        return piece == 'w' || piece == 'W';
    }

    return false;
}

bool Board::isKing(char piece) const {
    return piece == 'W' || piece == 'B';
}

void Board::promoteIfNeeded(int row, int col) {
    if (fields[row][col] == 'w' && row == 0) {
        fields[row][col] = 'W';
    }

    if (fields[row][col] == 'b' && row == 7) {
        fields[row][col] = 'B';
    }
}

bool Board::isPathClear(int fromRow, int fromCol, int toRow, int toCol) const {
    int rowStep = (toRow > fromRow) ? 1 : -1;
    int colStep = (toCol > fromCol) ? 1 : -1;

    int row = fromRow + rowStep;
    int col = fromCol + colStep;

    while (row != toRow && col != toCol) {
        if (fields[row][col] != '.') {
            return false;
        }

        row += rowStep;
        col += colStep;
    }

    return true;
}

bool Board::findCapturePieceOnPath(int fromRow, int fromCol, int toRow, int toCol, char player, int& capturedRow, int& capturedCol) const {
    int rowStep = (toRow > fromRow) ? 1 : -1;
    int colStep = (toCol > fromCol) ? 1 : -1;

    int row = fromRow + rowStep;
    int col = fromCol + colStep;

    bool foundOpponent = false;

    while (row != toRow && col != toCol) {
        char current = fields[row][col];

        if (current != '.') {
            if (isPlayerPiece(current, player)) {
                return false;
            }
            if (isOpponentPiece(current, player)) {
                if (foundOpponent) {
                    return false;
                }

                foundOpponent = true;
                capturedRow = row;
                capturedCol = col;
            }
        }

        row += rowStep;
        col += colStep;
    }

    return foundOpponent;
}

bool Board::canManCaptureFrom(int row, int col, char player) const {
    char piece = fields[row][col];

    if (!isPlayerPiece(piece, player) || isKing(piece)) {
        return false;
    }

    int directions[4][2] = {
        {-2, -2},
        {-2, 2},
        {2, -2},
        {2, 2}
    };

    for (int i = 0; i < 4; i++) {
        int targetRow = row + directions[i][0];
        int targetCol = col + directions[i][1];

        if (targetRow < 0 || targetRow >= 8 || targetCol < 0 || targetCol >= 8) {
            continue;
        }

        if (fields[targetRow][targetCol] != '.') {
            continue;
        }

        int middleRow = (row + targetRow) /2;
        int middleCol = (col + targetCol) / 2;

        if (isOpponentPiece(fields[middleRow][middleCol],player)) {
            return true;
        }
    }

    return false;
}

bool Board::canKingCaptureFrom(int row, int col, char player) const {
    char piece = fields[row][col];

    if (!isPlayerPiece(piece, player) || !isKing(piece)) {
        return false;
    }

    int directions[4][2] = {
        {-1, -1}, {-1, 1}, {1, -1}, {1,1}
    };

    for (int i = 0; i<4; i++) {
        bool foundOpponent = false;

        int currentRow = row + directions[i][0];
        int currentCol = col + directions[i][1];

        while (currentRow >= 0 && currentRow < 8 &&
             currentCol >= 0 && currentCol < 8) {

              char current = fields[currentRow][currentCol];

              if (current == '.') {
                if (foundOpponent) {
                    return true;
                }
              } else if (isPlayerPiece(current, player)) {
                break;
              } else if (isOpponentPiece(current, player)) {
                if (foundOpponent) {
                    break;
                }

                foundOpponent = true;

              }

              currentRow += directions[i][0];
              currentCol += directions[i][1];
                
        }
    }

    return false;
}

bool Board::hasAnyCapture(char player) const {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            char piece = fields[row][col];

            if (!isPlayerPiece(piece, player)) {
                continue;
            }

            if (isKing(piece)) {
                if (canKingCaptureFrom(row, col, player)) {
                    return true;
                }
            } else {
                if (canManCaptureFrom(row, col, player)) {
                    return true;
                }
            }
        }
    }

    return false;
    
}