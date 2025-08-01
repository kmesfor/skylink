//
// Created by Kian Mesforush on 7/31/25.
//

#ifndef ALGORITHMRESULT_H
#define ALGORITHMRESULT_H
#include <string>

#include "Airport.h"
#include "FlightRouteStatistics.h"

/**
 * Stores the results of an algorithm's execution. Useful for porting data to frontend easily
 */
struct AlgorithmResult {
	// Algorithm inputs
	std::string algorithm_name;
	const Airport* start;
	const Airport* end;
	WeightType edge_weight_type;

	// Duration of algorithm execution
	std::chrono::duration<double> elapsed_time;

	// Result of algorithm. Ordered. Holds a route and its respective statistics
	std::vector<std::pair<const AirportRoute*, FlightRouteStatistics>> results;

	// Compiles statistics for the overall journey. Some statistics (i.e. num_flights are not applicable)
	FlightRouteStatistics get_overall_statistics() const {
		FlightRouteStatistics result;

		// Calculate accumlated statistics for each leg of the journey
		double cancellation_rate_sum = 0.0;
		for (auto route : results) {
			result.avg_scheduled_time += route.second.avg_scheduled_time;
			result.avg_delay += route.second.avg_delay;
			result.avg_time += route.second.avg_time;
			cancellation_rate_sum += route.second.cancellation_rate;
		}

		result.cancellation_rate = cancellation_rate_sum / results.size();

		return result;
	}

	void print() const {
		std::cout << "Algorithm Result: " << algorithm_name << " (" << start->code << " -> " << end->code << ")" << std::endl;
		std::cout << "Elapsed time: " << elapsed_time.count() << " seconds, Weight Type: "
		<< (this->edge_weight_type == WeightType::DELAY ? "delay" : "distance") << std::endl;

		std::cout << "Overall Results: ";
		get_overall_statistics().print();

		std::cout << std::endl << "Individual Path Results:" << std::endl;
		for (auto route : results) {
			std::cout << route.first->origin_code << " -> " << route.first->destination_code << std::endl;
			route.second.print();
			std::cout << std::endl;
		}
	}
};

#endif //ALGORITHMRESULT_H
