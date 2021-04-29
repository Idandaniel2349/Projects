#include "GIS_316497817.h"
#include "algorithm"
#include "../Common/GISRegistration.h"

REGISTER_GIS(GIS_316497817)

using std::dynamic_pointer_cast;

const string GIS_316497817::logFilePath = "errors.log";

const double GIS_316497817::precision = 0.1;

//the function clears all data from the system and returns the number of entities that were cleared
std::size_t GIS_316497817::clear() {
    size_t entitiesCleared = entityById.size();
    try {
        waysLoaded = 0;
        //delete the memory allocated to the unordered maps entityById and entityByName
        entityById.clear();
        entityByName.clear();
        grid.clear();
        entitiesOrdered.clear();
    }
    catch (const std::exception& e) {
        string msg = "Got an exception while trying to clear the GIS. Exception message is ";
        logger.log(msg + e.what());
    }
    return entitiesCleared;
}

//the function loads a map file and returns a vector of all the EntityIds that were loaded (either Ids that appeared in file or generated) in the exact order as the objects that appeared in the file
//- in case the system had existing data, the data loaded from file is added to the existing data
//- in case entityId from file exists already the old data related to this id is replaced with the new data
//- in case an entity in file doesn’t have an Id, a new Id would be generated for it automatically
std::vector<EntityId> GIS_316497817::loadMapFile(const std::string& filename) {
    auto addedIds = vector<EntityId>();
    try {

        //get the entities from filename
        const auto &loadedEntities = entityJsonHandler.loadEntities(filename);
        for (const auto &entity : loadedEntities) {
            const auto &id = entity->getEntityId();
            if (!entityById.contains(id)) {
                //set the entities on the GIS
                addedIds.push_back(id);
                entityById[id] = entity;
                entityByName[entity->getName()].push_back(entity);
                grid.setEntityOnGrid(entity);
                entitiesOrdered.push_back(entity);
                if(entity->getType() == EntityType::way){
                    ++waysLoaded;
                    //add way with from key in waysByJunction
                    auto currentWay = dynamic_pointer_cast<Way>(entity);
                    const EntityId & fromId = currentWay->getFrom();
                    if (!waysByJunction.contains(fromId)) {
                        waysByJunction[fromId]={id};
                    }
                    else{
                        waysByJunction[fromId].push_back(id);
                    }

                    //if bidirectional add to as well
                    if(currentWay->getDirection()==direction::bidirectional){
                        const EntityId & toId = currentWay->getTO();
                        if (!waysByJunction.contains(toId)) {
                            waysByJunction[toId]={id};
                        }
                        else{
                            waysByJunction[toId].push_back(id);
                        }
                    }

                }
            }
            else{
                string msg = "got duplicate id " + (string)id + " ignoring it";
                logger.log(msg);
            }

        }


    }
    catch (const std::exception& e) {
        string msg = "Got an exception while trying to load map file with message: ";
        logger.log(msg + e.what());
    }
    return addedIds;
}

//the function saves all data into a map file and returns the number of entities that were saved, the saved entities must include their Ids
std::size_t GIS_316497817::saveMapFile(const std::string& filename) const{
    size_t NumOfSavedEntities=0;
    try {
        NumOfSavedEntities= entityJsonHandler.saveEntities(filename, entitiesOrdered);
    }
    catch (const std::exception& e) {
        string msg = "Got an exception while trying to save map file with message: ";
        logger.log(msg + e.what());
    }
    return NumOfSavedEntities;
}

//the function returns a vector of EntityIds for all entities that match the search_name
//all exact matches (case sensitive) shall be first in the vector
//right after shall optionally appear "partial matches", in any order (e.g. case insensitive match, matching based on the longer description, 
//match based on the category tags etc. partial match may work differently when the search is coordinates based or not  - note: partial match implementation is not mandatory, however it may grant you a bonus. 
//To qualify for bonus you must include in your submission zip a folder named bonus.partial_search, that contains three items: 
//partial_search.txt: explaining what you did and why qualify for bonus - include where code handels this exceptional partial search
//mapFile.json with 10-20 entities
//A file named: partial_search_test.cpp that includes a google test that passes and demonstrates your exceptional search
std::vector<EntityId> GIS_316497817::getEntities(const std::string& search_name) const {
    vector<EntityId> entitiesId;
    shared_ptr<Entity> currEntity;
    try {
        if (!entityByName.contains(search_name)) {
            return entitiesId;
        }

        auto &vector = entityByName.at(search_name);
        for (auto &it:vector) {
            currEntity = it;
            entitiesId.push_back(currEntity->getEntityId());
        }
    }
    catch (const std::exception& e) {
        string msg = "Got an exception while trying to get entities with search name: "+search_name+"  with message: ";
        logger.log(msg + e.what());
    }

    return entitiesId;
}

//same as above, but restricted to a search area of the given circle
std::vector<EntityId> GIS_316497817::getEntities(const std::string& search_name, const Coordinates& cord, Meters radius) const{
    vector<EntityId> ids;

    try {
        unordered_set<EntityId> foundIds;
        unordered_set<GridEntity> foundGeometries;
        SpiralCoordinatesTraverser traverser = SpiralCoordinatesTraverser(cord, precision);
        Meters minCellDistance = Meters(0);
        bool minCellDistanceInit = false;
        double buffer = 1.1;
        bool firstCell= true;

        //Runs untill all the grid cell on the perimeter of the search are further than the search radius
        while ((double) minCellDistance <= (double) radius*buffer) {
            minCellDistanceInit = false;
            //runs on all the cells on the perimeter of the search square
            do {
                auto cell = traverser.getNext();
                Meters distance = GridMath::getMinDistance(cell, cord, precision);
                if (!minCellDistanceInit || distance < minCellDistance) {
                    minCellDistanceInit = true;
                    minCellDistance = distance;
                }
                if ( distance <= radius || firstCell) {
                    firstCell = false;
                    const auto &entities = grid.getEntities(cell);
                    for (const auto &entity : entities) {
                        const EntityId &entityId = entity.getEntityId();
                        if (!foundIds.contains(entityId) && entity.getEntity()->getName() == search_name &&
                            !foundGeometries.contains(entity)) {
                            foundGeometries.insert(entity);
                            Meters distanceFromEntity = entity.getDistanceFrom(cord);
                            if ( distanceFromEntity <= radius) {
                                foundIds.insert(entityId);
                                ids.push_back(entityId);
                            }
                        }
                    }
                }
            } while (!traverser.onNewPerimeter());
        }
    }
    catch (const std::exception& e) {
        string msg = "Got an exception while trying to get entities with search name: "+search_name+", restricted to a given Circle with message: ";
        logger.log(msg + e.what());
    }

    return ids;
}

//the function shall get EntityId and Coordinates
//if the Id is not known, the function returns an empty optional
//otherwise, return the closest Coordinates, on entity’s perimeter, to the provided Coordinates.
//(the Coordinates shall be a point on a the entity’s geometry that is closest to the given Coordinates)
std::optional<Coordinates> GIS_316497817::getEntityClosestPoint(const EntityId& entityId, const Coordinates& coordinates) const{
    try {
        if (entityById.contains(entityId)) {
            auto e = entityById.at(entityId);
            auto closestPoint = e->closestPoint(coordinates);
            return std::optional<Coordinates>(closestPoint);
        }
    }
    catch (const std::exception& e) {
        string msg = "Got an exception while trying to get Entity Closest Point with message: ";
        logger.log(msg + e.what());
    }

    return std::optional<Coordinates>();
}

//this function shall get Coordinates and return the closest Coordinates of a Way (the Coordinates may be a point on a the way that is closest to the given Coordinates) and the EntityId of this way
std::tuple<Coordinates, EntityId, std::size_t> GIS_316497817::getWayClosestPoint(const Coordinates& coords) const{
    return getWayClosestPoint(coords, Restrictions(""));
}

std::vector<EntityId> GIS_316497817::getWaysByJunction(const EntityId& junctionId) const{
    if(!waysByJunction.contains(junctionId)){
        return vector<EntityId>();
    }
    return waysByJunction.at(junctionId);
}

std::tuple<Coordinates, EntityId, std::size_t> GIS_316497817::getWayClosestPoint(const Coordinates& coords, const Restrictions& res) const{

    std::tuple<Coordinates, EntityId, std::size_t> closestWay =
            std::tuple<Coordinates, EntityId, std::size_t>(Coordinates(Longitude(404), Latitude(404)), EntityId(""), 0);
    if(waysLoaded == 0){
        return closestWay;
    }
    try {
        unordered_set<GridEntity> foundGeometries;
        SpiralCoordinatesTraverser traverser = SpiralCoordinatesTraverser(coords, precision);
        Meters minCellDistance = Meters(0);
        Meters minWayDistance = Meters(0);
        bool minCellDistanceInit = false;
        bool wayFound = false;
        double buffer = 1.1;

        //Runs while it hasn't found an entity or all the cells in it's last square traversal are further away from the
        //origin than the nearest found way
        while ( minCellDistance <=  minWayDistance*buffer || !wayFound) {
            minCellDistanceInit = false;
            //runs on all the cells on the perimeter of the search square
            do {
                auto cell = traverser.getNext();
                Meters distance = GridMath::getMinDistance(cell, coords, precision);
                if (!minCellDistanceInit ||  distance < minCellDistance) {
                    minCellDistanceInit = true;
                    minCellDistance = distance;
                }
                if ( distance <= minWayDistance || !wayFound) {
                    const auto &ways = grid.getWays(cell);
                    for (const auto &way : ways) {
                        if (!foundGeometries.contains(way)) {
                            foundGeometries.insert(way);
                            Meters distanceFromEntity = way.getDistanceFrom(coords);
                            if (( distanceFromEntity <= minWayDistance || !wayFound)
                            &&(!isWayRestricted(res, *dynamic_pointer_cast<Way>(way.getEntity())))) {
                                wayFound = true;
                                minWayDistance = distanceFromEntity;
                                Coordinates closestPoint = way.getClosestPont(coords);
                                closestWay = std::make_tuple(closestPoint, way.getEntityId(), way.getGridId());
                            }
                        }
                    }
                }
            } while (!traverser.onNewPerimeter());
        }
    }
    catch (const std::exception& e) {
        string msg = "Got an exception while trying to get Way Closest Point with message: ";
        logger.log(msg + e.what());
    }
    return closestWay;

}

const AbstractWay& GIS_316497817::getWay(const EntityId& wayId) const{
    if(!entityById.contains(wayId)){
        string message="Way Id "+(string)wayId+" doesn't exist";
        throw IdNotFoundException(message);
    }
    if(entityById.at(wayId)->getType() != EntityType::way){
        string message="Id "+(string)wayId+" is not a Way";
        throw NotAWayException(message);
    }
    auto returnWay = dynamic_pointer_cast<AbstractWay>(entityById.at(wayId));
    return *returnWay;
}

bool GIS_316497817::isWayRestricted(Restrictions res, Way way) const{
    if((res.contains("highway") && way.isHighway()) || (res.contains("toll") && way.isToll())){
        return true;
    }
    return false;
}
