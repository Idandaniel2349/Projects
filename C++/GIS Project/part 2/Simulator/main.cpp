#include "../Common/Restrictions.h"
#include "SimulatorExceptions.h"
#include <cstdlib>
#include <dlfcn.h>
#include <string>
#include <filesystem>
#include "string"
#include "SOLoader.h"
#include "SafeQueue.h"
#include "NavigationRunner.h"
#include "ArgParser.h"
#include "ScoreCalculator.h"
#include "CSVIO.h"
#include "thread"

using std::string;
using std::thread;
using std::make_shared;

namespace fs = std::filesystem;

int main(int argc, char * argv[]) {
    //parse arguments using the ArgParser class
    ArgParser parser;
    parser.parseArgs(argv, argc);
    int numOfThreads = parser.getIntVal("num_of_threads").has_value()? parser.getIntVal("num_of_threads").value() : 3;
    if(numOfThreads < 3){
        throw InvalidArgsException("Number of threads should be bigger than 3");
    }
    string navigation = parser.getStringValOrThrow("navigation");
    string gis = parser.getStringValOrThrow("gis");
    string mapFile =parser.getStringValOrThrow("map_file");
    string navigationRequestsPath = parser.getStringValOrThrow("navigation_requests");
    string output = parser.getStringVal("output").has_value()? parser.getStringVal("output").value(): "";

    const string RES_FILE = "simulation.results";
    const string STRANGE_FILE = "strange_GIS_results.log";
    fs::path outputDir(output);
    fs::path resultFile(RES_FILE);
    fs::path strangeResultsFile(STRANGE_FILE);
    string resultPath = (outputDir / resultFile).c_str();
    string strangeResultPath = (outputDir / strangeResultsFile).c_str();
    map<int, unordered_map<string, int>> scores;

    vector<RouteResult> strangeResults;

    vector<pair<Coordinates, Coordinates>> requests = CSVIO::readRequests(navigationRequestsPath);
    ScoreCalculator scoreCalc(requests.size());

    {
        vector<thread> workerThreads;
        SafeQueue<function<void(void)>> taskQueue;
        unordered_map<string, shared_ptr<ConsensusCalculator>> consensusCalculators;

        //Load the GIS and put all tasks in the queue
        vector<string> gisLoaded = SOLoader::getInstance().loadGIS(gis, mapFile);
        unordered_map<string, function<unique_ptr<AbstractNavigation>(
                const NavigationGIS &)>> navigationFuncLoaded = SOLoader::getInstance().loadNavigation(navigation);
        for (const auto &nav : navigationFuncLoaded) {
            consensusCalculators[nav.first] = make_shared<ConsensusCalculator>(nav.first, requests.size());
        }
        for (const auto &gisName : gisLoaded) {
            for (const auto &nav : navigationFuncLoaded) {
                taskQueue.push(NavigationRunner(nav.second, gisName, requests, consensusCalculators[nav.first], nav.first));
            }
        }

        //run the threads preforming the tasks
        for (int i = 0; i < numOfThreads - 1; ++i) {
            workerThreads.push_back(thread([&taskQueue] {
                while (!taskQueue.isEmpty()) {
                    auto task = taskQueue.pop();
                    if (task.has_value()) {
                        task.value()();
                    }
                }
            }));
        }

        for (auto &thread : workerThreads) {
            thread.join();
        }

        for (auto &consensusCalculator : consensusCalculators) {
            consensusCalculator.second->calculateConsensus();
            scoreCalc.addConsensus(consensusCalculator.first, consensusCalculator.second->moveConsensusDistance(),
                                   consensusCalculator.second->moveConsensusTime());
            auto &navStrangeResults = consensusCalculator.second->getStrangeResults();
            strangeResults.insert(strangeResults.cbegin(), navStrangeResults.cbegin(), navStrangeResults.cend());
        }
    }
    SOLoader::getInstance().clearNavigationHandles();
    scores = scoreCalc.calculateScores();

    CSVIO::writeScores(resultPath, scores,requests);

    if(!strangeResults.empty()){
        CSVIO::writesStrangeGis(strangeResultPath, strangeResults);
    }

	return EXIT_SUCCESS;
}
