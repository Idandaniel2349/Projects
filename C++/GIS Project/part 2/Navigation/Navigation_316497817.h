#pragma once 

#include "../Common/NavigationGIS.h"
#include "vector"
#include "Routes.h"
#include "Route.h"
#include "functional"
#include "optional"
#include "RouteSection.h"
#include "../Common/AbstractNavigation.h"

using std::function;
using std::optional;
using std::pair;
using std::tuple;
using std::get;
using std::shared_ptr;
using std::vector;

/**
 * @brief Navigation class
 * 
 */
class Navigation_316497817 : public AbstractNavigation{
    static constexpr int maxSpeedLimit = 130;
    static const Meters maxDistanceFromHighWay;

    /**
     *
     * @param routeCords - the cords of the start or end of the route
     * @param oppositeEdgeCoordinate - the opposite edge cords to routeCords
     * @param way - the way the section is on
     * @param wayId - the EntityId of the way we are on
     * @param isStartSection - are we looking at the start section
     * @return the RouteSection of the appropriate edge
     */
    RouteSection getEdgeSection(Coordinates routeCords, size_t segment, Coordinates  oppositeEdgeCoordinate, const AbstractWay& way, EntityId wayId, bool isStartSection) const;

    /**
     * build a Route from its section
     * @param sections -  the sections of the Route
     * @param startPoint - the start of the Route
     * @param endPoint - the end of the Route
     * @return the built Route
     */
    Route buildRouteFromSections(const vector<shared_ptr<RouteSection>>& sections, const Coordinates& startPoint, const Coordinates& endPoint) const;

    /**
     * Get a vector of Routes and returns the appropriate ones according to the rules given in the assignments
     * (by length or time according to isShortestDistanceRoute)
     * @param routes - the vector of
     * @param isShortestDistanceRoute - if we need to check shortest or fastest
     * @return -  the appropriate Route
     */
    Route getBestRoute(vector<Route>& routes, bool isShortestDistanceRoute) const;

    /**
     * Gets the start and end point of the route on a way
     * @param edge the start or end coordinates
     * @return the start or end route coordinates
     */
    tuple<Coordinates, EntityId, std::size_t> getEdgeWayInfo(const Coordinates &edge) const;
public:
    /**
     * @brief Using parent constructor
     *
     */
    using AbstractNavigation::AbstractNavigation;

    /**
     * @brief Get the Routes object
     *
     * @param start
     * @param end
     * @return Routes
     */
    std::unique_ptr<AbstractRoutes> getRoutes(const Coordinates& start, const Coordinates& end) const override;
};

