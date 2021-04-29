//
// Created by idan on 19/11/2020.
//
#pragma once

#include "vector"
#include "stdexcept"
#include "MapEntities/Entity.h"
#include "string"
#include "Logger.h"
#include "unordered_map"
#include "rapidjson/document.h"// rapidjson's DOM-style API
#include "BasicEntityInfo.h"
#include "MapEntities/Junction.h"
#include "MapEntities/POI.h"
#include "MapEntities/Way.h"
#include "Grid/Grid.h"
#include "memory"
#include "BasicWayInfo.h"

using std::unordered_map;
using std::vector;
using std::string;
using std::make_shared;
using std::dynamic_pointer_cast;
using std::shared_ptr;
using rapidjson::Value;
using rapidjson::Document;


/**
 * A class Taking care of all Json related functions
 */
class JsonHandler{
    Logger* logger;
    //A pointer to the member in GIS to check if Id already exists
    unordered_map<EntityId,shared_ptr<Entity>>* entityById;
    //A pointer to the GIS logger
    int idCounter;
    unordered_map<EntityId, shared_ptr<Junction>> addedJunctions;

    /**
     * the functions checks whatever we need to load Id's to file or not by whether the first entry has an id or not
     * @param doc - the json document to load from
     * @return - if we should generate Id's for doc
     */
    bool shouldGenerateId(Document& doc);
    /**
     * the function will generate an EntityId and return it
     * @return - a nonexistent EntityId
     */
    EntityId generateId();
    /**
     * the function will save basic Entity info
     * @param itr - info to make Basic Entity Info from
     * @param shouldGenerateId - if we should generate an Id
     * @param info - object to write the entity info to
     * @return - if we successflly made itr to info object
     */
    bool createBasicEntityInfo(rapidjson::GenericValue<rapidjson::UTF8<>>::ConstValueIterator const &itr, bool shouldGenerateId,
                               BasicEntityInfo& info);

    /**
     * given the way entity from and to junction ids and it's json object generates the ways geometry and curves
     * @param from junction from id
     * @param to junction to id
     * @param itr the way json object
     * @return the ways geometries and json
     */
    BasicWayInfo getRoadGeometryAndCurves(const EntityId& from,
                                                                               const EntityId& to, Value::ConstValueIterator &itr);
    /**
     *  create a Junction entity from itr
     * @param itr - info to make Junction from
     * @param shouldGenerateId - if we should generate an Id
     * @return - the Junction Entity created
     */
    shared_ptr<Entity> createJunction(Value::ConstValueIterator& itr, bool shouldGenerateId);
    /**
     * create a POI entity from itr
     * @param itr - POI entity to make
     * @param shouldGenerateId -  if we should generate an Id
     * @return - the POI Entity created
     */
    shared_ptr<Entity> createPOI(Value::ConstValueIterator& itr, bool shouldGenerateId);
    /**
     * create a Way entity from itr
     * @param itr - Way entity to make
     * @param shouldGenerateId - if we should generate an Id
     * @return - the Way Entity created
     */
    shared_ptr<Entity> createWay(Value::ConstValueIterator& itr, bool shouldGenerateId);
    /**
     * creates a vector of entities from the json document
     * @param entities the vector to fill
     * @param doc json doc
     */
    void createEntities(vector<shared_ptr<Entity>> &entities, Document &doc);

    /**
     * saves BasicEntityInfo into object
     * @param entity - entity to save
     * @param object - object to save the entity info to
     * @param allocator - allocator for the json objects
     */
    void  saveBasicEntityInfo(shared_ptr<Entity> entity,Value& object,Document::AllocatorType& allocator);
    /**
     * make and saves POI object of entity to arrayOfObjects
     * @param entity - POI entity to save
     * @param arrayOfObjects - the json array to save entity to
     * @param allocator - allocator for the json objects
     */
    void savePOIInfo(shared_ptr<Entity> entity,Value& arrayOfObjects,Document::AllocatorType& allocator);
    /**
     * save Accessibility info to POIObject
     * @param accessVector -access field to save to POIObject
     * @param POIObject - object to save to
     * @param allocator - allocator for the json objects
     */
    void saveAccessibilityInfo(vector<accessibility>& accessVector,Value& POIObject,Document::AllocatorType& allocator);
    /**
     * save Circle info to circleObject
     * @param circle - circle field to save to POIObject
     * @param circleObject - object to save to
     * @param allocator - allocator for the json objects
     */
    void saveCircleInfo(shared_ptr<Circle> circle,Value& circleObject,Document::AllocatorType& allocator);
    /**
     * make and saves Junction object of entity to arrayOfObjects
     * @param entity - Junction entity to save
     * @param arrayOfObjects - the json array to save entity to
     * @param allocator - allocator for the json objects
     */
    void saveJunctionInfo(shared_ptr<Entity> entity,Value& arrayOfObjects,Document::AllocatorType& allocator);
    /**
     * make and saves Way object of entity to arrayOfObjects
     * @param entity - Way entity to save
     * @param arrayOfObjects - the json array to save entity to
     * @param allocator - allocator for the json objects
     */
    void saveWayInfo(shared_ptr<Entity> entity,Value& arrayOfObjects,Document::AllocatorType& allocator);
    /**
     * save Curves info to WayObject
     * @param curvesVector - curves field to save to WayObject
     * @param WayObject - object to save to
     * @param allocator - allocator for the json objects
     */
    void saveCurvesInfo(vector<Coordinates> curvesVector,Value& WayObject,Document::AllocatorType& allocator);
    /**
     * save restrictions info to WayObject
     * @param restrictionsVector - restrictions field to save to WayObject
     * @param WayObject - object to save to
     * @param allocator - allocator for the json objects
     */
    void saveRestrictionsInfo(vector<restricted> restrictionsVector,Value& WayObject,Document::AllocatorType& allocator);

    //these are just function to get a field frm a json safely without crashing everything
    /**
     * gets a string value from the specified field
     * @param itr the json obj
     * @param field the field to get the value from
     * @return the string in the field
     */
    string getStringValue(rapidjson::GenericValue<rapidjson::UTF8<>>::ConstValueIterator const &itr, const char *field) const;
    /**
     * gets int from field in itr
     * @param itr json obj
     * @param field field that contians int
     * @return the int in the field
     */
    int getIntValue(rapidjson::GenericValue<rapidjson::UTF8<>>::ConstValueIterator const &itr, const char *field) const;
    /**
     * gets a bool value from a field in json obj
     * @param itr json obj
     * @param field bool field
     * @return bool in field
     */
    bool getBoolValue(rapidjson::GenericValue<rapidjson::UTF8<>>::ConstValueIterator const &itr, const char *field) const;
    /**
     * gets an array obj from field in json obj
     * @param itr json obj
     * @param field array field
     * @return array in field
     */
    rapidjson::GenericValue<rapidjson::UTF8<>>::ConstArray getArrayValue(rapidjson::GenericValue<rapidjson::UTF8<>>::ConstValueIterator const &itr, const char *field) const;
    /**
     * gets object from json obj
     * @param itr json obj
     * @param field obj field
     * @return the obj in field
     */
    rapidjson::GenericValue<rapidjson::UTF8<>>::ConstObject getObectValue(rapidjson::GenericValue<rapidjson::UTF8<>>::ConstValueIterator const &itr, const char *field) const;
    /**
     * gets the string in the current json obj
     * @param itr json oj
     * @return string in obj
     */
    string getStringValue(rapidjson::GenericValue<rapidjson::UTF8<>>::ConstValueIterator const &itr) const;
    /**
     * gets double val in current obj
     * @param val current obj
     * @return double in obj
     */
    double getDoubleValue(Value::ValueType const& val) const;
    /**
     * gets array obj in current obj
     * @param itr current obj
     * @return array in obj
     */
    rapidjson::GenericValue<rapidjson::UTF8<>>::ConstArray
    getArrayValue(rapidjson::GenericValue<rapidjson::UTF8<>>::ConstValueIterator const &itr) const;
    /**
     * get strubg value in field in json obj
     * @param val json obj
     * @param field field name in obj
     * @return string in field
     */
    string getStringValue(const rapidjson::GenericValue<rapidjson::UTF8<>>::ConstObject &val, const char *field) const;
    /**
     * get arrray in field in json obj
     * @param val json obj
     * @param field array field
     * @return array in field
     */
    rapidjson::GenericValue<rapidjson::UTF8<>>::ConstArray
    getArrayValue(const rapidjson::GenericValue<rapidjson::UTF8<>>::ConstObject &val, const char *field) const;
    /**
     * gets double value in field in json obj
     * @param val json obj
     * @param field field name
     * @return double in field
     */
    double getDoubleValue(const rapidjson::GenericValue<rapidjson::UTF8<>>::ConstObject &val, const char *field) const;

public:
    JsonHandler(Logger* _logger, unordered_map<EntityId,shared_ptr<Entity>>* _entityById): logger(_logger), entityById(_entityById),
    idCounter(0), addedJunctions(){};

    /**
     * load Entities from file to a vector
     * @param file - file to load from
     * @return - the entities loaded
     */
    vector<shared_ptr<Entity>> loadEntities(const string& file);

    /**
     * save entities to file
     * @param file - file to save to
     * @param entities - entities to save to file
     * @return - the number of entities saved
     */
    std::size_t saveEntities(const string& file,const vector<shared_ptr<Entity>>& entities);



};

struct MissingJsonFieldException : std::runtime_error{
    MissingJsonFieldException(const char* msg): std::runtime_error(msg) {}

    MissingJsonFieldException(const string& msg): std::runtime_error(msg) {}
};

struct FieldWrongTypeException : std::runtime_error{
    FieldWrongTypeException(const char* msg): std::runtime_error(msg) {}

    FieldWrongTypeException(const string& msg): std::runtime_error(msg) {}
};

struct CoordinatesOutOfBoundsException : std::runtime_error{
    CoordinatesOutOfBoundsException(const char* msg): std::runtime_error(msg) {}

    CoordinatesOutOfBoundsException(const string& msg): std::runtime_error(msg) {}
};
