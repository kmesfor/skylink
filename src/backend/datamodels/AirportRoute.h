//
// Created by Kian Mesforush on 7/23/25.
//

#ifndef AIRPORTROUTE_H
#define AIRPORTROUTE_H

#include <string>
#include "../resources/json.hpp"

// https://stackoverflow.com/questions/44990068/using-c-typedef-using-type-alias
// Safeguard to ensure developers know that airport codes are not arbitrary strings, but still are backed by std::string type
using AirportCode = std::string;
using json = nlohmann::json;

struct AirportRoute {
	AirportCode origin_code;
	AirportCode destination_code;

	double scheduled_time;
	double average_air_time;
	double average_delay;
	double distance; // miles

	AirportRoute(const AirportCode& origin_code, const AirportCode& destination_code, const double scheduled_time, const double average_air_time, const double average_delay, const double distance) {
		this->origin_code = origin_code;
		this->destination_code = destination_code;
		this->scheduled_time = scheduled_time;
		this->average_air_time = average_air_time;
		this->average_delay = average_delay;
		this->distance = distance;
	}

	double calculate_weight() const {
		//TODO: use some sort of algorithm/formula here
		return 0;
	}

	// Convert the route data into JSON format to be saved
	// [[nodiscard]] ensures that the output of this function is used, not just called
	[[nodiscard]] json to_json() const {
		json data;
		data["origin_code"] = origin_code;
		data["destination_code"] = destination_code;
		data["scheduled_time"] = scheduled_time;
		data["average_air_time"] = average_air_time;
		data["average_delay"] = average_delay;
		data["distance"] = distance;

		return data;
	}
};

#endif //AIRPORTROUTE_H
