//
// Created by Kian Mesforush on 7/31/25.
//

#ifndef WINDOWNAMES_H
#define WINDOWNAMES_H
/**
 * Used to retrieve a window by lexicographical name rather than number when calling WindowManager#render_window
 */
enum class WindowNames {
	NONE = -1,
	MAIN = 0,
	RESULTS = 1
};
#endif //WINDOWNAMES_H
