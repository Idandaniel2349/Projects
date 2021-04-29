//
// Created by sagig on 11/25/2020.
//

#include "GridCell.h"

void GridCell::insertEntity(shared_ptr<Entity> entity, shared_ptr<Geometry> geometry, int gridId) {
    GridEntity newGridEntity = GridEntity(entity, gridId, geometry);
    cellEntities.push_back(newGridEntity);
}

void GridCell::insertWay(shared_ptr<Entity> entity, shared_ptr<Geometry> geometry, int gridId) {
    GridEntity newGridEntity = GridEntity(entity, gridId, geometry);
    cellEntities.push_back(newGridEntity);
    cellWays.push_back(newGridEntity);
}

void GridCell::removeEntity(shared_ptr<Entity> entity) {
    const auto newEnd = std::remove_if(cellEntities.begin(), cellEntities.end(),
                                       [entity](const auto &vecEntity) {
                                           return vecEntity.getEntity()->getEntityId() == entity->getEntityId();
                                       }
    );
    cellEntities.erase(newEnd, cellEntities.end());
}

void GridCell::removeWay(shared_ptr<Entity> entity) {
    const auto newEnd = std::remove_if(cellWays.begin(), cellWays.end(),
                                       [entity](const auto &vecEntity) {
                                           return vecEntity.getEntity()->getEntityId() == entity->getEntityId();
                                       }
    );
    cellWays.erase(newEnd, cellWays.end());
    removeEntity(entity);
}

void GridCell::clear() {
    cellEntities.clear();
    cellWays.clear();
}

const vector<GridEntity> &GridCell::getCellEntities() const {
    return cellEntities;
}

const vector<GridEntity> &GridCell::getCellWays() const {
    return cellWays;
}