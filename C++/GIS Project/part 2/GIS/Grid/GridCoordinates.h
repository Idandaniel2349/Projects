//
// Created by sagig on 12/2/2020.
//
#pragma once


#include "../../Common/GISNamedTypes.h"

/**
 * A class representing Coordinates on the Grid (for easier calculations and use on the Grid)
 * the longitude and latitude are scaled so that a step of precision is now 1
 */
class GridCoordinates {
    //current longitude
    long longitude;
    //current latitude
    long latitude;
    //the most eastern longitude
    long longitudeEastLimit;
    //the most western longitude
    long longitudeWestLimit;
    //the northernmost latitude
    long latitudeNorthLimit;
    //the southernmost latitude
    long latitudeSouthLimit;
    //did you corss a pole while going up or down
    bool crossedPole;

public:
    GridCoordinates(long _longitude, long _latitude, double _precision);
    GridCoordinates(const Coordinates& cords, double _precision);

    /**
     * Those function move the GridCoordinates in the needed direction
     * Note: this is to handle better the moves on the earth sphere
     */
    void goEast();
    void goWest();
    /**
     * in general goes north but if crossed north pole starts going south till the south poll and then flips agaian
     */
    void goUp();
    /*
     * like the previous function but reversed
     */
    void goDown();
    /**
     *
     * @return longitude
     */
    long getLongitude() const;

    /**
     *
     * @return latitude
     */
    long getLatitude() const;

    bool operator==(const GridCoordinates other) const{
        return longitude == other.longitude && latitude == other.latitude;
    }
};

namespace std {
    template <>
    struct hash<GridCoordinates>{
        std::size_t operator()(const GridCoordinates& k) const{
            std::size_t h1 = std::hash<long>{}(k.getLongitude());
            std::size_t h2 = std::hash<long>{}(k.getLatitude());
            return h1 ^ (h2 << 1);
        }
    };

}

