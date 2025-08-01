//
// Created by Kian Mesforush on 7/31/25. Reusing a similar design I created in my minesweeper project (https://github.com/kmesfor/Minesweeper)
//

#ifndef WINDOW_H
#define WINDOW_H

#include "WindowSignal.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "WindowNames.h"

/**
 * Abstract class to represent a Window. Provides basic functionality and interface that needs to be implemented
 */
class Window {
	friend class WindowManager;
protected:
	// Basic configuration
	unsigned int width;
	unsigned int height;
	WindowNames name;
	WindowSignal signal;

	// Destructor
	virtual ~Window() = default;

	/**
	 * Renders the windows contents. Should be called by WindowManager, not directly from other functions
	 * @param window WindowManager's main window
	 */
	virtual void draw(sf::RenderWindow& window) = 0;

	/**
	 * Handles window events. Should be called by WindowManager, not directly from other functions
	 * @param window WindowManager's main window
	 * @param event The event to handle
	 */
	virtual void handle_event(sf::RenderWindow& window, const std::optional<sf::Event>& event) = 0;

	/**
	* Sets the current state of the Window
	* @param signal the signal to set to
	*/
	void set_window_signal(const WindowSignal signal) {
		this->signal = signal;
	}

public: // Everything below here is public facing and can be interacted with outside of the WindowManager. Anything above should not.
	// Constructor
	Window() {
		this->width = 800;
		this->height = 600;
		this->name = WindowNames::NONE;
		this->signal = WindowSignal::NONE;
	}

	/**
	 * Gets the current state of the Window
	 * @return WindowSignal
	 */
	WindowSignal get_window_signal();
};

#endif //WINDOW_H
