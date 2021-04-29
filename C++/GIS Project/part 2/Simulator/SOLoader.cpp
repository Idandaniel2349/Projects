//
// Created by idan on 04/01/2021.
//

#include "SOLoader.h"
#include "SimulatorExceptions.h"
#include <cstdlib>
#include <dlfcn.h>
#include <string>
#include <filesystem>
#include <iostream>

using std::lock_guard;
namespace fs = std::filesystem;

SOLoader SOLoader::instance;

SOLoader& SOLoader::getInstance() {;
    return instance;
}

void SOLoader::setAbsGis(function<std::unique_ptr<AbstractGIS>()> &GISFactory) {
    absGis = GISFactory();
}

void SOLoader::setNavigationFactory(
        const function<unique_ptr<AbstractNavigation>(const NavigationGIS &)> &navigationFactory) {
    navigationFactoryIns = navigationFactory;
}

vector<string> SOLoader::loadGIS(const string& path, const string& mapFile) {
    vector<string> result;
    gisMap = mapFile;
    for (const auto & entry : fs::directory_iterator(path)){// for every file in the directory
        string fullname = entry.path().filename();
        size_t lastindex = fullname.find_last_of(".");
        string rawName = fullname.substr(0, lastindex);
        result.push_back(rawName);
        gisCache[rawName] = CachedGIS(entry.path().c_str());
    }

    return result;
}

unordered_map<string,function<unique_ptr<AbstractNavigation>(const NavigationGIS &)>> SOLoader::loadNavigation(string path) {
    unordered_map<string,function<unique_ptr<AbstractNavigation>(const NavigationGIS &)>> result;
    for (const auto & entry : fs::directory_iterator(path)){//for every file in the directory
        void* navigationHandle = dlopen(entry.path().c_str(), RTLD_LAZY);
        if (navigationHandle) {
            string fullname = entry.path().filename();
            size_t lastindex = fullname.find_last_of(".");
            string rawName = fullname.substr(0, lastindex);
            result[rawName] = navigationFactoryIns;
            navigationHandles.push_back(navigationHandle);
            /*
             * code for replacing the navigation factory with a function that doesn't use an SO so code wont
             * Seg fault when the destructor of this obj is called
             */
            navigationFactoryIns = [](NavigationGIS a){
                int c =3;
                int b =a.getUsageCounter();
                b = c;
                c=b;
                return nullptr;};
        } else {
            string msg = "failed to open so " + (string)entry.path().c_str();
            throw SOLoadFailedException(msg);
        }
        ++maxUses;
    }
    return result;
}

shared_ptr<AbstractGIS> SOLoader::getGIS(string gisName) {
    lock_guard<std::mutex> guard(lock);
    CachedGIS &cachedEntry = gisCache[gisName];
    if (cachedEntry.handle) {// if the handle is set
        return cachedEntry.gis;
    }
    cachedEntry.handle = dlopen(cachedEntry.path.c_str(), RTLD_LAZY);
    if (cachedEntry.handle) {// if opened load the map
        cachedEntry.gis = std::move(absGis);

        cachedEntry.gis->loadMapFile(gisMap);
    } else {
        string msg = "failed to open so " + (string)cachedEntry.path.c_str();
        throw SOLoadFailedException(msg);
    }
    return cachedEntry.gis;
}

bool SOLoader::releaseGIS(string gisName){
    lock_guard<std::mutex> guard(lock);
    CachedGIS &cachedEntry = gisCache[gisName];

    cachedEntry.releases++;
    if(cachedEntry.releases == maxUses){
        gisCache.erase(gisName);

        dlclose(cachedEntry.handle);
        return true;
    }
    return false;
}

void SOLoader::clearNavigationHandles() {
    for(const auto & handle: navigationHandles){
        dlclose(handle);
    }
    navigationHandles.clear();
}
