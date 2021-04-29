//
// Created by sagig on 1/4/2021.
//
#include "unordered_set"
#include "ConsensusCalculator.h"


using std::unordered_set;
using std::lock_guard;

ConsensusCalculator::ConsensusCalculator(const string &navigationName, int numOfRequests) : navigationName(navigationName) {
    for (int i = 0; i < numOfRequests; ++i) {
        resultsDistance[i] = {};
        resultsTime[i] = {};
    }
}

void ConsensusCalculator::addNavigationRoute(int request, NavigationResult res){
    lock_guard<mutex> guard(lock);
    //get the RouteSummary for shortest distance and time from res
    RouteSummary shortestDistanceRes = RouteSummary(res.routes->shortestDistance().totalLength(), res.routes->shortestDistance().estimatedDuration(), res.shortestDistanceValid);
    RouteSummary shortestTimeRes = RouteSummary(res.routes->shortestTime().totalLength(), res.routes->shortestTime().estimatedDuration(), res.shortestTimeValid);

    //add res to resultsDistance and resultsTime
    if(resultsDistance[request].contains(shortestDistanceRes)){
        resultsDistance[request][shortestDistanceRes].push_back(res);
    }
    else{
        resultsDistance[request][shortestDistanceRes] = {res};
    }

    if(resultsTime[request].contains(shortestTimeRes)){
        resultsTime[request][shortestTimeRes].push_back(res);
    }
    else{
        resultsTime[request][shortestTimeRes] = {res};
    }
}

void ConsensusCalculator::calculateConsensus(){
    calculateConsensusForCategory(resultsDistance, consensusDistance, true);
    calculateConsensusForCategory(resultsTime, consensusTime, false);
}

void ConsensusCalculator::calculateConsensusForCategory(unordered_map<int, unordered_map<RouteSummary, vector<NavigationResult>>>& results, unordered_map<int, optional<RouteResult>>& consensus, bool isShortestDistance){
    RouteSummary invalidResult;
    for (size_t i = 0; i < results.size(); ++i) {// for each request
        if(results[i].contains(invalidResult)){// if one of the route results for the request is invalid add ad an invalid RouteResult
            auto res = results[i][invalidResult][0];
            if(isShortestDistance){
                consensus[i] = RouteResult(res.gisName, navigationName, res.routes->shortestDistance().totalLength(), res.routes->shortestDistance().estimatedDuration(), isShortestDistance, res.request, res.count, res.shortestDistanceValid);
            }
            else{
                consensus[i] = RouteResult(res.gisName, navigationName, res.routes->shortestTime().totalLength(), res.routes->shortestTime().estimatedDuration(), isShortestDistance, res.request, res.count,res.shortestTimeValid);
            }
            //since invalid put all results in key i in strangeResults
            putAllresultsInStrange(isShortestDistance, results, i);
            continue;
        }
        //else all Routes are valid
        //calculate number of results
        size_t numOfResult = 0;
        for(const auto& group : results[i]){
            numOfResult += group.second.size();
        }
        //find the consensus from the groups in results[i]
        bool foundConsensus = false;
        for(const auto& group : results[i]){
            if(group.second.size() > 0.5*numOfResult){//if the consensus  set as consensus(we want the one with minimal requests to GIS)
                size_t min = 0;
                RouteResult minRes;
                for(const auto& res : group.second){
                    if(!min || res.count < min){
                        min = res.count;
                        if(isShortestDistance){
                            minRes = RouteResult(res.gisName, navigationName, res.routes->shortestDistance().totalLength(), res.routes->shortestDistance().estimatedDuration(), isShortestDistance, res.request, res.count,res.shortestDistanceValid);
                        }
                        else{
                           minRes = RouteResult(res.gisName, navigationName, res.routes->shortestTime().totalLength(), res.routes->shortestTime().estimatedDuration(), isShortestDistance, res.request,res.count,res.shortestTimeValid);
                        }
                    }
                }
                consensus[i] = minRes;
                foundConsensus = true;
                break;
            }
            else{//if not in the consensus add to strangeResults
                for(const auto& res : group.second){
                    if(isShortestDistance){
                        strangeResults.emplace_back(res.gisName, navigationName, res.routes->shortestDistance().totalLength(), res.routes->shortestDistance().estimatedDuration(), isShortestDistance, res.request,res.count,res.shortestDistanceValid);
                    }
                    else{
                        strangeResults.emplace_back(res.gisName, navigationName, res.routes->shortestTime().totalLength(), res.routes->shortestTime().estimatedDuration(), isShortestDistance, res.request,res.count,res.shortestTimeValid);
                    }
                }
            }
        }
        if(!foundConsensus) {//if didn't found consensus put all in strangeResults
            putAllresultsInStrange(isShortestDistance, results, i);
            consensus[i] = optional<RouteResult>();
        }
    }
}

void ConsensusCalculator::putAllresultsInStrange(bool isShortestDistance,
                                                 unordered_map<int, unordered_map<RouteSummary, vector<NavigationResult>>> &results,
                                                 size_t &i) {
    for(const auto& group : results[i]){// for all groups of unordered_map<RouteSummary, vector<NavigationResult>> in key i
        for(const auto& res: group.second){// for all vector<NavigationResult> values of the groups
            if(isShortestDistance){
                strangeResults.emplace_back(res.gisName, navigationName, res.routes->shortestDistance().totalLength(), res.routes->shortestDistance().estimatedDuration(), isShortestDistance, res.request, res.count,res.shortestDistanceValid);
            }
            else{
                strangeResults.emplace_back(res.gisName, navigationName, res.routes->shortestTime().totalLength(), res.routes->shortestTime().estimatedDuration(), isShortestDistance, res.request, res.count,res.shortestTimeValid);
            }
        }
    }
}

const string &ConsensusCalculator::getNavigationName() const {
    return navigationName;
}

unordered_map<int, optional<RouteResult>> ConsensusCalculator::moveConsensusDistance() {
    return std::move(consensusDistance);
}

unordered_map<int, optional<RouteResult>> ConsensusCalculator::moveConsensusTime() {
    return std::move(consensusTime);
}

const vector<RouteResult> &ConsensusCalculator::getStrangeResults() const {
    return strangeResults;
}
