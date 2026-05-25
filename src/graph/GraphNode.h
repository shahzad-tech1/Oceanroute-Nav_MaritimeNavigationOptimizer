#pragma once
/*=======================================================================
 * GraphNode.h — Port Graph Node & Utility Functions
 * OceanRoute Nav — Maritime Navigation Optimizer
 *=======================================================================*/

#include <string>
#include <SFML/Graphics.hpp>
#include "../core/DataStructures.h"
#include "../core/RouteData.h"
using namespace std;
using namespace sf;

/// Represents a port in the navigation graph
/// Each port has connections (destinations), scheduled trips,
/// a visual circle for rendering, and a docking queue for ships.
struct GraphNode {
    string portName;                     ///< Port identifier
    unsigned int portCharge;             ///< Docking fee in dollars
    Vector<Routes> trips;                ///< All departing routes
    Vector<GraphNode*> destinations;     ///< Adjacent ports in graph
    CircleShape circle;                  ///< Visual representation
    Vector<Vector<Vertex>> edges;        ///< Edge lines for rendering
    Queue<Ship> dockingQueue;            ///< Ships waiting to dock
    int availableDocks;                  ///< Max simultaneous processing slots
    Vector<Ship> processingShips;        ///< Ships currently being processed

    GraphNode() : portName(""), portCharge(500), availableDocks(2) {}
};

// ── Graph Utility Functions ─────────────────────────────────────────

/// Find index of a port by name in port list. Returns -1 if not found.
int searchNode(Vector<GraphNode*>& search, string t);

/// Get total cost (route cost + destination port charge) between two ports
int getCost(GraphNode* from, GraphNode* to);

/// Get travel time in minutes between two ports
int getTime(GraphNode* from, GraphNode* to);

/// Convert "HH:MM" time string to total minutes
int timeToMinutes(const string& t);

/// Encode "DD/MM/YY" date to integer for comparison
int encodeDate(string d);

/// Check if a connecting flight is time-valid (>= 1 hour layover)
bool isConnectionValid(Routes& prevFlight, Routes& nextFlight);

/// Convert radians to degrees
float radToDeg(float r);
