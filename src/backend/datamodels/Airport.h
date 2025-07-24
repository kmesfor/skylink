//
// Created by Kian Mesforush on 7/23/25.
//

#ifndef AIRPORT_H
#define AIRPORT_H

#include <string>
#include <map>
#include "../resources/json.hpp"

#include "AirportRoute.h"

// https://stackoverflow.com/questions/44990068/using-c-typedef-using-type-alias
// Safeguard to ensure developers know that airport codes are not arbitrary strings, but still are backed by std::string type
using AirportCode = std::string;
using json = nlohmann::json;

struct Airport {
	// Airport code, such as "BWI" or "IAD"
	std::string code;
	int in_degree = 0;
	int out_degree = 0;

	// Maps an airport
	std::vector<AirportRoute> outgoing_routes;

	// Convert the airport data into JSON format to be saved
	// [[nodiscard]] ensures that the output of this function is used, not just called
	[[nodiscard]] json to_json() const {
		json data;
		data["code"] = code;
		data["in"] = in_degree;
		data["out"] = out_degree;
		data["outgoing_routes"] = json::array();
		for (const auto& route : outgoing_routes) {
			data["outgoing_routes"].push_back(route.to_json());
		}

		return data;
	}
};



#endif //AIRPORT_H
