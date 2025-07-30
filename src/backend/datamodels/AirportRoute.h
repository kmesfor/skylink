//
// Created by Kian Mesforush on 7/23/25.
//

#ifndef AIRPORTROUTE_H
#define AIRPORTROUTE_H

#include <iostream>
#include <string>

#include "WeightType.h"
#include "../resources/json.hpp"

// https://stackoverflow.com/questions/44990068/using-c-typedef-using-type-alias
// Safeguard to ensure developers know that airport codes are not arbitrary strings, but still are backed by std::string type
using AirportCode = std::string;
using json = nlohmann::json;

// Forward declare SkylinkGraph for to make friend line work
struct SkylinkGraph;

struct AirportRoute {
	//Allow airport routes to be instantiated by LoadAirportRoutes function
	friend inline void LoadAirportRoutes(const std::string& path, SkylinkGraph* graph, int& count);

	// Ensure that only Airport can manage lifecycle (controlled through DataManager)
	friend struct Airport;

	AirportCode origin_code;
	AirportCode destination_code;

	double avg_scheduled_time;
	double avg_time;
	double avg_delay;
	double distance; // miles
	int cancelled; // number of cancelled flights

	int num_flights = 0; // track number of flights used to consolidate data (needed for initial calculations)

private:
	/**
	 * Create an AirportRoute from origin_code to destination_code. Lifecycle should be managed by DataManager.
	 * @param origin_code Origin airport code
	 * @param destination_code Destination airport code
	 * @param avg_scheduled_time Average scheduled amount of time for flight route (CRS_ELAPSED_TIME) (minutes)
	 * @param avg_time Average real time for flight route (minutes)
	 * @param avg_delay Average delay for flight route
	 * @param distance Distance between airports (miles)
	 * @param cancelled Number of cancelled flights
	 * @param num_flights Number of flight records used to create average data
	 */
	AirportRoute(const AirportCode& origin_code, const AirportCode& destination_code, const double avg_scheduled_time, const double avg_time, const double avg_delay, const double distance, const int cancelled, const int num_flights) {
		this->origin_code = origin_code;
		this->destination_code = destination_code;
		this->avg_scheduled_time = avg_scheduled_time;
		this->avg_time = avg_time;
		this->avg_delay = avg_delay;
		this->distance = distance;
		this->cancelled = cancelled;
		this->num_flights = num_flights;
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
	[[nodiscard]] double calculate_weight(WeightType edge_weight_type) const {
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
		data["avg_scheduled_time"] = avg_scheduled_time;
		data["avg_time"] = avg_time;
		data["avg_delay"] = avg_delay;
		data["distance"] = distance;
		data["cancelled"] = cancelled;
		data["num_flights"] = num_flights;

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
				data.at("avg_scheduled_time").get<double>(),
				data.at("avg_time").get<double>(),
				data.at("avg_delay").get<double>(),
				data.at("distance").get<double>(),
				data.at("cancelled").get<int>(),
				data.at("num_flights").get<int>()
			);
		} catch (const std::exception& e) {
			std::cerr << "WARNING: Airport route data is malformed or empty, returning a blank template";
			std::cerr << "Error Message: " << e.what() << std::endl;
			route = new AirportRoute("", "", 0.0, 0.0, 0.0, 0.0, false, 0);
		}

		return route;
	}
};

#endif //AIRPORTROUTE_H
