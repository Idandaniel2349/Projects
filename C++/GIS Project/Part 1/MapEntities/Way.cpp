//
// Created by idan on 14/11/2020.
//

#include "Way.h"

Way::Way(const EntityId &_id, const string &_name, const string &_description, const vector<string> &_categoryTags,
         const vector<shared_ptr<Geometry>> &_geometries, const EntityId &_from, const Coordinates& _fromCords, const EntityId &_to,
         const Coordinates& _toCords,const vector<Coordinates> &_curves, direction _direction, int _speed_limit,
         bool _toll_road,const vector<restricted> &_restrictions):
        Entity(EntityType::way,_id,_name,_description,_categoryTags,_geometries), from(_from),fromCords(_fromCords),
        to(_to), toCords(_toCords), curves(_curves),wayDirection(_direction), speed_limit(_speed_limit),
        toll_road(_toll_road), restrictions(_restrictions), length(-1)
{
    if (std::find(categoryTags.begin(), categoryTags.end(), "highway") != categoryTags.end()) {
        highway=true;
    }
    else {
        highway=false;
    }
}
direction Way::stringToDirection(const string& s)
{
    static unordered_map<string, direction> mapper = {{"unidirectional", direction::unidirectional},
                                                      {"bidirectional", direction::bidirectional},
    };

    return mapper[s];
}


string Way::directionToString(direction d)
{
    static unordered_map<direction, string> mapper = {{direction::unidirectional, "unidirectional"},
                                               {direction::bidirectional, "bidirectional"},
    };

    return mapper[d];
}

restricted Way::stringToRestriction(const string& s)
{
    static unordered_map<string, restricted> mapper = {{"public transportation", public_transportation},
                                                       {"trucks", trucks},
                                                       {"cars", cars},
                                                       {"motorcycles", motorcycles},
                                                       {"horses", horses},
                                                       {"bicycles", bicycles},
                                                       {"pedestrians", pedestrians}};
    return mapper[s];
}

string Way::restrictionToString(restricted r)
{
static unordered_map<restricted, string> mapper = {{public_transportation, "public_transportation"},
                                                   {trucks, "trucks"},
                                                   {cars, "cars"},
                                                   {motorcycles, "motorcycles"},
                                                   {horses, "horses"},
                                                   {bicycles, "bicycles"},
                                                   {pedestrians, "pedestrians"}};

return mapper[r];
}

Meters Way::getLength() const {
    if(length!=(Meters)-1){
        return length;
    }

    double CurrentLength=0;
    Coordinates currentCoordinate=fromCords;

    for(const auto & curve:curves){
        CurrentLength+=(double)CoordinatesMath::calculateDistance(currentCoordinate,curve);
        currentCoordinate=curve;
    }

    CurrentLength+=(double)CoordinatesMath::calculateDistance(currentCoordinate,toCords);

    length=(Meters)CurrentLength;

    return length;
}



