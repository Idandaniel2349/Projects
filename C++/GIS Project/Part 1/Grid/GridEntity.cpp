//
// Created by sagig on 11/25/2020.
//

#include "GridEntity.h"

bool GridEntity::operator==(const GridEntity &other) const{
    return entityId == other.entityId && gridId == other.getGridId();
}

Meters GridEntity::getDistanceFrom(const Coordinates& cord) const {
    Coordinates closestPoint = geometry->closestPoint(cord);
    return CoordinatesMath::calculateDistance(closestPoint, cord);
}

Coordinates GridEntity::getClosestPont(const Coordinates& cord) const{
    return geometry->closestPoint(cord);
}
