//
// Created by Kian Mesforush on 7/28/25.
//

#ifndef LOADAIRPORTCODES_H
#define LOADAIRPORTCODES_H
#include <fstream>
#include <iostream>
#include <sstream>

#include "backend/datamodels/SkylinkGraph.h"
#include "backend/datamodels/Airport.h"

/**
 * Load CSV data about airport codes and names. Populates airports and airport_lookup on SkylinkGraph. Expects L_AIRPORT.csv file input
 * @param path Filepath of CSV file
 * @param graph Reference to SkylinkGraph
 */
inline void LoadAirportCodes(const std::string& path, SkylinkGraph* graph) {
	std::ifstream file(path);

	// Display error and prevent continuation if file does not exist
	if (!file.is_open()) {
		std::cerr << "ERROR: Error loading airport codes. Failed to open file: " << path << std::endl;
		return;
	}

	std::string line;

	// Omits first line (header)
	std::getline(file, line);

	// Iterate each line
	while (std::getline(file, line)) {
		// Storage variables
		std::string code;
		std::string name;

		// StringStream to parse individual lines
		std::stringstream stream(line);

		// Get code and name separated by ',' delimiter
		std::getline(stream, code, ',');
		std::getline(stream, name);

		// Remove quotes from strings
		name.erase(remove(name.begin(), name.end(), '\"' ),name.end());

		// Add to graph
		graph->airports.push_back(code);

		auto* airport = new Airport();
		airport->code = code;
		airport->name = name;
		graph->airport_lookup[code] = airport;
	}
}


#endif //LOADAIRPORTCODES_H
