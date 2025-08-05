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

/**
 * The window to show when the user runs a search
 */
class ResultWindow : public Window {
	// Individual graph visualization instances for easy toggle
	GraphVisualization* dijkstra_vis;
	GraphVisualization* a_star_vis;

	// 3 side buttns
	Button* dijkstra_btn;
	Button* a_star_btn;
	Button* back_btn;

	// State for which visualization is being displayed
	bool dijkstra_vis_showing;
	GraphVisualization* current_vis;

public:
	/**
	 * Instantiate a ResultWindow. Reinstantiate new object when changing inputs
	 * @param comparator AlgorithmComparator data pipeline
	 */
	explicit ResultWindow(AlgorithmComparator& comparator) {
		// Initial component configuration and setup
		this->name = WindowNames::RESULTS;
		this->dijkstra_vis = new GraphVisualization(comparator.get_dijkstra_results());
		this->a_star_vis = new GraphVisualization(comparator.get_a_star_results());

		this->dijkstra_btn = new Button("View Dijkstra Results", Button::PRIMARY_INACTIVE);
		this->a_star_btn = new Button("View A* Results", Button::PRIMARY_ACTIVE);
		this->back_btn = new Button("Back", Button::SECONDARY_ACTIVE);

		// Initial component configuration (not needed on draw)
		dijkstra_btn->set_size({150, 50});
		dijkstra_btn->set_position({100, 100});
		dijkstra_btn->set_text_size(12);
		dijkstra_btn->set_click_action([this] () {
			toggle_visualization_state();
		});

		// Initial component configuration (not needed on draw)
		a_star_btn->set_size({150, 50});
		a_star_btn->set_position({100, 300});
		a_star_btn->set_text_size(12);
		a_star_btn->set_click_action([this] () {
			toggle_visualization_state();
		});

		// Initial component configuration (not needed on draw)
		back_btn->set_size({150, 50});
		back_btn->set_position({100, 500});
		back_btn->set_text_size(12);
		back_btn->set_click_action([this] () {
			this->set_window_signal(WindowSignal::BACK);
		});

		this->dijkstra_vis_showing = true;
		current_vis = dijkstra_vis;
	}

	/**
	 * Cleans up window when no longer being used
	 */
	~ResultWindow() override {
		delete this->dijkstra_vis;
		delete this->a_star_vis;
		delete this->dijkstra_btn;
		delete this->a_star_btn;
		delete this->back_btn;
	}

	/**
	 * Draws components to screen
	 * @param window RenderWindow to draw components on
	 */
	void draw(sf::RenderWindow& window) override {
		window.clear(VisualizationConfig::COLOR_BG);

		dijkstra_btn->draw(window);
		a_star_btn->draw(window);
		current_vis->draw(window, {200, 50});
		back_btn->draw(window);
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

			// Dispatch event to component handlers
			dijkstra_btn->handle_event(*event, window);
			a_star_btn->handle_event(*event, window);
			back_btn->handle_event(*event, window);
		}
	}

	/**
	 * Toggles between displaying the dijkstra and A* visualization
	 */
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
