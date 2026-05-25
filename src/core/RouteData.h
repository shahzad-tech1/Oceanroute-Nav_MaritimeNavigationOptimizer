#pragma once
/*=======================================================================
 * RouteData.h — Core Data Structures for Routes and Ships
 * OceanRoute Nav — Maritime Navigation Optimizer
 *=======================================================================*/

#include <string>
using namespace std;

/// Represents a single shipping route between two ports
struct Routes {
    string currentPort;       ///< Departure port name
    string destination;       ///< Arrival port name
    string date;              ///< Departure date (DD/MM/YY)
    string departureTime;     ///< Departure time (HH:MM)
    string arrivalTime;       ///< Arrival time (HH:MM)
    unsigned int cost;        ///< Route cost in dollars
    string shippingCompany;   ///< Operating shipping company
};

/// Represents a ship in the docking queue system
struct Ship {
    string shipName;           ///< Unique ship identifier
    string arrivalTime;        ///< Time of arrival at port
    int processingTime;        ///< Required processing hours
    bool isProcessing;         ///< Currently being processed at dock
    float queuePosition;       ///< Animation position in queue
    float processingProgress;  ///< Hours of processing completed

    Ship()
        : shipName(""), arrivalTime(""), processingTime(0),
          isProcessing(false), queuePosition(0), processingProgress(0) {}
};
