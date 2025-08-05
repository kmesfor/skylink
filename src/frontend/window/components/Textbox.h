//
// Created by Kian Mesforush on 8/4/25.
//

#ifndef TEXTBOX_H
#define TEXTBOX_H
#include <functional>

#include "frontend/frontend.h"
#include "frontend/graph-visualization/VisualizationConfig.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"

/**
 * Custom SFML component to handle drawing, state management, and event management of a text input box
 */
class Textbox {

	// Internal SFML components that comprise a textbox
	sf::RectangleShape body;
	sf::Text label;
	sf::Text input;

	// the user inputted text
	sf::String input_text;

	// Placeholder text to show if the user's text is blank
	std::string textbox_placeholder;

	// Primary and secondary coloring
	sf::Color primary = VisualizationConfig::COLOR_PRIMARY;
	sf::Color secondary = VisualizationConfig::COLOR_SECONDARY;

	// Function to determine if a textbox can be submitted. i.e. validate its contents
	std::function<bool()> can_submit_func;

	// Represents if a textbox can be submitted. this is updated every time text is inputted to allow easy access to textbox
	// state without constant recalculation
	bool can_submit;

	// distance offset between the textbox's body and its label
	sf::Vector2f label_offset;

	bool is_clicked; // state to check if that is the active textbox (multiple on screen)
	bool is_hovered = false; // state to determine if a textbox is being hovered
public:
	/**
	 * Create a Textbox object
	 * @param label The label to show above the textbox
	 * @param textbox_placeholder The string to display when no data is inputted
	 * @param label_offset The offset between textbox and its label
	 */
	Textbox(const std::string& label, const std::string& textbox_placeholder, const sf::Vector2f label_offset): label(Frontend::font, label), input(Frontend::font, textbox_placeholder) {
		body = sf::RectangleShape();
		this->is_clicked = false;
		this->can_submit = false;

		this->label_offset = label_offset;
		this->textbox_placeholder = textbox_placeholder;

		can_submit_func = [] { return false; };
	}

	/**
	 * Sets the size of a textbox and its internal components
	 * @param size sf::Vector2f size of box
	 */
	void set_size(const sf::Vector2f& size) {
		body.setSize(size);
		body.setOrigin({size.x / 2.0f, size.y / 2.0f});

		// Calculate the center of the input depending on its internal text bounds
		auto bounds = input.getLocalBounds();
		input.setOrigin({bounds.position.x + bounds.size.x / 2.0f,  bounds.position.y + bounds.size.y / 2.0f});

		// Calculate the center of the label depending on its internal text bounds
		bounds = label.getLocalBounds();
		label.setOrigin({bounds.position.x + bounds.size.x / 2.0f,  bounds.position.y + bounds.size.y / 2.0f});
	}

	/**
	 * Sets the text size of a textbox. Alters both label and input text size. Resizes and repositions components as needed
	 * @param size size (px) to set text to
	 */
	void set_text_size(const int size) {
		label.setCharacterSize(size);
		input.setCharacterSize(size);
		set_size(body.getSize());
		set_position(body.getPosition());
	}

	/**
	 * Sets the position to draw the component at
	 * @param position sf::Vector2f the position to draw the component at
	 */
	void set_position(const sf::Vector2f& position) {
		body.setPosition(position);
		// Draw the label at an offset, specified by user
		label.setPosition({ position.x + label_offset.x, position.y + label_offset.y });
		input.setPosition(position);
	}

	/**
	 * Sets the primary color of the text box
	 * @param color sf::Color primary color
	 */
	void set_primary(const sf::Color color) {
		primary = color;
	}

	/**
	 * Sets the secondary color of the text box
	 * @param color sf::Color secondary color
	 */
	void set_secondary(const sf::Color color) {
		secondary = color;
	}

	/**
	 * Draws the text box onto a RenderWindow screen.
	 * @param window the RenderWindow to draw the textbox on
	 */
	void draw(sf::RenderWindow& window) {
		// Dynamically renders the textbox based on being clicked/hovered
		if (is_clicked || is_hovered) {
			body.setFillColor(primary);
			input.setFillColor(secondary);
		} else {
			body.setFillColor(secondary);
			input.setFillColor(primary);
		}

		// If the input is empty, go back to placeholder text
		if (input_text.getSize() == 0) {
			input.setString(textbox_placeholder);
		} else {
			input.setString(input_text);
		}
		label.setFillColor(primary);

		// resize textbox and change position based on any internal text changes
		set_size(body.getSize());
		set_position(body.getPosition());

		// draw internal components
		window.draw(body);
		window.draw(label);
		window.draw(input);
	}

	/**
	 * Get if a textbox is currently being highlighted (triggered by a click)
	 * @return
	 */
	bool get_is_clicked() const {
		return is_clicked;
	}


	/**
	 * Sets the clicked state of a Textbox
	 * @param is_clicked boolean
	 */
	void set_is_clicked(const bool is_clicked) {
		this->is_clicked = is_clicked;
	}

	/**
	 * Checks if the window's mouse is within bounds of the textbox. Useful for handling click actions
	 * @param window the current RenderWindow
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
	void handle_event(const sf::Event& event, const sf::RenderWindow& window) {
		// If the textbox was clicked, toggle its highlight status
		if (event.getIf<sf::Event::MouseButtonPressed>()) {
			if (is_in_bounds(window)) {
				set_is_clicked(true);
			} else {
				set_is_clicked(false);
			}
		} else if (event.getIf<sf::Event::MouseMoved>()) {
			// Check if the textbox is being hovered actively
			if (is_in_bounds(window)) {
				is_hovered = true;
			} else {
				is_hovered = false;
			}
		} else if (is_clicked) {
			// Check if a valid keystroke occurred and update as necesasry
			handle_typing(event);
		}
	}

	/**
	 * Handles keystroke detection and conversion
	 * @param event sf::Event that was triggered
	 */
	void handle_typing(const sf::Event& event) {
		if (auto key = event.getIf<sf::Event::KeyPressed>()) {
			// key pressed is a-z or 0-9
			if ((key->code >= sf::Keyboard::Key::A && key->code <= sf::Keyboard::Key::Z) || (key->code >= sf::Keyboard::Key::Num0 && key->code <= sf::Keyboard::Key::Num9)) {
				// adds the new key to the end of the string
				input_text += sf::Keyboard::getDescription(key->scancode);
				// updates can submit state
				can_submit = can_submit_func();
			} else if (key->code == sf::Keyboard::Key::Backspace) {
				// backspace functionality, only if there is something to delete
				if (input_text.getSize() != 0) {
					input_text.erase(input_text.getSize() - 1);
					can_submit = can_submit_func();
				}
			}
		}
	}

	/**
	 * Passes a function to determine if a textbox can be submitted.
	 * @param can_submit_func std::function<bool>()
	 */
	void set_can_submit_func(const std::function<bool()>& can_submit_func) {
		this->can_submit_func = can_submit_func;
	}

	/**
	 * Returns whether a textbox can be submitted. Preferred over recalling can_submit function.
	 * @return
	 */
	bool get_can_submit() const {
		return can_submit;
	}

	/**
	 * Gets the input stored in the textbox
	 * @return std::string input text
	 */
	std::string get_input_text() const {
		return input_text;
	}
};

#endif //TEXTBOX_H
