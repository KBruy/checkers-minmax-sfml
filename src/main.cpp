#include "Board.h"

#include <iostream>
#include <string>


int main() {
    Board board;
    char currentPlayer = 'w';

    std::string from;
    std::string to;

    while (true) {
        board.print();

        if (currentPlayer == 'w') {
            std::cout << "Tura białych \n\n";
        } else {
            std::cout << "Tura czarnych \n\n";
        }

        if (board.hasAnyCapture(currentPlayer)) {
            std::cout << "###############################\n";
            std::cout << "    Dostępne jest bicie!\n";
            std::cout << "###############################\n\n";
        }

        std::cout << "Podaj ruch, np. c3 d4, albo q zeby wyjsc: ";
        std::cin >> from;

        if (from == "q") {
            break;
        }

        std::cin >> to;

        bool mustCapture = board.hasAnyCapture(currentPlayer);
        bool captureMove = board.isCaptureMove(from, to, currentPlayer);

        if (mustCapture && !captureMove) {
            std::cout << "Musisz wykonac bicie \n\n";
            continue;
        }

        if (board.movePiece(from, to, currentPlayer)) {
            std::cout<<"Ruch wykonany \n\n";

            if (currentPlayer == 'w') {
                currentPlayer = 'b';
            } else {
                currentPlayer = 'w';
            }
        } else {
            std::cout << "Niepoprawny ruch \n\n";
        }
    }
    return 0;
}