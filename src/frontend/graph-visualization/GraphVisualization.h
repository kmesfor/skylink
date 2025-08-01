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
class GraphVisualization {
	std::vector<std::tuple<sf::Vector2f, FlightRouteStatistics, const AlgorithmResult*>> vertices; // Position and algorithm data for each vertex to be used by the click handler
	sf::View scrolled_view; // Scrolled view to be used by click handler
	int clicked_vertex_index = -1;	// Index of vertex actively being highlighted, -1 if none

	/**
	 * Function to draw an airport vertex on the graph visualization
	 * @param code airport code
	 * @param pos position of vertex
	 * @param result AlgorithmResult of the entire set of vertices
	 * @param clicked T/F state if the vertex is clicked
	 */
	void draw_vertex(const std::string& code, const sf::Vector2f pos, const FlightRouteStatistics& result, bool clicked);

	/**
	 * Draw a connecting line from x_1_pos, y_1_pos to x_2_pos, y_2_pos
	 * @param start the starting vertex
	 * @param end the ending vertex
	 */
	void draw_line(const sf::Vector2f& start, const sf::Vector2f& end);

	/**
	 * Draw dynamic graph components such as vertices and lines that appear UNDER the static components
	 */
	void draw_graph_components();

	/**
	 * Draw static components that appear over the dynamic components (instructions, results)
	 */
	void draw_static_components(sf::RenderWindow& window, sf::Vector2f position) const;

	/**
	 * Handles setting view, creating graph sprite, and rendering scrollable content
	 */
	void draw_graph_sprite(sf::RenderWindow& window, sf::Vector2f position);

	/**
	 * Loads vertex positions for the graph to be easily re-accessed.
	 */
	void load_vertex_positions();

	/**
	 * Draw AlgorithmResult information to screen for the currently selected vertex
	 * @param result The AlgorithmResult to display
	 * @param window the window to draw on
	 * @param position window position
	 */
	void draw_result_info(sf::RenderWindow& window, sf::Vector2f position) const;

public:
	sf::RenderTexture graph; // Store the texture that the graph will be drawn on
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
		if (!font.openFromFile("JetBrainsMono-Regular.ttf")) { // Stored in dist/
			std::cerr << "Failed to load font JetBrainsMono-Regular.ttf" << std::endl;
			return;
		}

		load_vertex_positions();
	}

	/**
	 * Draw the graph onto Window in a certain position
	 * @param window Window to draw on
	 * @param position coordinates to draw at
	 */
	void draw(sf::RenderWindow& window, sf::Vector2f position) {
		// Clean up
		graph.setView(view);
		graph.clear(sf::Color::Transparent);

		// Draw dynamic components
		draw_graph_components();

		// Draw scrollable content
		draw_graph_sprite(window, position);

		// Draw fixed components
		draw_static_components(window, position);
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
			const float d_x = click.x - std::get<0>(vertices[i]).x;
			const float d_y = click.y - std::get<0>(vertices[i]).y;
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
