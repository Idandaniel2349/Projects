//
// Created by sagig on 11/25/2020.
//

#include "SpiralCoordinatesTraverser.h"
#include "GridMath.h"

SpiralCoordinatesTraverser::SpiralCoordinatesTraverser(const Coordinates& startingPoint, double _precision) : precision(_precision),
 currCord(startingPoint, _precision) {
    squaresInPerimeter = 0;
    squaresTraversedInPerimeter = 0;
}

GridCoordinates SpiralCoordinatesTraverser::getNext() {
    GridCoordinates cordsToReturn = currCord;
    // If done with the current square initializes the new cell to be that starting point of the square
    if(squaresTraversedInPerimeter == squaresInPerimeter){
        squaresInPerimeter += 8;
        squaresTraversedInPerimeter = 0;
        currCord.goUp();
        currCord.goWest();

        return cordsToReturn;
    }
    // check in which part of the traversal you are currently to know where to turn
    int turnIndex = floor(squaresTraversedInPerimeter /(squaresInPerimeter / 4));

    if(turnIndex == 0){
        currCord.goEast();
    }
    else if(turnIndex == 1){
        currCord.goDown();
    }
    else if(turnIndex == 2){
        currCord.goWest();
    }else{
        currCord.goUp();
    }
    ++squaresTraversedInPerimeter;

    return cordsToReturn;
}

bool SpiralCoordinatesTraverser::onNewPerimeter() {
    return squaresTraversedInPerimeter == 0;
}