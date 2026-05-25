#pragma once
/*=======================================================================
 * UserRouteList.h — Linked List for Multi-Leg Journeys
 * OceanRoute Nav — Maritime Navigation Optimizer
 *=======================================================================*/

#include "GraphNode.h"
#include "../core/DataStructures.h"
#include "../core/RouteData.h"

/// Node in the multi-leg route linked list
struct RouteNode {
    GraphNode* port;     ///< Port at this stop
    Routes flight;       ///< Flight used to reach next stop
    RouteNode* next;     ///< Next stop in journey

    RouteNode(GraphNode* p) : port(p), next(nullptr) {}
};

/// Singly-linked list representing a multi-leg journey
/// Supports add, insert, remove operations for route building
class UserRouteList {
private:
    RouteNode* head;

public:
    UserRouteList();
    ~UserRouteList();           ///< Properly cleans up all nodes

    bool isEmpty() const;

    /// Set the starting port of the journey
    void addStart(GraphNode* p);

    /// Add a stop at the end of the journey
    void addStop(GraphNode* p, Routes flightUsed);

    /// Insert a stop at a specific index
    void insertStop(int index, GraphNode* p, Routes flightUsed);

    /// Remove a stop at a specific index
    void removeStop(int index);

    /// Generate vertex points for rendering the route
    Vector<Vertex> generatePoints();

    /// Access the first node
    RouteNode* getHead();

    /// Access the last node
    RouteNode* getLast();

    /// Delete all nodes and reset to empty
    void clear();
};
