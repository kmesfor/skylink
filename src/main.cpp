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
	Airport* end = parser.graph->airport_lookup["OKC"];

	Dijkstra dijkstra(parser.graph, start, end, WeightType::DISTANCE);
	dijkstra.execute();

	std::cout << dijkstra.get_elapsed_time();

	// Frontend::display();
}
