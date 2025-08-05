//
// Created by Kian Mesforush on 7/17/25.
//

#include <catch2/catch_test_macros.hpp>
#include "AlgorithmComparator.h"
#include "datamodels/SkylinkGraph.h"
#include "datamodels/Airport.h"
#include "datamodels/AirportRoute.h"

TEST_CASE("Comparator Test 1: Basic Test", "[backend]") {
	REQUIRE(true);
}

TEST_CASE("Comparator Test 2: Compare Dijkstra and AStar on direct path", "[backend]") {
    auto* g = new SkylinkGraph();
    g->airports = {"A", "B"};
    for (auto& code : g->airports) {
        g->airport_lookup[code] = new Airport(code, code + " Name");
    }
    auto* rAB = new AirportRoute("A", "B", 3.0, 3.0, 0.0, 0, 1);
    g->airport_lookup["A"]->outgoing_routes.push_back(rAB);

    AlgorithmComparator comparator(g, g->airport_lookup["A"], g->airport_lookup["B"], WeightType::DISTANCE);
    comparator.execute();
    auto results = comparator.get_results();

    REQUIRE(results.size() == 2); // Dijkstra and AStar
    REQUIRE(total_cost(results[0]) == total_cost(results[1]));
    REQUIRE(total_cost(results[0]) == 3.0);
}

TEST_CASE("Comparator Test 3: No Path Exists for either algorithm", "[backend]") {
    auto* g = new SkylinkGraph();
    g->airports = {"X", "Y"};
    for (auto& code : g->airports) {
        g->airport_lookup[code] = new Airport(code, code + " Name");
    }
    // No routes added

    AlgorithmComparator comparator(g, g->airport_lookup["X"], g->airport_lookup["Y"], WeightType::DISTANCE);
    comparator.execute();
    auto results = comparator.get_results();

    REQUIRE(results.size() == 2); // Dijkstra and AStar
    REQUIRE(results[0].results.empty());
    REQUIRE(results[1].results.empty());
}