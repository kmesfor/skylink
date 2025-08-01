//
// Created by Kian Mesforush on 8/1/25.
//

#ifndef VISUALIZATIONCONFIG_H
#define VISUALIZATIONCONFIG_H
#include "SFML/Graphics/Color.hpp"

namespace VisualizationConfig {
	constexpr sf::Color COLOR_PRIMARY = sf::Color(91, 104, 169);
	constexpr sf::Color COLOR_SECONDARY = sf::Color(176, 185, 232);
	constexpr sf::Color COLOR_BG = sf::Color(252,252, 253);

	constexpr unsigned int WIDTH = 800;													// Window height
	constexpr unsigned int HEIGHT = 700;												// Window Width

	constexpr float VERTEX_RADIUS = 12.0;												// px radius of each vertex
	constexpr float VERTEX_TEXT_Y_OFFSET = 10.0;										// px below vertex that label should sit
	constexpr sf::Color VERTEX_COLOR = COLOR_PRIMARY;									// color of unselected vertex
	constexpr sf::Color VERTEX_SELECTED_COLOR = COLOR_SECONDARY;						// color of selected vertex
	constexpr int VERTEX_LABEL_FONT_SIZE = 16;											// px font size of vertex label
	constexpr sf::Color VERTEX_LABEL_COLOR = COLOR_PRIMARY;								// font color of vertex label

	constexpr sf::Color LINE_COLOR = sf::Color::Black;									// line color between vertices

	constexpr float BOX_THICKNESS = 2.0;												// px line thickness of border box
	constexpr sf::Color BOX_COLOR = sf::Color::Black;									// line color of border box

	constexpr int INSTRUCTION_FONT_SIZE = 16;											// px font size of instructions text
	constexpr sf::Color INSTRUCTION_COLOR = COLOR_PRIMARY;								// font color of instructions text
	constexpr float INSTRUCTION_TEXT_X = 110;											// relative x position of instructions text
	constexpr float INSTRUCTION_TEXT_Y = 225;											// relative y position of instructions text

	constexpr float STATS_WIDTH = WIDTH;												// width of statistics menu
	constexpr float STATS_HEIGHT = 250;													// height of statistics menu
	constexpr float STATS_X = 0;														// relative x start position of statistics menu
	constexpr float STATS_Y = 0;														// relative y start position of statistics menu
	constexpr sf::Color STATS_FILL_COLOR = COLOR_SECONDARY;								// background color of statistics menu
	constexpr sf::Color STATS_OUTLINE_COLOR = sf::Color::Black;							// outline color of statistics menu
	constexpr float STATS_OUTLINE_THICKNESS = 2.0;										// px outline thickness of statistics menu
	constexpr sf::Color STATS_SUMMARY_COLOR = COLOR_PRIMARY;							// stats summary text color
	constexpr sf::Color STATS_CONNECTION_COLOR = sf::Color::Black;						// stats connection text color
	constexpr float STATS_SUMMARY_FONT_SIZE = 16;										// px stats summary font size
	constexpr float STATS_CONNECTION_FONT_SIZE = 16;									// px stats connection font size

	constexpr float START_X = 200;														// x position to start rendering vertices
	constexpr float START_Y = 50.0 + STATS_HEIGHT;										// y position to start rendering vertices
	constexpr float X_OFFSET = 180.0;													// px horizontal offset between vertices
	constexpr float Y_OFFSET = 80.0;													// ox vertical offset between vertices
}

#endif //VISUALIZATIONCONFIG_H
