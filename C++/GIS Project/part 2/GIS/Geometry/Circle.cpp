//
// Created by idan on 27/11/2020.
//

#include "Geometry.h"

Coordinates Circle::closestPoint(const Coordinates& cord) const {
   return CoordinatesMath::closestPointOnCircle(cord,center,radius);
}

unordered_set<GridCoordinates> Circle::getCellsOfGeometry(double precision) {
    unordered_set<GridCoordinates> gridCords;
    Meters minDistance(0);
    bool minDistanceInit= false;
    SpiralCoordinatesTraverser traverser = SpiralCoordinatesTraverser(center, precision);
    gridCords.insert(traverser.getNext());

    // traverses the grid until all the square on the perimeter of the search area are further than than the radius
    while(minDistance<radius){
        minDistanceInit = false;
        do{
            GridCoordinates cell = traverser.getNext();

            Meters distance = GridMath::getMinDistance(cell, center, precision);
            if(!minDistanceInit || distance < minDistance){//happen only on first loop
                minDistanceInit = true;
                minDistance = distance;
            }
            if(distance <= radius){
                gridCords.insert(cell);
            }
        }while (!traverser.onNewPerimeter());
    }
    return gridCords;

}