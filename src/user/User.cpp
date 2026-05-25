/*=======================================================================
 * User.cpp — User Booking Implementation
 * OceanRoute Nav — Maritime Navigation Optimizer
 *=======================================================================*/

#include "User.h"

void User::setVoyages(Routes r) {
    voyages.push_back(r);
}

Vector<Routes>& User::getVoyages() {
    return voyages;
}

void User::setUserEdges(Vector<Vertex> point) {
    userEdges.push_back(point);
}

Vector<Vector<Vertex>>& User::getUserEdges() {
    return userEdges;
}
