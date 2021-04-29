//
// Created by sagig on 1/8/2021.
//
#pragma once

#include "../Common/GISNamedTypes.h"
#include "NavigationResult.h"

struct RouteSummary{
    Meters distance;
    Minutes estimatedTime;
    bool isValid;

    RouteSummary(): distance(0), estimatedTime(0), isValid(false){}

    RouteSummary(const Meters &distance, const Minutes &estimatedTime, bool isValid) : distance(distance),
                                                                                       estimatedTime(estimatedTime),
                                                                                       isValid(isValid) {}


    bool operator ==(const RouteSummary other) const{
        if(!other.isValid && !isValid){
            return true;
        }
        return distance == other.distance && estimatedTime == other.estimatedTime && isValid == other.isValid;
    }
};

namespace std {
    template<>
    struct hash<RouteSummary> {
        std::size_t operator()(const RouteSummary& t) const {
            if(!t.isValid){
                return std::hash<bool>{}(false);
            }
            std::size_t h1 = std::hash<double>{}(t.distance->roundToPrecision());
            std::size_t h2 = std::hash<double>{}(t.estimatedTime->roundToPrecision());
            return h1 ^ (h2 << 1);
        }
    };
}
