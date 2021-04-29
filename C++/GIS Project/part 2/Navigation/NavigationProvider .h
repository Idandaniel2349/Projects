//
// Created by sagig on 1/22/2021.
//
#pragma once

#include "Navigation_316497817.h"

class NavigationProvider {
public:
    static auto getNavigation(const NavigationGIS& navigation_gis) {
        return Navigation_316497817{navigation_gis};
    }
};