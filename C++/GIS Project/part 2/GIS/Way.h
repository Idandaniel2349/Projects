//
// Created by idan on 14/11/2020.
//

#pragma once

#include "vector"
#include "string"
#include "unordered_map"
#include "MapEntities/Entity.h"
#include "../Common/GISNamedTypes.h"
#include "../Common/AbstractWay.h"

using std::vector;
using std::string;
using std::unordered_map;

enum direction{bidirectional=1, unidirectional=2};
enum restricted{public_transportation=1,trucks=2,cars=3,motorcycles=4,horses=5,bicycles=6,pedestrians=7};

/**
 * A class representing  a Way in the GIS
 */
class Way:public Entity, public AbstractWay{
    EntityId from;
    Coordinates fromCords;
    EntityId to;
    Coordinates toCords;
    vector<Coordinates> curves;
    direction wayDirection;
    int speed_limit;
    bool toll_road;
    vector<restricted> restrictions;
    bool highway;
    mutable Meters length;
public:
    Way(const EntityId& _id, const string& _name, const string& _description, const vector<string>& _categoryTags,
        const vector<shared_ptr<Geometry>>& _geometries,const EntityId& _from,const Coordinates& _fromCords,const EntityId& _to,
        const Coordinates& _toCords, const vector<Coordinates>& _curves,direction _direction,int _speed_limit,
        bool _toll_road,const vector<restricted>& _restrictions);

    const EntityId& getFrom() const {return from;};

    const EntityId& getTO() const {return  to;};

    const Coordinates& getFromJunctionCoordinates() const override {return fromCords;};
    const Coordinates& getToJunctionCoordinates() const override {return toCords;};

    const vector<Coordinates>& getCurves() const {return curves;};

    const direction& getDirection() const{return  wayDirection;};

    int getSpeedLimit() const override{return speed_limit;};

    bool isToll() const override{return toll_road;};

    bool isBidirectional() const override {return wayDirection==bidirectional;};

    const vector<restricted>& getRestrictions() const {return restrictions;};

    Meters getLength() const override;

    bool isHighway() const override{return highway;};

    std::pair<EntityId, EntityId> getJunctions() const override{return std::pair<EntityId, EntityId>(from,to);};

    static direction stringToDirection(const string& s);

    static string directionToString(direction d);

    static restricted stringToRestriction(const string& s);

    static string restrictionToString(restricted r);

    std::pair<Meters, Meters> // = {from-junction->point, point->to-junction}
    getSegmentPartsOnWay(std::size_t segment, const Coordinates& c) const override;

    const EntityId &getId() const override;

};


