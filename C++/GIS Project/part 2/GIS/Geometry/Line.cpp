//
// Created by idan on 27/11/2020.
//

#include "Geometry.h"

using std::min;
using std::max;

Coordinates Line::closestPoint(const Coordinates& cord) const {
  return CoordinatesMath::closestPointOnSegment(cord,cord1,cord2);
}

void Line::addMidpointRec(const Coordinates& lineEnd1, const Coordinates& lineEnd2, double precision, unordered_set<GridCoordinates>* cells){
    //convert Coordinates to GridCoordinates
    GridCoordinates cell1 = GridCoordinates(lineEnd1, precision);
    GridCoordinates cell2 = GridCoordinates(lineEnd2, precision);

    //get the midpoint on the line and enter it the cells
    Coordinates midpoint =CoordinatesMath::midpoint(lineEnd1, lineEnd2);
    GridCoordinates midCell = GridCoordinates(midpoint, precision);
    cells->insert(midCell);

    //recursion will stop when the middle is one of the current line ends
    if(midCell != cell1 && midCell != cell2){
        addMidpointRec(lineEnd1, midpoint, precision, cells);
        addMidpointRec(midpoint, lineEnd2, precision, cells);
    }
}

unordered_set<GridCoordinates> Line::getCellsOfGeometry(double precision)  {
    auto gridCords = unordered_set<GridCoordinates>();
    GridCoordinates cell1 = GridCoordinates(cord1, precision);
    GridCoordinates cell2 = GridCoordinates(cord2, precision);

    gridCords.insert(cell1);
    gridCords.insert(cell2);

    //run the recursion to get all GridCoordinates the line occupy
    addMidpointRec(cord1, cord2, precision, &gridCords);
    return gridCords;
}

