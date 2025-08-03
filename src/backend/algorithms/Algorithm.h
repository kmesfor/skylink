//
// Created by Kian Mesforush on 7/17/25.
//

#ifndef ALGORITHM_H
#define ALGORITHM_H
#include "backend/datamodels/AlgorithmResult.h"
#include "backend/datamodels/SkylinkGraph.h"

/**
 * Abstract class to represent an Algorithm. Dijkstra and A* should extend this class
 * Simplifies implementation for timing and ensures correct variable access for A* and Dijkstra classes
 */
class Algorithm {
protected:
	// Use const to ensure the graph is not edited by algorithms
	const SkylinkGraph* graph;
	const Airport* start;
	const Airport* end;

	// Determines if an algorithm should use path distance or consider average delay statistics for edge weights
	WeightType edge_weight_type;

	// Stores multiple potential paths for the algorithm's result, sorted from best to worst
	std::vector<std::vector<const AirportRoute*>> result_paths = {};

	// Stores the elapsed time for executing the algorithm
	std::chrono::duration<double> elapsed_time = std::chrono::duration<double>(0);

	/**
	 * Runs the algorithm. Should not be called directly
	 * @param n the number of results to populate (at most)
	 */
	virtual void run_algorithm(int n) = 0;

public:
	// Returns the algorithm name, useful for frontend
	virtual std::string get_algorithm_name() = 0;

	/**
	 * Instantiate an Algorithm class. Virtual class.
	 * @param graph Skylink graph pointer to execute on
	 * @param start Starting airport pointer
	 * @param end Ending airport pointer
	 * @param edge_weight_type Specifies the type of calculation to use for edge weights
	 */
	Algorithm(const SkylinkGraph* graph, const Airport* start, const Airport* end, const WeightType edge_weight_type) {
		this->graph = graph;
		this->start = start;
		this->end = end;
		this->edge_weight_type = edge_weight_type;
	}

	/**
	 * Externally facing execution method. #run() should not be directly called.
	 * Times run() and saves duration to elapsed_time property
	 * @param n the number of results to populate (at most). To get only the best path, use n=1
	 */
	void execute(int n) {
		// Begin timer

		const auto begin = std::chrono::high_resolution_clock::now();
		// Execute algorithm
		run_algorithm(n);

		//End timer and update elapsed time
		const auto end = std::chrono::high_resolution_clock::now();
		elapsed_time = std::chrono::duration<double>(end - begin);
	}

	/**
	 * Gets the resulting paths
	 * @return vector of AlgorithmResults in order from best to worst
	 */
	[[nodiscard]] std::vector<AlgorithmResult> get_results() {
		std::vector<AlgorithmResult> results;

		// Iterate each of the results provided by the algorithm
		for (auto route : result_paths) {
			// For each result, create a new AlgorithmResult object
			AlgorithmResult result;
			result.algorithm_name = get_algorithm_name();
			result.start = start;
			result.end = end;
			result.edge_weight_type = edge_weight_type;
			result.elapsed_time = elapsed_time;
			// For each step in the result, create a new FlightRouteStatistics object and push it to the AlgorithmResult object
			for (auto step : route) {
				auto step_origin_name = step->origin_code;
				std::string step_destination_name = step->destination_code;

				step_origin_name = graph->airport_lookup.at(static_cast<std::string>(step->origin_code))->name;
				step_destination_name = graph->airport_lookup.at(static_cast<std::string>(step->destination_code))->name;
				result.results.push_back(std::make_pair(step, FlightRouteStatistics(step, step_origin_name, step_destination_name)));
			}
			results.push_back(result);
		}

		return results;
	}


	/**
	 * Destructor. Class does not own any pointers.
	 */
	virtual ~Algorithm() = default;
};



#endif //ALGORITHM_H
