//
// Created by Kian Mesforush on 7/23/25.
//

#ifndef SKYLINKGRAPH_H
#define SKYLINKGRAPH_H

#include <unordered_map>
#include <string>
#include <vector>

#include "Airport.h"
#include "AirportRoute.h"

using AirportCode = std::string;

struct SkylinkGraph {
	/* This operates like an adjacency list, but formatted differently.
	 * A list of airport codes is stored, from that, airport objects can be retrieved
	 * from their airport code. Each airport object stores its outgoing edges, creating
	 * an adjacency list setup. Allows each airport to own its routes and have more direct access.
	 * Also allows the frontend to easily have a list of all the airports without grabbing all their associated data
	 */
	std::vector<AirportCode> airports;

	// Lookup map to convert an airport code to airport object
	std::unordered_map<AirportCode, Airport> airport_lookup;
};

#endif //SKYLINKGRAPH_H
