//
// Created by sagig on 1/4/2021.
//
#pragma once
#include "string"
#include "unordered_map"
#include "../Common/GISNamedTypes.h"
#include "NavigationResult.h"
#include "RouteSummary.h"
#include "RouteResult.h"
#include "optional"
#include "mutex"

using std::string;
using std::unordered_map;
using std::pair;
using std::vector;
using std::optional;
using std::mutex;

/**
 * ConsensusCalculator Class responsible to calculate the consensus
 */
class ConsensusCalculator {
    string navigationName;
    mutex lock;
    unordered_map<int, unordered_map<RouteSummary, vector<NavigationResult>>> resultsDistance;
    unordered_map<int, unordered_map<RouteSummary, vector<NavigationResult>>> resultsTime;
    unordered_map<int, optional<RouteResult>> consensusDistance;
    unordered_map<int, optional<RouteResult>> consensusTime;
    vector<RouteResult> strangeResults;

    /**
     * calculate the needed consensus ( for distance or time according to isShortestDistance)
     * @param results - resultsDistance or resultsTime according to isShortestDistance
     * @param consensus - consensusDistance or consensusTime according to isShortestDistance
     * @param isShortestDistance - are we calculating distance or time
     */
    void calculateConsensusForCategory(unordered_map<int, unordered_map<RouteSummary, vector<NavigationResult>>>& results, unordered_map<int, optional<RouteResult>>& consensus, bool isShortestDistance);
    /**
     * put all results of the unordered_map<RouteSummary, vector<NavigationResult>> in key i of results in strangeResults
     * @param isShortestDistance - are the results for shortest distance(or for time)
     * @param results - an int to unordered_map<RouteSummary, vector<NavigationResult>>
     * @param i - the index of the needed group in results
     */
    void putAllresultsInStrange(bool isShortestDistance,
                                unordered_map<int, unordered_map<RouteSummary, vector<NavigationResult>>> &results,
                                size_t &i);

public:
    ConsensusCalculator(const string &navigationName, int numOfRequests);

    /**
     * add to resultsDistance in key request and resultsTime the results from res
     * @param request - the key to enter the information in
     * @param res - the result to enter
     */
    void addNavigationRoute(int request, NavigationResult res);

    /**
     * calculate consensus for distance and time
     */
    void calculateConsensus();

    const string &getNavigationName() const;

    unordered_map<int, optional<RouteResult>> moveConsensusDistance();

    unordered_map<int, optional<RouteResult>> moveConsensusTime();

    const vector<RouteResult> &getStrangeResults() const;

};

