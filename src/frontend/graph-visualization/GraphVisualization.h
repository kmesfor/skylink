//
// Created by Kian Mesforush on 7/31/25.
//

#ifndef GRAPHVISUALIZATION_H
#define GRAPHVISUALIZATION_H
#include <string>

#include "VisualizationConfig.h"
#include "backend/datamodels/AlgorithmResult.h"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"

using namespace VisualizationConfig;

/**
 * Visualization of the graph created on a RenderTexture to allow it to be a component like a "div" in HTML.
 * Has a #draw() function to draw onto a render window.
 */
class GraphVisualization final : sf::RenderTexture {
	std::vector<std::pair<sf::Vector2f, AlgorithmResult>> vertices; // Position and algorithm data for each vertex to be used by the click handler
	sf::View scrolled_view; // Scrolled view to be used by click handler
	int clicked_vertex_index = -1;	// Index of vertex actively being highlighted, -1 if none

	/**
	 * Function to draw an airport vertex on the graph visualization
	 * @param code airport code
	 * @param x_pos x position of vertex
	 * @param y_pos y position of vertex
	 * @param result AlgorithmResult of the entire set of vertices
	 * @param clicked T/F state if the vertex is clicked
	 */
	void draw_vertex(const std::string& code, float x_pos, float y_pos, const AlgorithmResult& result, bool clicked);

	/**
	 * Draw a connecting line from x_1_pos, y_1_pos to x_2_pos, y_2_pos
	 * @param start the starting vertex
	 * @param end the ending vertex
	 */
	void draw_line(const sf::Vector2f& start, const sf::Vector2f& end);

	/**
	 * Draw dynamic graph components such as vertices and lines that appear UNDER the static components
	 */
	void drawGraphComponents();

	/**
	 * Draw static components that appear over the dynamic components (instructions, results)
	 */
	void drawStaticComponents(sf::RenderWindow& window, sf::Vector2f position);

public:
	RenderTexture graph; // Store the texture that the graph will be drawn on
	sf::View view; // Use an SFML view to create scrollable content
	sf::Font font; // Store font used throughout graph
	const std::vector<AlgorithmResult>& results; // Results of algorithm execution


	/**
	 * Creates the graph ready to be rendered. Draws a graphical representation of airport results
	 */
	explicit GraphVisualization(const std::vector<AlgorithmResult>& results) : results(results) {
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

		drawGraphComponents();

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
		drawStaticComponents(window, position);
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

	/**
	 * Handle vertex clicking logic
	 * @param window RenderWindow the click occurred on
	 */
	void check_if_vertex_clicked(const sf::RenderWindow& window) {
		// Get the click position relative to the window (uses same coordinate system as vertex locations)
		const sf::Vector2f click = window.mapPixelToCoords(sf::Mouse::getPosition(window), scrolled_view);

		for (int i = 0; i < vertices.size(); i++) {
			// Use distance formula to see if click is within radius of vertex
			const float d_x = click.x - vertices[i].first.x;
			const float d_y = click.y - vertices[i].first.y;
			const float distance = std::sqrt((d_x * d_x) + (d_y * d_y));
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
