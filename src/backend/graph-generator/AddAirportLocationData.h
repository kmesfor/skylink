//
// Created by Kian Mesforush on 8/4/25.
//

#ifndef ADDAIRPORTLOCATIONDATA_H
#define ADDAIRPORTLOCATIONDATA_H
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>

#include "backend/datamodels/SkylinkGraph.h"

/**
 * Load latitude and longitude data from external data source
 * @param path Filepath of file
 * @param graph Pointer to Skylink Graph
 */
inline void AddAirportLocationData(const std::string& path, SkylinkGraph* graph) {
	std::ifstream file(path);

	if (!file.is_open()) {
		std::cerr << "ERROR: Error loading airport location data. Failed to open file: " << path << std::endl;
		return;
	}

	std::unordered_set<std::string> airports_needed(graph->airports.begin(), graph->airports.end());

	std::string line;

	// skip first
	std::getline(file, line);

	while (std::getline(file, line)) {
		// All of these names are directly from the table on https://www.partow.net/miscellaneous/airportdatabase/
		std::stringstream stream(line);
		std::string value;

		std::string iata_code, lat_decimal_degrees, long_decimal_degrees;

		int col_index = 1; // only grab the columns we want
		bool invalid_airport = false; // logic control to break out of loop quickly
		// Delim by colon
		while (std::getline(stream, value, ';')) {
			if (col_index == 1) {
				iata_code = value;
				if (airports_needed.find(iata_code) == airports_needed.end()) {
					invalid_airport = true;
					break;
				}
			} else if (col_index == 6) {
				lat_decimal_degrees = value;
			} else if (col_index == 7) {
				long_decimal_degrees = value;
				break;
			}
			col_index++;
		}

		// Break out of larger loop iteration
		if (invalid_airport) {
			continue;
		}

		graph->airport_lookup[iata_code]->lat = std::stof(lat_decimal_degrees);
		graph->airport_lookup[iata_code]->lon = std::stof(long_decimal_degrees);
	}
}
#endif //ADDAIRPORTLOCATIONDATA_H
