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

// A list of T_ONTIME_REPORTING.csv files, each file covers a certain month/year,
// including more of these files will help make the graph more accurate
const std::vector<std::string> T_ONTIME_REPORTING_FILEPATHS = {
	"./data/raw/JAN_2025_T_ONTIME_REPORTING.csv",
	"./data/raw/FEB_2025_T_ONTIME_REPORTING.csv",
	"./data/raw/MAR_2025_T_ONTIME_REPORTING.csv",
	"./data/raw/JAN_2024_T_ONTIME_REPORTING.csv",
	"./data/raw/FEB_2024_T_ONTIME_REPORTING.csv",
	"./data/raw/MAR_2024_T_ONTIME_REPORTING.csv",
	"./data/raw/APR_2024_T_ONTIME_REPORTING.csv",
	"./data/raw/MAY_2024_T_ONTIME_REPORTING.csv",
	"./data/raw/JUN_2024_T_ONTIME_REPORTING.csv",
	"./data/raw/JUL_2024_T_ONTIME_REPORTING.csv",
	"./data/raw/AUG_2024_T_ONTIME_REPORTING.csv",
	"./data/raw/SEP_2024_T_ONTIME_REPORTING.csv",
	"./data/raw/OCT_2024_T_ONTIME_REPORTING.csv",
	"./data/raw/NOV_2024_T_ONTIME_REPORTING.csv",
	"./data/raw/DEC_2024_T_ONTIME_REPORTING.csv",
	"./data/raw/JAN_2023_T_ONTIME_REPORTING.csv",
	"./data/raw/FEB_2023_T_ONTIME_REPORTING.csv",
	"./data/raw/MAR_2023_T_ONTIME_REPORTING.csv",
	"./data/raw/APR_2023_T_ONTIME_REPORTING.csv",
	"./data/raw/MAY_2023_T_ONTIME_REPORTING.csv",
	"./data/raw/JUN_2023_T_ONTIME_REPORTING.csv",
	"./data/raw/JUL_2023_T_ONTIME_REPORTING.csv",
	"./data/raw/AUG_2023_T_ONTIME_REPORTING.csv",
	"./data/raw/SEP_2023_T_ONTIME_REPORTING.csv",
	"./data/raw/OCT_2023_T_ONTIME_REPORTING.csv",
	"./data/raw/NOV_2023_T_ONTIME_REPORTING.csv",
	"./data/raw/DEC_2023_T_ONTIME_REPORTING.csv",
	"./data/raw/JAN_2022_T_ONTIME_REPORTING.csv",
	"./data/raw/FEB_2022_T_ONTIME_REPORTING.csv",
	"./data/raw/MAR_2022_T_ONTIME_REPORTING.csv",
	"./data/raw/APR_2022_T_ONTIME_REPORTING.csv",
	"./data/raw/MAY_2022_T_ONTIME_REPORTING.csv",
	"./data/raw/JUN_2022_T_ONTIME_REPORTING.csv",
	"./data/raw/JUL_2022_T_ONTIME_REPORTING.csv",
	"./data/raw/AUG_2022_T_ONTIME_REPORTING.csv",
	"./data/raw/SEP_2022_T_ONTIME_REPORTING.csv",
	"./data/raw/OCT_2022_T_ONTIME_REPORTING.csv",
	"./data/raw/NOV_2022_T_ONTIME_REPORTING.csv",
	"./data/raw/DEC_2022_T_ONTIME_REPORTING.csv",
};
/**
 * This is a part of a new executable named GenerateGraphFiles used to generate SkylinkGraph json files from
 * raw data (stored in dist/data/generated). Ideally, these scripts will only need to be run during development,
 * once a file is finalized for running the main program, they will not need to be changed.
 */
int main() {
	std::cout << "Generating graph files" << std::endl;
	const auto begin = std::chrono::high_resolution_clock::now();

	// Load graph from file
	std::cout << "Loading DataManager" << std::endl;
	DataManager manager(GRAPH_FILEPATH);

	// Clear graph's contents (start from a clean slate)
	manager.clear();

	// Load airport codes (airports property)
	std::cout << "Loading airport name map" << std::endl;
	LoadAirportCodes(L_AIRPORT_FILEPATH, manager.graph);

	// Store number of rows processed
	int count = 0;
	// Load airport objects (airport_lookup). Take an array of multiple monthly report files
	for (const auto& path : T_ONTIME_REPORTING_FILEPATHS) {
		std::cout << "Running report aggregation on: " << path << std::endl;
		LoadAirportRoutes(path, manager.graph, count);
	}
	std::cout << "Finish loading reports" << std::endl;

	// Delete any airport that has no incoming and no outgoing routes
	std::cout << "Deleting disconnected airports" << std::endl;
	DeleteDisconnectedRoutes(manager.graph);

	// std::cout << manager.graph->to_json();
	std::cout << "Saving graph to: " << manager.path << std::endl;
	manager.save();

	const auto end = std::chrono::high_resolution_clock::now();
	std::cout << "Done! " << count << " rows of data processed in " << std::chrono::duration<double>(end - begin).count() << " seconds" << std::endl;
}
