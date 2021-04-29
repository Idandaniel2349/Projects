//
// Created by idan on 13/11/2020.
//

#pragma once

#include "../../Common/GISNamedTypes.h"
#include "../../Common/CoordinatesMath.h"
#include "vector"
#include "unordered_set"
#include "queue"
#include "../Grid/GridMath.h"
#include "../Grid/SpiralCoordinatesTraverser.h"

using std::vector;
using std::unordered_set;

/**
 * A Geometry of an Entity
 */
class Geometry {
public:
    /**
     * Calculate the closest point on the geometry perimeter and the given Coordinate
     * @param cord - the given Coordinate
     * @return the closest point on the geometry perimeter and the given Coordinate
     */
    virtual Coordinates closestPoint(const Coordinates& cord) const = 0;

    /**
     * Calculates the cells on the grid the geometry occupy and returns them
     * @param precision - the precision of the Grid
     * @return all GridCoordinates that the Geometry occupy
     */
    virtual unordered_set<GridCoordinates> getCellsOfGeometry(double precision)=0;
    virtual ~Geometry(){};
};


/**
 * Circle geometry for POI
 */
class Circle:public Geometry {
    Coordinates center;
    Meters radius;
public:
    Circle(const Coordinates &_center, const Meters &_radius) : center(_center), radius(_radius) {};
    Coordinates getCenter() {return center;};
    Meters getRadius() {return radius;};
    Coordinates closestPoint(const Coordinates& cord) const override;
    unordered_set<GridCoordinates> getCellsOfGeometry(double precision) override;
};

/**
 * Point geometry for Junction
 */
class Point:public Geometry {
    Coordinates coordinates;
public:
    Point(const Coordinates &_coordinates) :coordinates(_coordinates){};
    Coordinates getCoordinates() {return coordinates;};
    Coordinates closestPoint(const Coordinates& cord) const override;
    unordered_set<GridCoordinates> getCellsOfGeometry(double precision) override;
};

/**
 * Line geometry for Way(which will be multiple lines)
 */
class Line: public Geometry{
    Coordinates cord1;
    Coordinates cord2;
    void addMidpointRec(const Coordinates& lineEnd1, const Coordinates& lineEnd2, double precision, unordered_set<GridCoordinates>* cells);

public:
    Line(const Coordinates& _cord1, const Coordinates& _cord2): cord1(_cord1), cord2(_cord2){}
    Coordinates closestPoint(const Coordinates& cord) const override;
    unordered_set<GridCoordinates> getCellsOfGeometry(double precision) override;


};
