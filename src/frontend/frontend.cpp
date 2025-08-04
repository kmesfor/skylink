//
// Created by Kian Mesforush on 7/17/25.
//

#include "frontend.h"

#include <vector>

#include "backend/datamodels/AlgorithmResult.h"
#include "window/windows/ResultWindow.h"
#include "window/WindowManager.h"
#include "window/windows/PrimaryWindow.h"

sf::Font Frontend::font;

void Frontend::display(AlgorithmComparator& comparator, SkylinkGraph* graph) {
    WindowManager wm;

    if (!font.openFromFile("JetBrainsMono-Regular.ttf")) { // Stored in dist/
        std::cerr << "Failed to load font JetBrainsMono-Regular.ttf" << std::endl;
        return;
    }

    PrimaryWindow primary_window(comparator);
    wm.add_window(primary_window);

    ResultWindow result_window(comparator);
    wm.add_window(result_window);

    wm.render_window(WindowNames::MAIN);
}
