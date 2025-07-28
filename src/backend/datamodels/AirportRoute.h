//
// Created by Kian Mesforush on 7/23/25.
//

#ifndef AIRPORTROUTE_H
#define AIRPORTROUTE_H

#include <iostream>
#include <string>
#include "../resources/json.hpp"

// https://stackoverflow.com/questions/44990068/using-c-typedef-using-type-alias
// Safeguard to ensure developers know that airport codes are not arbitrary strings, but still are backed by std::string type
using AirportCode = std::string;
using json = nlohmann::json;

struct AirportRoute {
	// Ensure that only Airport can manage lifecycle (controlled through DataManager)
	friend struct Airport;

	AirportCode origin_code;
	AirportCode destination_code;

	double scheduled_time;
	double average_air_time;
	double average_delay;
	double distance; // miles

private:
	/**
	 * Create an AirportRoute from origin_code to destination_code. Lifecycle should be managed by DataManager.
	 * @param origin_code Origin airport code
	 * @param destination_code Destination airport code
	 * @param scheduled_time Scheduled amount of time for flight route (CRS_ELAPSED_TIME) (minutes)
	 * @param average_air_time Average real airtime for flight route (minutes)
	 * @param average_delay Average delay for flight route
	 * @param distance Distance between airports (miles)
	 */
	AirportRoute(const AirportCode& origin_code, const AirportCode& destination_code, const double scheduled_time, const double average_air_time, const double average_delay, const double distance) {
		this->origin_code = origin_code;
		this->destination_code = destination_code;
		this->scheduled_time = scheduled_time;
		this->average_air_time = average_air_time;
		this->average_delay = average_delay;
		this->distance = distance;
	}

	/**
	 * Destructor. Lifecycle should be managed by DataManager
	 */
	~AirportRoute() = default;
public:
	/**
	 * Calculate the edge weight for this route
	 * @return edge weight
	 */
	[[nodiscard]] double calculate_weight() const {
		//TODO: use some sort of algorithm/formula here
		return 0;
	}

	// [[nodiscard]] ensures that the output of this function is used, not just called
	/**
	 * Convert route data into JSON format
	 * @return nlohmann::json
	 */
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

	/**
	 * Create an AirportRoute object from JSON. Lifecycle should be managed by DataManager
	 * @param data nlohmann::json data
	 * @return AirportRoute object pointer
	 */
	static AirportRoute* from_json(const json& data) {
		AirportRoute* route;
		try {
			// .get<type>() asserts types when grabbing from JSON
			route = new AirportRoute(
				data.at("origin_code").get<AirportCode>(),
				data.at("destination_code").get<AirportCode>(),
				data.at("scheduled_time").get<double>(),
				data.at("average_air_time").get<double>(),
				data.at("average_delay").get<double>(),
				data.at("distance").get<double>()
			);
		} catch (const std::exception& e) {
			std::cerr << "WARNING: Airport route data is malformed or empty, returning a blank template";
			std::cerr << "Error Message: " << e.what() << std::endl;
			route = new AirportRoute("", "", 0.0, 0.0, 0.0, 0.0);
		}

		return route;
	}
};

#endif //AIRPORTROUTE_H
