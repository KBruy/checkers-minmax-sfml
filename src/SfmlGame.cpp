#include "SfmlGame.h"

#include <SFML/Graphics.hpp>
#include <optional>

void runSfmlGame() {
    sf::RenderWindow window(sf::VideoMode({1000, 800}), "Checkers - Min-Max SFML");

    while(window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear(sf::Color(40,40,40));
        window.display();
    }
}