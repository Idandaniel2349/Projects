//
// Created by sagig on 12/20/2020.
//
#pragma once


#include <utility>

#include "../GISNamedTypes.h"
#include "../MapEntities/Way.h"

/**
 *  RouteSection class to represent a section of a Route defined by a way and an origin junction
 *
 */
class RouteSection{
    EntityId junctionId;
    EntityId wayId;
    Direction wayDirection;
    Way way;
    Meters length;
    Minutes duration;

    //is the section just the junction with no way leading to it (can only be the start or end sections)
    bool isJunctionSection;

public:
    RouteSection(EntityId  _junctionId, EntityId  _wayId, Direction _wayDirection, Way  _way): junctionId(std::move(_junctionId)),
    wayId(std::move(_wayId)), wayDirection(_wayDirection), way(std::move(_way)), length(way.getLength()), duration(Minutes(length / way.getSpeedLimit())*60),
    isJunctionSection(false){}


    RouteSection(EntityId  _junctionId, EntityId  _wayId, Direction _wayDirection, Way  _way, Meters _length): junctionId(std::move(_junctionId)),
    wayId(std::move(_wayId)), wayDirection(_wayDirection), way(std::move(_way)), length(_length), duration(Minutes(length / way.getSpeedLimit())*60),
    isJunctionSection(false){}

    RouteSection(EntityId  _junctionId, EntityId  _wayId, Direction _wayDirection, Way  _way, bool _isJunctionSection): junctionId(std::move(_junctionId)),
    wayId(std::move(_wayId)), wayDirection(_wayDirection), way(std::move(_way)), length(way.getLength()), duration(Minutes(length / way.getSpeedLimit())*60),
    isJunctionSection(_isJunctionSection){}

    const EntityId &getJunctionId() const {
        return junctionId;
    }

    Direction getWayDirection() const {
        return wayDirection;
    }

    const Way &getWay() const {
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

    bool getIsJunctionSection() const {
        return isJunctionSection;
    }
};
