//
// Created by Kian Mesforush on 7/31/25.
//

#ifndef TESTWINDOW_H
#define TESTWINDOW_H
#include "Window.h"
#include "frontend/graph-visualization/GraphVisualization.h"
#include "SFML/Graphics/CircleShape.hpp"

class TestWindow : public Window {
	std::vector<AlgorithmResult>* results;
public:
	TestWindow(std::vector<AlgorithmResult>* results) {
		this->width = 800;
		this->height = 600;
		this->name = WindowNames::MAIN;
		this->results = results;
	}

	void draw(sf::RenderWindow& window) override {
		window.clear(sf::Color::White);

		GraphVisualization vis(*results);

		vis.draw(window, {100, 100});
	}

	void handle_event(sf::RenderWindow& window, const std::optional<sf::Event>& event) override {
		return;
	}

};
#endif //TESTWINDOW_H
