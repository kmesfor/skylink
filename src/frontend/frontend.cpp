//
// Created by Kian Mesforush on 7/17/25.
//

#include "frontend.h"

#include <vector>

#include "backend/datamodels/AlgorithmResult.h"
#include "window/windows/ResultWindow.h"
#include "window/WindowManager.h"

sf::Font Frontend::font;

void Frontend::display(std::vector<AlgorithmResult>& allResults, SkylinkGraph* graph) {
    WindowManager wm;

    if (!font.openFromFile("JetBrainsMono-Regular.ttf")) { // Stored in dist/
        std::cerr << "Failed to load font JetBrainsMono-Regular.ttf" << std::endl;
        return;
    }

    ResultWindow test_window(&allResults);
    wm.add_window(test_window);

    wm.render_window(WindowNames::MAIN);
}
