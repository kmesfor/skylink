//
// Created by Kian Mesforush on 7/17/25.
//

#ifndef FRONTEND_H
#define FRONTEND_H
#include <vector>
#include "backend/datamodels/AlgorithmResult.h"
#include "SFML/Graphics/RenderWindow.hpp"

class Frontend {
public:
	static void display(std::vector<AlgorithmResult>& results, SkylinkGraph* graph);
};


#endif //FRONTEND_H
