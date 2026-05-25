/*=======================================================================
 * GraphNode.cpp — Graph Utility Function Implementations
 * OceanRoute Nav — Maritime Navigation Optimizer
 *=======================================================================*/

#include "GraphNode.h"
#include <cmath>
#include <iostream>

/// Search for a port by name in the ports vector
/// @return Index of the port, or -1 if not found
int searchNode(Vector<GraphNode*>& search, string t) {
    if (search.getSize() == 0) return -1;
    for (int i = 0; i < search.getSize(); i++) {
        GraphNode* temp = search[i];
        if (temp->portName == t) return i;
    }
    return -1;
}

/// Calculate total travel cost between two connected ports
/// Includes route fare + destination port docking charge
int getCost(GraphNode* from, GraphNode* to) {
    for (int i = 0; i < from->trips.getSize(); i++) {
        if (from->trips[i].destination == to->portName)
            return from->trips[i].cost + to->portCharge;
    }
    return -1;
}

/// Convert radians to degrees
float radToDeg(float r) {
    return r * 180.0f / 3.14159265358979323846f;
}

/// Parse "HH:MM" format into total minutes since midnight
int timeToMinutes(const string& t) {
    int hours = stoi(t.substr(0, 2));
    int minutes = stoi(t.substr(3, 2));
    return hours * 60 + minutes;
}

/// Calculate travel duration in minutes between two ports
/// Handles overnight routes (negative duration wraps to next day)
int getTime(GraphNode* from, GraphNode* to) {
    for (int i = 0; i < from->trips.getSize(); i++) {
        if (from->trips[i].destination == to->portName) {
            int depart = timeToMinutes(from->trips[i].departureTime);
            int arrive = timeToMinutes(from->trips[i].arrivalTime);
            int duration = arrive - depart;
            if (duration < 0) duration += 24 * 60;  // Overnight crossing
            return duration;
        }
    }
    return -1;
}

/// Encode date string "DD/MM/YY" to sortable integer YYMMDD
int encodeDate(string d) {
    int day   = stoi(d.substr(0, 2));
    int month = stoi(d.substr(3, 2));
    int year  = stoi(d.substr(6, 2));
    return year * 10000 + month * 100 + day;
}

/// Validate that a connecting route is time-feasible
/// Requires arrival at previous port + 60 min layover <= departure of next
bool isConnectionValid(Routes& prevFlight, Routes& nextFlight) {
    int prevDate = encodeDate(prevFlight.date);
    int nextDate = encodeDate(nextFlight.date);
    int prevArr  = timeToMinutes(prevFlight.arrivalTime);
    int nextDep  = timeToMinutes(nextFlight.departureTime);

    if (prevDate < nextDate) return true;   // Next day — always valid
    if (prevDate > nextDate) return false;   // Cannot travel backwards in time
    return prevArr + 60 <= nextDep;          // Same day — need 1hr minimum layover
}
