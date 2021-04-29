#pragma once

#include "../Common/GISNamedTypes.h"
#include "../Common/AbstractGIS.h"
#include "../Common/AbstractRoute.h"
#include "../Common/CoordinatesMath.h"

using std::pair;
/**
 * @brief NavigationValidator class to test validity or Route
 * 
 */
class NavigationValidator {
    static const Meters maxDistanceFromHighWay;
    const AbstractGIS& gis;
    /**
     * calculate distance for road edges(start and end points)
     * @param edgePoint - the edge we check
     * @param direction - the direction we are going
     * @param way - the way we are on
     * @param startingPoint - The start point of the Route
     * @return the calculated length in Meters
     */
    Meters calculateEdgePointDistance(const Coordinates& edgePoint, size_t segment, Direction direction, const AbstractWay& way, bool startingPoint) const;

    /**
    * return information of the edge
    * @param edge - the edge to get info on
    * @return the info of edge
    */
    std::tuple<Coordinates, EntityId, std::size_t> getEdgeWayInfo(const Coordinates &edge) const;

public:
    /**
     * @brief Construct a new Navigation Validator object
     * 
     * @param gis - the referenced GIS object - Note that you get a const ref
     */
    NavigationValidator(const AbstractGIS& gis): gis(gis) {};
    
    /**
     * @brief Validates the given Route from Start Point to End Point on referenced GIS
     * 
     * @param start - Start Point 
     * @param end - End Point
     * @param r - Route to be verified
     * @return true - Route is valid 
     * @return false - Route is not valid
     */
    bool validateRoute(const Coordinates& start, const Coordinates& end, const AbstractRoute& r) const;
};

