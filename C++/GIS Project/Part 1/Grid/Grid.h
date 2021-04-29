#pragma once

#include "vector"
#include "unordered_map"
#include "../GISNamedTypes.h"
#include "GridCell.h"
#include "GridCoordinates.h"

using std::unordered_map;
using std::vector;

/**
 * The grid that stores the entities on the map using their coordinates
 */
class Grid {
    double precision;
    unordered_map<GridCoordinates, GridCell> gridMapping;
    GridCell emptyCell;
	
public:
    explicit Grid(double _precision): precision(_precision), gridMapping(), emptyCell() {};

    /**
   * The function gets a new Entity and inserts it into all the zones it occupies
   * A new cell is created if this is the first Entity in it
   * @param entity - the Entity to insert to the Grid
   */
	void setEntityOnGrid(shared_ptr<Entity> entity);

    /**
     * the function gets an existing Entity and removes it from all the zones it occupies
     * @param entity - the entity to remove
     */
	void removeEntity(shared_ptr<Entity> entity);

	/**
	 * the function gets GridCoordinates and returns all entites on the GridCell that is the value to
	 * the GridCoordinates key in gridMapping
	 * @param cords - GridCoordinates of a cell
	 * @return a vector of all GridEntities that were mapped to the GridCell of cords
	 */
    const vector<GridEntity>& getEntities(const GridCoordinates& cords) const;

    /**
	 * the function gets GridCoordinates and returns all Ways on the GridCell that is the value to
	 * the GridCoordinates key in gridMapping
	 * @param cords - GridCoordinates of a cell
	 * @return a vector of all Ways GridEntities that were mapped to the GridCell of cords
	 */
    const vector<GridEntity>& getWays(const GridCoordinates& cords) const;

    /**
     * removes all the entries from the grid
     */
    void clear(){
        gridMapping.clear();
    }
};