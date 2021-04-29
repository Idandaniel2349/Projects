#pragma once

#include <string>
#include <vector>
#include "optional"
#include "unordered_map"
#include "unordered_set"
#include "../Common/GISNamedTypes.h"
#include "Grid/Grid.h"
#include "MapEntities/Entity.h"
#include "JsonHandler.h"
#include "Logger.h"
#include "Grid/SpiralCoordinatesTraverser.h"
#include "Grid/GridEntity.h"
#include "../Common/Restrictions.h"
#include "GISExceptions.h"
#include "memory"
#include "tuple"
#include "../Common/AbstractGIS.h"

using std::unordered_map;
using std::string;
using std::vector;
using std::shared_ptr;

/**
 * A class representing The GIS
 */
class GIS_316497817 : public AbstractGIS{
    static const string logFilePath;
    static const double precision;

    mutable Logger logger;
    unordered_map<EntityId,shared_ptr<Entity>> entityById;
    unordered_map<string,vector<shared_ptr<Entity>>> entityByName;
    unordered_map<EntityId,vector<EntityId>> waysByJunction;
    Grid grid;
    mutable JsonHandler entityJsonHandler;
    std::size_t waysLoaded;
    vector<shared_ptr<Entity>> entitiesOrdered;

    bool isWayRestricted(Restrictions res, Way way) const;
public:
    GIS_316497817(): logger(logFilePath), entityById(), entityByName(), grid(precision), entityJsonHandler(&logger, &entityById),
    waysLoaded(0){};

    // The function clears all data from the system
    // Returns:
    //  The number of entities that were cleared
    std::size_t clear();

    /**
     * @brief Loads a map file
     *
     * 	[1] in case the system had existing data, the data loaded from file is added to the existing data
     *  [2] in case entityId from file exists already the old data related to this id is replaced with the new data RELAXED: may be reject - proper error should be logged to errors.log
     *  [3] in case an entity in file doesn't have an Id, a new Id would be generated for it automatically
     *
     * @param filename - Provided mapfile
     * @return std::vector<EntityId> - A vector of all the EntityIds that were loaded (either Ids that appeared in file or generated) in the exact order the objects appeared in the file
     */
    std::vector<EntityId> loadMapFile(const std::string& filename) override;

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
    //- right after, shall optionally appear "partial matches", in any order (e.g. case insensitive match, matching based on the longer description,
    //  match based on the category tags, etc. Partial match may work differently when the search is coordinates based or not
    //  Note: partial match implementation is not mandatory, however it may grant you a bonus (See info in ex doc).
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

    /**
     * @brief The function shall get EntityId of a Junction and return a vector of Ids of all the ways that start at this junction and those which end at this junction and are bidirectional.
     *        If the given Id is not known or is not of a junction the function shall return an empty vector (you may log an error).
     *
     * @param junctionId
     * @return std::vector<EntityId> - A vector of Ids of all the Ways that start at given junction (also bidirectional Ways which end at this junction).
     */
    std::vector<EntityId> getWaysByJunction(const EntityId& junctionId) const override;

    /**
     * @brief Get the closest point along a Way that is closest to Referenced Point and the EntityId of that Way
     *
     * @param c - Referenced Point
     * @return std::pair<Coordinates, EntityId> - The Coordinates of closest point along a Way that is closest to Referenced Point and the EntityId of that Way
     */
    std::tuple<Coordinates, EntityId, std::size_t> getWayClosestPoint(const Coordinates& c) const override;

    /**
     * @brief Get the closest point to Referenced Point, along a Way that complies with provided Restrictions and the EntityId of that Way
     *        Restrictions can be provided as: Restrictions("highway") means avoid highway, Restrictions(" toll ") means avoid toll, Restrictions("highway , toll  ") or Restrictions("toll,highway") both mean avoid highway and toll, etc.
     *
     * @param c - Referenced Point
     * @param res - Restrictions
     * @return std::pair<Coordinates, EntityId> - The closest point to Referenced Point, along a Way that complies with provided Restrictions and the EntityId of that Way
     */
    std::tuple<Coordinates, EntityId, std::size_t> getWayClosestPoint(const Coordinates& c, const Restrictions& res) const override;

    /**
     * @brief Get the Way object et EntityId of a Way and return the Way itself.
     *        In case the id does not represent a valid way throw an exception of your choice.
     *
     * @param wayId - ID of Way
     * @return const Way&
     */
    const AbstractWay& getWay(const EntityId& wayId) const override;
};