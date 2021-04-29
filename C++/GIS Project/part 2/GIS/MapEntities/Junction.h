//
// Created by sagig on 11/20/2020.
//
#pragma once

#include "Entity.h"

/**
 * A class representing  a Junction in the GIS
 */
class Junction: public Entity{
    Coordinates location;
public:
    Junction(const EntityId& _id, const string& _name, const string& _description, const vector<string>& _categoryTags,
             const vector<shared_ptr<Geometry>>& _geometries, Coordinates _location):
             Entity(EntityType::junction, _id,_name,_description,_categoryTags,_geometries), location(_location){};

    const Coordinates& getLocation() const{return location;};

};