//
// Created by Kian Mesforush on 7/17/25.
//

#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <queue>

#include "Algorithm.h"


class Dijkstra final : public Algorithm {
	void run_algorithm() override {
		// Clear the previous results when re-calculating a solution
		result_path.clear();

		// If the path is not a real path, return nothing (should be prevented by frontend)
		if (end == start) return;

		// Initialize a list of distances for each airport with an initial value of INT_MAX (infinity)
		std::vector<double> dist(graph->airports.size(), INT_MAX);

		// Initialize a list of previous path routes for each airport with an initial value of nullptr (no prev node)
		std::vector<const AirportRoute*> prev(graph->airports.size(), nullptr);

		// Create a min heap priority queue for distances between nodes. Contains a pair which holds the distance to an airport and the airport pointer
		std::priority_queue<std::pair<double, const Airport*>, std::vector<std::pair<double, const Airport*>>, std::greater<>> pq;

		// Create a map that takes an airport code (i.e. IAD) and returns the index of IAD's entries in the vectors (to prevent needing multiple maps for dist and prev)
		std::unordered_map<AirportCode, int> airport_code_to_index(graph->airports.size());
		for (int i = 0; i < graph->airports.size(); i++) {
			airport_code_to_index[graph->airports[i]] = i;
		}

		// Initialize the dist for start to be 0
		dist[airport_code_to_index[start->code]] = 0.0;

		// Add start to the PQ to start the loop
		pq.emplace(0.0, start);

		// Main driver loop
		while (!pq.empty()) {
			// Get the edge distance, vertex travelling to, and index of that vertex (for use in dist and prev)
			double distance = pq.top().first;
			const Airport* airport = pq.top().second;
			int index = airport_code_to_index[airport->code];

			// Iterate PQ
			pq.pop();

			// If the distance to a previously found route is faster, ignore this
			if (distance > dist[index]) continue;

			// Check each outgoing route for an airport
			for (const auto& route : airport->outgoing_routes) {
				// Index of destination airport in dist and prev vectors
				int destination_index = airport_code_to_index[route->destination_code];

				double edge_weight = route->calculate_weight(edge_weight_type);

				// If the new path is faster, use it
				if (distance + edge_weight < dist[destination_index]) {
					dist[destination_index] = distance + edge_weight;
					prev[destination_index] = route;

					pq.emplace(dist[destination_index], graph->airport_lookup.at(route->destination_code));
				}
			}
		}

		// Reconstruct path
		auto curr = end;
		int curr_index = airport_code_to_index[curr->code];

		// Make sure path is possible
		if (prev[curr_index] != nullptr) {
			while (true) {
				if (prev[curr_index] == nullptr) {
					// Path broken or no predecessor - exit or error
					break;
				}
				// Add route parts until start is reached
				result_path.push_back(prev[curr_index]);
				if (curr == start) {
					break;
				} else {
					curr = graph->airport_lookup.at(prev[curr_index]->origin_code);
					curr_index = airport_code_to_index[curr->code];
				}
			}
			// Reverse the list (built backwards)
			std::reverse(result_path.begin(), result_path.end());
		}
	}

public:
	Dijkstra(const SkylinkGraph* graph, const Airport* start, const Airport* end, const WeightType edge_weight_type) : Algorithm(graph, start, end, edge_weight_type) {}

	std::string get_algorithm_name() override {
		return "Dijkstra";
	}
};



#endif //DIJKSTRA_H
