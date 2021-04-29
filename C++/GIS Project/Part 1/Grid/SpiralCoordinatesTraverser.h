//
// Created by sagig on 11/25/2020.
//
#pragma once

#include "../GISNamedTypes.h"
#include "array"
#include "GridCoordinates.h"

using std::array;
/**
 * A class for traversing a grid using a square spiral. ie it return the origin then the 8 squares that encompass it
 * in an anti clockwise order starting from the top right, the 16 that encompass them etc...
 */
class SpiralCoordinatesTraverser {
    double precision;
    GridCoordinates currCord;
    int squaresInPerimeter;
    int squaresTraversedInPerimeter;

public:
    /**
     * initializes a traverser
     * @param startingPoint origin point
     * @param _precision defines the edge size of the cells
     */
    SpiralCoordinatesTraverser(const Coordinates& startingPoint, double _precision);

    ~SpiralCoordinatesTraverser() = default;

    /**
     * get;s the next cell according to the order defined in the class
     * @return the next cell
     */
    GridCoordinates getNext();

    /**
     *
     * @return if the traverser is on the beginning of a new encompassing squre
     */
    bool onNewPerimeter();
};



