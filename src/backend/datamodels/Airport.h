//
// Created by Kian Mesforush on 7/23/25.
//

#ifndef AIRPORT_H
#define AIRPORT_H

#include <string>
#include <map>

#include "AirportRoute.h"

// https://stackoverflow.com/questions/44990068/using-c-typedef-using-type-alias
// Safeguard to ensure developers know that airport codes are not arbitrary strings, but still are backed by std::string type
using AirportCode = std::string;

struct Airport {
	// Airport code, such as "BWI" or "IAD"
	std::string code;
	int inDegree = 0;
	int outDegree = 0;

	// Maps an airport
	std::vector<AirportRoute> outgoing_routes;
};



#endif //AIRPORT_H
