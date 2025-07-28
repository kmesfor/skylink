//
// Created by Kian Mesforush on 7/27/25.
//
#include <iostream>

#include "backend/DataManager.h"

const std::string GRAPH_FILEPATH = "./data/generated/skylinkgraph.json";

/**
 * This is a part of a new executable named GenerateGraphFiles used to generate SkylinkGraph json files from
 * raw data (stored in dist/data/generated). Ideally, these scripts will only need to be run during development,
 * once a file is finalized for running the main program, they will not need to be changed.
 */
int main() {
	DataManager manager(GRAPH_FILEPATH);

	std::cout << manager.graph->to_json();
	manager.save();
}
