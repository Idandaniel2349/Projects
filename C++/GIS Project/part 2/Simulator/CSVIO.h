//
// Created by sagig on 1/9/2021.
//
#pragma once
#include "string"
#include "unordered_map"
#include "RouteResult.h"
#include "vector"
#include "map"


using std::string;
using std::unordered_map;
using std::vector;
using std::map;

/**
 * CSVIO Class responsible to writing to csv files
 */
class CSVIO {
    /**
     * write scores to the file in path
     * @param path - path to csv file
     * @param scores - the scores needed to write
     * @param sortedTotalScores - information sorted by totalScores of scores
     * @param requests - the requests
     */
    static void writeScoresToCSV(const string &path, const map<int, unordered_map<string, int>> &scores,
                                 map<int, vector<string>, std::greater<>> &sortedTotalScores,vector<pair<Coordinates, Coordinates>> requests);
    /**
     * Get a string where the coordinates are written and calculate and return the appropriate coordinates
     * @param coordsString - the string representing the coordinates
     * @return - the coordinates parsed from the string
     */
    static Coordinates parseCoordinatesFromString(const string &coordsString);
public:
    /**
     * Write the scores to simulation.results given in the path
     * @param path - path to write to
     * @param scores - the scores to write
     * @param requests - the requests
     */
    static void writeScores(const string& path, const map<int, unordered_map<string, int>>& scores,vector<pair<Coordinates, Coordinates>> requests);
    /**
     * Write strangeResults to strange_GIS_results.log given in the path
     * @param path - path to write to
     * @param strangeResults - the strange Results to write
     */
    static void writesStrangeGis(const string& path, const vector <RouteResult>& strangeResults);
    /**
     * Read all requests from requestPath file
     * @param requestPath - the path of the requests file
     * @return - the requests as a vector of Coordinates pairs
     */
    static vector<pair<Coordinates, Coordinates>> readRequests(const string& requestPath);

};
