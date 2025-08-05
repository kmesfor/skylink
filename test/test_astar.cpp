//
// Created by Kian Mesforush on 7/17/25.
//

#include <catch2/catch_test_macros.hpp>
#include <cassert>
#include <iostream>

#include "backend/algorithms/AStar.h"
#include "backend/datamodels/SkylinkGraph.h"

TEST_CASE("AStar Test 1: Basic Test", "[backend]") {
	REQUIRE(true);
}

//each test cases creates a graph and runs A* on it, checking results
TEST_CASE("AStar Test 2: Direct Path", "[backend]") {
    auto* g = new SkylinkGraph();
    g->airports = {"A", "B"};
    for (auto& code : g->airports) {
        auto a = new Airport();
        a->code = code;
        g->airport_lookup[code] = a;
        g->airport_lookup[code]->lat = 0;
        g->airport_lookup[code]->lon = 0;
    }

    auto* rXY = new AirportRoute("A", "B", 2.0, 2.0, 0.0, 0, 1, 1);
    g->airport_lookup["A"]->outgoing_routes.push_back(rXY);

    AStar ast(g, g->airport_lookup["A"], g->airport_lookup["B"], WeightType::DISTANCE);
    ast.execute(1);
    auto res = ast.get_results();

    REQUIRE(!res[0].results.empty());
}

TEST_CASE("AStar Test 3: No Path Exists", "[backend]") {
    auto* g = new SkylinkGraph();
    g->airports = {"P", "Q"};
    for (auto& code : g->airports) {
        auto a = new Airport();
        a->code = code;
        g->airport_lookup[code] = a;
        g->airport_lookup[code]->lat = 0;
        g->airport_lookup[code]->lon = 0;
    }
    // No routes added

    AStar ast(g, g->airport_lookup["P"], g->airport_lookup["Q"], WeightType::DISTANCE);
    ast.execute(1);
    auto res = ast.get_results();
    REQUIRE(res.size() == 1);
    REQUIRE(res[0].results.empty()); // Checking that no path is there

    delete g;
}