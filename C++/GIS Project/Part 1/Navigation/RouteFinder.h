//
// Created by sagig on 12/20/2020.
//
#pragma once


#include "optional"
#include "functional"
#include "Route.h"
#include "../MapEntities/Way.h"
#include "NavigationGIS.h"
#include "RouteSection.h"

using std::function;
using std::optional;
using std::unique_ptr;
using std::make_unique;

/**
 * RouteFinder class handling algorithms to finding the fastest and shortest paths
 */
class RouteFinder {
    NavigationGIS navigationGIS;
    Way defaultWay;

public:
    RouteFinder(const NavigationGIS &_navigationGIS) : navigationGIS(_navigationGIS), defaultWay(Way(EntityId(""), "", "", vector<string>(), vector<shared_ptr<Geometry>>(), EntityId(""), Coordinates(Longitude(0), Latitude(0)),
                                                                                                     EntityId(""), Coordinates(Longitude(0), Latitude(0)),vector<Coordinates>(),direction::unidirectional,1,
                                                                                                     false, vector<restricted>())){
    }

    /**
     * This method will run the A* algorithm
     * @param endCoordinates - Coordinates of the end of the routes we need to find
     * @param start - the start of the routes we need to find
     * @param end - the end of the routes we need to find
     * @param weightFunction - the weight function on RouteSection
     * @param heuristic - the heuristic function to use for A*
     * @return all shortest Routes from start to end according to the weightFunction and heuristic
     */
    optional<unique_ptr<vector<vector<shared_ptr<RouteSection>>>>> findRoute(const Coordinates &endCoordinates,const EntityId &start, const EntityId &end, function<double(Way)> weightFunction,
                          function<double(Coordinates, Coordinates)> heuristic);

    /**
     * this method will find the fastest route from start to end
     * @param endCoordinates - Coordinates of the end of the routes we need to find
     * @param start - EntityId of the start of the routes we need to find
     * @param end - EntityId of the end of the routes we need to find
     * @param maxSpeedLimit - the max speed allowed
     * @return the fastest route from start to end
     */
    optional<unique_ptr<vector<vector<shared_ptr<RouteSection>>>>> findFastestRoute(const Coordinates &endCoordinates,const EntityId &start, const EntityId &end,
                                                                int maxSpeedLimit);

    /**
     * this method will find the shortest route from start to end
     * @param endCoordinates - Coordinates of the end of the routes we need to find
     * @param start - EntityId of the start of the routes we need to find
     * @param end - EntityId of the end of the routes we need to find
     * @return the shortest route from start to end
     */
    optional<unique_ptr<vector<vector<shared_ptr<RouteSection>>>>> findShortestRoute(const Coordinates &endCoordinates,const EntityId &start, const EntityId &end);

    /**
     * builds the Routes with RoutesSections after we saved all and finished A*
     * @param cameFrom - unordered_map that saves information of where we came from
     * @param currentId - the EntityId of the current point of the section  we are on the Route
     * @param startId - where the Route start
     * @return the built Routes
     */
    unique_ptr<vector<vector<shared_ptr<RouteSection>>>> buildShortestRoutesFromSection(unordered_map<EntityId, vector<shared_ptr<RouteSection>>> cameFrom, EntityId currentId, EntityId startId);
};



