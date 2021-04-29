//
// Created by sagig on 11/25/2020.
//

#include "Grid.h"

void Grid::setEntityOnGrid(shared_ptr<Entity> entity) {
    int idCounter = 0;
    for(auto& geometry : entity->getGeometries()){
        //get the cells the entity geometry occupy
        auto relevantCellCords = geometry->getCellsOfGeometry(precision);

        for(auto& cellCord : relevantCellCords){
            if(!gridMapping.contains(cellCord)){// check if we need to make a new key
                gridMapping[cellCord] = GridCell();
            }
            if(entity->getType() == EntityType::way){
                gridMapping[cellCord].insertWay(entity, geometry, idCounter);
            }
            else{
                gridMapping[cellCord].insertEntity(entity, geometry, idCounter);
            }
        }
        ++idCounter;
    }
}

void Grid::removeEntity(shared_ptr<Entity> entity){
    for(auto& geometry : entity->getGeometries()){
        //get the cells the entity geometry occupy
        auto relevantCellCords = geometry->getCellsOfGeometry(precision);

        for(auto& cellCord : relevantCellCords){
            if(gridMapping.contains(cellCord)){//if the cell exists remove the entity from there
                if(entity->getType() == EntityType::way){
                    gridMapping[cellCord].removeWay(entity);
                }
                else{
                    gridMapping[cellCord].removeEntity(entity);
                }
                if(gridMapping[cellCord].getEntityCount() == 0){// if cell is now empty remove it
                    gridMapping.erase(cellCord);
                }
            }
        }
    }
}

const vector<GridEntity>& Grid::getEntities(const GridCoordinates& cords) const{
    if(gridMapping.contains(cords)){
        return gridMapping.at(cords).getCellEntities();
    }
    return emptyCell.getCellEntities();
}

const vector<GridEntity>& Grid::getWays(const GridCoordinates& cords) const{
    if(gridMapping.contains(cords)){
        return gridMapping.at(cords).getCellWays();
    }
    return emptyCell.getCellWays();
}