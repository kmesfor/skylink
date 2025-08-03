//
// Created by Kian Mesforush on 8/1/25.
//

#include "GraphVisualization.h"

#include "frontend/frontend.h"
#include "SFML/Graphics/CircleShape.hpp"

void GraphVisualization:: draw_vertex(const std::string& code, const sf::Vector2f pos, const FlightRouteStatistics& result, const bool clicked) {
	// Create the circle object and fill in necessary values
	sf::CircleShape point(VERTEX_RADIUS);
	point.setFillColor(clicked ? VERTEX_SELECTED_COLOR : VERTEX_COLOR);
	point.setOrigin({VERTEX_RADIUS, VERTEX_RADIUS});
	point.setPosition(pos);

	// Draw on the graph
	graph.draw(point);

	// Create a text label for the airport code
	sf::Text label(Frontend::font);
	label.setString(code);
	label.setCharacterSize(VERTEX_LABEL_FONT_SIZE);
	label.setFillColor(VERTEX_LABEL_COLOR);

	// Set the origin position of the label to the center of the text box
	label.setOrigin(label.getLocalBounds().getCenter());

	// Set position below the circle depending on offsets
	label.setPosition({pos.x, pos.y + VERTEX_RADIUS + VERTEX_TEXT_Y_OFFSET});

	// Draw to graph
	graph.draw(label);
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

void GraphVisualization::draw_graph_components() {
	// Draw lines in order, skipping line drawing on the first vertex of a row
	// and drawing lines between the previous vertex and the current
	int vertex_index = 0;
	for (int i = 0 ; i < results.size(); i++) {
		for (int j = 0; j < results[i].results.size(); j++) {
			if (j > 0) { // dont draw line on first vertex
				draw_line(std::get<0>(vertices[vertex_index - 1]), std::get<0>(vertices[vertex_index]));
			}
			vertex_index++;

			if (j == results[i].results.size() - 1) {
				// connect last vertices
				draw_line(std::get<0>(vertices[vertex_index - 1]), std::get<0>(vertices[vertex_index]));
				vertex_index++;
			}
		}
	}

	// Draw vertices afterward so they appear overtop of lines
	vertex_index = 0;
	for (int i = 0 ; i < results.size(); i++) {
		for (int j = 0; j < results[i].results.size(); j++) {
			draw_vertex(results[i].results[j].first->origin_code, std::get<0>(vertices[vertex_index]), std::get<1>(vertices[vertex_index]), clicked_vertex_index == vertex_index);
			vertex_index++;

			// Last vertex
			if (j == results[i].results.size() - 1) {
				draw_vertex(results[i].results[j].first->destination_code, std::get<0>(vertices[vertex_index]), std::get<1>(vertices[vertex_index]), clicked_vertex_index == vertex_index);
				vertex_index++;
			}
		}
	}
}

void GraphVisualization::draw_static_components(sf::RenderWindow& window, sf::Vector2f position) const {
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
	sf::Text instructions(Frontend::font);
	instructions.setCharacterSize(INSTRUCTION_FONT_SIZE);
	instructions.setFillColor(INSTRUCTION_COLOR);
	// Make INSTRUCTION_TEXT_X and INSTRUCTION_TEXT_Y be relative to the graph's position on the window
	instructions.setPosition({position.x + INSTRUCTION_TEXT_X, position.y + INSTRUCTION_TEXT_Y});
	instructions.setString("Use arrow keys to scroll graph! Click airports for more info");
	window.draw(instructions);

	if (clicked_vertex_index != -1) {
		draw_result_info(window, position);
	}
}

void GraphVisualization::draw_graph_sprite(sf::RenderWindow& window, sf::Vector2f position) {
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
	window.setView(window.getDefaultView());
	sprite.setPosition(position);
	window.draw(sprite);

	// Restore regular coordinate plane
	window.setView(prev_view);
}

void GraphVisualization::load_vertex_positions() {
	for (int i = 0; i < results.size(); i++) {
		float y = START_Y + (i * Y_OFFSET);
		for (int j = 0; j < results[i].results.size(); j++) {
			float x = START_X + (j * X_OFFSET);
				vertices.emplace_back(sf::Vector2f{x, y}, results[i].results[j].second, &results[i]);
			if (j == results[i].results.size() - 1) {
				vertices.emplace_back(sf::Vector2f{x + X_OFFSET, y}, results[i].results[j].second, &results[i]);
			}
		}
	}
}

void GraphVisualization::draw_result_info(sf::RenderWindow& window, sf::Vector2f position) const {
	auto result = std::get<2>(vertices[clicked_vertex_index]);

	sf::Text overall_info(Frontend::font);
	auto overall_stats = result->get_overall_statistics();
	overall_info.setFont(Frontend::font);

	std::ostringstream str;
	str << std::fixed << std::setprecision(5);
	str << "Summary: " << result->start->name << " (" << result->start->code << ")" << " to " << result->end->name << " (" << result->end->code << ")\n\n";
	str << "Algorithm: " << result->algorithm_name << " (" << result->elapsed_time.count() << " seconds)\n";
	str << std::fixed << std::setprecision(2);
	str << "Weight Type: " << (result->edge_weight_type == WeightType::DELAY ? "delay" : "distance") << "\n";
	str << "Connections: " << result->results.size() << "\n\n";
	str << "Cancellation: " << overall_stats.cancellation_rate << "%\n";
	str << "Average Scheduled Time: " << overall_stats.avg_scheduled_time << " mins" << "\n";
	str << "Average Delay: " << overall_stats.avg_delay << " mins" << "\n";
	str << "Average Total Time: " << overall_stats.avg_time << " mins" << "\n";

	overall_info.setString(str.str());


	overall_info.setCharacterSize(STATS_SUMMARY_FONT_SIZE);
	overall_info.setFillColor(STATS_SUMMARY_COLOR);
	overall_info.setPosition({position.x + STATS_X, position.y + STATS_Y});
	window.draw(overall_info);

	FlightRouteStatistics connection = std::get<1>(vertices[clicked_vertex_index]);

	sf::Text info(Frontend::font);
	info.setFont(Frontend::font);
	info.setString("\n" + connection.to_string());
	info.setCharacterSize(STATS_SUMMARY_FONT_SIZE);
	info.setFillColor(STATS_CONNECTION_COLOR);
	info.setPosition({position.x + STATS_X + STATS_WIDTH / 2, position.y + STATS_Y});
	window.draw(info);
}