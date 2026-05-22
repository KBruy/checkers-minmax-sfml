#include "SfmlGame.h"
#include "Board.h"
#include "Move.h"

#include <SFML/Graphics.hpp>
#include <optional>

#include <string>
#include <vector>
#include <iostream>

bool loadFont(sf::Font& font){
    return font.openFromFile("assets/fonts/DejaVuSans.ttf");
}

void drawText(sf::RenderWindow& window, const sf::Font& font, const std::string& textValue, float x, float y, unsigned int size){
    sf::Text text(font, textValue, size);
    text.setPosition({x,y});
    text.setFillColor(sf::Color(230,230,230));
    window.draw(text);
}

void drawBoard(sf::RenderWindow& window, const Board& board) {
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
                    sf::CircleShape kingMark;
                    kingMark.setRadius(12.0f);
                    kingMark.setPosition({x + 28.0f, y + 28.0f});
                    kingMark.setFillColor(sf::Color(190, 150, 40));
                    window.draw(kingMark);
                }
            }
        }
    }
}

void drawSidePanel(sf:: RenderWindow& window, const Board& board, char currentPlayer, const sf::Font& font, bool fontLoaded) {
    sf::RectangleShape panel;
    panel.setSize({260.0f, 800.0f});
    panel.setPosition({0.0f, 0.0f});
    panel.setFillColor(sf::Color(35,35,35));
    window.draw(panel);

    sf::RectangleShape line;
    line.setSize({2.0f, 800.0f});
    line.setPosition({260.0f, 0.0f});
    line.setFillColor(sf::Color(90,90,90));
    window.draw(line);

    if (!fontLoaded) {
        return;
    }

    std::vector<Move> legalMoves = board.generateMoves(currentPlayer);

    int captureCount = 0;
    int longestCaptureSteps = 0;

    for (int i =0; i<static_cast<int>(legalMoves.size()); i++) {
        if (legalMoves[i].isCapture) {
            captureCount++;

            int steps = static_cast<int>(legalMoves[i].rows.size()) -1;

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

    drawText(window, font, "Legalne ruchy: " + std::to_string(legalMoves.size()),30.0f,190.0f,18);

    drawText(window, font,"Bicia: " + std::to_string(captureCount), 30.0f, 220.0f, 18);

    if (captureCount > 0) {
        drawText(window, font, "Najdluzsze bicie: " + std::to_string(longestCaptureSteps), 30.0f, 250.0f, 18);

    }

    drawText(window, font, "Ocena bialych: " + std::to_string(board.evaluate('w')), 30.0f, 310.0f, 18);

    drawText(window, font, "Ocena czarnych: " + std::to_string(board.evaluate('b')),30.0f, 340.0f, 18);

    if (board.hasAnyCapture(currentPlayer)) {
        drawText(window, font, "Dostepne bicie!", 30.0f, 410.0f, 20);
    }


}

void runSfmlGame() {
    Board board;
    char currentPlayer = 'w';

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
        }

        window.clear(sf::Color(40,40,40));

        drawSidePanel(window, board, currentPlayer, font, fontLoaded);
        drawBoard(window, board);

        window.display();
    }
}

