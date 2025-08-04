//
// Created by Kian Mesforush on 8/4/25.
//

#ifndef PRIMARYWINDOW_H
#define PRIMARYWINDOW_H
#include "backend/AlgorithmComparator.h"
#include "frontend/window/Window.h"
#include "frontend/window/components/Button.h"
#include "frontend/window/components/Textbox.h"

class PrimaryWindow : public Window {
	AlgorithmComparator& comparator;

	Textbox* start;
	Textbox* end;
	Button* submit;

	sf::Text* welcome_text;
	sf::Text* subtext_one;
	sf::Text* subtext_two;
	sf::Text* subtext_three;
	sf::Text* subtext_four;

public:
	PrimaryWindow(AlgorithmComparator& comparator): comparator(comparator) {
		this->name = WindowNames::MAIN;

		start = new Textbox("Departure Airport Code", "e.g. MIA", {0, -70});
		end = new Textbox("Destination Airport Code", "e.g. LAX", {0, -70});

		start->set_size({300, 100});
		start->set_position({300, 500});
		start->set_text_size(18);
		start->set_can_submit_func([this] {
			return this->comparator.graph->airport_lookup.find(start->get_input_text()) != this->comparator.graph->airport_lookup.end();
		});

		end->set_size({300, 100});
		end->set_position({900, 500});
		end->set_text_size(18);
		end->set_can_submit_func([this] {
			return this->comparator.graph->airport_lookup.find(end->get_input_text()) != this->comparator.graph->airport_lookup.end();
		});

		welcome_text = new sf::Text(Frontend::font, "Welcome to Skylink!");
		welcome_text->setCharacterSize(26);
		welcome_text->setFillColor(VisualizationConfig::COLOR_PRIMARY);
		welcome_text->setPosition({600, 100});
		auto bounds = welcome_text->getLocalBounds();
		welcome_text->setOrigin({bounds.position.x + bounds.size.x / 2.0f,  bounds.position.y + bounds.size.y / 2.0f});

		subtext_one = new sf::Text(Frontend::font, "Easily search for the best route between airports considering delays, cancellation, distances, and more!");
		subtext_one->setCharacterSize(18);
		subtext_one->setFillColor(sf::Color(170, 172, 181));
		subtext_one->setPosition({600, 200});
		bounds = subtext_one->getLocalBounds();
		subtext_one->setOrigin({bounds.position.x + bounds.size.x / 2.0f,  bounds.position.y + bounds.size.y / 2.0f});

		subtext_two = new sf::Text(*subtext_one);
		subtext_two->setString("To get started, enter two airport codes");
		subtext_two->setPosition({600, 230});
		bounds = subtext_two->getLocalBounds();
		subtext_two->setOrigin({bounds.position.x + bounds.size.x / 2.0f,  bounds.position.y + bounds.size.y / 2.0f});

		subtext_three = new sf::Text(*subtext_one);
		subtext_three->setString("Click the text boxes below to type in airport codes, then click the submit button");
		subtext_three->setPosition({600, 260});
		bounds = subtext_three->getLocalBounds();
		subtext_three->setOrigin({bounds.position.x + bounds.size.x / 2.0f,  bounds.position.y + bounds.size.y / 2.0f});

		submit = new Button("Submit", Button::PRIMARY_INACTIVE);
		submit->set_size({300, 100});
		submit->set_position({600, 700});
		submit->set_text_size(18);

		subtext_four = new sf::Text(*subtext_one);
		subtext_four->setString("Make sure airports are valid before you submit, otherwise, you cannot press the submit button");
		subtext_four->setPosition({600, 630});
		bounds = subtext_four->getLocalBounds();
		subtext_four->setOrigin({bounds.position.x + bounds.size.x / 2.0f,  bounds.position.y + bounds.size.y / 2.0f});
	}

	~PrimaryWindow() override {
		delete start;
		delete end;
		delete welcome_text;
		delete subtext_one;
		delete subtext_two;
		delete subtext_three;
		delete submit;
		delete subtext_four;
	}

	void draw(sf::RenderWindow& window) override {
		window.clear(VisualizationConfig::COLOR_BG);

		start->draw(window);
		end->draw(window);
		window.draw(*welcome_text);
		window.draw(*subtext_one);
		window.draw(*subtext_two);
		window.draw(*subtext_three);
		window.draw(*subtext_four);
		submit->draw(window);
	}

	void handle_event(sf::RenderWindow& window, const std::optional<sf::Event>& event) override {
		if (event) {
			start->handle_event(*event, window);
			end->handle_event(*event, window);
			submit->handle_event(*event, window);

			if (start->get_can_submit() && end->get_can_submit()) {
				submit->set_state(Button::PRIMARY_ACTIVE);
			} else {
				submit->set_state(Button::PRIMARY_INACTIVE);
			}
		}
	}


};

#endif //PRIMARYWINDOW_H
