//
// Created by Kian Mesforush on 7/31/25.
//

#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H
#include "../Window.h"
#include "frontend/graph-visualization/GraphVisualization.h"
#include "frontend/window/components/Button.h"

constexpr float SCROLL_SPEED = 10; // px rate at which to scroll at

class ResultWindow : public Window {
	std::vector<AlgorithmResult>* results;
	GraphVisualization* vis = nullptr;
	Button* button = nullptr;
public:
	explicit ResultWindow(std::vector<AlgorithmResult>* results) {
		this->name = WindowNames::MAIN;
		this->results = results;
		this->vis = new GraphVisualization(*results);
		this->button = new Button("test", Button::PRIMARY_ACTIVE);

		button->set_size({50, 50});
		button->set_position({50, 50});
		button->set_text_size(8);
		button->set_click_action([this] () {
			button->toggle_state();
		});
	}

	~ResultWindow() override {
		delete this->vis;
		delete this->button;
	}

	void draw(sf::RenderWindow& window) override {
		window.clear(VisualizationConfig::COLOR_BG);

		button->draw(window);
		vis->draw(window, {200, 50});
	}

	// Handle keyboard left, right, up, down clicks in reference to scrolling
	void handle_event(sf::RenderWindow& window, const std::optional<sf::Event>& event) override {
		// Check if the event is a key press
		if (event) {
			if (event->getIf<sf::Event::KeyPressed>()) {
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
			// Check for click
			} else if (event->getIf<sf::Event::MouseButtonPressed>()) {
				// Pass click to graph handler
				vis->check_if_vertex_clicked(window);
			}

			button->handle_event(*event, window);
		}
	}

};
#endif //RESULTWINDOW_H
