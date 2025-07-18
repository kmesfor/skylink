//
// Created by Kian Mesforush on 7/17/25.
//

#include "frontend.h"

#include <SFML/Graphics.hpp>

void Frontend::display() {
	sf::VideoMode videoMode;
	videoMode.size = sf::Vector2u(500, 500);

	sf::RenderWindow window(videoMode, "SFML Test");

	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
		}

		window.clear(sf::Color::Blue);
		window.display();
	}
}
