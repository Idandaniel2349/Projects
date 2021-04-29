//
// Created by idan on 10/12/2020.
//

#include "NavigationValidator.h"
#include "iostream"

using std::cout;
using std::endl;

using std::get;
using std::find;
const Meters NavigationValidator::maxDistanceFromHighWay(3);

bool NavigationValidator::validateRoute(const Coordinates& start, const Coordinates& end, const AbstractRoute& r) const{
    constexpr double km_h_to_minutes = 0.060;
    auto startWayInfo = getEdgeWayInfo(start);
    auto endWayInfo = getEdgeWayInfo(end);
    const auto startRouteCords = get<0>(startWayInfo);
    const auto endRouteCords = get<0>(endWayInfo);

    if(r.getWayStartPoint() != startRouteCords || r.getWayEndPoint() != endRouteCords){
        return false;
    }
    if(r.getWays().empty() || r.getWays()[0].first != get<1>(startWayInfo) || r.getWays()[r.getWays().size() - 1].first != get<1>(endWayInfo)){
        return false;
    }

    const auto& routeWays = r.getWays();
    Minutes totalTime = Minutes(0);
    Meters totalLength = Meters(0);
    for(size_t i = 0; i < routeWays.size(); ++i) {
        const AbstractWay &way = gis.getWay(routeWays[i].first);
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
                auto length = calculateEdgePointDistance(startRouteCords, get<2>(startWayInfo), direction, way, true);
                totalLength += length;
                totalTime += Minutes(length / way.getSpeedLimit())*km_h_to_minutes;
            }
            else{
                auto length = way.getLength();
                totalLength+=length;
                totalTime += Minutes(length/way.getSpeedLimit())*km_h_to_minutes;
            }
        }
        else{
            auto length = calculateEdgePointDistance(endRouteCords, get<2>(endWayInfo), direction, way, false);
            totalLength += length;
            totalTime += Minutes(length / way.getSpeedLimit())*km_h_to_minutes;
        }
    }

    if(totalTime != r.estimatedDuration() || totalLength != r.totalLength()){
        return false;
    }

    return true;
}

std::tuple<Coordinates, EntityId, std::size_t> NavigationValidator::getEdgeWayInfo(const Coordinates &edge) const {
    auto wayInfo = gis.getWayClosestPoint(edge);

    auto wayId = get<1>(wayInfo);
    const AbstractWay& way = gis.getWay(wayId);
    if(way.isHighway() && CoordinatesMath::calculateDistance(edge, get<0>(wayInfo)) > maxDistanceFromHighWay){
        wayInfo = gis.getWayClosestPoint(edge, Restrictions("highway"));
    }
    return wayInfo;
}


Meters NavigationValidator::calculateEdgePointDistance(const Coordinates& edgePoint, size_t segment, Direction direction, const AbstractWay& way, bool startingPoint) const{
    Meters length = Meters(0);
    if((direction == Direction::A_to_B && startingPoint) || (direction == Direction::B_to_A && !startingPoint)){
        if(edgePoint == way.getFromJunctionCoordinates()){
            length = way.getLength();
        }
        else if (edgePoint != way.getToJunctionCoordinates()){
            length = way.getSegmentPartsOnWay(segment, edgePoint).second;
        }

    }
    else{
        if(edgePoint == way.getToJunctionCoordinates()){
            length = way.getLength();
        }
        else if (edgePoint != way.getFromJunctionCoordinates()){
            length = way.getSegmentPartsOnWay(segment, edgePoint).first;
        }
    }

    return length;
}

