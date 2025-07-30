//
// Created by Kian Mesforush on 7/17/25.
//

#ifndef ALGORITHM_H
#define ALGORITHM_H
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

	// Stores the optimal path result of the algorithm
	std::vector<AirportRoute* const> result_path = {};

	// Stores the elapsed time for executing the algorithm
	std::chrono::duration<double> elapsed_time = std::chrono::duration<double>(0);

	/**
	 * Runs the algorithm. Should not be called directly
	 */
	virtual void run_algorithm() = 0;

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
	explicit Algorithm(const SkylinkGraph* graph, const Airport* start, const Airport* end, const WeightType edge_weight_type) {
		this->graph = graph;
		this->start = start;
		this->end = end;
		this->edge_weight_type = edge_weight_type;
	}

	/**
	 * Externally facing execution method. #run() should not be directly called.
	 * Times run() and saves duration to elapsed_time property
	 */
	void execute() {
		// Begin timer

		const auto begin = std::chrono::high_resolution_clock::now();
		// Execute algorithm
		run_algorithm();

		//End timer and update elapsed time
		const auto end = std::chrono::high_resolution_clock::now();
		elapsed_time = std::chrono::duration<double>(end - begin);
	}

	/**
	 * Gets the result path
	 * @return vector of airport routes in order
	 */
	[[nodiscard]] const std::vector<AirportRoute* const>& get_result_path() const {
		return result_path;
	}

	/**
	 * Gets the elapsed time of algorithm execution
	 * @return double representing elapsed time
	 */
	[[nodiscard]] double get_elapsed_time() const {
		return elapsed_time.count();
	}


	/**
	 * Destructor. Class does not own any pointers.
	 */
	virtual ~Algorithm() = default;
};



#endif //ALGORITHM_H
