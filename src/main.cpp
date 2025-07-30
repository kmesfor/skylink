//
// Created by Kian Mesforush on 7/17/25.
//
#include <iostream>
#include <frontend/frontend.h>

#include "backend/DataManager.h"
#include "backend/algorithms/Dijkstra.h"
#include "backend/datamodels/AirportRoute.h"
#include "backend/datamodels/SkylinkGraph.h"

int main() {
	DataManager parser("./data/generated/skylinkgraph.json");

	Airport* start = parser.graph->airport_lookup["IAD"];
	Airport* end = parser.graph->airport_lookup["GNV"];

	Dijkstra dijkstra(parser.graph, start, end, WeightType::DISTANCE);
	dijkstra.execute();

	std::cout << dijkstra.get_elapsed_time() << std::endl;

	auto res = dijkstra.get_result_path();
	for (auto i : res) {
		std::cout << i->origin_code << " -> " << i->destination_code << std::endl;
	}
	// Frontend::display();
}
