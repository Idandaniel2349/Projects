//
// Created by idan on 10/12/2020.
//

#include "NavigationValidator.h"

using std::find;
const Meters NavigationValidator::maxDistanceFromHighWay(3);

bool NavigationValidator::validateRoute(const Coordinates& start, const Coordinates& end, const Route& r) const{
    pair<Coordinates, EntityId> startWayInfo = getEdgeWayInfo(start);
    pair<Coordinates, EntityId> endWayInfo = getEdgeWayInfo(end);
    const auto startRouteCords = startWayInfo.first;
    const auto endRouteCords = endWayInfo.first;

    if(r.getWayStartPoint() != startRouteCords || r.getWayEndPoint() != endRouteCords){
        return false;
    }

    const auto& routeWays = r.getWays();
    Minutes totalTime = Minutes(0);
    Meters totalLength = Meters(0);
    for(size_t i = 0; i < routeWays.size(); ++i) {
        const Way &way = gis.getWay(routeWays[i].first);
        Direction direction = routeWays[i].second;
        if (direction == Direction::B_to_A && !way.isBidirectional()) {
            return false;
        }
        if (i != routeWays.size() - 1) {
            EntityId nextJunction = (direction == Direction::A_to_B) ? way.getJunctions().second
                                                                     : way.getJunctions().first;
            const auto &waysOfNextJunction = gis.getWaysByJunction(nextJunction);
            if (find(waysOfNextJunction.begin(), waysOfNextJunction.end(), routeWays[i + 1].first) ==
                waysOfNextJunction.end()) {
                return false;
            }
            if (i == 0) {
                auto length = calculateEdgePointDistance(startRouteCords, direction, way, true);
                totalLength += length;
                totalTime += Minutes(length / way.getSpeedLimit())*60;
            }
            else{
                auto length = way.getLength();
                totalLength+=length;
                totalTime += Minutes(length/way.getSpeedLimit())*60;
            }
        }
        else{
            auto length = calculateEdgePointDistance(endRouteCords, direction, way, false);
            totalLength += length;
            totalTime += Minutes(length / way.getSpeedLimit())*60;
        }
    }

    if(totalTime != r.estimatedDuration() || totalLength != r.totalLength()){
        return false;
    }

    return true;
}

pair<Coordinates, EntityId> NavigationValidator::getEdgeWayInfo(const Coordinates &edge) const {
    auto wayInfo = gis.getWayClosestPoint(edge);

    auto wayId = wayInfo.second;
    auto way = gis.getWay(wayId);
    if(way.isHighway() && CoordinatesMath::calculateDistance(edge, wayInfo.first) > maxDistanceFromHighWay){
        wayInfo = gis.getWayClosestPoint(edge, Restrictions("highway"));
    }
    return wayInfo;
}


Meters NavigationValidator::calculateEdgePointDistance(const Coordinates& edgePoint, Direction direction, const Way& way, bool startingPoint) const{
    Meters length = Meters(0);
    if((direction == Direction::A_to_B && startingPoint) || (direction == Direction::B_to_A && !startingPoint)){
        if(edgePoint == way.getFromJunctionCoordinates()){
            length = way.getLength();
        }
        else if (edgePoint != way.getToJunctionCoordinates()){
            length = way.getLength() - CoordinatesMath::calculateDistance(edgePoint, way.getFromJunctionCoordinates());
        }

    }
    else{
        if(edgePoint == way.getToJunctionCoordinates()){
            length = way.getLength();
        }
        else if (edgePoint != way.getFromJunctionCoordinates()){
            length = way.getLength() - CoordinatesMath::calculateDistance(edgePoint, way.getToJunctionCoordinates());
        }
    }

    return length;
}

