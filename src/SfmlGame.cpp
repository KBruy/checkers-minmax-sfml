#include "SfmlGame.h"
#include "Board.h"

#include <SFML/Graphics.hpp>
#include <optional>

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

void drawSidePanel(sf::RenderWindow& window) {
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
}

void runSfmlGame() {
    Board board;

    sf::RenderWindow window(
        sf::VideoMode({1000, 800}),
        "Checkers - Min-Max SFML"
    );

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear(sf::Color(40, 40, 40));

        drawSidePanel(window);
        drawBoard(window, board);

        window.display();
    }
}