#pragma once

#include "../../Common/CoordinatesMath.h"
#include "../../Common/GISNamedTypes.h"
#include "algorithm"
#include "../Geometry/Geometry.h"
#include "GridCoordinates.h"

/**
 * A class taking care of math related to the Grid
 */
class GridMath {
public:
    /**
    * get the minimum distance between a point on the cell to the coordinate
    * @param cellCoordinate the coordinate that defines the cell
    * @param coordinate coordinate to get the distance from
    * @param precision the precision that defines the cell size
    * @return the minimum distance between the cell to the coordinate
    */
    static Meters getMinDistance(GridCoordinates cellCoordinate, const Coordinates& coordinate, double precision );


};



