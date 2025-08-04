//
// Created by Kian Mesforush on 8/4/25.
//

#ifndef PRIMARYWINDOW_H
#define PRIMARYWINDOW_H
#include "backend/AlgorithmComparator.h"
#include "frontend/window/Window.h"

class PrimaryWindow : public Window {
	AlgorithmComparator& comparator;
public:
	explicit PrimaryWindow(AlgorithmComparator& comparator): comparator(comparator) {
		this->name = WindowNames::MAIN;

	}
};

#endif //PRIMARYWINDOW_H
