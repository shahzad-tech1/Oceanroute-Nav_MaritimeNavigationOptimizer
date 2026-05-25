/*=======================================================================
 * UserRouteList.cpp — Linked List Implementation
 * OceanRoute Nav — Maritime Navigation Optimizer
 *=======================================================================*/

#include "UserRouteList.h"

UserRouteList::UserRouteList() : head(nullptr) {}

UserRouteList::~UserRouteList() {
    clear();
}

bool UserRouteList::isEmpty() const {
    return head == nullptr;
}

void UserRouteList::addStart(GraphNode* p) {
    clear();  // Reset before starting new route
    head = new RouteNode(p);
}

void UserRouteList::addStop(GraphNode* p, Routes flightUsed) {
    if (!head) {
        head = new RouteNode(p);
        head->flight = flightUsed;
        return;
    }
    RouteNode* temp = head;
    while (temp->next) temp = temp->next;
    temp->next = new RouteNode(p);
    temp->next->flight = flightUsed;
}

void UserRouteList::insertStop(int index, GraphNode* p, Routes flightUsed) {
    if (index == 0) {
        RouteNode* newNode = new RouteNode(p);
        newNode->next = head;
        newNode->flight = flightUsed;
        head = newNode;
        return;
    }
    RouteNode* temp = head;
    for (int i = 0; temp && i < index - 1; i++)
        temp = temp->next;
    if (!temp) return;
    RouteNode* newNode = new RouteNode(p);
    newNode->next = temp->next;
    newNode->flight = flightUsed;
    temp->next = newNode;
}

void UserRouteList::removeStop(int index) {
    if (!head) return;
    if (index == 0) {
        RouteNode* del = head;
        head = head->next;
        delete del;
        return;
    }
    RouteNode* temp = head;
    for (int i = 0; temp && i < index - 1; i++)
        temp = temp->next;
    if (!temp || !temp->next) return;
    RouteNode* del = temp->next;
    temp->next = del->next;
    delete del;
}

Vector<Vertex> UserRouteList::generatePoints() {
    Vector<Vertex> pts;
    RouteNode* temp = head;
    while (temp) {
        Vertex v;
        v.position = temp->port->circle.getPosition();
        pts.push_back(v);
        temp = temp->next;
    }
    return pts;
}

RouteNode* UserRouteList::getHead() {
    return head;
}

RouteNode* UserRouteList::getLast() {
    if (!head) return nullptr;
    RouteNode* temp = head;
    while (temp->next != nullptr)
        temp = temp->next;
    return temp;
}

/// Delete all nodes — fixes memory leak from original code
void UserRouteList::clear() {
    RouteNode* current = head;
    while (current) {
        RouteNode* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
}
