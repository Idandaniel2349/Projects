//
// Created by sagig on 1/8/2021.
//

#pragma once

#include "string"
#include "../Common/GISNamedTypes.h"

using std::string;
using std::pair;

struct RouteResult{
    string gisName;
    string navigationName;
    Meters length;
    Minutes duration;
    bool isShortestDistance;
    pair<Coordinates,Coordinates> request;
    size_t gisCount;
    bool isValid;

    RouteResult(const string &gisName, const string &navigationName, const Meters &length, const Minutes &duration,
                bool isShortestDistance, const pair<Coordinates, Coordinates> &request, size_t gisCount, bool isValid) : gisName(gisName),
                                                                                            navigationName(
                                                                                                    navigationName),
                                                                                            length(length),
                                                                                            duration(duration),
                                                                                            isShortestDistance(
                                                                                                    isShortestDistance),
                                                                                            request(request), gisCount(gisCount),
                                                                                            isValid(isValid){}

    RouteResult(): length(0), duration(0), request({Coordinates(Longitude(0), Latitude(0)), Coordinates(Longitude(0), Latitude(0))}){}
};
