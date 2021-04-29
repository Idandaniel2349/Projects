//
// Created by sagig on 12/20/2020.
//

#include "PriorityQueue.h"
#include "RouteFinder.h"
#include "../Common/CoordinatesMath.h"

using std::function;
using std::make_shared;

optional<unique_ptr<vector<vector<shared_ptr<RouteSection>>>>> RouteFinder::findRoute(const Coordinates &endCoordinates,const EntityId &start, const EntityId &end, function<double(const AbstractWay&)> weightFunction,
                                  function<double(Coordinates, Coordinates)> heuristic) {
    optional<unique_ptr<vector<vector<shared_ptr<RouteSection>>>>> routes;
    PriorityQueue<EntityId,double> priorityQueue;
    unordered_map<EntityId, vector<shared_ptr<RouteSection>>> cameFrom;
    unordered_map<EntityId, double> costSoFar;

    priorityQueue.put(start, 0);
    cameFrom.insert({start, vector<shared_ptr<RouteSection>>()});
    costSoFar[start] = 0;

    while (!priorityQueue.empty()) {
        EntityId currentJunction = priorityQueue.get();

        if (currentJunction == end) {
            routes = buildShortestRoutesFromSection(cameFrom, end, start);

            break;
        }

        //Updates the cost of junctions that are connected to by ways leaving the current junction
        for (const EntityId& currentWayId : navigationGIS.getWaysByJunction(currentJunction)) {
            const AbstractWay& currentWay = navigationGIS.getWay(currentWayId);
            double new_cost = costSoFar[currentJunction] + weightFunction(currentWay);
            Direction wayDirection = (currentWay.getJunctions().first == currentJunction) ? Direction::A_to_B : Direction::B_to_A;
            EntityId nextJunction =(wayDirection == Direction::A_to_B) ? currentWay.getJunctions().second : currentWay.getJunctions().first;
            Coordinates nextJunctionCoordinates =(wayDirection == Direction::A_to_B) ? currentWay.getToJunctionCoordinates() : currentWay.getFromJunctionCoordinates();

            if (costSoFar.find(nextJunction) == costSoFar.end()
                || new_cost < costSoFar[nextJunction]) {
                costSoFar[nextJunction] = new_cost;
                double priority = new_cost + heuristic(nextJunctionCoordinates, endCoordinates);
                priorityQueue.put(nextJunction, priority);
                auto currentSection = make_shared<RouteSection>(currentJunction, currentWayId, wayDirection, currentWay);
                cameFrom[nextJunction] = vector<shared_ptr<RouteSection>>({currentSection});
            }//if route already exists to junction with this weight add it to the lightest routes vector
            else if(new_cost == costSoFar[nextJunction]){
                auto currentSection = make_shared<RouteSection>(currentJunction, currentWayId, wayDirection, currentWay);
                cameFrom[nextJunction].push_back(currentSection);
            }
        }
    }
    return routes;
}

optional<unique_ptr<vector<vector<shared_ptr<RouteSection>>>>> RouteFinder::findFastestRoute(const Coordinates &endCoordinates,const EntityId &start, const EntityId &end, int maxSpeedLimit){

    return findRoute(endCoordinates, start, end, [](const AbstractWay& way){return (double)way.getLength()/way.getSpeedLimit();},
                     [maxSpeedLimit](Coordinates a, Coordinates b){return (double)CoordinatesMath::calculateDistance(a, b)/maxSpeedLimit;
    });
}

optional<unique_ptr<vector<vector<shared_ptr<RouteSection>>>>> RouteFinder::findShortestRoute(const Coordinates &endCoordinates,const EntityId &start, const EntityId &end){
    return findRoute(endCoordinates, start, end, [](const AbstractWay& way){return (double)way.getLength();},
                     [](Coordinates a, Coordinates b){return (double)CoordinatesMath::calculateDistance(a, b);
                     });
}

unique_ptr<vector<vector<shared_ptr<RouteSection>>>> RouteFinder::buildShortestRoutesFromSection(unordered_map<EntityId, vector<shared_ptr<RouteSection>>> cameFrom, EntityId currentId, EntityId startId){
    if(currentId == startId){
        return nullptr;
    }
    unique_ptr<vector<vector<shared_ptr<RouteSection>>>> currentRoutes = make_unique<vector<vector<shared_ptr<RouteSection>>>>();
    auto currentSections = cameFrom.at(currentId);
    for(const auto& currentSection : currentSections){
        unique_ptr<vector<vector<shared_ptr<RouteSection>>>> subRoutes = buildShortestRoutesFromSection(cameFrom,
                                                                                                        currentSection->getJunctionId(),startId);
        //if the start leads to this junction initialize the route with the section leading from the start to the current junction
        if(subRoutes== nullptr){
            currentRoutes->push_back(vector<shared_ptr<RouteSection>>({currentSection}));
        }
        else {
            for (const auto &subRoute : *subRoutes) {
                //get all the routes leading to the previous junction and append the current route to the start of them
                vector<shared_ptr<RouteSection>> currRoute(subRoute);
                currRoute.push_back(currentSection);
                currentRoutes->push_back(currRoute);
            }
        }
    }
    return currentRoutes;
}