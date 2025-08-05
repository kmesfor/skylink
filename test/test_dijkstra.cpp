//
// Created by Kian Mesforush on 7/17/25.
//

#include <catch2/catch_test_macros.hpp>

#include "backend/datamodels/SkylinkGraph.h"
#include "backend/datamodels/Airport.h"
#include "backend/datamodels/AirportRoute.h"
#include "backend/algorithms/Dijkstra.h"
#include "backend/algorithms/AStar.h"


TEST_CASE("Dijkstra Test 3: No Path Exists", "[backend]") {
    auto* g = new SkylinkGraph();
    g->airports = {"X", "Y"};
    for (auto& code : g->airports) {
        auto a = new Airport();
        a->code = code;
        g->airport_lookup[code] = a;
        g->airport_lookup[code]->lat = 0;
        g->airport_lookup[code]->lon = 0;
    }
    // No routes added

    Dijkstra dijkstra(g, g->airport_lookup["X"], g->airport_lookup["Y"], WeightType::DISTANCE);
    dijkstra.execute(1);
    auto res = dijkstra.get_results();
    REQUIRE(res.empty()); // No path should be there
    delete g;
}