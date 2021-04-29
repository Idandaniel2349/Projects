//
// Created by sagig on 11/25/2020.
//
#pragma once
#include "vector"
#include "algorithm"
#include "GridEntity.h"

using std::vector;

/**
 * A cell in the grid that holds map geometries and their entities in its area
 */
class GridCell {
    // entities in the cell including the ways
    vector<GridEntity> cellEntities;
    // only the ways entities
    vector<GridEntity> cellWays;
public:
    /**
     *
     * @return entities in the cell
     */
    const vector<GridEntity> &getCellEntities() const;

    /**
     *
     * @return ways in the cell
     */
    const vector<GridEntity> &getCellWays() const;

public:
    GridCell() : cellEntities(), cellWays(){};

    /**
     * insert an entity and one of it's geometries in to the cell
     * @param entity the entity to insert
     * @param geometry the geometry of the the entity
     * @param gridId the id assigned to the geometry. different geometries need different ids
     */
    void insertEntity(shared_ptr<Entity> entity, shared_ptr<Geometry> geometry, int gridId);

    /**
     * inserts one way curve to the cell. inserts it to both vectors
     * @param entity way to insert
     * @param geometry the way curve
     * @param gridId the id assigned to the curve. different curves need different ids
     */
    void insertWay(shared_ptr<Entity> entity, shared_ptr<Geometry> geometry, int gridId);

    /**
     * removes all the geometries of the entity from the cell
     * @param entity to remove
     */
    void removeEntity(shared_ptr<Entity> entity);

    /**
     * removes all ways curves from the cell from both vectors
     * @param entity way to remove
     */
    void removeWay(shared_ptr<Entity> entity);

    /**
     * clears all the entities from the cell
     */
    void clear();

    /**
     * gets the number of geometries of the cell
     * @return amount of entities in cell
     */
    size_t getEntityCount(){ return cellEntities.size(); };

};

