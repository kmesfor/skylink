//resources:
//https://www.geeksforgeeks.org/dsa/a-search-algorithm/
//A* uses heuristic function to find shortest path

#ifndef ASTAR_H
#define ASTAR_H

#include <queue>
#include <set>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>

#include "Algorithm.h"
#include "../datamodels/Airport.h"
#include "../datamodels/SkylinkGraph.h"
#include "../datamodels/AirportRoute.h"


class AStar final : public Algorithm {

public:
    AStar(SkylinkGraph* graph,
    const Airport* start,
    const Airport* end,
      const WeightType edge_weight_type)
    : Algorithm(graph, start, end, edge_weight_type) {}

    std::string get_algorithm_name() override {
        return "A*";
    }

private:

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


    static double haversine_miles(double lat1, double lon1, double lat2, double lon2) {
        static constexpr double kEarthRadiusMiles = 3958.7613;
        auto deg2rad = [](double d) { return d * M_PI / 180.0; }; //deg to rad conversion needed bc the trig functions calculate in rads

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
        auto itA = graph->airport_lookup.at(a->code);
        auto itB = graph->airport_lookup.at(b->code);

        double miles = haversine_miles(itA->lat, itA->lon,
            itB->lat, itB->lon);
        return miles / 10.0;
    }

    void perform_astar(std::set<const AirportRoute*>& removed_routes) {
        
        const size_t N = graph->airports.size();
        
        // std::vector<double> dist(graph->airports.size(), INT_MAX);

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
        g_score[start_index] = 0.0;
        // double f_start = heuristic(start, end);
        open.emplace(heuristic(start, end), start);

        while (!open.empty()){
            auto [f_curr, curr] = open.top();
            open.pop();
            // const Airport* curr = open.top().second;
            int curr_index = airport_code_to_index[curr->code];
            double curr_g = g_score[curr_index];

            double exp_f = curr_g + heuristic(curr, end);
            if (f_curr > exp_f + 1e-9) {
                continue; // Skipping if the estimated cost is worse than the current
            }

            if (curr == end) {
                break;
            }

            for (const auto& edge : curr->outgoing_routes) {
                if (removed_routes.count(edge) == 1) continue;

                int neighbor_index = airport_code_to_index[edge->destination_code];
                double tentative_g = curr_g + edge->calculate_weight(edge_weight_type);

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
		std::vector<const AirportRoute*> path;
		// int curr_index = airport_code_to_index[curr->code];
		int end_index = airport_code_to_index[end->code];

		// Make sure path is possible
		if (prev[end_index] != nullptr) {
            const Airport* node = end;
            int node_index = end_index;

			while (true) {
				if (prev[node_index] == nullptr) {
					// Path broken or no predecessor - exit or error
					break;
				}
				// Add route parts until start is reached
				path.push_back(prev[node_index]);
				if (node == start) {
					break;
				} else {
					node = graph->airport_lookup.at(prev[node_index]->origin_code);
					node_index = airport_code_to_index[node->code];
				}
            
			}

            std::reverse(path.begin(), path.end());

        }

			// Add this iteration of A*'s results to a list of results for the overall algorithm
			result_paths.push_back(path);

    }

};


#endif //ASTAR_H
