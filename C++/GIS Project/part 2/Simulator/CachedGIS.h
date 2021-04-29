//
// Created by sagig on 1/4/2021.
//
#pragma once

#include "../Common/AbstractGIS.h"
#include "memory"

using std::shared_ptr;
using std::string;

/**
 * CachedGIS responsible to cache the GIS
 */
struct CachedGIS{
    shared_ptr<AbstractGIS> gis;
    void* handle{};
    size_t releases{};
    string path;

    CachedGIS(const string &path) : path(path) {}

    CachedGIS()= default;
};
