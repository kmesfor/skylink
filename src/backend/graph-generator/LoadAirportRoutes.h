//
// Created by Kian Mesforush on 7/29/25.
//

#ifndef LOADAIRPORTROUTES_H
#define LOADAIRPORTROUTES_H
#include <string>

#include "backend/datamodels/SkylinkGraph.h"

/**
 * Load CSV data about airport routes. Fills in information on airports in SkylinkGraph. Expects T_ONTIME_REPORTING.csv file input
 * @param path Filepath of CSV file
 * @param graph Pointer to SkylinkGraph
 * @param count Reference to an integer count of number of rows processed
 */
inline void LoadAirportRoutes(const std::string& path, SkylinkGraph* graph, int& count) {

	std::ifstream file(path);

	// Display error and prevent continuation if file does not exist
	if (!file.is_open()) {
		std::cerr << "ERROR: Error loading airport route data. Failed to open file: " << path << std::endl;
		return;
	}

	std::string line;

	// Omits first line (header)
	std::getline(file, line);

	// Iterate each line
	while (std::getline(file, line)) {
		count++;
		// Storage variables
		std::string origin, destination, arr_delay, arr_delay_new_str, cancelled_str, crs_elapsed_time_str, actual_elapsed_time_str, air_time, distance_str;
		double arr_delay_new, crs_elapsed_time, actual_elapsed_time, distance;
		bool cancelled;

		try {
			// StringStream to parse individual lines
			std::stringstream stream(line);

			// Parse and store information into variables
			std::getline(stream, origin, ',');
			std::getline(stream, destination, ',');
			std::getline(stream, arr_delay, ',');
			std::getline(stream, arr_delay_new_str, ',');
			std::getline(stream, cancelled_str, ',');
			std::getline(stream, crs_elapsed_time_str, ',');
			std::getline(stream, actual_elapsed_time_str, ',');
			std::getline(stream, air_time, ',');
			std::getline(stream, distance_str, ',');
		} catch (std::exception& e) {
			std::cout << "ERROR: " << e.what() << line << std::endl;
			continue;
		}

		cancelled = cancelled_str == "1.00";
		distance = std::stod(distance_str);

		// arr_delay_new, arr_delay, actual_elapsed_time, and air_time will all be null if cancelled
		// Only need these variables to contribute to the average if not cancelled
		if (!cancelled) {
			try {
				// Convert string variables into proper types
				arr_delay_new = std::stod(arr_delay_new_str);
				crs_elapsed_time = std::stod(crs_elapsed_time_str);
				actual_elapsed_time = std::stod(actual_elapsed_time_str);
			} catch (const std::exception& e) {
				// For some reason, some non-cancelled flights are missing data, in this case, ignore their records
				continue;
			}
		}


		// See if an entry for the route has already been created
		AirportRoute* found_route = nullptr;
		for (auto* route : graph->airport_lookup[origin]->outgoing_routes) {
			if (route->destination_code == destination) {
				found_route = route;
				break;
			}
		}

		// If an entry for the route has already been created, modify statistics, otherwise create route
		if (found_route != nullptr) {

			// Only contribute to average statistics if a flight was not cancelled
			if (!cancelled) {
				// Only consider non-cancelled flights when decoding averages
				int prev_num_average = found_route->num_flights - cancelled;
				// new = ((old avg * old avg count) + new data point) / (old avg count + 1)
				found_route->avg_scheduled_time = ((found_route->avg_scheduled_time * prev_num_average) + crs_elapsed_time) / (prev_num_average + 1);
				found_route->avg_time = ((found_route->avg_time * prev_num_average) + actual_elapsed_time) / (prev_num_average + 1);
				found_route->avg_delay = ((found_route->avg_delay * prev_num_average) + arr_delay_new) / (prev_num_average + 1);
			} else {
				// Increment cancelled count if specific flight record was cancelled
				found_route->cancelled++;
			}

			// Increment count when new data is added
			found_route->num_flights++;
		} else {
			// Choosing arr_delay_new not arr_delay because A* and Dijkstra do not consider negative weights
			if (cancelled) {
				// Do not add any to averages if flight is cancelled
				graph->airport_lookup[origin]->outgoing_routes.emplace_back(new AirportRoute(origin, destination, 0, 0, 0, distance, 1, 1));
			} else {
				graph->airport_lookup[origin]->outgoing_routes.emplace_back(new AirportRoute(origin, destination, crs_elapsed_time, actual_elapsed_time, arr_delay_new, distance, 0, 1));
			}

			// Increment in degree and out degree only when routes are created for the first time
			graph->airport_lookup[origin]->out_degree++;
			graph->airport_lookup[destination]->in_degree++;
		}
	}
}
#endif //LOADAIRPORTROUTES_H
