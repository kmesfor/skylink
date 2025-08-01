//
// Created by Kian Mesforush on 7/31/25.
//

#ifndef GRAPHVISUALIZATION_H
#define GRAPHVISUALIZATION_H
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Sprite.hpp"

#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"

constexpr unsigned int WIDTH = 600;
constexpr unsigned int HEIGHT = 600;

constexpr float VERTEX_RADIUS = 15.0; // px radius of each vertex
constexpr float VERTEX_TEXT_Y_OFFSET = 10.0; //px below vertex that label should sit
constexpr sf::Color VERTEX_COLOR = sf::Color::Red;
constexpr sf::Color VERTEX_SELECTED_COLOR = sf::Color::Green;

constexpr sf::Color LINE_COLOR = sf::Color::Black; // line color between vertices

constexpr int LABEL_FONT_SIZE = 12;
constexpr sf::Color LABEL_COLOR = sf::Color::Green;

constexpr float BOX_THICKNESS = 2.0; // line thickness in px
constexpr sf::Color BOX_COLOR = sf::Color::Black;

constexpr int INSTRUCTION_FONT_SIZE = 12;
constexpr sf::Color INSTRUCTION_COLOR = sf::Color::Black;
constexpr float INSTRUCTION_TEXT_X = 200;
constexpr float INSTRUCTION_TEXT_Y = 0;

constexpr float STATS_WIDTH = WIDTH;
constexpr float STATS_HEIGHT = 100;
constexpr float STATS_X = 0;
constexpr float STATS_Y = 0;
constexpr sf::Color STATS_FILL_COLOR = sf::Color(200, 200, 200);
constexpr sf::Color STATS_OUTLINE_COLOR = sf::Color::Green;
constexpr float STATS_OUTLINE_THICKNESS = 2.0;

constexpr float START_X = 50.0; // x position to start rendering components
constexpr float START_Y = 50.0 + STATS_HEIGHT; // y position to start rendering components
constexpr float X_OFFSET = 120.0; // horizontal offset between vertices
constexpr float Y_OFFSET = 60.0; // vertical offset between vertices

/**
 * Visualization of the graph created on a RenderTexture to allow it to be a component like a "div" in HTML.
 * Has a #draw() function to draw onto a render window.
 */
class GraphVisualization final : sf::RenderTexture {
	/**
	 * Function to draw an airport vertex on the graph visualization
	 * @param code airport code
	 * @param x_pos x position of vertex
	 * @param y_pos y position of vertex
	 * @param font  font to draw in
	 */
	void draw_vertex(const std::string& code, float x_pos, float y_pos, const sf::Font& font, AlgorithmResult result, bool clicked) {
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
		label.setCharacterSize(LABEL_FONT_SIZE);
		label.setFillColor(LABEL_COLOR);

		// Set the origin position of the label to the center of the text box
		label.setOrigin(label.getLocalBounds().getCenter());

		// Set position below the circle depending on offsets
		label.setPosition({x_pos, y_pos + VERTEX_RADIUS + VERTEX_TEXT_Y_OFFSET});

		// Draw to graph
		graph.draw(label);
		vertices.push_back({{x_pos, y_pos}, result});
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

	// Store positions of vertexes drawn for click handler
	std::vector<std::pair<sf::Vector2f, AlgorithmResult>> vertices;

	// Store scrolled view to use in click handler
	sf::View scrolled_view;

	// Index of vertex actively being highlighted, -1 if none
	int clicked_vertex_index = -1;
public:
	// Store the texture that the graph will be drawn on
	RenderTexture graph;

	// Use an SFML view to create scrollable content
	sf::View view;

	// Store font to use in other functions
	sf::Font font;

	const std::vector<AlgorithmResult>& results;

	/**
	 * Creates the graph ready to be rendered. Draws a graphical representation of airport results
	 */
	GraphVisualization(const std::vector<AlgorithmResult>& results) : results(results) {
		// Create a new RenderTexture to draw components on
		graph = sf::RenderTexture({WIDTH, HEIGHT});

		// Initialize the view starting at 0, 0
		view = sf::View(sf::FloatRect({0, 0}, {WIDTH, HEIGHT}));

		// Load the font used for text labels
		if (!font.openFromFile("arial.ttf")) { // Stored in dist/
			std::cerr << "Failed to load font arial.ttf" << std::endl;
			return;
		}
	}

	/**
	 * Draw the graph onto Window in a certain position
	 * @param window Window to draw on
	 * @param position coordinates to draw at
	 */
	void draw(sf::RenderWindow& window, sf::Vector2f position) {
		graph.clear(sf::Color::White);
		vertices.clear();


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
				draw_vertex(results[i].results[j].first->origin_code, x, y, font, results[i], clicked_vertex_index == vertex_index);
				vertex_index++;

				// On the last flight, also draw the destination vertex to ensure all vertices are drawn
				if (j == results[i].results.size() - 1) {
					// Increment x position forward, then draw
					draw_vertex(results[i].results[j].first->destination_code, START_X + (j+1) * X_OFFSET, y, font, results[i], clicked_vertex_index == vertex_index);
					vertex_index++;
				}
			}
		}


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

	/**
	 * Alter the view by a Vector2f (x and y coordinates)
	 * @param change Vector of coordinates to change by (i.e. -1, -1)
	 */
	void scroll(const sf::Vector2f& change) {
		sf::Vector2f center = view.getCenter();
		center += change;
		view.setCenter(center);
	}

	void check_if_vertex_clicked(sf::RenderWindow& window) {
		// Get the click position relative to the window (uses same coordinate system as vertex locations)
		sf::Vector2f click = window.mapPixelToCoords(sf::Mouse::getPosition(window), scrolled_view);

		for (int i = 0; i < vertices.size(); i++) {
			// Use distance formula to see if click is within radius of vertex
			float d_x = click.x - vertices[i].first.x;
			float d_y = click.y - vertices[i].first.y;
			float distance = std::sqrt((d_x * d_x) + (d_y * d_y));
			if (distance <= VERTEX_RADIUS) {
				if (clicked_vertex_index == i) {
					clicked_vertex_index = -1;
				} else {
					clicked_vertex_index = i;
				}
			}
		}
;	}
};

#endif //GRAPHVISUALIZATION_H
