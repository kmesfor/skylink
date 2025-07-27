//
// Created by Kian Mesforush on 7/17/25.
//
#include <iostream>
#include <frontend/frontend.h>

#include "backend/DataManager.h"
#include "backend/datamodels/AirportRoute.h"
#include "backend/datamodels/SkylinkGraph.h"

int main() {
	DataManager parser("./data/generated/graph2.json");
	parser.save();


	DataManager parser2("./data/generated/graph.json");

	std::cout << parser2.graph->to_json();
	// Frontend::display();
}
