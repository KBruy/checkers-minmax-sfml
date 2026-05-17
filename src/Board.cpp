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

    std::cout << "   a b c d e f g h\n";
}

