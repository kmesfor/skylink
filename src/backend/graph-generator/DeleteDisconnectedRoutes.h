//
// Created by Kian Mesforush on 7/29/25.
//

#ifndef DELETEDISCONNECTEDROUTES_H
#define DELETEDISCONNECTEDROUTES_H
#include "backend/datamodels/SkylinkGraph.h"

/**
 * Remove any nodes from the graph that are disconnected. (0 in degree and 0 out degree).
 * Removes from both airport_lookup and airports properties on SkylinkGraph
 * @param graph pointer to the graph to edit
 */
inline void DeleteDisconnectedRoutes(SkylinkGraph* graph) {
	// Store a list of airports to remove. If we remove within this iteration, the iteration could get messed up
	std::vector<std::string> to_remove;
	for (const auto& [code, airport] : graph->airport_lookup) {
		// Only remove airports where both in degree and out degree are 0 (no flight data available, typically for smaller or local airports)
		if (airport->in_degree == 0 && airport->out_degree == 0) {
			to_remove.push_back(code);
		}
	}

	// Iterate each airport to remove
	for (const auto& code : to_remove) {
		const auto* airport = graph->airport_lookup[code];

		// Remove airport object
		graph->airport_lookup.erase(code);
		delete airport;

		// Delete airport from the airports list
		auto vector_elem = std::find(graph->airports.begin(), graph->airports.end(), code);
		if (vector_elem != graph->airports.end()) {
			graph->airports.erase(vector_elem);

		}
	}
}
#endif //DELETEDISCONNECTEDROUTES_H
