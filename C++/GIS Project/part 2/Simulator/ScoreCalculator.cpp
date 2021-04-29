//
// Created by sagig on 1/8/2021.
//

#include "ScoreCalculator.h"

ScoreCalculator::ScoreCalculator(size_t numOfRequets){
    for (size_t i = 0; i < numOfRequets; ++i) {
        hasConsensus[i] = true;
        consensusesDistance[i] = {};
        consensusesTime[i] = {};
    }
}

void ScoreCalculator::addConsensus(string navName, unordered_map<int, optional<RouteResult>> navConsensusesDistance, unordered_map<int, optional<RouteResult>> navConsensusesTime){
    addConsensusCategory(navName, navConsensusesDistance, consensusesDistance);
    addConsensusCategory(navName, navConsensusesTime, consensusesTime);
}

void ScoreCalculator::addConsensusCategory(string navName, unordered_map<int, optional<RouteResult>>& navConsensuses,  unordered_map<int, unordered_map<string , optional<RouteResult>>>& consensuses){
    for(const auto& cons : navConsensuses){
        if(hasConsensus[cons.first]){
            if(cons.second.has_value()){
                consensuses[cons.first][navName] = cons.second.value();
            }
            else{
                hasConsensus[cons.first] = false;
                consensusesDistance.erase(cons.first);
                consensusesTime.erase(cons.first);
            }
        }
    }
}

map<int, unordered_map<string, int>> ScoreCalculator::calculateScores(){
    map<int, unordered_map<string, int>> scores;
    for(auto const& req: hasConsensus){//for every request that has consensuses
        if(!req.second){//if value false don't calculate
            continue;
        }
        int reqNum = req.first;
        scores[reqNum] = {};
        bool minInnit = false;
        Meters minDistance(-1);
        size_t gisCount = 0;
        //find the min distance and gis count
        for (const auto& conDistance : consensusesDistance[reqNum]) {//for every navigation in the distance consensuses of the request
            if((!minInnit || conDistance.second.value().length < minDistance) && conDistance.second.value().isValid){
                minInnit = true;
                minDistance = conDistance.second.value().length;
                gisCount = conDistance.second.value().gisCount;
            }
            else if(conDistance.second.value().length == minDistance && conDistance.second.value().gisCount < gisCount){
                gisCount = conDistance.second.value().gisCount;
            }
        }
        //calculate total scores
        for (const auto& conDistance : consensusesDistance[reqNum]) {//for every navigation in the distance consensuses of the request
            if(!conDistance.second.value().isValid){
                scores[reqNum][conDistance.first] = -1;
            }
            else{
                if(conDistance.second.value().length->roundToPrecision() <= minDistance->roundToPrecision()){
                    scores[reqNum][conDistance.first] = 1;
                    if(conDistance.second.value().gisCount == gisCount){
                        scores[reqNum][conDistance.first]++;
                    }
                }
                else{
                    scores[reqNum][conDistance.first] = 0;
                }
            }
        }
    }


    for(auto const& req: hasConsensus){//for every request that has consensuses
        if(!req.second){//if value false don't calculate
            continue;
        }
        int reqNum = req.first;
        bool minInnit = false;
        Minutes minTime(-1);
        size_t gisCount = 0;
        //find the min time and gis count
        for (const auto& conTime : consensusesTime[reqNum]) {//for every navigation in the time consensuses of the request
            if((!minInnit || conTime.second.value().duration < minTime) && conTime.second.value().isValid){
                minInnit = true;
                minTime = conTime.second.value().duration;
                gisCount = conTime.second.value().gisCount;
            }
            else if(conTime.second.value().duration == minTime && conTime.second.value().gisCount < gisCount){
                gisCount = conTime.second.value().gisCount;
            }
        }
        //calculate total scores
        for (const auto& conTime : consensusesTime[reqNum]) {//for every navigation in the time consensuses of the request
            if(!conTime.second.value().isValid){
                scores[reqNum][conTime.first]--;
            }
            else{
                if(conTime.second.value().duration->roundToPrecision() <= minTime->roundToPrecision()){
                    scores[reqNum][conTime.first]++;
                    if(conTime.second.value().gisCount == gisCount){
                        scores[reqNum][conTime.first]++;
                    }
                }
            }
        }
    }

    return scores;
}