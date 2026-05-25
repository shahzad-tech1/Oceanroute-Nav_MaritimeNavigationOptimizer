#pragma once
/*=======================================================================
 * PortManagementSystem.h — Graph Construction & Pathfinding Algorithms
 * OceanRoute Nav — Maritime Navigation Optimizer
 *
 * Core system that:
 * - Parses route and port charge data files
 * - Builds the navigation graph
 * - Implements Dijkstra, A*, Custom Filtered Dijkstra
 * - Provides DFS-based valid path enumeration
 *=======================================================================*/

#include "../core/DataStructures.h"
#include "../core/RouteData.h"
#include "GraphNode.h"
#include <string>
using namespace std;

class PortManagementSystem {
private:
    Vector<Routes> routes;       ///< All routes parsed from file
    Vector<GraphNode*> ports;    ///< All port nodes in graph

public:
    /// Parse routes.txt and portCharges.txt to build the graph
    void readFromFile(const string& filename, const string& portCharges);

    /// Assign visual positions and generate edge lines for all ports
    void placeAllPortCircles();

    /// Dijkstra's algorithm — find cheapest path by cost
    /// @param path [out] Ordered list of port indices in shortest path
    /// @param exploredOrder [out] Order in which nodes were explored (for visualization)
    void shortestPathDijkstra(string startPort, string endPort,
                              Vector<int>& path, Vector<int>& exploredOrder);

    /// A* algorithm — find fastest path by travel time with heuristic
    void shortestPathAStar(string startPort, string endPort,
                           Vector<int>& path, Vector<int>& exploredOrder);

    /// Euclidean distance heuristic for A*
    float heuristic(GraphNode* a, GraphNode* b);

    /// Validate if a shipping company name exists in the route data
    bool isValidShippingCompany(const string& company);

    /// Custom Dijkstra with shipping company preference and port exclusion
    void customShortestPath(string startPort, string endPort,
                            const string& preferredCompany,
                            Vector<int>& excludedPortIndices,
                            Vector<int>& path, Vector<int>& exploredOrder);

    /// DFS to find all time-valid multi-hop paths between two ports
    void dfsValidPaths(GraphNode* current, GraphNode* destination,
                       Vector<GraphNode*>& ports, Vector<bool>& visited,
                       Vector<string>& path, Routes* lastFlight,
                       Vector<Vector<Vertex>>& points,
                       Vector<Vertex>& currentPoints,
                       Vector<Vector<Routes>>& routePaths,
                       Vector<Routes>& currentRouteList);

    /// Wrapper to initiate DFS valid path search
    void printAllValidFlightPaths(int startIndex, int endIndex,
                                  Vector<GraphNode*>& ports,
                                  Vector<Vector<Vertex>>& points,
                                  Vector<Vector<Routes>>& routePaths);

    /// Access port list BY REFERENCE (avoids expensive copies)
    Vector<GraphNode*>& getPorts();

    /// Access route list BY REFERENCE
    Vector<Routes>& getRoutes();

    /// Clean up allocated GraphNode memory
    ~PortManagementSystem();
};
