//
// Created by sagig on 11/20/2020.
//
#include "Entity.h"

using std::pair;

// find and return the closest point on the entity perimeter to the given point (cord)
Coordinates Entity::closestPoint(const Coordinates& cord) const{
    Meters minDistance = Meters(-1);
    Coordinates closestPoint = cord;

    //go over all entity geometries and find the closest point(have more then one iteration when the entity is a
    // Way and then the geometries are lines)
    for(auto iter = geometries.begin(); iter < geometries.end(); ++iter)
    {
        Coordinates point =(*iter)->closestPoint(cord);
        auto distance = CoordinatesMath::calculateDistance(point, cord);
        if(minDistance == (Meters)-1 || (double)distance < (double)minDistance)
        {
            minDistance = distance;
            closestPoint = point;
        }
    }
    return closestPoint;
}


