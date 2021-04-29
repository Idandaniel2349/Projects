//
// Created by sagig on 12/12/2020.
//
#pragma once

#include "vector"
#include "memory"
#include "../Common/GISNamedTypes.h"
#include "Geometry/Geometry.h"

using std::vector;
using std::shared_ptr;

struct BasicWayInfo{
    Coordinates toCords;
    Coordinates fromCords;
    vector<shared_ptr<Geometry>> geometries;
    vector<Coordinates> curves;

    BasicWayInfo(): toCords((Longitude)0,(Latitude)0), fromCords((Longitude)0,(Latitude)0){};
    BasicWayInfo(const Coordinates &toCords, const Coordinates &fromCords,
                 const vector<shared_ptr<Geometry>> &geometries, const vector<Coordinates> &curves) : toCords(toCords),
                                                                                                      fromCords(
                                                                                                              fromCords),
                                                                                                      geometries(
                                                                                                              geometries),
                                                                                                      curves(curves) {}
};
