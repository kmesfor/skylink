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

	/**
	 * Initialize route statistics through an airport. Avoids exposing unnecessary data to frontend
	 * @param airport_route airport route to genereate data from
	 */
	explicit FlightRouteStatistics(const AirportRoute* airport_route) {
		this->cancellation_rate = static_cast<double>(airport_route->cancelled) / airport_route->num_flights;
		this->avg_scheduled_time = airport_route->avg_scheduled_time;
		this->avg_delay = airport_route->avg_delay;
		this->avg_time = airport_route->avg_time;
		this->num_flights = airport_route->num_flights;
		this->from = airport_route->origin_code;
		this->to = airport_route->destination_code;
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
		oss << "Cancellation rate: " << this->cancellation_rate << "%, Avg Scheduled Time: "
			<< this->avg_scheduled_time << ", Avg Delay: " << this->avg_delay
			<< ", Avg Time: " << this->avg_time << ", Num flights: " << this->num_flights;

		return oss.str();
	}
	void print() const {
		std::cout << to_string() << std::endl;
	}
};

#endif //FLIGHTROUTESTATISTICS_H
