//
// Created by Kian Mesforush on 7/17/25.
//

#ifndef DIJKSTRA_H
#define DIJKSTRA_H
#include <thread>

#include "Algorithm.h"


class Dijkstra final : public Algorithm {
	void run_algorithm() override {
		std::cout << "Dijkstra algorithm started" << std::endl;
		// wait 20 seconds
		std::this_thread::sleep_for(std::chrono::seconds(20));
		std::cout << "Dijkstra algorithm finished" << std::endl;
	}

public:
	Dijkstra(const SkylinkGraph* graph, const Airport* start, const Airport* end, const WeightType edge_weight_type) : Algorithm(graph, start, end, edge_weight_type) {}

	std::string get_algorithm_name() override {
		return "Dijkstra";
	}
};



#endif //DIJKSTRA_H
