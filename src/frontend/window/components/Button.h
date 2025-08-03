//
// Created by Kian Mesforush on 8/3/25.
//

#ifndef BUTTON_H
#define BUTTON_H
#include <functional>
#include <string>

#include "frontend/graph-visualization/VisualizationConfig.h"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"

class Button {
public:
	enum ButtonState {
		PRIMARY_ACTIVE,
		SECONDARY_ACTIVE,
		PRIMARY_INACTIVE,
		SECONDARY_INACTIVE
	};
private:
	sf::RectangleShape body;
	sf::Text text;

	sf::Color primary = VisualizationConfig::COLOR_PRIMARY;
	sf::Color inactive = sf::Color(170, 172, 181);
	sf::Color secondary = VisualizationConfig::COLOR_SECONDARY;

	ButtonState state;

	bool is_hovered = false;

	std::function<void()> action;
public:
	Button(const std::string& label, const ButtonState state): text(Frontend::font, label) {
		body = sf::RectangleShape();
		this->state = state;

		action = []() {};
	}

	void set_size(const sf::Vector2f& size) {
		body.setSize(size);
		body.setOrigin({size.x / 2.0f, size.y / 2.0f});

		const auto bounds = text.getLocalBounds();
		text.setOrigin({bounds.position.x + bounds.size.x / 2.0f,  bounds.position.y + bounds.size.y / 2.0f});
	}

	void set_text_size(const int size) {
		text.setCharacterSize(size);
		set_size(body.getSize());
		set_position(body.getPosition());
	}

	void set_position(const sf::Vector2f& position) {
		body.setPosition(position);
		text.setPosition(position);
	}

	void set_primary(const sf::Color color) {
		primary = color;
	}

	void set_secondary(const sf::Color color) {
		secondary = color;
	}

	void set_inactive(const sf::Color color) {
		inactive = color;
	}

	void set_state(const ButtonState state) {
		this->state = state;
	}

	void toggle_state() {
		if (state == PRIMARY_ACTIVE) {
			state = PRIMARY_INACTIVE;
		} else if (state == SECONDARY_ACTIVE) {
			state = SECONDARY_INACTIVE;
		} else if (state == PRIMARY_INACTIVE) {
			state = PRIMARY_ACTIVE;
		} else {
			state = SECONDARY_ACTIVE;
		}
	}

	ButtonState get_state() const {
		return state;
	}

	void set_text_color(const sf::Color color) {
		text.setFillColor(color);
	}

	void draw(sf::RenderWindow& window) {
		if (is_hovered) {
			if (state == PRIMARY_ACTIVE) {
				body.setFillColor(secondary);
			} else if (state == SECONDARY_ACTIVE) {
				body.setFillColor(primary);
			} else {
				body.setFillColor(inactive);
			}
		} else {
			if (state == PRIMARY_ACTIVE) {
				body.setFillColor(primary);
			} else if (state == SECONDARY_ACTIVE) {
				body.setFillColor(secondary);
			} else {
				body.setFillColor(inactive);
			}
		}

		window.draw(body);
		window.draw(text);
	}

	bool is_in_bounds(const sf::RenderWindow& window) const {
		const sf::Vector2f mouse_position = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		return body.getGlobalBounds().contains(mouse_position);
	}

	void handle_event(const sf::Event& event, sf::RenderWindow& window) {
		if (event.getIf<sf::Event::MouseButtonPressed>()) {
			if (is_in_bounds(window)) {
				handle_click();
			}
		} else if (event.getIf<sf::Event::MouseMoved>()) {
			if (is_in_bounds(window)) {
				is_hovered = true;
			} else {
				is_hovered = false;
			}
		}
	}

	void handle_click() const {
		if (state == PRIMARY_ACTIVE || state == SECONDARY_ACTIVE) {
			action();
		}
	}

	void set_click_action(const std::function<void()>& action) {
		this->action = action;
	}
};
#endif //BUTTON_H
