#include "Board.h"

#include <iostream>
#include <string>


int main() {
    Board board;

    std::string from;
    std::string to;

    while (true) {
        board.print();

        std::cout << "Podaj ruch ( np.b6 a5) albo q zeby wyjsc: ";
        std::cin >> from;

        if (from == "q") {
            break;
        }

        std::cin >> to;

        if (board.movePiece(from, to)) {
            std::cout << "Ruch wykonany.\n\n";
        } else {
            std::cout << "Niepoprawny ruch \n\n";
        }
    }

    return 0;
}