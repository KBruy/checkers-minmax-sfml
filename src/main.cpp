#include "ConsoleGame.h"
#include "SfmlGame.h"

#include <iostream>

int main() {
    int choice;

    std::cout << "===============================\n";
    std::cout << "      Checkers - Min-Max\n";
    std::cout << "===============================\n";
    std::cout << "Wybierz tryb gry:\n";
    std::cout << "1. Konsola\n";
    std::cout << "2. SFML\n";
    std::cout << "Wybor: ";
    std::cin >> choice;

  //wybranie wersji
    if (choice == 1) {
        runConsoleGame();
    } else if (choice == 2) {
        runSfmlGame();
    } else {
        std::cout << "Niepoprawny wybor.\n";
    }
    return 0;
}
