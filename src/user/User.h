#pragma once
/*=======================================================================
 * User.h — User Booking Data
 * OceanRoute Nav — Maritime Navigation Optimizer
 *=======================================================================*/

#include "../core/DataStructures.h"
#include "../core/RouteData.h"
#include <SFML/Graphics.hpp>
using namespace sf;

/// Stores the user's booked voyages and their visual representations
class User {
private:
    Vector<Routes> voyages;               ///< All booked route segments
    Vector<Vector<Vertex>> userEdges;     ///< Visual path lines for bookings

public:
    void setVoyages(Routes r);
    Vector<Routes>& getVoyages();

    void setUserEdges(Vector<Vertex> point);
    Vector<Vector<Vertex>>& getUserEdges();
};
