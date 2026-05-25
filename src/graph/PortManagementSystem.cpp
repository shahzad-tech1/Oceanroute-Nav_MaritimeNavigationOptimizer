/*=======================================================================
 * PortManagementSystem.cpp — Graph Construction & Pathfinding
 * OceanRoute Nav — Maritime Navigation Optimizer
 *
 * Preserves ALL original algorithm logic exactly.
 * Changes: getPorts()/getRoutes() return by reference, destructor added.
 *=======================================================================*/

#include "PortManagementSystem.h"
#include "../core/Constants.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <climits>
using namespace std;

// ─────────────────────────────────────────────────────────────────────
// DESTRUCTOR — Free all allocated GraphNode memory
// ─────────────────────────────────────────────────────────────────────
PortManagementSystem::~PortManagementSystem() {
    for (int i = 0; i < ports.getSize(); i++) {
        delete ports[i];
    }
}

// ─────────────────────────────────────────────────────────────────────
// FILE PARSING — Build graph from routes.txt and portCharges.txt
// ─────────────────────────────────────────────────────────────────────
void PortManagementSystem::readFromFile(const string& filename, const string& portCharges) {
    ifstream file(filename);
    if (!file) {
        cout << "Error Opening file: " << filename << "\n";
        return;
    }

    string currentWord = "";
    char ch;
    int i = 0;
    Routes tempRoute;
    GraphNode* temp = nullptr;
    GraphNode* dtemp = nullptr;

    while (file.get(ch)) {
        if (ch == ' ' || ch == '\n') {
            if (!currentWord.empty()) {
                if (i == 0) {
                    tempRoute.currentPort = currentWord;
                    int idx = searchNode(ports, currentWord);
                    if (idx >= 0) {
                        temp = ports[idx];
                    } else {
                        temp = new GraphNode();
                        ports.push_back(temp);
                        temp->portName = currentWord;
                    }
                } else if (i == 1) {
                    tempRoute.destination = currentWord;
                    int idx = searchNode(ports, currentWord);
                    int didx = searchNode(temp->destinations, currentWord);
                    if (idx >= 0) {
                        dtemp = ports[idx];
                        if (didx == -1)
                            temp->destinations.push_back(dtemp);
                    } else {
                        dtemp = new GraphNode();
                        dtemp->portName = currentWord;
                        ports.push_back(dtemp);
                        if (didx == -1)
                            temp->destinations.push_back(dtemp);
                    }
                } else if (i == 2) {
                    tempRoute.date = currentWord;
                } else if (i == 3) {
                    tempRoute.departureTime = currentWord;
                } else if (i == 4) {
                    tempRoute.arrivalTime = currentWord;
                } else if (i == 5) {
                    tempRoute.cost = stoi(currentWord);
                } else if (i == 6) {
                    tempRoute.shippingCompany = currentWord;
                    i = -1;
                    routes.push_back(tempRoute);
                    temp->trips.push_back(tempRoute);
                }
                currentWord = "";
                i++;
            }
        } else {
            currentWord += ch;
        }
    }
    if (!currentWord.empty()) {
        tempRoute.shippingCompany = currentWord;
        routes.push_back(tempRoute);
    }
    file.close();

    // Parse port charges
    ifstream file2(portCharges);
    if (!file2) {
        cout << "Error Opening file Port Charges: " << portCharges << "\n";
        return;
    }

    string newcurrentWord = "";
    char cd;
    GraphNode* tempPortCharge = nullptr;

    while (file2.get(cd)) {
        if (cd == ' ' || cd == '\n') {
            if (!newcurrentWord.empty()) {
                if (cd == ' ') {
                    int idx = searchNode(ports, newcurrentWord);
                    if (idx >= 0) {
                        tempPortCharge = ports[idx];
                    } else {
                        tempPortCharge = new GraphNode();
                        ports.push_back(tempPortCharge);
                        tempPortCharge->portName = newcurrentWord;
                    }
                } else {
                    tempPortCharge->portCharge = stoi(newcurrentWord);
                }
                newcurrentWord = "";
            }
        } else {
            newcurrentWord += cd;
        }
    }
    if (!newcurrentWord.empty()) {
        tempPortCharge->portCharge = stoi(newcurrentWord);
    }
    file2.close();

    cout << "[OceanRoute] Loaded " << routes.getSize() << " routes, "
         << ports.getSize() << " ports.\n";
}

// ─────────────────────────────────────────────────────────────────────
// PORT POSITIONING — Place port circles using normalized coordinates
// ─────────────────────────────────────────────────────────────────────
void PortManagementSystem::placeAllPortCircles() {
    float w = Config::REF_WIDTH;
    float h = Config::REF_HEIGHT;

    // Set default radius and color for all ports
    for (int i = 0; i < ports.getSize(); i++) {
        ports[i]->circle.setRadius(Config::UI::PORT_RADIUS);
        ports[i]->circle.setFillColor(Config::Colors::PORT_DEFAULT);
        ports[i]->circle.setOrigin(Config::UI::PORT_RADIUS, Config::UI::PORT_RADIUS);
        ports[i]->circle.setOutlineThickness(1.5f);
        ports[i]->circle.setOutlineColor(Color(0, 212, 255, 80));
    }

    // Position ports using normalized coordinates from Constants.h
    for (int i = 0; i < ports.getSize(); i++) {
        string p = ports[i]->portName;
        for (int j = 0; j < Config::PORT_COUNT; j++) {
            if (p == Config::PORT_POSITIONS[j].name) {
                float px = Config::PORT_POSITIONS[j].nx * w;
                float py = Config::PORT_POSITIONS[j].ny * h;
                ports[i]->circle.setPosition(px, py);
                break;
            }
        }
    }

    // Generate edge lines between connected ports
    for (int i = 0; i < ports.getSize(); i++) {
        for (int j = 0; j < ports[i]->trips.getSize(); j++) {
            Vector<Vertex> line;
            for (int k = 0; k < 2; k++) {
                Vertex point;
                point.color = Config::Colors::EDGE_DEFAULT;
                if (k == 0)
                    point.position = ports[i]->circle.getPosition();
                else
                    point.position = ports[searchNode(ports, ports[i]->trips[j].destination)]->circle.getPosition();
                line.push_back(point);
            }
            // Offset duplicate edges slightly
            for (int l = 0; l < ports[i]->edges.getSize(); l++) {
                Vector<Vertex>& existing = ports[i]->edges[l];
                bool sameStart = existing[0].position == line[0].position;
                bool sameEnd   = existing[1].position == line[1].position;
                if (sameStart && sameEnd) {
                    line[1].position.y += 10;
                    line[0].position.y += 10;
                }
            }
            ports[i]->edges.push_back(line);
        }
    }
}

// ─────────────────────────────────────────────────────────────────────
// DIJKSTRA'S ALGORITHM — Cost-optimized shortest path
// ─────────────────────────────────────────────────────────────────────
void PortManagementSystem::shortestPathDijkstra(string startPort, string endPort,
                                                 Vector<int>& path,
                                                 Vector<int>& exploredOrder) {
    int size = ports.getSize();
    Vector<int> dist;
    Vector<bool> visited;
    Vector<int> previous;

    for (int i = 0; i < size; i++) {
        dist.push_back(INT_MAX);
        visited.push_back(false);
        previous.push_back(-1);
    }

    int start = searchNode(ports, startPort);
    int end   = searchNode(ports, endPort);
    dist[start] = 0;

    MinHeap pq;
    HeapNode startNode;
    startNode.priority = 0;
    startNode.index = start;
    pq.push(startNode);

    while (!pq.empty()) {
        HeapNode hn = pq.pop();
        int distSoFar = hn.priority;
        int nodeIndex = hn.index;
        int from = nodeIndex;

        if (visited[from]) continue;
        visited[from] = true;
        exploredOrder.push_back(from);
        if (from == end) break;

        GraphNode* fromPort = ports[from];
        for (int i = 0; i < fromPort->destinations.getSize(); i++) {
            GraphNode* toPort = fromPort->destinations[i];
            int to = searchNode(ports, toPort->portName);
            if (!visited[to]) {
                int cost = getCost(fromPort, toPort);
                if (cost >= 0) {
                    int newDist = dist[from] + cost;
                    if (newDist < dist[to]) {
                        dist[to] = newDist;
                        previous[to] = from;
                        HeapNode nextNode;
                        nextNode.priority = dist[to];
                        nextNode.index = to;
                        pq.push(nextNode);
                    }
                }
            }
        }
    }

    // Reconstruct path (reversed)
    path = Vector<int>();
    for (int i = end; i != -1; i = previous[i])
        path.push_back(i);

    // Reverse to get start→end order
    Vector<int> temp;
    for (int i = path.getSize() - 1; i >= 0; i--)
        temp.push_back(path[i]);
    path = temp;
}

// ─────────────────────────────────────────────────────────────────────
// A* ALGORITHM — Time-optimized shortest path with Euclidean heuristic
// ─────────────────────────────────────────────────────────────────────
float PortManagementSystem::heuristic(GraphNode* a, GraphNode* b) {
    Vector2f pa = a->circle.getPosition();
    Vector2f pb = b->circle.getPosition();
    float dx = pa.x - pb.x;
    float dy = pa.y - pb.y;
    return sqrt(dx * dx + dy * dy);
}

void PortManagementSystem::shortestPathAStar(string startPort, string endPort,
                                              Vector<int>& path,
                                              Vector<int>& exploredOrder) {
    Vector<GraphNode*>& p = getPorts();
    int size = p.getSize();

    Vector<float> gScore, fScore;
    Vector<bool> visited;
    Vector<int> previous;

    for (int i = 0; i < size; i++) {
        gScore.push_back((float)INT_MAX);
        fScore.push_back((float)INT_MAX);
        visited.push_back(false);
        previous.push_back(-1);
    }

    int start = searchNode(p, startPort);
    int end   = searchNode(p, endPort);
    gScore[start] = 0;
    fScore[start] = heuristic(p[start], p[end]);

    MinHeap pq;
    pq.push({ (int)fScore[start], start });

    while (!pq.empty()) {
        HeapNode hn = pq.pop();
        int current = hn.index;

        if (visited[current]) continue;
        visited[current] = true;
        exploredOrder.push_back(current);
        if (current == end) break;

        GraphNode* from = p[current];
        for (int i = 0; i < from->destinations.getSize(); i++) {
            GraphNode* toPort = from->destinations[i];
            int to = searchNode(p, toPort->portName);
            if (visited[to]) continue;

            int travel = getTime(from, toPort);
            if (travel < 0) continue;

            float tentative = gScore[current] + travel;
            if (tentative < gScore[to]) {
                gScore[to] = tentative;
                fScore[to] = tentative + heuristic(toPort, p[end]);
                previous[to] = current;
                pq.push({ (int)fScore[to], to });
            }
        }
    }

    // Reconstruct path
    path = Vector<int>();
    for (int at = end; at != -1; at = previous[at])
        path.push_back(at);

    Vector<int> rev;
    for (int i = path.getSize() - 1; i >= 0; i--)
        rev.push_back(path[i]);
    path = rev;
}

// ─────────────────────────────────────────────────────────────────────
// SHIPPING COMPANY VALIDATION
// ─────────────────────────────────────────────────────────────────────
bool PortManagementSystem::isValidShippingCompany(const string& company) {
    for (int i = 0; i < routes.getSize(); i++) {
        if (routes[i].shippingCompany == company)
            return true;
    }
    return false;
}

// ─────────────────────────────────────────────────────────────────────
// CUSTOM FILTERED DIJKSTRA — Preferred company + excluded ports
// ─────────────────────────────────────────────────────────────────────
void PortManagementSystem::customShortestPath(string startPort, string endPort,
                                               const string& preferredCompany,
                                               Vector<int>& excludedPortIndices,
                                               Vector<int>& path,
                                               Vector<int>& exploredOrder) {
    int size = ports.getSize();
    Vector<int> dist;
    Vector<bool> visited;
    Vector<int> previous;

    for (int i = 0; i < size; i++) {
        dist.push_back(INT_MAX);
        visited.push_back(false);
        previous.push_back(-1);
    }

    int start = searchNode(ports, startPort);
    int end   = searchNode(ports, endPort);

    // Validate start/end aren't excluded
    for (int i = 0; i < excludedPortIndices.getSize(); i++) {
        if (excludedPortIndices[i] == start || excludedPortIndices[i] == end) {
            cout << "Start or end port is excluded!\n";
            return;
        }
    }

    dist[start] = 0;
    MinHeap pq;
    HeapNode startNode;
    startNode.priority = 0;
    startNode.index = start;
    pq.push(startNode);

    while (!pq.empty()) {
        HeapNode hn = pq.pop();
        int from = hn.index;

        if (visited[from]) continue;
        visited[from] = true;
        exploredOrder.push_back(from);
        if (from == end) break;

        GraphNode* fromPort = ports[from];
        for (int i = 0; i < fromPort->destinations.getSize(); i++) {
            GraphNode* toPort = fromPort->destinations[i];
            int to = searchNode(ports, toPort->portName);

            // Skip excluded ports
            bool isExcluded = false;
            for (int j = 0; j < excludedPortIndices.getSize(); j++) {
                if (excludedPortIndices[j] == to) {
                    isExcluded = true;
                    break;
                }
            }
            if (isExcluded || visited[to]) continue;

            // Check preferred shipping company
            bool hasPreferredCompany = false;
            int routeCost = -1;
            for (int j = 0; j < fromPort->trips.getSize(); j++) {
                if (fromPort->trips[j].destination == toPort->portName) {
                    if (preferredCompany.empty() ||
                        fromPort->trips[j].shippingCompany == preferredCompany) {
                        hasPreferredCompany = true;
                        routeCost = fromPort->trips[j].cost + toPort->portCharge;
                        break;
                    }
                }
            }

            if (hasPreferredCompany && routeCost >= 0) {
                int newDist = dist[from] + routeCost;
                if (newDist < dist[to]) {
                    dist[to] = newDist;
                    previous[to] = from;
                    HeapNode nextNode;
                    nextNode.priority = dist[to];
                    nextNode.index = to;
                    pq.push(nextNode);
                }
            }
        }
    }

    // Reconstruct path
    path = Vector<int>();
    for (int i = end; i != -1; i = previous[i])
        path.push_back(i);

    Vector<int> temp;
    for (int i = path.getSize() - 1; i >= 0; i--)
        temp.push_back(path[i]);
    path = temp;
}

// ─────────────────────────────────────────────────────────────────────
// DFS VALID PATHS — Enumerate all time-connected paths
// ─────────────────────────────────────────────────────────────────────
void PortManagementSystem::dfsValidPaths(
    GraphNode* current, GraphNode* destination,
    Vector<GraphNode*>& ports, Vector<bool>& visited,
    Vector<string>& path, Routes* lastFlight,
    Vector<Vector<Vertex>>& points, Vector<Vertex>& currentPoints,
    Vector<Vector<Routes>>& routePaths, Vector<Routes>& currentRouteList)
{
    int idx = searchNode(ports, current->portName);
    if (idx == -1) return;

    visited[idx] = true;
    path.push_back(current->portName);

    Vertex v;
    v.position = current->circle.getPosition();
    currentPoints.push_back(v);

    if (current == destination) {
        // Found a valid path — save it
        for (int i = 0; i < path.getSize(); i++) {
            cout << path[i];
            if (i < path.getSize() - 1) cout << " -> ";
        }
        cout << "\n";

        Vector<Vertex> savedPath;
        for (int i = 0; i < currentPoints.getSize(); i++)
            savedPath.push_back(currentPoints[i]);
        points.push_back(savedPath);

        Vector<Routes> savedRoutes;
        for (int i = 0; i < currentRouteList.getSize(); i++)
            savedRoutes.push_back(currentRouteList[i]);
        routePaths.push_back(savedRoutes);

        visited[idx] = false;
        path.pop_back();
        currentPoints.pop_back();
        return;
    }

    // Explore neighbors
    for (int i = 0; i < current->destinations.getSize(); i++) {
        GraphNode* next = current->destinations[i];
        int nextIdx = searchNode(ports, next->portName);
        if (nextIdx == -1 || visited[nextIdx]) continue;

        for (int f = 0; f < current->trips.getSize(); f++) {
            Routes& flight = current->trips[f];
            if (flight.destination != next->portName) continue;

            if (lastFlight != nullptr) {
                if (!isConnectionValid(*lastFlight, flight))
                    continue;
            }

            currentRouteList.push_back(flight);
            dfsValidPaths(next, destination, ports, visited, path,
                          &flight, points, currentPoints, routePaths, currentRouteList);
            currentRouteList.pop_back();
        }
    }

    path.pop_back();
    currentPoints.pop_back();
    visited[idx] = false;
}

void PortManagementSystem::printAllValidFlightPaths(
    int startIndex, int endIndex,
    Vector<GraphNode*>& ports,
    Vector<Vector<Vertex>>& points,
    Vector<Vector<Routes>>& routePaths)
{
    if (startIndex == -1 || endIndex == -1) {
        cout << "Invalid port names!\n";
        return;
    }

    Vector<bool> visited;
    for (int i = 0; i < ports.getSize(); i++)
        visited.push_back(false);

    Vector<string> path;
    Vector<Vertex> currentPoints;
    Vector<Routes> currentRouteList;

    cout << "Valid time-connected paths:\n";
    dfsValidPaths(ports[startIndex], ports[endIndex], ports, visited,
                  path, nullptr, points, currentPoints, routePaths, currentRouteList);
}

// ─────────────────────────────────────────────────────────────────────
// ACCESSORS — Return by reference to avoid expensive copies
// ─────────────────────────────────────────────────────────────────────
Vector<GraphNode*>& PortManagementSystem::getPorts() {
    return ports;
}

Vector<Routes>& PortManagementSystem::getRoutes() {
    return routes;
}
