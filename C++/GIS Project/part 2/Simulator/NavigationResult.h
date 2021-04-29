//
// Created by idan on 04/01/2021.
//

#pragma once

#include "string"
#include "../Common/AbstractRoutes.h"
#include "memory"

using std::string;
using std::shared_ptr;
using std::pair;

/**
 * NavigationResult struct to save navigation results
 */
struct NavigationResult{
    string gisName;
    shared_ptr<AbstractRoutes> routes;
    size_t count;
    pair<Coordinates,Coordinates> request;
    bool shortestDistanceValid;
    bool shortestTimeValid;

    NavigationResult(const string &gisName, shared_ptr<AbstractRoutes> &routes,
                     size_t count, const pair<Coordinates, Coordinates> &request,
                     bool shortestDistanceValid, bool shortestTimeValid) : gisName(gisName),routes(std::move(routes)), count(count),
                                                                                    request(request), shortestDistanceValid(shortestDistanceValid),
                                                                                    shortestTimeValid(shortestTimeValid){}

    NavigationResult(): request({Coordinates(Longitude(0), Latitude(0)), Coordinates(Longitude(0), Latitude(0))}){}


};
