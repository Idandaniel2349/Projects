//
// Created by sagig on 12/20/2020.
//
#pragma once


#include <utility>

#include "../Common/AbstractWay.h"
#include "../Common/GISNamedTypes.h"

/**
 *  RouteSection class to represent a section of a Route defined by a way and an origin junction
 *
 */
class RouteSection{
    static constexpr double km_h_to_minutes = 0.060;
    EntityId junctionId;
    EntityId wayId;
    Direction wayDirection;
    const AbstractWay& way;
    Meters length;
    Minutes duration;


public:
    RouteSection(EntityId  _junctionId, EntityId  _wayId, Direction _wayDirection, const AbstractWay&  _way): junctionId(std::move(_junctionId)),
    wayId(std::move(_wayId)), wayDirection(_wayDirection), way(std::move(_way)), length(way.getLength()), duration(Minutes(length / way.getSpeedLimit())*km_h_to_minutes){}


    RouteSection(EntityId  _junctionId, EntityId  _wayId, Direction _wayDirection, const AbstractWay&  _way, Meters _length): junctionId(std::move(_junctionId)),
    wayId(std::move(_wayId)), wayDirection(_wayDirection), way(std::move(_way)), length(_length), duration(Minutes(length / way.getSpeedLimit())*km_h_to_minutes){}


    const EntityId &getJunctionId() const {
        return junctionId;
    }

    Direction getWayDirection() const {
        return wayDirection;
    }

    const AbstractWay &getWay() const {
        return way;
    }

    const Meters &getLength() const {
        return length;
    }

    const EntityId &getWayId() const {
        return wayId;
    }

    const Minutes &getDuration() const {
        return duration;
    }
};
