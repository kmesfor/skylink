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

class Textbox {

	sf::RectangleShape body;
	sf::Text label;
	sf::Text input;

	sf::String input_text;
	std::string textbox_placeholder;

	sf::Color primary = VisualizationConfig::COLOR_PRIMARY;
	sf::Color secondary = VisualizationConfig::COLOR_SECONDARY;

	std::function<bool()> can_submit_func;

	sf::Vector2f label_offset;

	bool is_clicked; // state to check if that is the active textbox (multiple on screen)
public:
	Textbox(const std::string& label, const std::string& textbox_placeholder, const sf::Vector2f label_offset): label(Frontend::font, label), input(Frontend::font, textbox_placeholder) {
		body = sf::RectangleShape();
		this->is_clicked = false;

		this->label_offset = label_offset;
		this->textbox_placeholder = textbox_placeholder;

		can_submit_func = [] { return false; };
	}

	void set_size(const sf::Vector2f& size) {
		body.setSize(size);
		body.setOrigin({size.x / 2.0f, size.y / 2.0f});

		auto bounds = input.getLocalBounds();
		input.setOrigin({bounds.position.x + bounds.size.x / 2.0f,  bounds.position.y + bounds.size.y / 2.0f});

		bounds = label.getLocalBounds();
		label.setOrigin({bounds.position.x + bounds.size.x / 2.0f,  bounds.position.y + bounds.size.y / 2.0f});
	}

	void set_text_size(const int size) {
		label.setCharacterSize(size);
		input.setCharacterSize(size);
		set_size(body.getSize());
		set_position(body.getPosition());
	}

	void set_position(const sf::Vector2f& position) {
		body.setPosition(position);
		label.setPosition({ position.x + label_offset.x, position.y + label_offset.y });
		input.setPosition(position);
	}

	void set_primary(const sf::Color color) {
		primary = color;
	}

	void set_secondary(const sf::Color color) {
		secondary = color;
	}

	void draw(sf::RenderWindow& window) {
		if (is_clicked) {
			body.setFillColor(primary);
			input.setFillColor(secondary);
		} else {
			body.setFillColor(secondary);
			input.setFillColor(primary);
		}

		if (input_text.getSize() == 0) {
			input.setString(textbox_placeholder);
		} else {
			input.setString(input_text);
		}
		label.setFillColor(primary);

		set_size(body.getSize());
		set_position(body.getPosition());

		window.draw(body);
		window.draw(label);
		window.draw(input);
	}

	bool get_is_clicked() const {
		return is_clicked;
	}

	void set_is_clicked(const bool is_clicked) {
		this->is_clicked = is_clicked;
	}

	bool is_in_bounds(const sf::RenderWindow& window) const {
		const sf::Vector2f mouse_position = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		return body.getGlobalBounds().contains(mouse_position);
	}

	void handle_event(const sf::Event& event, const sf::RenderWindow& window) {
		if (event.getIf<sf::Event::MouseButtonPressed>()) {
			if (is_in_bounds(window)) {
				set_is_clicked(true);
			} else {
				set_is_clicked(false);
			}
		} else if (is_clicked) {
			handle_typing(event);
		}
	}

	void handle_typing(const sf::Event& event) {
		if (auto key = event.getIf<sf::Event::KeyPressed>()) {
			// key pressed is a-z
			if (key->code >= sf::Keyboard::Key::A && key->code <= sf::Keyboard::Key::Z) {
				input_text += sf::Keyboard::getDescription(key->scancode);
			} else if (key->code == sf::Keyboard::Key::Backspace) {
				if (input_text.getSize() != 0) {
					input_text.erase(input_text.getSize() - 1);
				}
			}
		}
	}

	void set_can_submit(const std::function<bool()>& can_submit_func) {
		this->can_submit_func = can_submit_func;
	}

	bool can_submit() const {
		return can_submit_func();
	}
};

#endif //TEXTBOX_H
