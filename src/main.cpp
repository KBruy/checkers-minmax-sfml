#include "AI.h"
#include "Board.h"
#include "Move.h"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

std::string positionToText(int row, int col) {
    char file = 'a' + col;
    char rank = '8' - row;

    std::string position = "";
    position += file;
    position += rank;

    return position;
}

void printMove(const Move& move) {
    for (int i = 0; i < static_cast<int>(move.rows.size()); i++) {
        std::cout << positionToText(move.rows[i], move.cols[i]);

        if (i < static_cast<int>(move.rows.size()) - 1) {
            std::cout << " -> ";
        }
    }

    std::cout << "\n";
}

int main() {
    Board board;
    AI ai('b', 3);

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

        std::vector<Move> legalMoves = board.generateMoves(currentPlayer);

        if (legalMoves.empty()) {
            std::cout << "******************************\n";

            if (currentPlayer == 'w') {
                std::cout << "       Biale nie maja ruchu. Czarne wygraly.\n";
            } else {
                std::cout << "       Czarne nie maja ruchu. Biale wygraly.\n";
            }

            std::cout << "******************************\n\n";
            break;
        }

        int captureCount = 0;
        int longestCaptureSteps = 0;

        for (int i = 0; i < static_cast<int>(legalMoves.size()); i++) {
            if (legalMoves[i].isCapture) {
                captureCount++;

                int steps = static_cast<int>(legalMoves[i].rows.size()) - 1;

                if (steps > longestCaptureSteps) {
                    longestCaptureSteps = steps;
                }
            }
        }

        std::cout << "-------------------------------\n";
        std::cout << "Liczba legalnych ruchow: " << legalMoves.size() << '\n';
        std::cout << "Liczba bic wsrod legalnych ruchow: " << captureCount << '\n';

        if (captureCount > 0) {
            std::cout << "Najdluzsza sekwencja bicia: "
                      << longestCaptureSteps << " ruch/y\n";
        }

        std::cout << "Ocena dla bialych: " << board.evaluate('w') << "\n";
        std::cout << "Ocena dla czarnych: " << board.evaluate('b') << "\n";
        std::cout << "-------------------------------\n\n";

        if (currentPlayer == 'b') {
            std::cout << "Tura komputera, czarne mysla...\n";

            std::this_thread::sleep_for(std::chrono::milliseconds(700));

            Move aiMove = ai.findBestMove(board);

            if (aiMove.rows.size() < 2) {
                std::cout << "Komputer nie znalazl ruchu.\n";
                break;
            }

            std::cout << "Komputer wybral ruch: ";
            printMove(aiMove);

            std::this_thread::sleep_for(std::chrono::milliseconds(700));

            board.applyMove(aiMove);

            std::cout << "Komputer wykonal ruch.\n\n";

            currentPlayer = 'w';
            forcedFrom = "";

            continue;
        }

        std::cout << "Tura bialych.\n";

        if (forcedFrom != "") {
            std::cout << "###############################\n";
            std::cout << "Musisz kontynuowac bicie pionkiem z pola "
                      << forcedFrom << ".\n";
            std::cout << "###############################\n\n";
        } else if (board.hasAnyCapture(currentPlayer)) {
            std::cout << "###############################\n";
            std::cout << "      Dostepne jest bicie.\n";
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
            std::cout << "Musisz ruszyc tym samym pionkiem.\n";
            std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n";
            continue;
        }

        bool mustCapture = board.hasAnyCapture(currentPlayer);
        bool captureMove = board.isCaptureMove(from, to, currentPlayer);

        if (forcedFrom != "" && !captureMove) {
            std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
            std::cout << "Musisz kontynuowac bicie.\n";
            std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n";
            continue;
        }

        if (mustCapture && !captureMove) {
            std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
            std::cout << "Musisz wykonac bicie.\n";
            std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n";
            continue;
        }

        if (board.movePiece(from, to, currentPlayer)) {
            std::cout << "Ruch wykonany.\n\n";

            if (captureMove && board.canCaptureFromPosition(to, currentPlayer)) {
                forcedFrom = to;

                std::cout << "###############################\n";
                std::cout << "Mozesz bic dalej tym samym pionkiem.\n";
                std::cout << "###############################\n\n";
            } else {
                forcedFrom = "";
                currentPlayer = 'b';
            }
        } else {
            std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
            std::cout << "         Niepoprawny ruch.\n";
            std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n";
        }
    }

    return 0;
}