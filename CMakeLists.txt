#CMakeLists file adapted from previous class project templates

cmake_minimum_required(VERSION 3.22)
project(Skylink)

set(CMAKE_CXX_STANDARD 17)

# Output executables in /dist folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../dist)

#compile flags to match Gradescope test environment
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Werror") # remove -Wall if you don't want as many warnings treated as errors
# gradescope does use -Werror, so if you remove it here you may run into issues when trying to submit
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

#pull in desired version of catch through cmake and sfml automatically, make it available
Include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG        3.0.1 # Use an updated version of SFML (2.5 used in Minesweeper) to resolve deprecation issues with OpenGL
)
FetchContent_MakeAvailable(SFML)

include_directories(src dist/data)

add_executable(Skylink
        src/main.cpp
        src/backend/algorithms/AStar.h
        src/backend/algorithms/Dijkstra.h
        src/backend/DataManager.h
        src/backend/algorithms/Algorithm.h
        src/backend/AlgorithmComparator.h
        src/frontend/frontend.cpp
        src/frontend/frontend.h
        src/backend/datamodels/Airport.h
        src/backend/datamodels/AirportRoute.h
        src/backend/datamodels/SkylinkGraph.h
        src/backend/datamodels/AlgorithmResult.h
        src/backend/datamodels/FlightRouteStatistics.h
        src/frontend/graph-visualization/VisualizationConfig.h
        src/frontend/graph-visualization/GraphVisualization.h
        src/frontend/window/Window.h
        src/frontend/window/WindowSignal.h
        src/frontend/window/WindowManager.h
        src/frontend/window/windows/ResultWindow.h
        src/frontend/window/WindowNames.h
        src/frontend/graph-visualization/GraphVisualization.cpp
)

add_executable(GenerateGraphFiles
        src/backend/graph-generator/GenerateGraphFiles.cpp
        src/backend/DataManager.h
        src/backend/graph-generator/LoadAirportCodes.h
        src/backend/graph-generator/LoadAirportRoutes.h
        src/backend/graph-generator/DeleteDisconnectedRoutes.h
        src/backend/datamodels/WeightType.h
        src/frontend/window/components/Button.h
        src/backend/graph-generator/AddAirportLocationData.h
)

target_link_libraries(Skylink sfml-system sfml-window sfml-graphics)

# comment everything below out if you are using CLion
#include(CTest)
#include(Catch)
#catch_discover_tests(Tests) # must be named the same as your test executable
