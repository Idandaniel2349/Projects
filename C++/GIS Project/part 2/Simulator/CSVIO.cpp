//
// Created by sagig on 1/9/2021.
//

#include "CSVIO.h"
#include "SimulatorExceptions.h"
#include <iostream>
#include <fstream>
#include <sstream>

using std::pair;


void CSVIO::writeScores(const string& path, const map<int, unordered_map<string, int>>& scores,vector<pair<Coordinates, Coordinates>> requests){
    if(scores.empty()){
        return;
    }
    unordered_map<string,int> navigationTotalScores;
    const auto& requestMap = scores.at((scores.cbegin())->first);
    for (const auto& entry : requestMap) {// init the unordered_map
        navigationTotalScores[entry.first] = 0;
    }

    //calculate the totalScores of for each navigation
    for(const auto& req : scores){
        for (const auto& entry : req.second) {
            navigationTotalScores[entry.first] += entry.second;
        }
    }
    //add the totalScores to a map(they will be sorted)
    map<int, vector<string>, std::greater<>> sortedTotalScores;
    for(const auto& totalScore : navigationTotalScores){
        if(!sortedTotalScores.contains(totalScore.second))
        {
            sortedTotalScores[totalScore.second] ={};
        }
        sortedTotalScores[totalScore.second].push_back(totalScore.first);
    }
    writeScoresToCSV(path, scores, sortedTotalScores,requests);


}

void CSVIO::writeScoresToCSV(const string &path, const map<int, unordered_map<string, int>> &scores,
                             map<int, vector<string>, std::greater<>> &sortedTotalScores,vector<pair<Coordinates, Coordinates>> requests) {
    std::ofstream csvFile;
    csvFile.open(path);
    if(csvFile.is_open()){// if we succeeded to open the file
        std::ostringstream header;
        header << "Navigation Algorithm";
        for(const auto& req : scores){//write the requests in the header
            header<<", {" << requests[req.first].first.longitude()->roundToPrecision()<<", "<<requests[req.first].first.latitude()->roundToPrecision()<<"}";
            header<<"-{" << requests[req.first].second.longitude()->roundToPrecision()<<", "<<requests[req.first].second.latitude()->roundToPrecision()<<"}";
        }
        header<<", Total Score";
        header<<"\n";
        csvFile<<header.str();

        for (const auto& navWithSameScores : sortedTotalScores) {//for every navigation by totalScores(sorted)
            for(const auto& nav : navWithSameScores.second) {//for every navigation with the current totalScores write scores in a line
                std::ostringstream navScores;
                navScores << nav;
                for (const auto &req : scores) {
                    navScores << ", " << scores.at(req.first).at(nav);
                }
                navScores << ", " << navWithSameScores.first << "\n";
                csvFile << navScores.str();
            }
        }
        csvFile.close();
    }
    else{
        std::ostringstream msg;
        msg<<"failed to open csv in path "<<path<<" while trying to write scores";
        throw FailedOpenCSVException(msg.str());
    }
}

void CSVIO::writesStrangeGis(const string& path, const vector <RouteResult>& strangeResults){
    std::ofstream csvFile;
    csvFile.open(path);
    if(csvFile.is_open()){// if we succeeded to open the file
        for(const auto& strangeResult : strangeResults){//for every RouteResult in strangeResults write in the needed fomrat to file
            std::ostringstream csvLine;
            csvLine<<strangeResult.navigationName<<", ";
            string distanceOrTine = (strangeResult.isShortestDistance)? "distance":"time";
            csvLine<<"shortest_"<<distanceOrTine<<", ";
            csvLine<<strangeResult.gisName<<", ";
            csvLine<<"{"<<strangeResult.request.first.longitude()->roundToPrecision()<<", "<<strangeResult.request.first.latitude()->roundToPrecision()<<"}-";
            csvLine<<"{"<<strangeResult.request.second.longitude()->roundToPrecision()<<", "<<strangeResult.request.second.latitude()->roundToPrecision()<<"}, ";
            csvLine<<strangeResult.length->roundToPrecision()<<", ";
            csvLine<<strangeResult.duration->roundToPrecision()<<", ";
            csvLine<<strangeResult.isValid<<"\n";
            csvFile<<csvLine.str();

        }
        csvFile.close();
    }
    else{
        std::ostringstream msg;
        msg<<"failed to open csv in path "<<path<<" while trying to write strange results";
        throw FailedOpenCSVException(msg.str());
    }
}

vector<pair<Coordinates, Coordinates>> CSVIO::readRequests(const string& requestPath){
    vector<pair<Coordinates, Coordinates>> requests;
    std::ifstream inputCSV;
    inputCSV.open(requestPath);
    if(inputCSV.is_open()){// if we succeeded to open the file
        for(string line; getline( inputCSV, line ); )
        {//for every line (request) in the file
            size_t delimPos = line.find("}-{", 0);
            string firstCoordString = line.substr(1, delimPos);
            string secCoordString = line.substr(delimPos + 3, line.length() - delimPos - 1);
            requests.emplace_back(parseCoordinatesFromString(firstCoordString), parseCoordinatesFromString(secCoordString));
        }
        inputCSV.close();
    }
    else{
        std::ostringstream msg;
        msg<<"failed to open csv in path "<<requestPath<<" while trying to read navigation requests";
        throw FailedOpenCSVException(msg.str());
    }
    return requests;
}

Coordinates CSVIO::parseCoordinatesFromString(const string& coordsString){
    // get as "Longtitue,latitude" string and return as Coordinates
    size_t commaPos = coordsString.find(",");
    string longitudeString = coordsString.substr(0, commaPos);
    Longitude longitude = Longitude(std::stod(longitudeString));
    string latitudeString = coordsString.substr(commaPos + 1);
    Latitude latitude = Latitude(std::stod(latitudeString));
    return {longitude, latitude};
}