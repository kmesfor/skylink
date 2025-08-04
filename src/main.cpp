//
// Created by Kian Mesforush on 7/17/25.
//
#include <iostream>
#include <frontend/frontend.h>

#include "backend/DataManager.h"
#include "backend/algorithms/Dijkstra.h"
#include "backend/datamodels/AirportRoute.h"
#include "backend/datamodels/SkylinkGraph.h"
#include "backend/datamodels/AlgorithmResult.h"

int main() {
	DataManager parser("./data/generated/skylinkgraph.json");

	Frontend::display(parser.graph);
}
