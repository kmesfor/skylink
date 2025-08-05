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

/**
 * Custom SFML component to handle drawing, state management, and event management of a button
 */
class Button {
public:
	// Different states available, active or inactive and primary or secondary (design)
	enum ButtonState {
		PRIMARY_ACTIVE,
		SECONDARY_ACTIVE,
		PRIMARY_INACTIVE,
		SECONDARY_INACTIVE
	};
private:
	// Internal SFML components backing the larger components
	sf::RectangleShape body;
	sf::Text text;

	sf::Color primary = VisualizationConfig::COLOR_PRIMARY;
	sf::Color inactive = sf::Color(170, 172, 181);
	sf::Color secondary = VisualizationConfig::COLOR_SECONDARY;

	// Represents the current state that the button is in
	ButtonState state;

	// Represents if the button is actively being hovered by the mouse
	bool is_hovered = false;

	// A click action to occur when the button is pressed (and can be pressed)
	std::function<void()> action;
public:
	/**
	 * Instantiate a Button object
	 * @param label Text label of the button
	 * @param state Initial ButtonState state of the button
	 */
	Button(const std::string& label, const ButtonState state): text(Frontend::font, label) {
		body = sf::RectangleShape();
		this->state = state;

		action = []() {};
	}

	/**
	 * Set the size of the button and its internal components
	 * @param size sf::Vector2f size
	 */
	void set_size(const sf::Vector2f& size) {
		body.setSize(size);
		body.setOrigin({size.x / 2.0f, size.y / 2.0f});

		const auto bounds = text.getLocalBounds();
		text.setOrigin({bounds.position.x + bounds.size.x / 2.0f,  bounds.position.y + bounds.size.y / 2.0f});
	}

	/**
	 * Set the text size (px) of a button's text label
	 * @param size size in px
	 */
	void set_text_size(const int size) {
		text.setCharacterSize(size);
		set_size(body.getSize());
		set_position(body.getPosition());
	}

	/**
	 * Set the text string displayed on a button. Actively resize components depending on text length
	 * @param text the text string to be displayed
	 */
	void set_text(const std::string& text) {
		this->text.setString(text);
		this->set_size(body.getSize());
	}

	/**
	 * Sets the position in space that the button is drawned
	 * @param position a sf::Vector2f coordinate position
	 */
	void set_position(const sf::Vector2f& position) {
		body.setPosition(position);
		text.setPosition(position);
	}

	/**
	 * Sets the primary color of a button
	 * @param color sf::Color primary color
	 */
	void set_primary(const sf::Color color) {
		primary = color;
	}

	/**
	 * Sets the secondary color of a button
	 * @param color sf::Color secondary color
	 */
	void set_secondary(const sf::Color color) {
		secondary = color;
	}

	/**
	 * Sets the inactive color of a button
	 * @param color sf::Color inactive color
	 */
	void set_inactive(const sf::Color color) {
		inactive = color;
	}

	/**
	 * Updates the state of a button
	 * @param state the new state of the button
	 */
	void set_state(const ButtonState state) {
		this->state = state;
	}

	/**
	 * Toggles the state of the button from active to inactive or vice versa while keeping primary/secondary constant
	 */
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

	/**
	 * Get the ButtonState of the button
	 * @return the button's current state
	 */
	ButtonState get_state() const {
		return state;
	}

	/**
	 * Set the color of a button's text
	 * @param color sf::Color text color
	 */
	void set_text_color(const sf::Color color) {
		text.setFillColor(color);
	}

	/**
	 * Render the button onto a RenderWindow. Dynamically render button based on hovered state
	 * @param window the RenderWindow to draw the button on
	 */
	void draw(sf::RenderWindow& window) {
		// Invert button colors if hovered
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

		// Draw components
		window.draw(body);
		window.draw(text);
	}

	/**
	 * Check if the current mouse pointer is within bounds of the button. Useful for checking if a button was clicked
	 * @param window the current RenderWindow being displayed
	 * @return boolean
	 */
	bool is_in_bounds(const sf::RenderWindow& window) const {
		const sf::Vector2f mouse_position = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		return body.getGlobalBounds().contains(mouse_position);
	}

	/**
	 * Pass events to the button through this function. Handles clicking and hovering states
	 * @param event sf::Event event generated
	 * @param window sf::RenderWindow being displayed
	 */
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

	/**
	 * Perform the button's action if the button's state is active
	 */
	void handle_click() const {
		if (state == PRIMARY_ACTIVE || state == SECONDARY_ACTIVE) {
			action();
		}
	}

	/**
	 * Set the action of the button
	 * @param action std::function<void()> action to perform on click
	 */
	void set_click_action(const std::function<void()>& action) {
		this->action = action;
	}
};
#endif //BUTTON_H
