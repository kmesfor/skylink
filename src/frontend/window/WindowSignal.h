//
// Created by Kian Mesforush on 7/31/25.
//

#ifndef WINDOWSIGNAL_H
#define WINDOWSIGNAL_H

/**
 * Enum represent different states that the window manager uses to navigate through the UI. Set through different Window classes
 */
enum class WindowSignal {
	NONE,						// No change in state needed
	SHOW_ALGORITHM,				// Show the algorithm result screen
	EXIT,						// Exit the program
	BACK						// Go back to the main screen
};

#endif //WINDOWSIGNAL_H
