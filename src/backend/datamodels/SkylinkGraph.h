//
// Created by Kian Mesforush on 7/23/25.
//

#ifndef SKYLINKGRAPH_H
#define SKYLINKGRAPH_H

#include <unordered_map>
#include <string>
#include <vector>
#include "../resources/json.hpp"

#include "Airport.h"
#include "AirportRoute.h"

using AirportCode = std::string;
using json = nlohmann::json;

/**
 * Class to represent a collection of Airports and AirportRoutes in a graph.
 * Lifecycle should be managed by DataManager
 */
struct SkylinkGraph {
	// Ensures that only DataManager can instantiate a SkylinkGraph, ensuring that SkylinkGraph's lifecycle is managed by DataManager
	friend class DataManager;
private:

	/**
	 * Constructor. Lifecycle should be managed by DataManager. Prevents instantiation from a public scope
	 */
	SkylinkGraph() = default;

	/**
	 * Destructor. Lifecycle should be managed by DataManager. Prevents destruction from a public scope
	 */
	~SkylinkGraph() {
		for (const auto& airport : airport_lookup) {
			delete airport.second;
		}
	}

	/**
	 * Create a SkylinkGraph object from JSON. Lifecycle should be managed by DataManager
	 * @param data nlohman::json data
	 * @return SkylinkGraph object pointer
	 */
	static SkylinkGraph* from_json(json data) {
		// Initialize a new graph object (deleted from memory on save)
		auto* graph = new SkylinkGraph();
		if (data.contains("airports")) {
			graph->airports = data.at("airports").get<std::vector<AirportCode>>();
		} else {
			graph->airports = {};
			std::cerr << "WARNING: SkylinkGraph airports data is malformed or empty, returning a blank template" << std::endl;
		}

		if (data.contains("airport_lookup")) {
			try {
				for (const auto& airport_elem : data.at("airport_lookup")) {
					auto* airport = Airport::from_json(airport_elem);
					graph->airport_lookup[airport->code] = airport;
				}
			} catch (const std::exception& e) {
				std::cerr << "WARNING: SkylinkGraph airport_lookup data is malformed or empty" << std::endl;
				std::cerr << "Error Message: " << e.what() << std::endl;
			}
		} else {
			graph->airport_lookup = {};
			std::cerr << "WARNING: airport_lookup is empty on SkylinkGraph" << std::endl;
		}
		return graph;
	}
public:
	/**
	 * Operates like an adjacency list, but formatted differently.
	 * A list of airport codes is stored, from that, airport objects can be retrieved
	 * from their airport code. Each airport object stores its outgoing edges, creating
	 * an adjacency list structure. Allows each airport to own its routes and have direct access
	 * Also allows the frontend to easily have a list of all airports without loading all associated data
	 */
	std::vector<AirportCode> airports;

	/**
	 * Lookup map to get an Airport object from its AirportCode. i.e. "IAD"
	 */
	std::unordered_map<AirportCode, Airport*> airport_lookup;

	/**
	 * Convert the graph data into JSON format
	 * @return nlohmann::json
	 */
	[[nodiscard]] std::string to_json() const {
		json data;
		data["airports"] = airports;

		data["airport_lookup"] = json::object();
		for (const auto& airport : airport_lookup) {
			data["airport_lookup"][airport.first] = airport.second->to_json();
		}

		return data.dump();
	}
};

#endif //SKYLINKGRAPH_H
