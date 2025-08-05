//
// Created by Kian Mesforush on 8/4/25.
//

#ifndef PRIMARYWINDOW_H
#define PRIMARYWINDOW_H
#include "backend/AlgorithmComparator.h"
#include "frontend/window/Window.h"
#include "frontend/window/components/Button.h"
#include "frontend/window/components/Textbox.h"

/**
 * The main window to show when the user enters the program
 */
struct PrimaryWindow : public Window {
	// Data pipeline
	AlgorithmComparator& comparator;

	WeightType edge_weight_type;

	// Internal components
	Textbox* start;
	Textbox* end;
	Textbox* num_results;
	Button* submit;
	Button* edge_weight_btn;

	sf::Text* welcome_text;
	sf::Text* subtext_one;
	sf::Text* subtext_two;
	sf::Text* subtext_three;
	sf::Text* subtext_four;
	sf::Text* weight_type_label;

	/**
	 * Initialize PrimaryWindow
	 * @param comparator AlgorithmComparator reference
	 */
	PrimaryWindow(AlgorithmComparator& comparator): comparator(comparator) {
		// Setup default attributes
		this->name = WindowNames::MAIN;

		this->edge_weight_type = WeightType::DELAY;

		// Initialize textboxes
		start = new Textbox("Departure Airport Code", "e.g. MIA", {0, -70});
		end = new Textbox("Destination Airport Code", "e.g. LAX", {0, -70});
		num_results = new Textbox("Max Number of Results to Generate", "e.g. 10", {0, -70});

		// Configure base component view (dont need to do this on draw)
		start->set_size({300, 100});
		start->set_position({300, 550});
		start->set_text_size(18);
		// Can submit if in the airport_lookup (valid airport code)
		start->set_can_submit_func([this] {
			return this->comparator.graph->airport_lookup.find(start->get_input_text()) != this->comparator.graph->airport_lookup.end();
		});

		// Configure base component view (dont need to do this on draw)
		end->set_size({300, 100});
		end->set_position({900, 550});
		end->set_text_size(18);
		// Can submit if in the airport_lookup (valid airport code)
		end->set_can_submit_func([this] {
			return this->comparator.graph->airport_lookup.find(end->get_input_text()) != this->comparator.graph->airport_lookup.end();
		});

		// Configure base component view (dont need to do this on draw)
		num_results->set_size({300, 100});
		num_results->set_position({300, 400});
		num_results->set_text_size(18);
		// Can submit if its a number between 0 and 100
		num_results->set_can_submit_func([this] {
			try {
				int num = std::stoi(this->num_results->get_input_text());
				return (num > 0 && num < 100);
			} catch (std::exception& e) {
				return false;
			}
		});

		// Configure main welcome text (dont need to do this on every draw)
		welcome_text = new sf::Text(Frontend::font, "Welcome to Skylink!");
		welcome_text->setCharacterSize(26);
		welcome_text->setFillColor(VisualizationConfig::COLOR_PRIMARY);
		welcome_text->setPosition({600, 100});
		auto bounds = welcome_text->getLocalBounds();
		welcome_text->setOrigin({bounds.position.x + bounds.size.x / 2.0f,  bounds.position.y + bounds.size.y / 2.0f});

		// Configure subtext text (dont need to do this on every draw)
		subtext_one = new sf::Text(Frontend::font, "Easily search for the best route between airports considering delays, cancellation, distances, and more!");
		subtext_one->setCharacterSize(18);
		subtext_one->setFillColor(sf::Color(170, 172, 181));
		subtext_one->setPosition({600, 200});
		bounds = subtext_one->getLocalBounds();
		subtext_one->setOrigin({bounds.position.x + bounds.size.x / 2.0f,  bounds.position.y + bounds.size.y / 2.0f});

		// Configure subtext text (dont need to do this on every draw)
		subtext_two = new sf::Text(*subtext_one);
		subtext_two->setString("To get started, enter two airport codes");
		subtext_two->setPosition({600, 230});
		bounds = subtext_two->getLocalBounds();
		subtext_two->setOrigin({bounds.position.x + bounds.size.x / 2.0f,  bounds.position.y + bounds.size.y / 2.0f});

		// Configure subtext text (dont need to do this on every draw)
		subtext_three = new sf::Text(*subtext_one);
		subtext_three->setString("Fill in the appropriate information below. Click the text boxes below to type");
		subtext_three->setPosition({600, 260});
		bounds = subtext_three->getLocalBounds();
		subtext_three->setOrigin({bounds.position.x + bounds.size.x / 2.0f,  bounds.position.y + bounds.size.y / 2.0f});

		// Configure submit button
		submit = new Button("Submit", Button::PRIMARY_INACTIVE);
		submit->set_size({300, 100});
		submit->set_position({600, 700});
		submit->set_text_size(18);
		submit->set_click_action([this] {
			set_window_signal(WindowSignal::SHOW_ALGORITHM);
		});

		// Configure subtext
		subtext_four = new sf::Text(*subtext_one);
		subtext_four->setString("Make sure airports are valid before you submit, otherwise, you cannot press the submit button");
		subtext_four->setPosition({600, 630});
		bounds = subtext_four->getLocalBounds();
		subtext_four->setOrigin({bounds.position.x + bounds.size.x / 2.0f,  bounds.position.y + bounds.size.y / 2.0f});

		// Configure edge weight configuration button
		edge_weight_btn = new Button("Delay", Button::SECONDARY_ACTIVE);
		edge_weight_btn->set_size({300, 100});
		edge_weight_btn->set_position({900, 400});
		edge_weight_btn->set_text_size(18);
		// Alternate between distance and delay
		edge_weight_btn->set_click_action([this] {
			if (edge_weight_type == WeightType::DISTANCE) {
				edge_weight_type = WeightType::DELAY;
				edge_weight_btn->set_text("Delay");
			} else {
				edge_weight_type = WeightType::DISTANCE;
				edge_weight_btn->set_text("Distance");
			}
		});

		// Edge weight label (not a Textbox)
		weight_type_label = new sf::Text(Frontend::font, "Weight Type");
		weight_type_label->setCharacterSize(18);
		weight_type_label->setFillColor(VisualizationConfig::COLOR_PRIMARY);
		weight_type_label->setPosition({900, 330});
		bounds = weight_type_label->getLocalBounds();
		weight_type_label->setOrigin({bounds.position.x + bounds.size.x / 2.0f,  bounds.position.y + bounds.size.y / 2.0f});
	}

	~PrimaryWindow() override {
		// Delete heap allocated memory
		delete start;
		delete end;
		delete welcome_text;
		delete subtext_one;
		delete subtext_two;
		delete subtext_three;
		delete submit;
		delete subtext_four;
		delete num_results;
		delete edge_weight_btn;
		delete weight_type_label;
	}

	void draw(sf::RenderWindow& window) override {
		// Clear bg
		window.clear(VisualizationConfig::COLOR_BG);

		// draw components to window
		start->draw(window);
		end->draw(window);
		window.draw(*welcome_text);
		window.draw(*subtext_one);
		window.draw(*subtext_two);
		window.draw(*subtext_three);
		window.draw(*subtext_four);
		submit->draw(window);
		num_results->draw(window);
		edge_weight_btn->draw(window);
		window.draw(*weight_type_label);
	}

	/**
	 * Handles events for the primary window. Dispatches events to necessary components
	 * @param window RenderWindow instance
	 * @param event Event triggered
	 */
	void handle_event(sf::RenderWindow& window, const std::optional<sf::Event>& event) override {
		if (event) {
			// Pass event to all handlers
			start->handle_event(*event, window);
			end->handle_event(*event, window);
			num_results->handle_event(*event, window);
			submit->handle_event(*event, window);
			edge_weight_btn->handle_event(*event, window);

			// only allow submitting if input is valid
			if (start->get_can_submit() && end->get_can_submit() && num_results->get_can_submit()) {
				submit->set_state(Button::PRIMARY_ACTIVE);
			} else {
				submit->set_state(Button::PRIMARY_INACTIVE);
			}
		}
	}


};

#endif //PRIMARYWINDOW_H
