//
// Created by idan on 10/12/2020.
//

#include "Navigation_316497817.h"
#include "queue"
#include "RouteFinder.h"
#include "../Common/CoordinatesMath.h"
#include "../Common/NavigationRegistration.h"

using std::priority_queue;
using std::unique_ptr;
using std::make_shared;

const Meters Navigation_316497817::maxDistanceFromHighWay(3);
REGISTER_NAVIGATION(Navigation_316497817)

std::unique_ptr<AbstractRoutes> Navigation_316497817::getRoutes(const Coordinates &start, const Coordinates &end) const {
    RouteFinder routeFinder(gis);
    auto startWayInfo = getEdgeWayInfo(start);
    auto startWayId = get<1>(startWayInfo);
    auto endWayInfo = getEdgeWayInfo(end);
    auto endWayId = get<1>(endWayInfo);
    if(get<0>(startWayInfo) == Coordinates(Longitude(404), Latitude(404))){
        Route invalidRoute = Route(std::vector<std::pair<EntityId, Direction>>(),start,
                                   end, Meters(0), Minutes(0));
        return make_unique<Routes>(Routes(false, invalidRoute, invalidRoute, "Can't find ways to build route"));
    }
    const AbstractWay& startWay = gis.getWay(startWayId);
    const AbstractWay& endWay = gis.getWay(endWayId);
    const auto startRouteCords = get<0>(startWayInfo);
    const auto endRouteCords = get<0>(endWayInfo);

    if(startWayId == endWayId){
        Route invalidRoute = Route(std::vector<std::pair<EntityId, Direction>>(),start,
                                   end, Meters(0), Minutes(0));
        return make_unique<Routes>(Routes(false, invalidRoute, invalidRoute, "Can't find route on the same way"));
    }

    const auto startSection = getEdgeSection(startRouteCords, get<2>(startWayInfo), endRouteCords, startWay, startWayId, true);
    const auto endSection = getEdgeSection(endRouteCords, get<2>(endWayInfo), startRouteCords, endWay, endWayId, false);
    const auto& endJunctionId = endSection.getJunctionId();

    EntityId startSearchJunction = (startSection.getWayDirection() == Direction::A_to_B)? startSection.getWay().getJunctions().second :
                                   startSection.getWay().getJunctions().first;


    Coordinates endJunctionCords = (endSection.getWayDirection() == Direction::A_to_B)? endSection.getWay().getFromJunctionCoordinates():
                                   endSection.getWay().getToJunctionCoordinates();

    const auto shortestWaySectionsOpt = routeFinder.findShortestRoute(endJunctionCords, startSearchJunction, endJunctionId);
    if(!shortestWaySectionsOpt){
        Route invalidRoute = Route(std::vector<std::pair<EntityId, Direction>>(),start,
        end, Meters(0), Minutes(0));
        return make_unique<Routes>(Routes(false, invalidRoute, invalidRoute, "No ways connecting start and end point"));
    }
    auto& shortestWaySections = shortestWaySectionsOpt.value();
    vector<Route> shortestRoutes;
    //adds the start and end sections to all the lightest routes
    if(shortestWaySections) {
        for (auto &sectionRoute: *shortestWaySections) {
            sectionRoute.insert(sectionRoute.cbegin(), make_shared<RouteSection>(startSection));
            sectionRoute.push_back(make_shared<RouteSection>(endSection));
            shortestRoutes.push_back(buildRouteFromSections(sectionRoute, startRouteCords, endRouteCords));
        }
    }
    else{
        vector<shared_ptr<RouteSection>> emptyRouteSections;
        emptyRouteSections.push_back(make_shared<RouteSection>(startSection));
        emptyRouteSections.push_back(make_shared<RouteSection>(endSection));
        shortestRoutes.push_back(buildRouteFromSections(emptyRouteSections,startRouteCords, endRouteCords));
    }
    auto fastestWaySectionsOpt = routeFinder.findFastestRoute(endJunctionCords, startSearchJunction, endJunctionId, maxSpeedLimit);
    auto& fastestWaySections = fastestWaySectionsOpt.value();
    vector<Route> fastestRoutes;
    //adds the start and end sections to all the lightest routes
    if(fastestWaySections) {
        for (auto &sectionRoute: *fastestWaySections) {
            sectionRoute.insert(sectionRoute.cbegin(), make_shared<RouteSection>(startSection));
            sectionRoute.push_back(make_shared<RouteSection>(endSection));
            fastestRoutes.push_back(buildRouteFromSections(sectionRoute, startRouteCords, endRouteCords));
        }
    }
    else{
        vector<shared_ptr<RouteSection>> emptyRouteSections;
        emptyRouteSections.push_back(make_shared<RouteSection>(startSection));
        emptyRouteSections.push_back(make_shared<RouteSection>(endSection));
        fastestRoutes.push_back(buildRouteFromSections(emptyRouteSections,startRouteCords, endRouteCords));
    }
    return make_unique<Routes>(Routes(true, getBestRoute(shortestRoutes, true),
                  getBestRoute(fastestRoutes, false)));
}

RouteSection Navigation_316497817::getEdgeSection(Coordinates routeCords, size_t segment, Coordinates  oppositeEdgeCoordinate, const AbstractWay& way, EntityId wayId, bool isStartSection) const{

    if (!way.isBidirectional()) {
        if (isStartSection) {
            Meters length = way.getSegmentPartsOnWay(segment, routeCords).second;
            return RouteSection(way.getJunctions().first, wayId, Direction::A_to_B, way,
                                length);
        } else {
            Meters length = way.getSegmentPartsOnWay(segment, routeCords).first;
            return RouteSection(way.getJunctions().first, wayId, Direction::A_to_B, way,
                                length);
        }
    } else {
        //Picks the junction base on distance from the opposite edge coord and if its equal than from the distance from the route cord
        pair<Meters, Meters> distancesFrom(
                CoordinatesMath::calculateDistance(oppositeEdgeCoordinate, way.getFromJunctionCoordinates()),
                way.getSegmentPartsOnWay(segment, routeCords).first);
        pair<Meters, Meters> distancesTo(
                CoordinatesMath::calculateDistance(oppositeEdgeCoordinate, way.getToJunctionCoordinates()),
                way.getSegmentPartsOnWay(segment, routeCords).second);
        Direction fromDir = Direction::B_to_A;
        Direction toDir = Direction::A_to_B;
        Meters legnthAtoB = way.getSegmentPartsOnWay(segment, routeCords).second;
        Meters legnthBtoA = way.getSegmentPartsOnWay(segment, routeCords).first;
        if (!isStartSection) {
            fromDir = Direction::A_to_B;
            toDir = Direction::B_to_A;
        }
        return distancesFrom < distancesTo ?
               RouteSection(way.getJunctions().first, wayId, fromDir, way, legnthBtoA) :
               RouteSection(way.getJunctions().second, wayId, toDir, way, legnthAtoB);
    }
}


Route Navigation_316497817::buildRouteFromSections(const vector<shared_ptr<RouteSection>>& sections, const Coordinates& startPoint, const Coordinates& endPoint) const{
    vector<pair<EntityId, Direction>> ways;
    Meters length(0);
    Minutes time(0);
    
    for(const auto& section : sections) {
        ways.push_back({section->getWayId(), section->getWayDirection()});
        auto sectionLength = section->getLength();
        length += sectionLength;
        time += section->getDuration();

    }

    return Route(ways, startPoint, endPoint, length, time);
}

Route Navigation_316497817::getBestRoute(vector<Route>& routes, bool isShortestDistanceRoute) const{
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

tuple<Coordinates, EntityId, std::size_t> Navigation_316497817::getEdgeWayInfo(const Coordinates &edge) const {
    auto wayInfo = gis.getWayClosestPoint(edge);

    auto wayId = get<1>(wayInfo);
    if(get<0>(wayInfo) == Coordinates(Longitude(404), Latitude(404))){
        return wayInfo;
    }
    const AbstractWay& way = gis.getWay(wayId);
    if(way.isHighway() && CoordinatesMath::calculateDistance(edge, get<0>(wayInfo)) > maxDistanceFromHighWay){
        wayInfo = gis.getWayClosestPoint(edge, Restrictions("highway"));
    }
    return wayInfo;
}



