//
// Created by sagig on 11/25/2020.
//
#pragma once

#include "../MapEntities/Entity.h"
#include "../../Common/CoordinatesMath.h"

/**
 * a geometry on the grid
 */
class GridEntity{
    // the entity the geometry belongs to
    shared_ptr<Entity> entity;
    // an id to differentiate geometries the belong to the same entity
    size_t gridId;
    // the geometry stored
    shared_ptr<Geometry> geometry;
    // the id of the entity
    EntityId entityId;
public:
    /**
     *
     * @param _entity the entity the geomtry belongs to
     * @param _gridId the id of the geometry
     * @param _geometry the geomtry
     */
    GridEntity(shared_ptr<Entity> _entity, int _gridId, shared_ptr<Geometry> _geometry): entity(_entity), gridId(_gridId), geometry(_geometry),
    entityId(_entity->getEntityId()){}
    /**
     * comares the ids
     * @param other
     * @return
     */
    bool operator==(const GridEntity& other) const;

    /**
     *  gets the distance from the cord to the geometry
     * @param cord to calculate the distance from
     * @return the distance
     */
    Meters getDistanceFrom(const Coordinates& cord) const;

    /**
     * gets the closest point on the geometry on the entity from the cord
     * @param cord
     * @return the closest point
     */
    Coordinates getClosestPont(const Coordinates& cord) const;

    /**
     *
     * @return gets the entity
     */
    shared_ptr<Entity> getEntity() const {
        return entity;
    }

    /**
     *
     * @return the grid id
     */
    size_t getGridId() const {
        return gridId;
    }

    /**
     *
     * @return the entity id
     */
    const EntityId &getEntityId() const {
        return entityId;
    }
};

// hash to GridEntity
namespace std {

    template <>
    struct hash<GridEntity>{
        std::size_t operator()(const GridEntity& k) const{
            using std::size_t;
            using std::hash;
            using std::string;


            return ((hash<EntityId>()(k.getEntityId())^ (hash<size_t>()(k.getGridId()) << 1)));
        }
    };

}
