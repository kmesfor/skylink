//
// Created by Kian Mesforush on 7/31/25.
//

#ifndef FLIGHTROUTESTATISTICS_H
#define FLIGHTROUTESTATISTICS_H
#include <sstream>

/**
 * Stores statistics about a flight route. Useful for frontend data
 */
struct FlightRouteStatistics {
	double cancellation_rate;
	double avg_scheduled_time;
	double avg_delay;
	double avg_time;
	int num_flights;
	std::string from;
	std::string to;
	std::string from_name;
	std::string to_name;

	/**
	 * Initialize route statistics through an airport. Avoids exposing unnecessary data to frontend
	 * @param airport_route airport route to genereate data from
	 * @param from_name name of outgoing airport
	 * @param to_name name of destination airport
	 */
	explicit FlightRouteStatistics(const AirportRoute* airport_route, const std::string& from_name, const std::string& to_name) {
		this->cancellation_rate = static_cast<double>(airport_route->cancelled) / airport_route->num_flights;
		this->avg_scheduled_time = airport_route->avg_scheduled_time;
		this->avg_delay = airport_route->avg_delay;
		this->avg_time = airport_route->avg_time;
		this->num_flights = airport_route->num_flights;
		this->from = airport_route->origin_code;
		this->to = airport_route->destination_code;
		this->from_name = from_name;
		this->to_name = to_name;
	}

	/**
	 * Initialize blank route statistics. Used to compile all the statistics for an entire journey
	 */
	FlightRouteStatistics() {
		this->cancellation_rate = 0.0;
		this->avg_scheduled_time = 0.0;
		this->avg_delay = 0.0;
		this->avg_time = 0.0;
		this->num_flights = 0;
	}

	std::string to_string() const {
		std::ostringstream oss;
		oss << std::fixed << std::setprecision(2);
		oss << "\nConnection from " << from_name << " (" << from << ")\nto " << to_name << " (" << to << ")\n\n";
		oss << "Cancellation: " << cancellation_rate << "%\n";
		oss << "Average Scheduled Time: " << avg_scheduled_time << " mins\n";
		oss << "Average Delay: " << avg_delay << " mins\n";
		oss << "Average Total Time: " << avg_time << " mins\n";
		oss << num_flights << " flights flown";

		return oss.str();
	}
	void print() const {
		std::cout << to_string() << std::endl;
	}
};

#endif //FLIGHTROUTESTATISTICS_H
