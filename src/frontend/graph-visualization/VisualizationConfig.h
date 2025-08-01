//
// Created by Kian Mesforush on 8/1/25.
//

#ifndef VISUALIZATIONCONFIG_H
#define VISUALIZATIONCONFIG_H
#include "SFML/Graphics/Color.hpp"

namespace VisualizationConfig {
	constexpr unsigned int WIDTH = 600;													// Window height
	constexpr unsigned int HEIGHT = 600;												// Window Width

	constexpr float VERTEX_RADIUS = 15.0;												// px radius of each vertex
	constexpr float VERTEX_TEXT_Y_OFFSET = 10.0;										// px below vertex that label should sit
	constexpr sf::Color VERTEX_COLOR = sf::Color::Red;									// color of unselected vertex
	constexpr sf::Color VERTEX_SELECTED_COLOR = sf::Color::Green;						// color of selected vertex
	constexpr int VERTEX_LABEL_FONT_SIZE = 12;											// px font size of vertex label
	constexpr sf::Color VERTEX_LABEL_COLOR = sf::Color::Green;							// font color of vertex label

	constexpr sf::Color LINE_COLOR = sf::Color::Black;									// line color between vertices

	constexpr float BOX_THICKNESS = 2.0;												// px line thickness of border box
	constexpr sf::Color BOX_COLOR = sf::Color::Black;									// line color of border box

	constexpr int INSTRUCTION_FONT_SIZE = 12;											// px font size of instructions text
	constexpr sf::Color INSTRUCTION_COLOR = sf::Color::Black;							// font color of instructions text
	constexpr float INSTRUCTION_TEXT_X = 200;											// relative x position of instructions text
	constexpr float INSTRUCTION_TEXT_Y = 0;												// relative y position of instructions text

	constexpr float STATS_WIDTH = WIDTH;												// width of statistics menu
	constexpr float STATS_HEIGHT = 100;													// height of statistics menu
	constexpr float STATS_X = 0;														// relative x start position of statistics menu
	constexpr float STATS_Y = 0;														// relative y start position of statistics menu
	constexpr sf::Color STATS_FILL_COLOR = sf::Color(200, 200, 200);		// background color of statistics menu
	constexpr sf::Color STATS_OUTLINE_COLOR = sf::Color::Green;							// outline color of statistics menu
	constexpr float STATS_OUTLINE_THICKNESS = 2.0;										// px outline thickness of statistics menu

	constexpr float START_X = 50.0;														// x position to start rendering vertices
	constexpr float START_Y = 50.0 + STATS_HEIGHT;										// y position to start rendering vertices
	constexpr float X_OFFSET = 120.0;													// px horizontal offset between vertices
	constexpr float Y_OFFSET = 60.0;													// ox vertical offset between vertices
}

#endif //VISUALIZATIONCONFIG_H
