//
// Created by Kian Mesforush on 7/17/25.
//

#ifndef DATAMANAGER_H
#define DATAMANAGER_H
#include <fstream>
#include <string>
#include "datamodels/SkylinkGraph.h"

using json = nlohmann::json;

/**
 * Class to manage persistent data for SkylinkGraph objects. Ideally, a separate script will initialize an object,
 * then the frontend and algorithms will load these objects with the DataManager and close without editing.
 * DataManager should own SkylinkGraph. To create a new blank SkylinkGraph, load an empty / non-existent JSON file.
 */
class DataManager {
public:
	SkylinkGraph* graph;
	std::string path;

	/**
	 * Instantiates a DataManager class from a path. Automatically calls #load(). If the file is empty, a blank SkylinkGraph will be created
	 * @param path The path of the input/output file for the graph
	 */
	explicit DataManager(const std::string& path) {
		// Automatically call #load() to load from the file or create a blank SkylinkGraph if the file is empty
		this->path = path;
		this->graph = nullptr;
		this->load();
	}

	/**
	 * Deletes the SkylinkGraph object from memory without saving. (Calls this#close())
	 */
	~DataManager() {
		this->close();
	}

	/**
	* Close the DataManager. Delete graph's memory without saving to file. Should be used when graph data is intended
	* to be fixed. i.e. once fully parsed and finalized.
	*/
	void close() {
		delete graph;
		graph = nullptr;
	}

	/**
	 * Clear the DataManger graph instance. Delete a graph's memory without saving to file. Reallocates a new graph object
	 */
	void clear() {
		delete graph;
		graph = new SkylinkGraph();
	}

	/**
	 * Saves the currently stored SkylinkGraph from memory to JSON
	 * @throws Runtime error if graph is not set
	 * @throws Runtime error if file is not accessible
	 */
	void save() {
		// Ensure the graph object to save exists, throw an error if it doesn't
		if (graph == nullptr) {
			throw std::runtime_error("DataManger#save(): SkylinkGraph is not set");
		}

		// Get the directory path for the file
		std::filesystem::path directory = std::filesystem::path(path).parent_path();

		// Create directory for path if it doesn't exist
		if (!directory.empty() && !std::filesystem::exists(directory)) {
			std::filesystem::create_directories(directory);
		}

		std::ofstream file(path);

		// Throw error if there are problems opening / reading the output file
		if (!file.is_open()) {
			throw std::runtime_error("DataManager#save(): Failed to open file " + path + ". Could not create");
		}

		// Store a json representation of the graph in the file
		file << graph->to_json();
		file.close();

		// Clean up this DataManager instance, deletes graph pointer from memory
		this->close();
	}

	/**
	 * Loads a SkylinkGraph from json storage into memory. Overrides the DataManager.graph property
	 * @throws Runtime error if graph is already set
	 */
	void load() {
		// Throw an error if the graph is already set (override would cause a dangling pointer)
		if (graph != nullptr) {
			throw std::runtime_error("DataManger#load(): SkylinkGraph is already set. Delete before overriding to avoid memory management issues");
		}

		// Initialize the file stream
		std::ifstream file(path);

		// If the file is empty / newly created, return a blank SkylinkGraph
		if (!file || file.peek() == std::ifstream::traits_type::eof()) {
			graph = new SkylinkGraph();
			return;
		}

		// Store the file's contents as a SkylinkGraph in memory
		json data;
		file >> data;
		graph = SkylinkGraph::from_json(data);
	}
};

#endif //DATAMANAGER_H
