//
// Created by Kian Mesforush on 7/31/25. Reusing a similar design I created in my minesweeper project (https://github.com/kmesfor/Minesweeper)
//

#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H
#include <iostream>
#include <ostream>

#include "WindowNames.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "Window.h"

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;
const std::string WINDOW_NAME = "Skylink";

/**
 * A management class to handle the rendering, event handling, and lifecycle of Window objects
 */
class WindowManager {
public:
	/**
	 * Initialize WindowManager by creating the primary window for the program.
	 */
	WindowManager() {
		this->_window = sf::RenderWindow(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), WINDOW_NAME);
	}

	/**
	 * Adds a Window available for WindowManager to use
	 * @param window Window object
	 */
	void add_window(Window& window) {
		this->_windows[window.name] = &window;
	}

	/**
	 * Renders a Window by name, making it current
	 * @param name WindowName of the Window
	 */
	void render_window(const WindowNames& name) {
		// If a window does not exist, return an error
		if (_windows.find(name) == _windows.end()) {
			std::cerr << "Window does not exist" << std::endl;
			return;
		}

		// Get the Window object represented by the WindowName
		auto* window = _windows[name];

		// Update current_window to reflect the change
		current_window = name;

		// Create a new window so multiple windows can be open at one time
		_window.create(sf::VideoMode({window->width, window->height}), WINDOW_NAME);

		// Standard window event loop
		while (_window.isOpen()) {
			// Event handler
			while (const std::optional event = _window.pollEvent()) {
				if (event->is<sf::Event::Closed>()) {
					_window.close();
				} else {
					// Pass event handling to the Window object
					window->handle_event(_window, event);
				}
			}

			// Draw and display the window
			window->draw(_window);
			_window.display();
		}
	}

	/**
	 * Grab the Window object of the currently displayed window
	 * @return Window pointer
	 */
	Window* get_current_window() const {
		return _windows.find(current_window)->second;
	}

	/**
	 * Gets the WindowName of the currently displayed window
	 * @return WindowName
	 */
	WindowNames get_current_window_name() const {
		return current_window;
	}

private:
	// Internal primary window shared by all windows
	sf::RenderWindow _window;

	// Internal storage of windows
	std::unordered_map<WindowNames, Window*> _windows;

	// Pointer to the window being currently displayed by the manager
	WindowNames current_window = WindowNames::NONE;
};
#endif //WINDOWMANAGER_H
