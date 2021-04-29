//
// Created by sagig on 12/2/2020.
//

#include "GridCoordinates.h"

GridCoordinates::GridCoordinates(long _longitude, long _latitude, double _precision): longitude(_longitude), latitude(_latitude){
    crossedPole = false;
    double magnitude = 1/_precision;
    longitudeEastLimit = round(-180 *magnitude);
    longitudeWestLimit = round(180 *magnitude);
    latitudeNorthLimit = round(-90 *magnitude);
    latitudeSouthLimit = round(90 *magnitude);
    if(longitude == longitudeEastLimit){
        longitude = longitudeWestLimit;
    }
    if(latitude == latitudeSouthLimit){
        latitude = latitudeSouthLimit - 1;
    }
}

GridCoordinates::GridCoordinates(const Coordinates& cords, double _precision){
    crossedPole = false;
    longitude = floor(((double)(cords.longitude())) / _precision);
    latitude = floor(((double)(cords.latitude())) / _precision);
    double magnitude = 1/_precision;
    longitudeEastLimit = round(-180 *magnitude);
    longitudeWestLimit = round(180 *magnitude);
    latitudeNorthLimit = round(-90 *magnitude);
    latitudeSouthLimit = round(90 *magnitude);
    if(longitude == longitudeEastLimit){
        longitude = longitudeWestLimit;
    }
    if(latitude == latitudeSouthLimit){
        latitude = latitudeSouthLimit - 1;
    }
}

void GridCoordinates::goEast(){
    longitude--;
    if(longitude == longitudeEastLimit){
        longitude = longitudeWestLimit;
    }
}
void GridCoordinates::goWest(){
    longitude++;
    if(longitude == longitudeWestLimit + 1){
        longitude = longitudeEastLimit + 1;
    }
}

void GridCoordinates::goUp(){
    if(!crossedPole) {
        latitude--;
        if (latitude == latitudeNorthLimit - 1) {
            crossedPole = true;
            latitude = latitudeNorthLimit;
            if (longitude > 0){
                longitude = longitude - longitudeWestLimit;
            }
            else{
                longitude = longitude + longitudeWestLimit;
            }
        }
    }
    else{
        latitude++;
        if(latitude == latitudeSouthLimit){
            crossedPole = false;
            latitude = latitudeSouthLimit - 1;
            if (longitude > 0){
                longitude = longitude - longitudeWestLimit;
            }
            else{
                longitude = longitude + longitudeWestLimit;
            }

        }
    }
}

void GridCoordinates::goDown(){
    if(!crossedPole) {
        latitude++;
        if(latitude == latitudeSouthLimit){
            crossedPole = true;
            latitude = latitudeSouthLimit - 1;
            if (longitude > 0){
                longitude = longitude - longitudeWestLimit;
            }
            else{
                longitude = longitude + longitudeWestLimit;
            }

        }
    }
    else{
        latitude--;
        if (latitude == latitudeNorthLimit - 1) {
            crossedPole = false;
            latitude = latitudeNorthLimit;
            if (longitude > 0){
                longitude = longitude - longitudeWestLimit;
            }
            else{
                longitude = longitude + longitudeWestLimit;
            }
        }
    }
}

long GridCoordinates::getLongitude() const {
    return longitude;
}

long GridCoordinates::getLatitude() const {
    return latitude;
}
