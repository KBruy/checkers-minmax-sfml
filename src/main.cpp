#include "Board.h"

#include <iostream>
#include <string>

int main() {
    Board board;
    char currentPlayer = 'w';
    std::string forcedFrom = "";

    std::string from;
    std::string to;

    while (true) {
        board.print();

        if (!board.hasPieces('w')) {
        std::cout << "******************************\n";
        std::cout << "       Czarne wygraly.\n";
        std::cout << "******************************\n\n";
        break;
        }

        if (!board.hasPieces('b')) {
        std::cout << "******************************\n";
        std::cout << "       Biale wygraly.\n";
        std::cout << "******************************\n\n";
        break;
        }

        if (currentPlayer == 'w') {
            std::cout << "Tura bialych.\n";
        } else {
            std::cout << "Tura czarnych.\n";
        }

        if (forcedFrom != "") {
            std::cout << "###############################\n";
            std::cout << "Musisz kontynuowac bicie pionkiem z pola "
                      << forcedFrom << ".\n";
            std::cout << "###############################\n\n";
        } else if (board.hasAnyCapture(currentPlayer)) {
            std::cout << "###############################\n";
            std::cout << "Dostepne jest bicie.\n";
            std::cout << "###############################\n\n";
        }

        std::cout << "Podaj ruch, np. c3 d4 albo q zeby wyjsc: ";
        std::cin >> from;

        if (from == "q") {
            break;
        }

        std::cin >> to;

        if (forcedFrom != "" && from != forcedFrom) {
            std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
            std::cout << "Musisz ruszyc tym samym pionkiem.\n\n";
             std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n";
            continue;
        }

        bool mustCapture = board.hasAnyCapture(currentPlayer);
        bool captureMove = board.isCaptureMove(from, to, currentPlayer);

        if (forcedFrom != "" && !captureMove) {
             std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
            std::cout << "Musisz kontynuowac bicie.\n\n";
             std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n";
            continue;
        }

        if (mustCapture && !captureMove) {
             std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
            std::cout << "Musisz wykonac bicie.\n\n";
             std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n";
            continue;
        }

        if (board.movePiece(from, to, currentPlayer)) {
            std::cout << "Ruch wykonany.\n\n";

            if (captureMove && board.canCaptureFromPosition(to, currentPlayer)) {
                forcedFrom = to;
                std::cout << "Mozesz bic dalej tym samym pionkiem.\n\n";
            } else {
                forcedFrom = "";

                if (currentPlayer == 'w') {
                    currentPlayer = 'b';
                } else {
                    currentPlayer = 'w';
                }
            }
        } else {
             std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
            std::cout << "Niepoprawny ruch.\n\n";
             std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n";
        }
    }

    return 0;
}