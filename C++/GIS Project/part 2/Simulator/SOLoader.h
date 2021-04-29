//
// Created by idan on 04/01/2021.
//
#pragma once

#include "string"
#include "../Common/AbstractGIS.h"
#include "../Common/AbstractNavigation.h"
#include "memory"
#include "unordered_map"
#include "vector"
#include "mutex"
#include "CachedGIS.h"

using std::string;
using std::unique_ptr;
using std::function;
using std::unordered_map;
using std::vector;
using std::mutex;

/**
 * This class is responsible for loading GIS and Navigation from so files
 */
class SOLoader {
    mutex lock;
    string gisMap;
    unordered_map<string, CachedGIS> gisCache;
    size_t maxUses;
    unique_ptr<AbstractGIS> absGis;
    function<unique_ptr<AbstractNavigation>(const NavigationGIS&)> navigationFactoryIns;
    vector<void *> navigationHandles;

    static SOLoader instance;
    SOLoader() = default;
public:
    static SOLoader& getInstance();

    /**
     * set the absGis
     * @param GISFactory - the factory generating the needed GIS
     */
    void setAbsGis(function<std::unique_ptr<AbstractGIS>()> &GISFactory);
    /**
     * set the class NavigationFactory
     * @param navigationFactory - the value to set
     */
    void setNavigationFactory(const function<unique_ptr<AbstractNavigation>(const NavigationGIS &)> &navigationFactory);
    /**
     * Load the GIS from directory path and mapFile
     * @param path - the directory path
     * @param mapFile - the map to load
     * @return - a vector of GIS names loaded
     */
    vector<string> loadGIS(const string& path, const string& mapFile);
    /**
     * Load navigations from directory path
     * @param path - the directory path
     * @return - an unordered+map from navigation names loaded to function (navigation factory)
     */
    unordered_map<string,function<unique_ptr<AbstractNavigation>(const NavigationGIS &)>> loadNavigation(string path);
    /**
     * Get the cached info of gisName( cache if doesn't exist)
     * @param gisName - name of the GIS
     * @return - smart pointer to the GIS
     */
    shared_ptr<AbstractGIS> getGIS(string gisName);
    /**
     * Release the cached info of gisName
     * @param gisName - name of the GIS
     * @return - whatever the cache was released
     */
    bool releaseGIS(string gisName);
    /**
     * clear all the navigation handles
     */
    void clearNavigationHandles();
};


