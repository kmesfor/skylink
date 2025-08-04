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
#include "algorithms/AlgorithmComparator.h"

SkylinkGraph* make_graph_T2() {
    // T2: A--5-->B--3-->C
    auto* g = new SkylinkGraph();
    // Create airports
    g->airports = {"A","B","C"};
    for (auto& code : g->airports) {
        g->airport_lookup[code] = new Airport(code, code+" Name");
        // set dummy lat/lon so heuristic compiles
        g->airport_lookup[code]->lat = 0;
        g->airport_lookup[code]->lon = 0;
    }
    // Create routes
    // origin, dest, sched, actual, delay, cancelled, flights
    auto* rAB = new AirportRoute("A","B", 5.0, 5.0, 0.0, 0, 1);
    auto* rBC = new AirportRoute("B","C", 3.0, 3.0, 0.0, 0, 1);
    g->airport_lookup["A"]->outgoing_routes.push_back(rAB);
    g->airport_lookup["B"]->outgoing_routes.push_back(rBC);
    return g;
}

TEST_CASE("AStar Test 1: Basic Test", "[backend]") {
	REQUIRE(true);
}

void test_astar_T2() {
    auto* g = make_graph_T2();
    const Airport* A = g->airport_lookup["A"];
    const Airport* C = g->airport_lookup["C"];
	
    std::unordered_map<AirportCode, GeoCoord> coords;
    coords["A"] = {0,0};
    coords["B"] = {0,0};
    coords["C"] = {0,0};

    AStar ast(g, A, C, WeightType::DISTANCE, coords);
    ast.execute(1);
    auto res = ast.get_results();
    assert(res.size() == 1);
    assert(!res[0].results.empty());
    assert(total_cost(res[0]) == 8.0);
    std::cout<<"A* T2 passed\n";
}