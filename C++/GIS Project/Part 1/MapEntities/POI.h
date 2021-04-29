//
// Created by idan on 14/11/2020.
//

#pragma once

#include "../GISNamedTypes.h"
#include "../Geometry/Geometry.h"
#include "Entity.h"
#include "string"
#include "unordered_map"



using std::string;
using std::unordered_map;

enum accessibility {by_car=1,by_foot=2,by_wheelchair=3};

/**
 * A class representing  a POI in the GIS
 */
class POI:public Entity {
    vector<accessibility> access;
public:
    POI(const EntityId& _id, const string& _name, const string& _description, const vector<string>& _categoryTags,
        const vector<shared_ptr<Geometry>>& _geometries, const vector<accessibility>& _access):
                Entity(EntityType::poi, _id,_name,_description,_categoryTags,_geometries),access(_access){};

    const vector<accessibility>& getAccessibility()  const{return access;};

    static string accessibilityToString(accessibility a);

    static accessibility stringToAccessibility(const string& s);
};

