#pragma once

#include <string>
#include <vector>
#include "optional"
#include "unordered_map"
#include "unordered_set"
#include "GISNamedTypes.h"
#include "Grid/Grid.h"
#include "MapEntities/Entity.h"
#include "JsonHandler.h"
#include "Logger.h"
#include "Grid/SpiralCoordinatesTraverser.h"
#include "Grid/GridEntity.h"
#include "Navigation/Restrictions.h"
#include "GISExceptions.h"
#include "memory"

using std::unordered_map;
using std::string;
using std::vector;
using std::shared_ptr;

/**
 * A class representing The GIS
 */
class GIS {
    static const string logFilePath;
    static const double precision;

    mutable Logger logger;
    unordered_map<EntityId,shared_ptr<Entity>> entityById;
    unordered_map<string,vector<shared_ptr<Entity>>> entityByName;
    unordered_map<EntityId,vector<EntityId>> waysByJunction;
    Grid grid;
    mutable JsonHandler entityJsonHandler;
    std::size_t waysLoaded;
public:
    GIS(): logger(logFilePath), entityById(), entityByName(), grid(precision), entityJsonHandler(&logger, &entityById),
    waysLoaded(0){};

    ~GIS();
	// Your GIS should support the following public API:
	//
	// NOTE: all functions below may or may not be const 
	// (i.e. may oblige not to alter the GIS system, or decide not to oblige), the tests would not assume constness of the functions, but you should make them const if you see it appropriate.
	//

	// The function clears all data from the system
	// Returns:
	//  The number of entities that were cleared
	std::size_t clear();

	// The function loads a map file
	// Returns:
	//  A vector of all the EntityIds that were loaded (either Ids that appeared in file or generated) in the exact order the objects appeared in the file
	//
	//- in case the system had existing data, the data loaded from file is added to the existing data
	//- in case entityId from file exists already the old data related to this id is replaced with the new data
	//- in case an entity in file doesn't have an Id, a new Id would be generated for it automatically
	std::vector<EntityId> loadMapFile(const std::string& filename);

	// The function saves all data into a map file
	// Returns: 
	//  The number of entities that were saved
	//
	//- all saved entities must include their Ids
	std::size_t saveMapFile(const std::string& filename) const;

	// The function searches entities 
	// Returns:
	//  A vector of EntityIds of all entities that match the search_name
	//
	//- all exact matches (case sensitive) of name field shall be first in the vector

	std::vector<EntityId> getEntities(const std::string& search_name) const;

	// The function searches entities - Same as above, but restricted to a search area of the given circle
	// Returns:
    //  A vector of EntityIds of all entities that match the search_name
	std::vector<EntityId> getEntities(const std::string& search_name, const Coordinates& cord, Meters radius) const;

	// The function returns closest point on given enity's perimeter to the given Coordinates
	// Returns:
	// - If the Id is not known, the function returns an empty optional
	// - Otherwise, returns the closest Coordinates, on entity's perimeter, to the provided Coordinates.
	//   (the Coordinates shall be a point on a the entity's geometry that is closest to the given Coordinates)
	std::optional<Coordinates> getEntityClosestPoint(const EntityId&, const Coordinates&) const;

	// The function returns the closest point on a way that is closest to the given Coordinates
	// Returns:
	//  An std::pair containing the Coordinates of closest point and the EntityId of the way that contains the aforementioned the point. 
	std::pair<Coordinates, EntityId> getWayClosestPoint(const Coordinates&) const;

    //The function shall get EntityId of a Junction and return a vector of Ids of all the ways that start at this
    //junction and those which end at this junction and are bidirectional. If the given Id is not known or is not
    //of a junction the function shall return an empty vector (you may log an error)
    std::vector<EntityId> getWaysByJunction(const EntityId& junctionId) const;

    //The function shall get Coordinates and Restrictions and return the closest Coordinates along a Way that
    //complies to the restrictions provided and the EntityId of this way, Restrictions can be provided as:
    //Restrictions("highway") means avoid highway, Restrictions(" toll ") means avoid toll,
    //Restrictions("highway , toll  ") or Restrictions("toll,highway") both mean avoid highway and toll, etc.
    std::pair<Coordinates, EntityId> getWayClosestPoint(const Coordinates&, const Restrictions& res) const;

    //The function shall get EntityId of a Way and return the Way itself. In case the id does not represent a
    //valid way throw an exception of your choice.
    const Way& getWay(const EntityId& wayId) const;



};