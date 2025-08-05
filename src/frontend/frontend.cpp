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
AlgorithmComparator* Frontend::comparator = nullptr;

void Frontend::display(SkylinkGraph* graph) {
    WindowManager wm;

    // Load font
    if (!font.openFromFile("JetBrainsMono-Regular.ttf")) { // Stored in dist/
        std::cerr << "Failed to load font JetBrainsMono-Regular.ttf" << std::endl;
        return;
    }

    // Load default comparator
    comparator = new AlgorithmComparator("IAD", "GNV", graph, WeightType::DELAY, 5);

    // add windows to window manager
    auto* primary_window = new PrimaryWindow(*comparator);
    wm.add_window(*primary_window);

    auto* result_window = new ResultWindow(*comparator);
    wm.add_window(*result_window);


    // render main window
    wm.render_window(WindowNames::MAIN);
}
