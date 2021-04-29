//
// Created by idan on 19/11/2020.
//

#pragma once
#include "../GISNamedTypes.h"
#include "string"
#include "vector"
#include "../Geometry/Geometry.h"
#include "memory"

using std::string;
using std::vector;
using std::move;
using std::shared_ptr;

enum EntityType{poi=1,junction=2,way=3};

/**
 * A class representing  an Entity in the GIS
 */
class Entity{
protected:
    EntityType type;
    EntityId id;
    string name;
    string description;
    vector<string> categoryTags;
    vector<shared_ptr<Geometry>> geometries;

    //Entity is a base class, Shouldn't be able to create an object of type Entity
    Entity(EntityType _type, const EntityId& _id, const string& _name, const string& _description, const vector<string>& _categoryTags, const vector<shared_ptr<Geometry>>& _geometries):
            type(_type), id(_id), name(_name), description(_description), categoryTags(_categoryTags), geometries(_geometries){};

public:
    virtual~Entity() = default;

    const EntityType &getType() const{return type;} ;

    const EntityId &getId() const {return  id;};

    const string &getName() const{return name;};

    const string &getDescription() const{return description;};

    const vector<string> &getCategoryTags() const{return categoryTags;};

    const vector<shared_ptr<Geometry>> &getGeometries() const{return geometries;};


    /**
    * Find the closest point on the entity perimeter to the given point
    * @param cord - the given Coordinate
    * @return The closest point on Entity perimeter to cord
    */
    Coordinates closestPoint(const Coordinates& cord) const;
};


