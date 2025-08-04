//
// Created by Kian Mesforush on 7/17/25.
//

#ifndef FRONTEND_H
#define FRONTEND_H
#include <vector>

#include "backend/AlgorithmComparator.h"
#include "backend/datamodels/AlgorithmResult.h"
#include "SFML/Graphics/Font.hpp"

class Frontend {
public:
	static sf::Font font;
	static void display(AlgorithmComparator& comparator, SkylinkGraph* graph);
};


#endif //FRONTEND_H
