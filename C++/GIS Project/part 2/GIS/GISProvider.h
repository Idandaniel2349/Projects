//
// Created by sagig on 1/22/2021.
//

#pragma once

#include "GIS_316497817.h"

class GISProvider {
    GISProvider() {}

public:
    static GIS_316497817 getGIS() {
        //You may change the implemntation below
        //(e.g. GIS may be a singelton)
        return GIS_316497817{};
    }
};