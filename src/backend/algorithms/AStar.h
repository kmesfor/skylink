//resources:
//https://www.geeksforgeeks.org/dsa/a-search-algorithm/
//A* uses heuristic to find shortest path

#include <queue>
#include <set>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>

#include "Algorithm.h"

#ifndef ASTAR_H
#define ASTAR_H

struct GeoCoord {
    double lat_deg;
    double lon_deg;
};


class AStar final : public Algorithm {

    public:
    AStar(const SkylinkGraph* graph, 
        const Airport* start, 
        const Airport* end, 
        const WeightType edge_weight_type) 
        : Algorithm(graph, start, end, edge_weight_type),
        coords_(coords) {}

    std::string get_algorithm_name() override {
        return "A*";
    }

    private:

        const std:: unordered_map<AirportCode, GeoCoord>& coords_;
    
    void run_algorithm(int n) override {
        // Clear the previous results when re-calculating a solution
        result_paths.clear();

        // If the path is not a real path, return nothing (should be prevented by frontend)
        if (end == start) return;

        // Perform A* n times (if possible) and remove an edge from the graph each time to force a new (worse) combination
        std::set<const AirportRoute*> removed_routes;
        for (int i = 0; i < n; i++) {
            perform_astar(removed_routes);

            // No new path added since the last iteration
            if (result_paths.size() <= i) break;

            const auto& last = result_paths.back();

            // If the last path is empty, stop, no new results will be generated
            if (last.empty()) break;

            // Remove the last edge from the last generated path, forcing A* to take a new path next time
            removed_routes.insert(last.back());
        }
    }

    // double haversine(double lat1, double lon1, double lat2, double lon2) {
    //     //helper function for the heuristic
    //     //https://en.wikipedia.org/wiki/Haversine_formula
    //     const double R = 3958.7613; // Radius of the Earth in miles
    //     const double dLat = (lat2 - lat1) * M_PI / 180.0;
    //     const double dLon = (lon2 - lon1) * M_PI / 180.0;

    //     const double a = std::sin(dLat / 2) * std::sin(dLat / 2) +
    //                      std::cos(lat1 * M_PI / 180.0) * std::cos(lat2 * M_PI / 180.0) *
    //                      std::sin(dLon / 2) * std::sin(dLon / 2);
    //     const double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

    //     return R * c; // Distance in kilometers
    // } 
    static double haversine_miles(double lat1, double lon1, double lat2, double lon2) {
        static constexpr double kEarthRadiusMiles = 3958.7613;
        auto deg2rad = [](double d) { return d * M_PI / 180.0; };

        double phi1 = deg2rad(lat1);
        double phi2 = deg2rad(lat2);
        double dphi = deg2rad(lat2 - lat1);
        double dl = deg2rad(lon2 - lon1);

        double a = std::sin(dphi / 2) * std::sin(dphi / 2) +
                   std::cos(phi1) * std::cos(phi2) * std::sin(dl / 2) * std::sin(dl / 2);
        double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
        return kEarthRadiusMiles * c;
    }

    double heuristic(const Airport* a, const Airport* b) {
        auto itA = coords.find(a->code);
        auto itG = coords.find(b->code);
        if (itA == coords.end() || itB == coords.end()) {
            return 0.0;
        }
        double miles = haversine_miles(itA->second.lat_deg, itA->second.lon_deg, 
            itG->second.lat_deg, itG->second.lon_deg);
        return miles / 10.0;
    }



    void perform_astar(std::set<const AirportRoute*>& removed_routes) {
        
        const size_t N = graph->airports.size();
        const double INFINITY = std::numeric_limits<double>::infinity();
        
        std::vector<double> dist(graph->airports.size(), INT_MAX);

        std::vector<double> g_score(N, INFINITY);
        std::vector<const AirportRoute*> prev(N, nullptr);

        std::unordered_map<AirportCode, int> airport_code_to_index;
        airport_code_to_index.reserve(N);
        for (size_t i = 0; i < N; ++i) {
            airport_code_to_index[graph->airports[i]] = i;
        }

        //creating priority queue to select next node (airport) with lowest estimated cost which is f value
        using PQitem = std::pair<double, const Airport*>;
        std::priority_queue<PQitem, std::vector<PQitem>, std::greater<>> open;

        int start_index = airport_code_to_index[start->code];
        g[start_index] = 0.0;
        double f_start = heuristic(start, end);
        open.emplace(f_start, start);

        while (!open.empty()){
            double f_curr = open.top().first;
            const Airport* curr = open.top().second;
            open.pop();

            int curr_index = airport_code_to_index[curr->code];
            double best_f_now = g[u_index] + heuristic(airport, end);
            if (f > best_f_now) continue;

            if (airport == end) {
                break;
            }

            for (const auto& edge : current->outgoing_routes) {
                if (removed_routes.count(edge) == 1) continue;

                int neighbor_index = code_to_index[edge->destination_code];
                double tentative_g = current_g + edge->calculate_weight(edge_weight_type);

                if (tentative_g + 1e-9 < g_score[neighbor_index]) {
                    g_score[neighbor_index] = tentative_g;
                    prev[neighbor_index] = edge;

                    const Airport* neighbor = graph->airport_lookup.at(edge->destination_code);
                    double f_score = tentative_g + heuristic(neighbor, end);
                    open.emplace(f_score, neighbor);
                }
            }

        }


        // Reconstruct path
		auto curr = end;
		int curr_index = airport_code_to_index[curr->code];
		std::vector<const AirportRoute*> path;

		// Make sure path is possible
		if (prev[curr_index] != nullptr) {
			while (true) {
				if (prev[curr_index] == nullptr) {
					// Path broken or no predecessor - exit or error
					break;
				}
				// Add route parts until start is reached
				path.push_back(prev[curr_index]);
				if (curr == start) {
					break;
				} else {
					curr = graph->airport_lookup.at(prev[curr_index]->origin_code);
					curr_index = airport_code_to_index[curr->code];
				}
			}
			// Reverse the list (built backwards)
			std::reverse(path.begin(), path.end());

        }

			// Add this iteration of Dijkstra's results to a list of results for the overall algorithm
			result_paths.push_back(path);
		}

        
};



#endif //ASTAR_H
