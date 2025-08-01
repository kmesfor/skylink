//
// Created by Kian Mesforush on 7/17/25.
//

#include "frontend.h"

#include <vector>

#include "backend/datamodels/AlgorithmResult.h"
#include "window/TestWindow.h"
#include "window/WindowManager.h"

void Frontend::display(std::vector<AlgorithmResult>& allResults, SkylinkGraph* graph) {
    WindowManager wm;
    TestWindow test_window(&allResults);
    wm.add_window(test_window);

    wm.render_window(WindowNames::MAIN);
}
