//
// Created by idan on 27/11/2020.
//

#include "Geometry.h"

Coordinates Point::closestPoint(const Coordinates& cord) const {
    //here just so the project will compile
    auto temp = cord;
    auto temp1 =cord;
    temp = temp1;
    ///////////////////////////////////////
    return coordinates;
}

unordered_set<GridCoordinates> Point::getCellsOfGeometry(double precision) {
    unordered_set<GridCoordinates> gridCords;
    gridCords.insert(GridCoordinates(coordinates, precision));
    return gridCords;
}