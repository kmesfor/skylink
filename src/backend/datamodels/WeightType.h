//
// Created by Kian Mesforush on 7/30/25.
//

#ifndef WEIGHTTYPE_H
#define WEIGHTTYPE_H

/**
 * Enum representing different weight modes.
 * DISTANCE = use physical distance between airports to find the most optimal path
 * DELAY = consider average airport delays to find the most optimal path
 */
enum class WeightType {
	DISTANCE,
	DELAY
};

#endif //WEIGHTTYPE_H
