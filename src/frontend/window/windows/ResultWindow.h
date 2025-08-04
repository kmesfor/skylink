//
// Created by Kian Mesforush on 7/31/25.
//

#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H
#include "../Window.h"
#include "frontend/graph-visualization/GraphVisualization.h"
#include "frontend/window/components/Button.h"
#include "frontend/window/components/Textbox.h"

constexpr float SCROLL_SPEED = 20; // px rate at which to scroll at

class ResultWindow : public Window {
	GraphVisualization* dijkstra_vis;
	GraphVisualization* a_star_vis;

	Button* dijkstra_btn;
	Button* a_star_btn;

	bool dijkstra_vis_showing;
	GraphVisualization* current_vis;

public:
	explicit ResultWindow(AlgorithmComparator& comparator) {
		this->name = WindowNames::RESULTS;
		this->dijkstra_vis = new GraphVisualization(comparator.get_dijkstra_results());
		this->a_star_vis = new GraphVisualization(comparator.get_a_star_results());

		this->dijkstra_btn = new Button("View Dijkstra Results", Button::PRIMARY_INACTIVE);
		this->a_star_btn = new Button("View A* Results", Button::PRIMARY_ACTIVE);

		dijkstra_btn->set_size({150, 50});
		dijkstra_btn->set_position({100, 100});
		dijkstra_btn->set_text_size(12);
		dijkstra_btn->set_click_action([this] () {
			toggle_visualization_state();
		});

		a_star_btn->set_size({150, 50});
		a_star_btn->set_position({100, 300});
		a_star_btn->set_text_size(12);
		a_star_btn->set_click_action([this] () {
			toggle_visualization_state();
		});

		this->dijkstra_vis_showing = true;
		current_vis = dijkstra_vis;
	}

	~ResultWindow() override {
		delete this->dijkstra_vis;
		delete this->a_star_vis;
		delete this->dijkstra_btn;
		delete this->a_star_btn;
	}

	void draw(sf::RenderWindow& window) override {
		window.clear(VisualizationConfig::COLOR_BG);

		dijkstra_btn->draw(window);
		a_star_btn->draw(window);
		current_vis->draw(window, {200, 50});
	}

	// Handle keyboard left, right, up, down clicks in reference to scrolling
	void handle_event(sf::RenderWindow& window, const std::optional<sf::Event>& event) override {
		// Check if the event is a key press
		if (event) {
			if (event->getIf<sf::Event::KeyPressed>()) {
				// Handle appropriate scrolling movement based on key direction
				if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Right) {
					current_vis->scroll({-SCROLL_SPEED, 0});
				} else if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Left) {
					current_vis->scroll({SCROLL_SPEED, 0});
				} else if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Up) {
					current_vis->scroll({0, SCROLL_SPEED});
				} else if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Down) {
					current_vis->scroll({0, -SCROLL_SPEED});
				}
			// Check for click
			} else if (event->getIf<sf::Event::MouseButtonPressed>()) {
				// Pass click to graph handler
				current_vis->check_if_vertex_clicked(window);
			}

			dijkstra_btn->handle_event(*event, window);
			a_star_btn->handle_event(*event, window);
		}
	}

	void toggle_visualization_state() {
		this->dijkstra_vis_showing = !this->dijkstra_vis_showing;

		if (dijkstra_vis_showing == true) {
			dijkstra_btn->set_state(Button::ButtonState::PRIMARY_INACTIVE);
			a_star_btn->set_state(Button::ButtonState::PRIMARY_ACTIVE);
			current_vis = dijkstra_vis;
		} else {
			dijkstra_btn->set_state(Button::ButtonState::PRIMARY_ACTIVE);
			a_star_btn->set_state(Button::ButtonState::PRIMARY_INACTIVE);
			current_vis = a_star_vis;
		}
	}

};
#endif //RESULTWINDOW_H
