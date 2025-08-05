//
// Created by Kian Mesforush on 7/17/25.
//

#include <catch2/catch_test_macros.hpp>
#include <cassert>
#include <iostream>

#include "datamodels/SkylinkGraph.h"
#include "datamodels/Airport.h"
#include "datamodels/AirportRoute.h"
#include "algorithms/Dijkstra.h"
#include "algorithms/AStar.h"
#include "AlgorithmComparator.h"

TEST_CASE("AStar Test 1: Basic Test", "[backend]") {
	REQUIRE(true);
}

//each test cases creates a graph and runs A* on it, checking results
TEST_CASE("AStar Test 2: Direct Path", "[backend]") {
    auto* g = new SkylinkGraph();
    g->airports = {"A", "B"};
    for (auto& code : g->airports) {
        g->airport_lookup[code] = new Airport(code, code + " Name");
        g->airport_lookup[code]->lat = 0;
        g->airport_lookup[code]->lon = 0;
    }
    auto* rXY = new AirportRoute("A", "B", 2.0, 2.0, 0.0, 0, 1);
    g->airport_lookup["A"]->outgoing_routes.push_back(rXY);

    std::unordered_map<AirportCode, GeoCoord> coords;
    coords["A"] = {0, 0};
    coords["B"] = {0, 0};

    AStar ast(g, g->airport_lookup["X"], g->airport_lookup["Y"], WeightType::DISTANCE, coords);
    ast.execute(1);
    auto res = ast.get_results();

    REQUIRE(res.size() == 1);
    REQUIRE(total_cost(res[0]) == 2.0);
}

TEST_CASE("AStar Test 3: No Path Exists", "[backend]") {
    auto* g = new SkylinkGraph();
    g->airports = {"P", "Q"};
    for (auto& code : g->airports) {
        g->airport_lookup[code] = new Airport(code, code + " Name");
        g->airport_lookup[code]->lat = 0;
        g->airport_lookup[code]->lon = 0;
    }
    // No routes added

    std::unordered_map<AirportCode, GeoCoord> coords;
    coords["P"] = {0, 0};
    coords["Q"] = {0, 0};

    AStar ast(g, g->airport_lookup["P"], g->airport_lookup["Q"], WeightType::DISTANCE, coords);
    ast.execute(1);
    auto res = ast.get_results();
    REQUIRE(res.size() == 1);
    REQUIRE(res[0].results.empty()); // Checking that no path is there
}