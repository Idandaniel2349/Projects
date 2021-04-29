//
// Created by idan on 04/01/2021.
//

#pragma once

#include "memory"
#include "../Common/AbstractGIS.h"
#include "../Common/AbstractNavigation.h"
#include "tuple"
#include "string"
#include "NavigationResult.h"
#include "ConsensusCalculator.h"

using std::unique_ptr;
using std::function;
using std::make_unique;
using std::tuple;
using std::string ;
using std::pair;


class NavigationRunner {
    function<unique_ptr<AbstractNavigation>(const NavigationGIS &)> navigationFactory;
    string gisName;
    vector<pair<Coordinates,Coordinates>> requests;
    shared_ptr<ConsensusCalculator> consensusCalculator;
    string navName;
public:
    NavigationRunner(const function<unique_ptr<AbstractNavigation>(const NavigationGIS &)> &navigationFactory,const string &gisName,
                     const vector<pair<Coordinates,Coordinates>>& requests, shared_ptr<ConsensusCalculator> consensusCalculator,const string &navName) :
                     navigationFactory(navigationFactory),
                                                     gisName(gisName), requests(requests), consensusCalculator(consensusCalculator), navName(navName){}

    void operator () ();

};
