#include "SfmlGame.h"
#include "AI.h"
#include "Board.h"
#include "Move.h"

#include <SFML/Graphics.hpp>

#include <chrono>
#include <iostream>
#include <optional>
#include <string>
#include <thread>
#include <vector>

bool loadFont(sf::Font& font) {
    return font.openFromFile("assets/fonts/DejaVuSans.ttf");
}

void drawText(sf::RenderWindow& window,
              const sf::Font& font,
              const std::string& textValue,
              float x,
              float y,
              unsigned int size) {
    sf::Text text(font, textValue, size);
    text.setPosition({x, y});
    text.setFillColor(sf::Color(230, 230, 230));
    window.draw(text);
}

bool mouseToBoardPosition(int mouseX, int mouseY, int& row, int& col) {
    // przelicza klikniecie myszy na pole planszy
    const float panelWidth = 260.0f;
    const float boardLeft = panelWidth + 50.0f;
    const float boardTop = 80.0f;
    const float squareSize = 80.0f;

    if (mouseX < boardLeft || mouseY < boardTop) {
        return false;
    }

    col = static_cast<int>((mouseX - boardLeft) / squareSize);
    row = static_cast<int>((mouseY - boardTop) / squareSize);

    if (row < 0 || row >= 8 || col < 0 || col >= 8) {
        return false;
    }

    return true;
}

std::string positionToText(int row, int col) {
    // zamiana indeksow tablicy na zapis typu "a1".
    char file = 'a' + col;
    char rank = '8' - row;

    std::string position = "";
    position += file;
    position += rank;

    return position;
}

void drawBoard(sf::RenderWindow& window,
               const Board& board,
               int selectedRow,
               int selectedCol,
               const Move& lastAiMove) {
    const float panelWidth = 260.0f;
    const float boardLeft = panelWidth + 50.0f;
    const float boardTop = 80.0f;
    const float squareSize = 80.0f;

    sf::RectangleShape square;
    square.setSize({squareSize, squareSize});

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            float x = boardLeft + col * squareSize;
            float y = boardTop + row * squareSize;

            square.setPosition({x, y});

            if ((row + col) % 2 == 0) {
                square.setFillColor(sf::Color(235, 210, 170));
            } else {
                square.setFillColor(sf::Color(120, 70, 40));
            }

            window.draw(square);

            char piece = board.getPiece(row, col);

            if (piece != '.') {
                // Pionki rysujemy jako kola na polach.
                sf::CircleShape circle;
                circle.setRadius(30.0f);
                circle.setPosition({x + 10.0f, y + 10.0f});

                if (piece == 'w' || piece == 'W') {
                    circle.setFillColor(sf::Color(230, 230, 230));
                    circle.setOutlineColor(sf::Color(40, 40, 40));
                } else {
                    circle.setFillColor(sf::Color(30, 30, 30));
                    circle.setOutlineColor(sf::Color(220, 220, 220));
                }

                circle.setOutlineThickness(3.0f);
                window.draw(circle);

                if (piece == 'W' || piece == 'B') {
                    // Male zlote kolko oznacza damke.
                    sf::CircleShape kingMark;
                    kingMark.setRadius(12.0f);
                    kingMark.setPosition({x + 28.0f, y + 28.0f});
                    kingMark.setFillColor(sf::Color(190, 150, 40));
                    window.draw(kingMark);
                }
            }
        }
    }

    if (lastAiMove.rows.size() >= 2) {
        // Niebieska ramka pokazujaca ruch ai
        for (int i = 0; i < static_cast<int>(lastAiMove.rows.size()); i++) {
            int row = lastAiMove.rows[i];
            int col = lastAiMove.cols[i];

            float x = boardLeft + col * squareSize;
            float y = boardTop + row * squareSize;

            sf::RectangleShape aiSelection;
            aiSelection.setSize({squareSize - 8.0f, squareSize - 8.0f});
            aiSelection.setPosition({x + 4.0f, y + 4.0f});
            aiSelection.setFillColor(sf::Color(0, 0, 0, 0));
            aiSelection.setOutlineColor(sf::Color(60, 160, 255));
            aiSelection.setOutlineThickness(4.0f);

            window.draw(aiSelection);
        }
    }

    if (selectedRow >= 0 && selectedCol >= 0) {
        // Zolta ramka pokazuje wybrany pionek gracza
        float x = boardLeft + selectedCol * squareSize;
        float y = boardTop + selectedRow * squareSize;

        sf::RectangleShape selection;
        selection.setSize({squareSize, squareSize});
        selection.setPosition({x, y});
        selection.setFillColor(sf::Color(0, 0, 0, 0));
        selection.setOutlineColor(sf::Color(255, 220, 0));
        selection.setOutlineThickness(5.0f);

        window.draw(selection);
    }
}

void drawSidePanel(sf::RenderWindow& window,
                   const Board& board,
                   char currentPlayer,
                   const std::string& statusMessage,
                   int lastVisitedNodes,
                   const sf::Font& font,
                   bool fontLoaded) {
    sf::RectangleShape panel;
    panel.setSize({260.0f, 800.0f});
    panel.setPosition({0.0f, 0.0f});
    panel.setFillColor(sf::Color(35, 35, 35));
    window.draw(panel);

    sf::RectangleShape line;
    line.setSize({2.0f, 800.0f});
    line.setPosition({260.0f, 0.0f});
    line.setFillColor(sf::Color(90, 90, 90));
    window.draw(line);

    if (!fontLoaded) {
        return;
    }

    std::vector<Move> legalMoves = board.generateMoves(currentPlayer);

    int captureCount = 0;
    int longestCaptureSteps = 0;

    // panel pokazuje proste statystyki aktualnej pozycji
    for (int i = 0; i < static_cast<int>(legalMoves.size()); i++) {
        if (legalMoves[i].isCapture) {
            captureCount++;

            int steps = static_cast<int>(legalMoves[i].rows.size()) - 1;

            if (steps > longestCaptureSteps) {
                longestCaptureSteps = steps;
            }
        }
    }

    std::string playerText;

    if (currentPlayer == 'w') {
        playerText = "Tura: biale";
    } else {
        playerText = "Tura: czarne";
    }

    drawText(window, font, "Checkers", 30.0f, 40.0f, 28);
    drawText(window, font, "Min-Max", 30.0f, 75.0f, 22);

    drawText(window, font, playerText, 30.0f, 140.0f, 20);

    drawText(window, font,
             "Legalne ruchy: " + std::to_string(legalMoves.size()),
             30.0f,
             190.0f,
             18);

    drawText(window, font,
             "Bicia: " + std::to_string(captureCount),
             30.0f,
             220.0f,
             18);

    if (captureCount > 0) {
        drawText(window, font,
                 "Najdluzsze bicie: " + std::to_string(longestCaptureSteps),
                 30.0f,
                 250.0f,
                 18);
    }

    drawText(window, font,
             "Ocena bialych: " + std::to_string(board.evaluate('w')),
             30.0f,
             310.0f,
             18);

    drawText(window, font,
             "Ocena czarnych: " + std::to_string(board.evaluate('b')),
             30.0f,
             340.0f,
             18);

    drawText(window, font,
             "Stany AI: " + std::to_string(lastVisitedNodes),
             30.0f,
             375.0f,
             18);

    if (board.hasAnyCapture(currentPlayer)) {
        drawText(window, font,
                 "Dostepne bicie!",
                 30.0f,
                 410.0f,
                 20);
    }

    drawText(window, font, "Status:", 30.0f, 460.0f, 18);
    drawText(window, font, statusMessage, 30.0f, 490.0f, 15);

    drawText(window, font, "Zolta ramka: wybor", 30.0f, 660.0f, 15);
    drawText(window, font, "Niebieska: ruch AI", 30.0f, 685.0f, 15);
}

void runSfmlGame() {
    // glebokosc min-max
    const int aiDepth = 3;
    Board board;
    AI ai('b', aiDepth);

    char currentPlayer = 'w';

    int selectedRow = -1;
    int selectedCol = -1;

    Move lastAiMove;
    int lastVisitedNodes = 0;

    std::string statusMessage = "Kliknij bialy pionek.";

    bool gameOver = false;
    sf::Clock gameOverClock;

    sf::Font font;
    bool fontLoaded = loadFont(font);

    if (!fontLoaded) {
        std::cout << "Nie udalo sie wczytac fontu assets/fonts/DejaVuSans.ttf\n";
    }

    sf::RenderWindow window(sf::VideoMode({1000, 800}), "Checkers - Min-Max");

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButton->button == sf::Mouse::Button::Left &&
                    currentPlayer == 'w' &&
                    !gameOver) {
                    int row;
                    int col;

                    if (mouseToBoardPosition(mouseButton->position.x,
                                             mouseButton->position.y,
                                             row,
                                             col)) {
                        char piece = board.getPiece(row, col);

                        if (selectedRow == -1 && selectedCol == -1) {
                            // Pierwsze klikniecie wybiera pionek
                            if (piece == 'w' || piece == 'W') {
                                selectedRow = row;
                                selectedCol = col;
                                statusMessage = "Wybrano pole: " + positionToText(row, col);
                            } else {
                                statusMessage = "Wybierz bialy pionek.";
                            }
                        } else {
                            if (piece == 'w' || piece == 'W') {
                                // Klikniecie innego bialego pionka zmienia wybor
                                selectedRow = row;
                                selectedCol = col;
                                statusMessage = "Zmieniono wybor na: " + positionToText(row, col);
                            } else {
                                std::string from = positionToText(selectedRow, selectedCol);
                                std::string to = positionToText(row, col);

                                bool mustCapture = board.hasAnyCapture(currentPlayer);
                                bool captureMove = board.isCaptureMove(from, to, currentPlayer);

                                // Drugie klikniecie probuje wykonac ruch
                                if (mustCapture && !captureMove) {
                                    statusMessage = "Musisz wykonac bicie.";
                                    selectedRow = -1;
                                    selectedCol = -1;
                                } else if (board.movePiece(from, to, currentPlayer)) {
                                    lastAiMove = Move();
                                    lastVisitedNodes = 0;

                                    statusMessage = "Ruch: " + from + " -> " + to;

                                    if (captureMove && board.canCaptureFromPosition(to, currentPlayer)) {
                                        // Po biciu zostaje zaznaczony ten sam pionek
                                        selectedRow = row;
                                        selectedCol = col;
                                        statusMessage = "Kontynuuj bicie z pola " + to;
                                    } else {
                                        selectedRow = -1;
                                        selectedCol = -1;
                                        currentPlayer = 'b';
                                    }
                                } else {
                                    statusMessage = "Niepoprawny ruch.";
                                    selectedRow = -1;
                                    selectedCol = -1;
                                }
                            }
                        }
                    }
                }
            }
        }

        if (!gameOver) {
            // Co klatke sprawdzamy, czy gra juz sie skonczyla
            std::vector<Move> currentMoves = board.generateMoves(currentPlayer);

            if (!board.hasPieces('w')) {
                gameOver = true;
                gameOverClock.restart();
                statusMessage = "Koniec gry. Czarne wygraly. Reset za 10 s.";
                selectedRow = -1;
                selectedCol = -1;
            } else if (!board.hasPieces('b')) {
                gameOver = true;
                gameOverClock.restart();
                statusMessage = "Koniec gry. Biale wygraly. Reset za 10 s.";
                selectedRow = -1;
                selectedCol = -1;
            } else if (currentMoves.empty()) {
                gameOver = true;
                gameOverClock.restart();
                selectedRow = -1;
                selectedCol = -1;

                if (currentPlayer == 'w') {
                    statusMessage = "Biale nie maja ruchu. Czarne wygraly. Reset za 10 s.";
                } else {
                    statusMessage = "Czarne nie maja ruchu. Biale wygraly. Reset za 10 s.";
                }
            }
        }

        if (gameOver) {
            // Po koncu gry plansza sama resetuje sie po kilku sekundach
            int elapsedSeconds = static_cast<int>(gameOverClock.getElapsedTime().asSeconds());
            int secondsLeft = 10 - elapsedSeconds;

            if (secondsLeft <= 0) {
                board = Board();
                ai = AI('b', aiDepth);

                currentPlayer = 'w';

                selectedRow = -1;
                selectedCol = -1;

                lastAiMove = Move();
                lastVisitedNodes = 0;

                gameOver = false;
                statusMessage = "Nowa gra. Kliknij bialy pionek.";
            }
        }

        if (currentPlayer == 'b' && !gameOver) {
            statusMessage = "Komputer mysli...";

            // Ruch komputera jest liczony automatycznie przez AI
            Move aiMove = ai.findBestMove(board);
            lastVisitedNodes = ai.getVisitedNodes();

            if (aiMove.rows.size() >= 2) {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));

                board.applyMove(aiMove);
                lastAiMove = aiMove;

                statusMessage = "Komputer wykonal ruch.";
                currentPlayer = 'w';
            } else {
                statusMessage = "Komputer nie ma ruchu.";
                gameOver = true;
                gameOverClock.restart();
            }
        }

        window.clear(sf::Color(40, 40, 40));

        drawSidePanel(window, board, currentPlayer, statusMessage, lastVisitedNodes, font, fontLoaded);
        drawBoard(window, board, selectedRow, selectedCol, lastAiMove);

        window.display();
    }
}
