//
// Created by Kian Mesforush on 7/17/25.
//

#ifndef DATAPARSER_H
#define DATAPARSER_H
#include <string>
#include "datamodels/SkylinkGraph.h"


class DataParser {
public:
	SkylinkGraph graph;

	DataParser(std::string filepath);

private:
	void loadFromFile(std::string filepath);
};



#endif //DATAPARSER_H
