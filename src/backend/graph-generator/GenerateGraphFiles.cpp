//
// Created by Kian Mesforush on 7/27/25.
//
#include <iostream>

#include "DeleteDisconnectedRoutes.h"
#include "LoadAirportCodes.h"
#include "LoadAirportRoutes.h"
#include "backend/DataManager.h"

const std::string GRAPH_FILEPATH = "./data/generated/skylinkgraph.json";
const std::string L_AIRPORT_FILEPATH = "./data/raw/L_AIRPORT.csv";
const std::string T_ONTIME_REPORTING_FILEPATH = "./data/raw/T_ONTIME_REPORTING.csv";

/**
 * This is a part of a new executable named GenerateGraphFiles used to generate SkylinkGraph json files from
 * raw data (stored in dist/data/generated). Ideally, these scripts will only need to be run during development,
 * once a file is finalized for running the main program, they will not need to be changed.
 */
int main() {
	// Load graph from file
	DataManager manager(GRAPH_FILEPATH);

	// Clear graph's contents (start from a clean slate)
	manager.clear();

	// Load airport codes (airports property)
	LoadAirportCodes(L_AIRPORT_FILEPATH, manager.graph);

	// Load airport objects (airport_lookup)
	LoadAirportRoutes(T_ONTIME_REPORTING_FILEPATH, manager.graph);

	// Delete any airport that has no incoming and no outgoing routes
	DeleteDisconnectedRoutes(manager.graph);

	// std::cout << manager.graph->to_json();
	manager.save();
}
