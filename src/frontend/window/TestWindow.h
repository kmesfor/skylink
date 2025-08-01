//
// Created by Kian Mesforush on 7/31/25.
//

#ifndef TESTWINDOW_H
#define TESTWINDOW_H
#include "Window.h"
#include "frontend/graph-visualization/GraphVisualization.h"

constexpr float SCROLL_SPEED = 10; // px rate at which to scroll at

class TestWindow : public Window {
	std::vector<AlgorithmResult>* results;
	GraphVisualization* vis = nullptr;
public:
	TestWindow(std::vector<AlgorithmResult>* results) {
		this->width = 800;
		this->height = 1000;
		this->name = WindowNames::MAIN;
		this->results = results;
		this->vis = new GraphVisualization(*results);
	}

	~TestWindow() override {
		delete this->vis;
	}

	void draw(sf::RenderWindow& window) override {
		window.clear(sf::Color::White);
		vis->draw(window, {100, 300});
	}

	// Handle keyboard left, right, up, down clicks in reference to scrolling
	void handle_event(sf::RenderWindow& window, const std::optional<sf::Event>& event) override {
		// Check if the event is a key press
		if (event && event->getIf<sf::Event::KeyPressed>()) {
			// Handle appropriate scrolling movement based on key direction
			if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Right) {
				vis->scroll({-SCROLL_SPEED, 0});
			} else if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Left) {
				vis->scroll({SCROLL_SPEED, 0});
			} else if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Up) {
				vis->scroll({0, SCROLL_SPEED});
			} else if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Down) {
				vis->scroll({0, -SCROLL_SPEED});
			}
		}
	}

};
#endif //TESTWINDOW_H
