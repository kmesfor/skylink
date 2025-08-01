//
// Created by Kian Mesforush on 7/31/25.
//

#ifndef GRAPHVISUALIZATION_H
#define GRAPHVISUALIZATION_H
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Sprite.hpp"

constexpr unsigned int WIDTH = 600;
constexpr unsigned int HEIGHT = 600;
constexpr float START_X = 50.0; // x position to start rendering components
constexpr float START_Y = 50.0; // y position to start rendering components
constexpr float X_OFFSET = 120.0; // horizontal offset between vertices
constexpr float Y_OFFSET = 60.0; // vertical offset between vertices
constexpr float VERTEX_RADIUS = 15.0; // px radius of each vertex
constexpr float VERTEX_TEXT_Y_OFFSET = 10.0; //px below vertex that label should sit
constexpr sf::Color LINE_COLOR = sf::Color::Black; // line color between vertices
constexpr sf::Color VERTEX_COLOR = sf::Color::Red;
constexpr int LABEL_FONT_SIZE = 12;
constexpr float BOX_THICKNESS = 2.0; // line thickness in px
constexpr sf::Color BOX_COLOR = sf::Color::Black;

/**
 * Visualization of the graph created on a RenderTexture to allow it to be a component like a "div" in HTML.
 * Has a #draw() function to draw onto a render window.
 */
class GraphVisualization final : sf::RenderTexture {
	/**
	 * Function to draw a airport vertex on the graph visualization
	 * @param code airport code
	 * @param x_pos x position of vertex
	 * @param y_pos y position of vertex
	 * @param font  font to draw in
	 */
	void draw_vertex(const std::string& code, float x_pos, float y_pos, sf::Font& font) {
		// Create the circle object and fill in necessary values
		sf::CircleShape point(VERTEX_RADIUS);
		point.setFillColor(VERTEX_COLOR);
		point.setOrigin({VERTEX_RADIUS, VERTEX_RADIUS});
		point.setPosition({x_pos, y_pos});

		// Draw on the graph
		graph.draw(point);

		// Create a text label for the airport code
		sf::Text label(font);
		label.setString(code);
		label.setCharacterSize(LABEL_FONT_SIZE);
		label.setFillColor(sf::Color::Black);

		// Set the origin position of the label to the center of the text box
		label.setOrigin(label.getLocalBounds().getCenter());

		// Set position below the circle depending on offsets
		label.setPosition({x_pos, y_pos + VERTEX_RADIUS + VERTEX_TEXT_Y_OFFSET});

		// Draw to graph
		graph.draw(label);
	}

	/**
	 * Draw a connecting line from x_1_pos, y_1_pos to x_2_pos, y_2_pos
	 * @param start the starting vertex
	 * @param end the ending vertex
	 */
	void draw_line(const sf::Vector2f& start, const sf::Vector2f& end) {
		sf::Vertex line[2];

		// Start and end points for line
		line[0].position = start;
		line[1].position = end;
		line[0].color = LINE_COLOR;
		line[1].color = LINE_COLOR;

		graph.draw(line, 2, sf::PrimitiveType::Lines);
	}
public:
	// Store the texture that the graph will be drawn on
	RenderTexture graph;

	// Use an SFML view to create scrollable content
	sf::View view;

	/**
	 * Creates the graph ready to be rendered. Draws a graphical representation of airport results
	 */
	GraphVisualization(const std::vector<AlgorithmResult>& results) {
		// Create a new RenderTexture to draw components on
		graph = sf::RenderTexture({WIDTH, HEIGHT});

		// Initialize the view starting at 0, 0
		view = sf::View(sf::FloatRect({0, 0}, {WIDTH, HEIGHT}));

		// Load the font used for text labels
		sf::Font font;
		if (!font.openFromFile("arial.ttf")) { // Stored in dist/
			std::cerr << "Failed to load font arial.ttf" << std::endl;
			return;
		}

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
		for (int i = 0; i < results.size(); i++) {
			float y = START_Y + (i * Y_OFFSET);
			for (int j = 0; j < results[i].results.size(); j++) {
				float x = START_X + (j * X_OFFSET);
				// Draw the origin vertex
				draw_vertex(results[i].results[j].first->origin_code, x, y, font);

				// On the last flight, also draw the destination vertex to ensure all vertices are drawn
				if (j == results[i].results.size() - 1) {
					// Increment x position forward, then draw
					draw_vertex(results[i].results[j].first->destination_code, START_X + (j+1) * X_OFFSET, y, font);
				}
			}
		}
	}

	/**
	 * Draw the graph onto Window in a certain position
	 * @param window Window to draw on
	 * @param position coordinates to draw at
	 */
	void draw(sf::RenderWindow& window, sf::Vector2f position) {
		// First, render the components on the graph
		graph.display();

		// Next, turn the graph into a sprite than can be drawn onto windows
		sf::Sprite sprite(graph.getTexture());

		// Set the position
		sprite.setPosition(position);

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

		// Create a visual box to show where the graph's contents scrollable region is
		sf::RectangleShape box;
		box.setSize(sf::Vector2f(WIDTH, HEIGHT));
		box.setPosition(position);
		box.setOutlineColor(BOX_COLOR);
		box.setOutlineThickness(BOX_THICKNESS);

		// Draw the box
		window.draw(box);

		// Activate the altered coordinate plane
		window.setView(scroll_view);

		// Render sprite onto window on the altered coordinate plane
		window.draw(sprite);

		// Restore regular coordinate plane
		window.setView(prev_view);
	}

	/**
	 * Alter the view by a Vector2f (x and y coordinates)
	 * @param change Vector of coordinates to change by (i.e. -1, -1)
	 */
	void scroll(const sf::Vector2f& change) {
		sf::Vector2f center = view.getCenter();
		center += change;
		view.setCenter(center);
	}
};

#endif //GRAPHVISUALIZATION_H
