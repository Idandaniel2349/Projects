//
// Created by sagig on 1/8/2021.
//
#pragma once
#include "RouteResult.h"
#include "unordered_map"
#include "map"
#include "optional"

using std::unordered_map;
using std::map;
using std::optional;

/**
 * ScoreCalculator Class responsible for calculating the navigation scores
 */
class ScoreCalculator {
    unordered_map<int, unordered_map<string , optional<RouteResult>>> consensusesDistance;
    unordered_map<int, unordered_map<string , optional<RouteResult>>> consensusesTime;
    unordered_map<int, bool> hasConsensus;

    /**
     * set the consensus elements for navName for a category(distance or time)
     * @param navName - name of the navigation
     * @param navConsensuses - the consensuses to write from
     * @param consensuses - the consensuses to write to
     */
    void addConsensusCategory(string navName, unordered_map<int, optional<RouteResult>>& navConsensuses,  unordered_map<int, unordered_map<string , optional<RouteResult>>>& consensuses);

public:
    ScoreCalculator(size_t numOfRequets);
    /**
     * set the consensus elements for navName
     * @param navName - name of the navigation
     * @param navConsensusesDistance - distance consensuses
     * @param navConsensusesTime - time consensuses
     */
    void addConsensus(string navName, unordered_map<int, optional<RouteResult>> navConsensusesDistance, unordered_map<int, optional<RouteResult>> navConsensusesTime);
    /**
     * calculate scores for the consensuses
     * @return - return a map with key as request and value as a map from navigation to score
     */
    map<int, unordered_map<string, int>> calculateScores();
};

