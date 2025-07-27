//
// Created by Kian Mesforush on 7/23/25.
//

#ifndef AIRPORT_H
#define AIRPORT_H

#include <string>
#include "../resources/json.hpp"

#include "AirportRoute.h"

// https://stackoverflow.com/questions/44990068/using-c-typedef-using-type-alias
// Safeguard to ensure developers know that airport codes are not arbitrary strings, but still are backed by std::string type
using AirportCode = std::string;
using json = nlohmann::json;

/**
 * Class to represent an airport. i.e. "IAD". Holds information about outgoing routes and in degree and out degree
 */
struct Airport {
	// Ensure only SkylinkGraph can manage lifecycle (controlled via DataManager)
	friend struct SkylinkGraph;

	// Airport code, such as "BWI" or "IAD"
	std::string code;
	int in_degree = 0;
	int out_degree = 0;

	// Maps an airport
	std::vector<AirportRoute*> outgoing_routes;

	// Convert the airport data into JSON format to be saved
	// [[nodiscard]] ensures that the output of this function is used, not just called
	/**
	 * Convert the airport data into JSON format
	 * @return nlohmann::json
	 */
	[[nodiscard]] json to_json() const {
		json data;
		data["code"] = code;
		data["in"] = in_degree;
		data["out"] = out_degree;
		data["outgoing_routes"] = json::array();
		for (const auto& route : outgoing_routes) {
			data["outgoing_routes"].push_back(route->to_json());
		}

		return data;
	}
private:
	// Prevent public instantiation
	Airport() = default;

	/**
	 * Create an Airport object from JSON. Lifecycle should be managed by DataManager
	 * @param data nlohmann::json data
	 * @return Airport object pointer
	 */
	static Airport* from_json(const json& data) {
		auto* airport = new Airport();
		airport->code = data.at("code");
		airport->in_degree = data.at("in");
		airport->out_degree = data.at("out");
		for (const auto& route : data.at("outgoing_routes")) {
			airport->outgoing_routes.emplace_back(AirportRoute::from_json(route));
		}
		return airport;
	}

	/**
	 * Destructor. Lifecycle should be managed by DataManager
	 */
	~Airport() {
		for (const auto route : outgoing_routes) {
			delete route;
		}
	}
};



#endif //AIRPORT_H
