//
// Created by Kian Mesforush on 7/17/25.
//

#include <catch2/catch_test_macros.hpp>

#include "backend/AlgorithmComparator.h"
#include "backend/datamodels/SkylinkGraph.h"


TEST_CASE("Comparator Test 2: No Path Exists for either algorithm", "[backend]") {
    auto* g = new SkylinkGraph();
    g->airports = {"A", "B"};
    for (auto& code : g->airports) {
        auto a = new Airport();
        a->code = code;
        g->airport_lookup[code] = a;
    }
    auto* rAB = new AirportRoute("A", "B", 2.0, 2.0, 0.0, 0, 1, 1);
    g->airport_lookup["A"]->outgoing_routes.push_back(rAB);

    AlgorithmComparator comparator("A", "B", g, WeightType::DISTANCE, 2);
    comparator.run();
    auto results = comparator.get_dijkstra_results();

    REQUIRE(results.size() == 1); // Dijkstra and AStar
}