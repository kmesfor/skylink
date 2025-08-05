//
// Created by Kian Mesforush on 7/17/25.
//

#ifndef ALGORITHMCOMPARATOR_H
#define ALGORITHMCOMPARATOR_H
#include <string>

#include "DataManager.h"
#include "algorithms/AStar.h"
#include "algorithms/Dijkstra.h"
#include "datamodels/AlgorithmResult.h"


/**
 * Structure to facilitate running algorithms, comparing their results, and supplying data to the frontend
 */
struct AlgorithmComparator {
	std::string start;
	std::string end;
	SkylinkGraph* graph;
	WeightType weight_type;
	int num_results;

	std::vector<AlgorithmResult> dijkstra_results;
	std::vector<AlgorithmResult> a_star_results;


	/**
	 * Create an AlgorithmComparator instance.
	 * @param start airport code of start airport
	 * @param end airport code of end airport
	 * @param graph pointer to Skylink graph
	 * @param weight_type WeightType to use for calculations
	 * @param num_results the number of results to populate
	 */
	AlgorithmComparator(const std::string& start, const std::string& end, SkylinkGraph* graph, const WeightType weight_type, const int num_results) {
		this->start = start;
		this->end = end;
		this->graph = graph;
		this->weight_type = weight_type;
		this->num_results = num_results;
	}

	/**
	 * Run algorithms and store results. Pass to frontend for data access
	 */
	void run() {
		const Airport* start_airport = graph->airport_lookup[start];
		const Airport* end_airport = graph->airport_lookup[end];


		Dijkstra dijkstra(graph, start_airport, end_airport, weight_type);
		dijkstra.execute(num_results);
		dijkstra_results = dijkstra.get_results();

		AStar AStar(graph, start_airport, end_airport, weight_type);
		AStar.execute(num_results);
		a_star_results = AStar.get_results();
	}

	/**
	 * Gets the results of Dijkstra's algorithm
	 * @return vector of AlgorithmResult objects
	 */
	[[nodiscard]] std::vector<AlgorithmResult>& get_dijkstra_results() {
		return dijkstra_results;
	}

	/**
	 * Gets the results of A* algorithm
	 * @return vector of AlgorithmResult objects

	 */
	[[nodiscard]] std::vector<AlgorithmResult>& get_a_star_results() {
		return a_star_results;
	}
};



#endif //ALGORITHMCOMPARATOR_H
