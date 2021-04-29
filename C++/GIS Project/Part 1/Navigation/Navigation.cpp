//
// Created by idan on 10/12/2020.
//

#include "Navigation.h"
#include "queue"
#include "RouteFinder.h"

using std::priority_queue;
using std::unique_ptr;

const Meters Navigation::maxDistanceFromHighWay(3);

Routes Navigation::getRoutes(const Coordinates &start, const Coordinates &end) const {
    RouteFinder routeFinder(navigationGis);

    auto startWayInfo = getEdgeWayInfo(start);
    auto startWayId = startWayInfo.second;
    auto endWayInfo = getEdgeWayInfo(end);
    auto endWayId = endWayInfo.second;

    if(startWayInfo.first == Coordinates(Longitude(404), Latitude(404))){
        Route invalidRoute = Route(std::vector<std::pair<EntityId, Direction>>(),start,
                                   end, Meters(0), Minutes(0));
        return Routes(false, invalidRoute, invalidRoute, "Can't find ways to build route");
    }
    auto startWay = navigationGis.getWay(startWayId);
    auto endWay = navigationGis.getWay(endWayId);
    const auto startRouteCords = startWayInfo.first;
    const auto endRouteCords = endWayInfo.first;

    if(startWayId == endWayId){
        Route invalidRoute = Route(std::vector<std::pair<EntityId, Direction>>(),start,
                                   end, Meters(0), Minutes(0));
        return Routes(false, invalidRoute, invalidRoute, "Can't find route on the same way");
    }

    const auto startSection = getEdgeSection(startRouteCords, endRouteCords, startWay, startWayId, true);
    const auto endSection = getEdgeSection(endRouteCords, startRouteCords, endWay, endWayId, false);
    const auto& startJunctionId = startSection.getJunctionId();
    const auto& endJunctionId = endSection.getJunctionId();

    EntityId startSearchJunction = (startSection.getWayDirection() == Direction::A_to_B)? startSection.getWay().getJunctions().second :
                                   startSection.getWay().getJunctions().first;

    //if the start coordinates are on a junction we dont need the startingWay and can just start the search from this junction
    if(startSection.getIsJunctionSection()){
        startSearchJunction = startJunctionId;
    }

    Coordinates endJunctionCords = (endSection.getWayDirection() == Direction::A_to_B)? endSection.getWay().getFromJunctionCoordinates():
                                   endSection.getWay().getToJunctionCoordinates();

    const auto shortestWaySectionsOpt = routeFinder.findShortestRoute(endJunctionCords, startSearchJunction, endJunctionId);
    if(!shortestWaySectionsOpt){
        Route invalidRoute = Route(std::vector<std::pair<EntityId, Direction>>(),start,
        end, Meters(0), Minutes(0));
        return Routes(false, invalidRoute, invalidRoute, "No ways connecting start and end point");
    }
    auto& shortestWaySections = shortestWaySectionsOpt.value();
    vector<Route> shortestRoutes;
    //adds the start and end sections to all the lightest routes
    for(auto& sectionRoute: *shortestWaySections){
        sectionRoute.insert(sectionRoute.cbegin(), make_shared<RouteSection>(startSection));
        sectionRoute.push_back(make_shared<RouteSection>(endSection));
        shortestRoutes.push_back(buildRouteFromSections(sectionRoute, startRouteCords, endRouteCords));
    }

    auto fastestWaySectionsOpt = routeFinder.findFastestRoute(endJunctionCords, startSearchJunction, endJunctionId, maxSpeedLimit);
    auto& fastestWaySections = fastestWaySectionsOpt.value();
    vector<Route> fastestRoutes;
    //adds the start and end sections to all the lightest routes
    for(auto& sectionRoute: *fastestWaySections){
        sectionRoute.insert(sectionRoute.cbegin(), make_shared<RouteSection>(startSection));
        sectionRoute.push_back(make_shared<RouteSection>(endSection));
        fastestRoutes.push_back(buildRouteFromSections(sectionRoute, startRouteCords, endRouteCords));
    }

    return Routes(true, getBestRoute(shortestRoutes, true),
                  getBestRoute(fastestRoutes, false));
}

RouteSection Navigation::getEdgeSection(Coordinates routeCords, Coordinates  oppositeEdgeCoordinate, const Way& way, EntityId wayId, bool isStartSection) const{
    if(routeCords != way.getToJunctionCoordinates() && routeCords != way.getFromJunctionCoordinates()) {
        if (!way.isBidirectional()) {
            if (isStartSection) {
                Meters length = way.getLength() -
                                CoordinatesMath::calculateDistance(routeCords, way.getFromJunctionCoordinates());
                return RouteSection(way.getJunctions().first, wayId, Direction::A_to_B, way,
                                    length);
            } else {
                Meters length = way.getLength() -
                                CoordinatesMath::calculateDistance(routeCords, way.getToJunctionCoordinates());
                return RouteSection(way.getJunctions().first, wayId, Direction::A_to_B, way,
                                    length);
            }
        } else {
            //Picks the junction base on distance from the opposite edge coord and if its equal than from the distance from the route cord
            pair<Meters, Meters> distancesFrom(
                    CoordinatesMath::calculateDistance(oppositeEdgeCoordinate, way.getFromJunctionCoordinates()),
                    CoordinatesMath::calculateDistance(routeCords, way.getFromJunctionCoordinates()));
            pair<Meters, Meters> distancesTo(
                    CoordinatesMath::calculateDistance(oppositeEdgeCoordinate, way.getToJunctionCoordinates()),
                    CoordinatesMath::calculateDistance(routeCords, way.getToJunctionCoordinates()));
            Direction fromDir = Direction::B_to_A;
            Direction toDir = Direction::A_to_B;
            Meters legnthAtoB =
                    way.getLength() - CoordinatesMath::calculateDistance(routeCords, way.getFromJunctionCoordinates());
            Meters legnthBtoA =
                    way.getLength() - CoordinatesMath::calculateDistance(routeCords, way.getToJunctionCoordinates());
            if (!isStartSection) {
                fromDir = Direction::A_to_B;
                toDir = Direction::B_to_A;
            }
            return distancesFrom < distancesTo ?
                   RouteSection(way.getJunctions().first, wayId, fromDir, way, legnthBtoA) :
                   RouteSection(way.getJunctions().second, wayId, toDir, way, legnthAtoB);
        }
    }
    else{
        //if the edge coordinates are equal to a junctions coordinates we can discard the way we found and use only the junction for searching
        EntityId junctionId = (routeCords == way.getFromJunctionCoordinates()) ? way.getJunctions().first : way.getJunctions().second;
        return RouteSection(junctionId, wayId, Direction::A_to_B, way, true);
    }
}

Route Navigation::buildRouteFromSections(const vector<shared_ptr<RouteSection>>& sections, const Coordinates& startPoint, const Coordinates& endPoint) const{
    vector<pair<EntityId, Direction>> ways;
    Meters length(0);
    Minutes time(0);
    
    for(const auto& section : sections) {
        if (!section->getIsJunctionSection()) {
            ways.push_back({section->getWayId(), section->getWayDirection()});
            auto sectionLength = section->getLength();
            length += sectionLength;
            time += section->getDuration();
        }
    }

    return Route(ways, startPoint, endPoint, length, time);
}

Route Navigation::getBestRoute(vector<Route>& routes, bool isShortestDistanceRoute) const{
    vector<Route> bestRoutes;
    //finds the best route according to the secondary metric
    if(isShortestDistanceRoute){
        Minutes bestTime = std::min_element(routes.cbegin(), routes.cend(), [](Route a, Route b){
            return a.estimatedDuration() < b.estimatedDuration();
        })->estimatedDuration();
        for(const auto& route : routes){
            if(route.estimatedDuration() == bestTime){
                bestRoutes.push_back(route);
            }
        }
    }
    else{
        Meters shortestLength = std::min_element(routes.cbegin(), routes.cend(), [](Route a, Route b){
            return a.totalLength() < b.totalLength();
        })->totalLength();
        for(const auto& route : routes){
            if(route.totalLength() == shortestLength){
                bestRoutes.push_back(route);
            }
        }
    }
    //amongst the best routes in both metrics returns one of the routes with the least junctions
    auto numOfJunctions = std::min_element(routes.cbegin(), routes.cend(), [](Route a, Route b){
        return a.getWays().size() < b.getWays().size();
    })->getWays().size();
    for(auto& route : bestRoutes){
        if(route.getWays().size() == numOfJunctions){
            return route;
        }
    }
    return bestRoutes[0];
}

pair<Coordinates, EntityId> Navigation::getEdgeWayInfo(const Coordinates &edge) const {
    auto wayInfo = navigationGis.getWayClosestPoint(edge);

    auto wayId = wayInfo.second;
    if(wayInfo.first == Coordinates(Longitude(404), Latitude(404))){
        return wayInfo;
    }
    auto way = navigationGis.getWay(wayId);
    if(way.isHighway() && CoordinatesMath::calculateDistance(edge, wayInfo.first) > maxDistanceFromHighWay){
        wayInfo = navigationGis.getWayClosestPoint(edge, Restrictions("highway"));
    }
    return wayInfo;
}



