//
// Created by idan on 04/01/2021.
//

#include "NavigationRunner.h"
#include "SOLoader.h"
#include "NavigationValidator.h"
#include "iostream"

void NavigationRunner::operator()() {
    int reqNum = 0;
    {
        shared_ptr<AbstractGIS> gis = SOLoader::getInstance().getGIS(gisName);
        for (const auto &request:requests) {
            NavigationValidator validator(*gis);
            NavigationGIS navigationGis(*gis);
            unique_ptr<AbstractNavigation> navigation = navigationFactory(navigationGis);
            shared_ptr<AbstractRoutes> routes = navigation->getRoutes(request.first, request.second);
            bool isRoutesValid = routes->isValid();
            consensusCalculator->addNavigationRoute(reqNum,
                                                    NavigationResult(gisName, routes, navigationGis.getUsageCounter(),
                                                                     request,
                                                                     isRoutesValid ? validator.validateRoute(
                                                                             request.first, request.second,
                                                                             routes->shortestDistance()) : false,
                                                                     isRoutesValid ? validator.validateRoute(
                                                                             request.first, request.second,
                                                                             routes->shortestTime()) : false));
            ++reqNum;
        }
    }
    SOLoader::getInstance().releaseGIS(gisName);
}