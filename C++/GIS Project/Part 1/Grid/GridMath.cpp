//
// Created by idan on 24/11/2020.
//

#include "GridMath.h"

Meters GridMath::getMinDistance(GridCoordinates cellCoordinate, const Coordinates& coordinate, double precision) {
    //get the four vertices of the GridCell of cellCoordinate
    GridCoordinates vert1 = cellCoordinate;
    GridCoordinates vert2 = GridCoordinates(cellCoordinate.getLongitude(), cellCoordinate.getLatitude() + 1, precision);
    cellCoordinate.goWest();
    GridCoordinates vert3 = cellCoordinate;
    GridCoordinates vert4 = GridCoordinates(cellCoordinate.getLongitude(), cellCoordinate.getLatitude() + 1, precision);

    //convert the GridCoordinates to Coordinates
    Coordinates cord1 = Coordinates(Longitude(vert1.getLongitude()*precision), Latitude(vert1.getLatitude()*precision));
    Coordinates cord2 = Coordinates(Longitude(vert2.getLongitude()*precision), Latitude(vert2.getLatitude()*precision));
    Coordinates cord3 = Coordinates(Longitude(vert3.getLongitude()*precision), Latitude(vert3.getLatitude()*precision));
    Coordinates cord4 = Coordinates(Longitude(vert4.getLongitude()*precision), Latitude(vert4.getLatitude()*precision));

    //gets GridCell lines
    Line line1 = Line(cord1,cord2);
    Line line2 = Line(cord1,cord3);
    Line line3 = Line(cord3,cord4);
    Line line4 = Line(cord2,cord4);

    //find the closest point on each cell line to coordinate
    Coordinates closestPoint1=line1.closestPoint(coordinate);
    Coordinates closestPoint2=line2.closestPoint(coordinate);
    Coordinates closestPoint3=line3.closestPoint(coordinate);
    Coordinates closestPoint4=line4.closestPoint(coordinate);

    //get the minimum distance from coordinate to cell lines
    Meters distance1 = CoordinatesMath::calculateDistance(coordinate,closestPoint1);
    Meters distance2 = CoordinatesMath::calculateDistance(coordinate,closestPoint2);
    Meters distance3 = CoordinatesMath::calculateDistance(coordinate,closestPoint3);
    Meters distance4 = CoordinatesMath::calculateDistance(coordinate,closestPoint4);

    //return the minimum distance between the cell to the coordinate
    Meters minDistance=(Meters)std::min(std::min(std::min(distance1,distance2),distance3),distance4);

    return minDistance;
}
