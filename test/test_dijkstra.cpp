//
// Created by Kian Mesforush on 7/17/25.
//

#include "datamodels/SkylinkGraph.h"
#include "datamodels/Airport.h"
#include "datamodels/AirportRoute.h"
#include "algorithms/Dijkstra.h"
#include "algorithms/AStar.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Dijkstra Test 1: Basic Test", "[backend]") {
	REQUIRE(true);
}

TEST_CASE("Dijkstra Test 2: Direct Path", "[backend]") {
    auto* g = new SkylinkGraph();
    g->airports = {"A", "B"};
    for (auto& code : g->airports) {
        g->airport_lookup[code] = new Airport(code, code + " Name");
    }
    auto* rAB = new AirportRoute("A", "B", 5.0, 5.0, 0.0, 0, 1);
    g->airport_lookup["A"]->outgoing_routes.push_back(rAB);

    Dijkstra dijkstra(g, g->airport_lookup["A"], g->airport_lookup["B"], WeightType::DISTANCE);
    dijkstra.execute(1);
    auto res = dijkstra.get_results();
    REQUIRE(res.size() == 1);
    REQUIRE(total_cost(res[0]) == 5.0);
}

TEST_CASE("Dijkstra Test 3: No Path Exists", "[backend]") {
    auto* g = new SkylinkGraph();
    g->airports = {"X", "Y"};
    for (auto& code : g->airports) {
        g->airport_lookup[code] = new Airport(code, code + " Name");
    }
    // No routes added

    Dijkstra dijkstra(g, g->airport_lookup["X"], g->airport_lookup["Y"], WeightType::DISTANCE);
    dijkstra.execute(1);
    auto res = dijkstra.get_results();
    REQUIRE(res.size() == 1);
    REQUIRE(res[0].results.empty()); // No path should be there
}