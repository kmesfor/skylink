//
// Created by Kian Mesforush on 8/1/25.
//

#include "GraphVisualization.h"

#include "SFML/Graphics/CircleShape.hpp"

void GraphVisualization:: draw_vertex(const std::string& code, float x_pos, float y_pos, const AlgorithmResult& result, const bool clicked) {
	// Create the circle object and fill in necessary values
	sf::CircleShape point(VERTEX_RADIUS);
	point.setFillColor(clicked ? VERTEX_SELECTED_COLOR : VERTEX_COLOR);
	point.setOrigin({VERTEX_RADIUS, VERTEX_RADIUS});
	point.setPosition({x_pos, y_pos});

	// Draw on the graph
	graph.draw(point);

	// Create a text label for the airport code
	sf::Text label(font);
	label.setString(code);
	label.setCharacterSize(VERTEX_LABEL_FONT_SIZE);
	label.setFillColor(VERTEX_LABEL_COLOR);

	// Set the origin position of the label to the center of the text box
	label.setOrigin(label.getLocalBounds().getCenter());

	// Set position below the circle depending on offsets
	label.setPosition({x_pos, y_pos + VERTEX_RADIUS + VERTEX_TEXT_Y_OFFSET});

	// Draw to graph
	graph.draw(label);
	vertices.push_back({{x_pos, y_pos}, result});
}

void GraphVisualization::draw_line(const sf::Vector2f& start, const sf::Vector2f& end) {
	sf::Vertex line[2];

	// Start and end points for line
	line[0].position = start;
	line[1].position = end;
	line[0].color = LINE_COLOR;
	line[1].color = LINE_COLOR;

	graph.draw(line, 2, sf::PrimitiveType::Lines);
}

void GraphVisualization::drawGraphComponents() {
	// Iterate each result, draw lines first so they appear below vertices
	for (int i = 0 ; i < results.size(); i++) {
		// Starting at START_Y, move down by Y_OFFSET for every new result set
		float y = START_Y + (i * Y_OFFSET);
		// Iterate each route in the result
		for (int j = 0; j < results[i].results.size(); j++) {
			// Starting at START_X, move rightwards by X_OFFSET for each new route in the result
			float x = START_X + j * X_OFFSET;
			// If not the last vertex, draw the line
			if (j < results[i].results.size()) {
				draw_line({x, y}, {x + X_OFFSET, y});
			}

		}
	}

	// Iterate each result, draw vertices second so they appear above vertices
	int vertex_index = 0;
	for (int i = 0; i < results.size(); i++) {
		float y = START_Y + (i * Y_OFFSET);
		for (int j = 0; j < results[i].results.size(); j++) {
			float x = START_X + (j * X_OFFSET);
			// Draw the origin vertex
			draw_vertex(results[i].results[j].first->origin_code, x, y, results[i], clicked_vertex_index == vertex_index);
			vertex_index++;

			// On the last flight, also draw the destination vertex to ensure all vertices are drawn
			if (j == results[i].results.size() - 1) {
				// Increment x position forward, then draw
				draw_vertex(results[i].results[j].first->destination_code, START_X + (j+1) * X_OFFSET, y, results[i], clicked_vertex_index == vertex_index);
				vertex_index++;
			}
		}
	}
}

void GraphVisualization::drawStaticComponents(sf::RenderWindow& window, sf::Vector2f position) {
	// Create a visual box to show where the graph's contents scrollable region is
	sf::RectangleShape box;
	box.setSize(sf::Vector2f(WIDTH, HEIGHT));
	box.setPosition(position);
	box.setOutlineColor(BOX_COLOR);
	box.setOutlineThickness(BOX_THICKNESS);
	box.setFillColor(sf::Color::Transparent); // Make graph content below viewable

	// Draw the box
	window.draw(box);

	// Draw fixed statistics box
	sf::RectangleShape statistics;
	statistics.setSize(sf::Vector2f(STATS_WIDTH, STATS_HEIGHT));
	statistics.setPosition({position.x + STATS_X, position.y + STATS_Y});
	statistics.setOutlineColor(STATS_OUTLINE_COLOR);
	statistics.setOutlineThickness(STATS_OUTLINE_THICKNESS);
	statistics.setFillColor(STATS_FILL_COLOR);
	window.draw(statistics);

	// Add instructions for using scroll feature, make it fixed to the top (like box)
	sf::Text instructions(font);
	instructions.setCharacterSize(INSTRUCTION_FONT_SIZE);
	instructions.setFillColor(INSTRUCTION_COLOR);
	// Make INSTRUCTION_TEXT_X and INSTRUCTION_TEXT_Y be relative to the graph's position on the window
	instructions.setPosition({position.x + INSTRUCTION_TEXT_X, position.y + INSTRUCTION_TEXT_Y});
	instructions.setString("Use arrow keys to scroll graph!");
	window.draw(instructions);
}

void GraphVisualization::drawGraphSprite(sf::RenderWindow& window, sf::Vector2f position) {
	// First, render the components on the graph
	graph.display();

	// Next, turn the graph into a sprite than can be drawn onto windows
	sf::Sprite sprite(graph.getTexture());

	// Make the sprite start drawing at (0,0) within its box, not the position on the window (doubly accounting for distance)
	sprite.setPosition({0, 0});

	// Store the previous view, basically scroll_view is used to draw the GraphVisualization sprite on an "altered" coordinate plane,
	// then setting the window's view back to previous_view restores the coordinate plane back to normal
	sf::View prev_view = window.getView();
	sf::View scroll_view = view;

	// Set the scrolled view values as a value between 0 and 1 relative to the main view
	scroll_view.setViewport(sf::FloatRect(
		{position.x / window.getSize().x,
		position.y / window.getSize().y},
		{static_cast<float>(WIDTH) / window.getSize().x,
		static_cast<float>(HEIGHT) / window.getSize().y}
	));

	scrolled_view = scroll_view;

	// Activate the altered coordinate plane and render sprite onto window
	// Do this step above other components to make the graph appear below the graph's 'UI' (stats / instructions)
	window.setView(scroll_view);
	window.draw(sprite);

	// Restore regular coordinate plane
	window.setView(prev_view);
}
