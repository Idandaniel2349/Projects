//
// Created by idan on 19/11/2020.
//

#include "JsonHandler.h"
#include "rapidjson/prettywriter.h"
#include <iostream>

using rapidjson::Document;
using rapidjson::ParseResult;
using rapidjson::Value;
using rapidjson::StringBuffer;

bool JsonHandler::shouldGenerateId(Document& doc)
{
    Value::ConstValueIterator itr = doc.Begin();
    if(itr != doc.End() && (*itr).HasMember("id")){
        return false;
    }
    return true;
}

EntityId JsonHandler::generateId()
{
    EntityId id = EntityId("");
    do {
        ++idCounter;
        id = EntityId(std::to_string(idCounter));
    }
    while (entityById->contains(id));

    return id;
}

bool JsonHandler::createBasicEntityInfo(Value::ConstValueIterator const &itr,
                                    bool shouldGenerateId, BasicEntityInfo& info) {
    EntityId id = EntityId("");
    string description;
    string name;
    std::vector<string> categoryTags;

    if (shouldGenerateId) {
        if ((*itr).HasMember("id")) {
            logger->log("Got entity with defined id when expected id to be undefined. Ignoring entity");
            return false;
        }
        id = generateId();
    } else {
        if (!(*itr).HasMember("id")) {
            logger->log("Got entity with undefined id when expected id to be defined. Ignoring entity");
            return false;
        }
        id = EntityId(getStringValue(itr, "id"));
    }

    name = getStringValue(itr, "name");
    if((*itr).HasMember("description"))
    {
        description = getStringValue(itr, "description");
    }
    if((*itr).HasMember("category_tags"))
    {
        auto categories = getArrayValue(itr, "category_tags");
        for(auto categoryItr = categories.begin(); categoryItr < categories.end(); ++categoryItr){
            categoryTags.push_back(getStringValue(categoryItr));
        }
    }

    info = BasicEntityInfo(id, name, description, categoryTags);

    return true;
}

BasicWayInfo JsonHandler::getRoadGeometryAndCurves(const EntityId& from, const EntityId& to, Value::ConstValueIterator &itr)
{
    vector<shared_ptr<Geometry>> geometries;
    vector<Coordinates> curves;
    if((addedJunctions.contains(from) || entityById->contains(from)) &&
       (addedJunctions.contains(to) || entityById->contains(to))){
        Coordinates fromCords((Longitude)0,(Latitude)0);
        Coordinates toCords((Longitude)0,(Latitude)0);
        if(addedJunctions.contains(from)){
            fromCords = addedJunctions.at(from)->getLocation();
        }
        else{
            if(entityById->at(from)->getType() == junction){
                fromCords = dynamic_pointer_cast<Junction>(entityById->at(from))->getLocation();
            }
            else{
                logger->log("Road from has to have junction ID");
                return BasicWayInfo();
            }
        }
        if(addedJunctions.contains(to)){
            toCords = addedJunctions.at(to)->getLocation();
        }
        else{
            if(entityById->at(to)->getType() == junction){
                toCords = dynamic_pointer_cast<Junction>(entityById->at(to))->getLocation();
            }
            else{
                logger->log("Road to has to have junction ID");
                return BasicWayInfo();
            }
        }
        Coordinates currentPoint = fromCords;
        if((*itr).HasMember("curves")){
            auto curvesArray = getArrayValue(itr, "curves");
            for(auto curveItr = curvesArray.begin(); curveItr < curvesArray.end(); curveItr++){
                auto cordsVaules =getArrayValue(curveItr);
                if(cordsVaules.Size() == 2) {
                    double longitudeVal = getDoubleValue(cordsVaules[0]);
                    double latitudeVal = getDoubleValue(cordsVaules[1]);
                    if(longitudeVal <= 180 && longitudeVal >= -180 && latitudeVal <= 90 && latitudeVal >= -90)
                    {
                        Coordinates newCords = Coordinates(Longitude(longitudeVal),
                                                           Latitude(latitudeVal));
                        geometries.push_back(make_shared<Line>(currentPoint, newCords));
                        curves.push_back(newCords);
                        currentPoint = newCords;
                    }
                    else{
                        throw CoordinatesOutOfBoundsException("Got coordinates with illegal values. ignoring entity");
                    }

                }
                else{
                    throw FieldWrongTypeException("expected 2 in array describing coordinates");
                }
            }
        }
        geometries.push_back(make_shared<Line>(currentPoint, toCords));
        return BasicWayInfo(toCords, fromCords, geometries, curves);
    }
    else{
        logger->log("Way contains junction ids that do not exist. skipping entries");
        return BasicWayInfo();
    }
}

shared_ptr<Entity> JsonHandler::createJunction(Value::ConstValueIterator &itr, bool shouldGenerateId)
{
    try {
        vector<shared_ptr<Geometry>> geometries;
        Coordinates cords((Longitude)0,(Latitude)0);
        auto cordsElem = getArrayValue(itr, "coordinates");
        if(cordsElem.Size() != 2){
            throw FieldWrongTypeException("expected 2 in array describing coordinates");
        }
        double longitudeVal = getDoubleValue(cordsElem[0]);
        double latitudeVal = getDoubleValue(cordsElem[1]);
        if(longitudeVal <= 180 && longitudeVal >= -180 && latitudeVal <= 90 && latitudeVal >= -90) {
            cords = Coordinates(Longitude(longitudeVal),
                                            Latitude(latitudeVal));
            geometries.push_back(make_shared<Point>(cords));
        }
        else{
            throw CoordinatesOutOfBoundsException("Got coordinates with illegal values. ignoring entity");
        }

        BasicEntityInfo info;
        if(!createBasicEntityInfo(itr, shouldGenerateId, info))
        {
            return nullptr;
        }
        auto newJunction = make_shared<Junction>(info.id, info.name, info.description, info.categoryTags, geometries, cords);
        addedJunctions.insert({newJunction->getId(), newJunction});
        return newJunction;

    }
    catch (const std::exception& e) {
        string msg = "Got an exception while trying to create junction entity. Exception message is ";
        logger->log(msg + e.what());
        return nullptr;
    }

}

shared_ptr<Entity> JsonHandler::createPOI(Value::ConstValueIterator &itr, bool shouldGenerateId)
{
    try {
        vector<accessibility> access;
        vector<shared_ptr<Geometry>> geometries;
        if((*itr).HasMember("accessibility")) {
            auto accessArray = getArrayValue(itr, "accessibility");
            for (auto accIter = accessArray.begin(); accIter < accessArray.end(); accIter++) {
                access.push_back(POI::stringToAccessibility(getStringValue(accIter)));
            }
        }
        auto geometryField = getObectValue(itr, "geometry");
        string geometryType = getStringValue(geometryField, "type");
        if(geometryType == "Circle")
        {
            auto cordsElem = getArrayValue(geometryField, "coordinates");
            if(cordsElem.Size() != 2){
                throw FieldWrongTypeException("expected 2 in array describing coordinates");
            }
            double longitudeVal = getDoubleValue(cordsElem[0]);
            double latitudeVal = getDoubleValue(cordsElem[1]);
            if(longitudeVal <= 180 && longitudeVal >= -180 && latitudeVal <= 90 && latitudeVal >= -90) {
                Coordinates center = Coordinates(Longitude(longitudeVal),
                                                 Latitude(latitudeVal));

                Meters radius = Meters(getDoubleValue(geometryField, "radius"));
                geometries.push_back(make_shared<Circle>(center, radius));
            } else{
                throw CoordinatesOutOfBoundsException("Got coordinates with illegal values. ignoring entity");
            }
        }
        else
        {
            logger->log("Unsupported geometry type " + geometryType + ". Ignoring entry");
            return nullptr;
        }
        BasicEntityInfo info;
        if(!createBasicEntityInfo(itr, shouldGenerateId, info))
        {
            return nullptr;
        }
        return make_shared<POI>(info.id, info.name, info.description, info.categoryTags, geometries, access);


    }
    catch (const std::exception& e) {
        string msg = "Got an exception while trying to create POI entity. Exception message is ";
        logger->log(msg + e.what());
        return nullptr;
    }
}

shared_ptr<Entity> JsonHandler::createWay(Value::ConstValueIterator &itr, bool shouldGenerateId)
{
    try {
        const auto fromObj = getObectValue(itr, "from");
        const auto toObj = getObectValue(itr, "to");
        const EntityId from = EntityId(getStringValue(fromObj, "entity_id"));
        const EntityId to = EntityId(getStringValue(toObj, "entity_id"));
        const auto wayInfo = getRoadGeometryAndCurves(from, to, itr);
        if(wayInfo.geometries.empty()){
            return nullptr;
        }
        direction wayDirection = Way::stringToDirection(getStringValue(itr,"direction"));
        vector<restricted> restrictions;
        if((*itr).HasMember("restricted")){
            auto restrictedArray = getArrayValue(itr,"restricted");
            for(auto restrictionIter = restrictedArray.begin(); restrictionIter < restrictedArray.end(); restrictionIter++){
                restrictions.push_back(Way::stringToRestriction(getStringValue(restrictionIter)));
            }
        }
        const bool tollRoad = getBoolValue(itr, "toll_road");
        const int speedLimit = getIntValue(itr, "speed_limit");
        BasicEntityInfo info;
        if(!createBasicEntityInfo(itr, shouldGenerateId, info))
        {
            return nullptr;
        }
        return make_shared<Way>(info.id, info.name, info.description, info.categoryTags, wayInfo.geometries,
                       from, wayInfo.fromCords, to, wayInfo.toCords, wayInfo.curves, wayDirection, speedLimit, tollRoad, restrictions);

    }
    catch (const std::exception& e) {
        string msg = "Got an exception while trying to create way entity. Exception message is ";
        logger->log(msg + e.what());
        return nullptr;
    }
}

vector<shared_ptr<Entity>> JsonHandler::loadEntities(const string& file)
{
    vector<shared_ptr<Entity>> entities;
    std::ifstream iFile(file, std::ios::ate);
    try {

        if (!iFile) {
            logger->log("Unable to open file" + file + " to load entities");
            return entities;
        }

        std::streamsize size = iFile.tellg();
        iFile.seekg(0, std::ios::beg);
        std::vector<char> buffer(size);
        if (!iFile.read(buffer.data(), size)) {
            logger->log("Unable to read file " + file + " to load entities");
            return entities;
        }
        iFile.close();
        if (buffer[size - 1] != 0) {
            buffer.push_back(0);
        }
        Document doc;
        ParseResult parseResult = doc.Parse(buffer.data());
        if (!parseResult) {
            logger->log("Unable to parse file " + file + " as json");
            return entities;
        }
        if (!doc.IsArray()) {
            logger->log("Json in file " + file + " should have an array root");
            return entities;
        }
        createEntities(entities, doc);
        addedJunctions.clear();
    }
    catch (const std::exception& e) {
        string msg = "Got an exception while trying to load entities ";
        logger->log(msg + e.what());
        iFile.close();
    }
    return entities;
}

void JsonHandler::createEntities(vector<shared_ptr<Entity>> &entities, Document &doc) {
    bool idGenerated = shouldGenerateId(doc);
    for (Value::ConstValueIterator itr = doc.Begin(); itr != doc.End(); ++itr) {
        if (itr->HasMember("type")) {
            string entityType = getStringValue(itr,"type");
            shared_ptr<Entity> e = nullptr;
            if(entityType == "POI")
            {
                e = createPOI(itr, idGenerated);

            } else if(entityType == "Way")
            {
                e = createWay(itr, idGenerated);
            } else if(entityType == "Junction")
            {
                e = createJunction(itr, idGenerated);
            } else
            {
                logger->log("Ignoring entity of unknown type " + entityType);
            }
            if(e)
            {
                entities.push_back(e);
            }
        }
        else
        {
            logger->log("Encountered entry with no type. skipping to next entry");
        }
    }
}

std::size_t JsonHandler::saveEntities(const string& file,const vector<shared_ptr<Entity>>& entities){
    ofstream saveFile;
    shared_ptr<Entity> currEntity;
    std::size_t numSaved=0;

    //open file
    saveFile.open(file,std::ios::out);
    if(!saveFile){
        logger->log("Unable to open file" + file + " to save entities");
        return numSaved;
    }

    //create document for allocator
    Document document;
    document.SetObject();
    Document::AllocatorType& allocator = document.GetAllocator();

    Value arrayOfObjects(rapidjson::kArrayType);

    for(auto itr = entities.cbegin(); itr != entities.cend(); ++itr){
        currEntity=*itr;
        numSaved++;
        switch (currEntity->getType()) {
            case poi:
                savePOIInfo(currEntity,arrayOfObjects,allocator);
                break;
            case junction:
                saveJunctionInfo(currEntity,arrayOfObjects,allocator);
                break;
            case way:
                saveWayInfo(currEntity,arrayOfObjects,allocator);
                break;
        }
    }

    try {
        //write to saveFile
        StringBuffer buf;
        rapidjson::PrettyWriter<StringBuffer> writer(buf);
        arrayOfObjects.Accept(writer);
        string json(buf.GetString(), buf.GetSize());
        saveFile << json;
    }
    catch(...){
        logger->log("Unable to write to file" + file + " to save entities");
        return 0;
    }
    saveFile.close();
    return numSaved;

}

void JsonHandler::saveBasicEntityInfo(shared_ptr<Entity> entity,Value& object,Document::AllocatorType& allocator){
    string str;
    Value strVal;
    Value categoryArray;
    categoryArray.SetArray();

    //add id
    str=entity->getId();
    strVal.SetString(str.c_str(),str.length(),allocator);
    object.AddMember("id",strVal,allocator);

    //add name
    str=entity->getName();
    strVal.SetString(str.c_str(),str.length(),allocator);
    object.AddMember("name",strVal,allocator);

    //add Description
    str=entity->getDescription();
    strVal.SetString(str.c_str(),str.length(),allocator);
    object.AddMember("description", strVal, allocator);

    //add Category_tags
    vector<string> categoryVector=entity->getCategoryTags();
    if(!categoryVector.empty()) {
        for (auto &it:categoryVector) {
            str = it;
            strVal.SetString(str.c_str(), str.length(), allocator);
            categoryArray.PushBack(strVal, allocator);
        }
        object.AddMember("category_tags", categoryArray, allocator);
    }
}

void JsonHandler::savePOIInfo(shared_ptr<Entity> entity,Value& arrayOfObjects,Document::AllocatorType& allocator) {
    Value POIObject(rapidjson::kObjectType);
    shared_ptr<Circle>  circle;
    POIObject.AddMember("type","POI",allocator);
    saveBasicEntityInfo(entity,POIObject,allocator);

    //add accessibility to POIObject if exists
    vector<accessibility> accessVector = dynamic_pointer_cast<POI>(entity)->getAccessibility();
    if (!accessVector.empty()) {
        saveAccessibilityInfo(accessVector, POIObject, allocator);
    }


    //add Circle to POIObject
    Value circleObject(rapidjson::kObjectType);
    circle=dynamic_pointer_cast<Circle>(entity->getGeometries()[0]);
    saveCircleInfo(circle,circleObject,allocator);
    POIObject.AddMember("geometry", circleObject, allocator);


    //save POI to arrayOfObjects
    arrayOfObjects.PushBack(POIObject,allocator);
}

void JsonHandler::saveAccessibilityInfo(vector<accessibility> &accessVector, Value &POIObject,Document::AllocatorType &allocator) {
    Value arrayAccess(rapidjson::kArrayType);
    arrayAccess.SetArray();
    string str;
    Value strVal;
    for (auto& it:accessVector) {
        str=POI::accessibilityToString(it);
        strVal.SetString(str.c_str(),str.length(),allocator);
        arrayAccess.PushBack(strVal, allocator);
    }
    POIObject.AddMember("accessibility", arrayAccess, allocator);
}

void JsonHandler::saveCircleInfo(shared_ptr<Circle> circle, Value &circleObject, Document::AllocatorType &allocator) {
    circleObject.AddMember("type", "Circle", allocator);
    Value arrayCoordinates(rapidjson::kArrayType);
    arrayCoordinates.SetArray();
    arrayCoordinates.PushBack((double)(circle->getCenter().longitude()), allocator);
    arrayCoordinates.PushBack((double)(circle->getCenter().latitude()), allocator);
    circleObject.AddMember("coordinates", arrayCoordinates, allocator);
    circleObject.AddMember("radius", (double )(circle->getRadius()), allocator);
}


void JsonHandler::saveJunctionInfo(shared_ptr<Entity> entity,Value& arrayOfObjects,Document::AllocatorType& allocator) {
    Value JunctionObject(rapidjson::kObjectType);
    JunctionObject.AddMember("type","Junction",allocator);
    saveBasicEntityInfo(entity,JunctionObject,allocator);

    //add coordinates to JunctionObject
    Value arrayCoordinates(rapidjson::kArrayType);
    auto point = dynamic_pointer_cast<Point>(entity->getGeometries()[0]);
    arrayCoordinates.PushBack((double )(point->getCoordinates().longitude()), allocator);
    arrayCoordinates.PushBack((double)(point->getCoordinates().latitude()), allocator);
    JunctionObject.AddMember("coordinates", arrayCoordinates, allocator);


    //save Junction to arrayOfObjects
    arrayOfObjects.PushBack(JunctionObject,allocator);
}

void JsonHandler::saveWayInfo(shared_ptr<Entity> entity,Value& arrayOfObjects,Document::AllocatorType& allocator) {
    Value WayObject(rapidjson::kObjectType);
    string str;
    Value strVal;
    WayObject.AddMember("type","Way",allocator);
    saveBasicEntityInfo(entity,WayObject,allocator);

    //add From to WayObject
    Value FromObject(rapidjson::kObjectType);
    str=dynamic_pointer_cast<Way>( entity)->getFrom();
    strVal.SetString(str.c_str(),str.length(),allocator);
    FromObject.AddMember("entity_id", strVal, allocator);
    WayObject.AddMember("from", FromObject, allocator);


    //add To to WayObject
    Value ToObject(rapidjson::kObjectType);
    str=dynamic_pointer_cast<Way>(entity)->getTO();
    strVal.SetString(str.c_str(),str.length(),allocator);
    ToObject.AddMember("entity_id", strVal, allocator);
    WayObject.AddMember("to", ToObject, allocator);


    //add curves to WayObject if exists
    vector<Coordinates> curvesVector = dynamic_pointer_cast<Way>(entity)->getCurves();
    if (!curvesVector.empty()) {
        saveCurvesInfo(curvesVector, WayObject, allocator);
    }


    //add direction to WayObject
    str=Way::directionToString(dynamic_pointer_cast<Way>(entity)->getDirection());
    strVal.SetString(str.c_str(),str.length(),allocator);
    WayObject.AddMember("direction", strVal, allocator);


    WayObject.AddMember("speed_limit", dynamic_pointer_cast<Way>(entity)->getSpeedLimit(), allocator);
    WayObject.AddMember("toll_road", (dynamic_pointer_cast<Way>(entity)->isToll()), allocator);

    //add restrictions to WayObject if exists
    vector<restricted> restrictionsVector=dynamic_pointer_cast<Way>(entity)->getRestrictions();
    if(!restrictionsVector.empty()) {
        saveRestrictionsInfo(restrictionsVector, WayObject, allocator);
    }


    //save Way to arrayOfObjects
    arrayOfObjects.PushBack(WayObject,allocator);

}

void JsonHandler::saveCurvesInfo(vector<Coordinates> curvesVector, Value &WayObject,Document::AllocatorType &allocator) {
    Value arrayCurves(rapidjson::kArrayType);
    arrayCurves.SetArray();
    Value arrayCoordinates(rapidjson::kArrayType);
    arrayCoordinates.SetArray();
    //add each coordinate in the curves to the arrayCurves
    for(auto& it:curvesVector){
        arrayCoordinates.Clear();
        arrayCoordinates.PushBack((double )(it.longitude()),allocator);
        arrayCoordinates.PushBack((double )(it.latitude()),allocator);
        arrayCurves.PushBack(arrayCoordinates,allocator);
    }
    WayObject.AddMember("curves",arrayCurves,allocator);
}

void JsonHandler::saveRestrictionsInfo(vector<restricted> restrictionsVector, Value &WayObject,Document::AllocatorType &allocator) {
    Value arrayRestrictions(rapidjson::kArrayType);
    arrayRestrictions.SetArray();
    string str;
    Value strVal;
    for(auto & it:restrictionsVector){
        str=Way::restrictionToString(it);
        strVal.SetString(str.c_str(),str.length(),allocator);
        arrayRestrictions.PushBack(strVal,allocator);
    }
    WayObject.AddMember("restricted",arrayRestrictions,allocator);
}

string JsonHandler::getStringValue(const Value::ConstValueIterator& itr, const char* field) const{
    string strField = string (field);
    if(!itr->HasMember(field)){
        string message = "Entry did not contain the field " + strField;
        throw MissingJsonFieldException(message);
    }
    if(!(*itr)[field].IsString()){
        string message = "expected field " + strField;
        message += " to be string";
        throw FieldWrongTypeException(message);
    }

    return (*itr)[field].GetString();
}

string JsonHandler::getStringValue(const Value::ConstValueIterator& itr) const{
    if(!(*itr).IsString()){
        throw FieldWrongTypeException("excpected string in iterator");
    }
    return (*itr).GetString();
}

string JsonHandler::getStringValue(const Value::ConstObject & val, const char* field) const{
    string strField = string (field);
    if(!val.HasMember(field)){
        string message = "Entry did not contain the field " + strField;
        throw MissingJsonFieldException(message);
    }
    if(!val[field].IsString()){
        string message = "expected field " + strField;
        message += " to be string";
        throw FieldWrongTypeException(message);
    }

    return val[field].GetString();
}

double JsonHandler::getDoubleValue(const Value::ConstObject & val, const char* field) const{
    string strField = string (field);
    if(!val.HasMember(field)){
        string message = "Entry did not contain the field " + strField;
        throw MissingJsonFieldException(message);
    }
    if(!val[field].IsDouble()){
        if(val[field].IsInt()){
            return val[field].GetInt();
        }
        string message = "expected field " + strField;
        message += " to be double";
        throw FieldWrongTypeException(message);
    }

    return val[field].GetDouble();
}

double JsonHandler::getDoubleValue(const Value::ValueType& val) const{
    if(!val.IsDouble()){
        if(val.IsInt()){
            return val.GetInt();
        }
        throw FieldWrongTypeException("excpected double in iterator");
    }
    return val.GetDouble();
}

Value::ConstArray JsonHandler::getArrayValue(const Value::ConstValueIterator& itr) const{
    if(!(*itr).IsArray()){
        throw FieldWrongTypeException("excpected array in iterator");
    }
    return (*itr).GetArray();
}

int JsonHandler::getIntValue(const Value::ConstValueIterator& itr, const char* field) const{
    string strField = string (field);
    if(!itr->HasMember(field)){
        string message = "Entry did not contain the field " + strField;
        throw MissingJsonFieldException(message);
    }
    if(!(*itr)[field].IsInt()){
        string message = "expected field " + strField;
        message += " to be int";
        throw FieldWrongTypeException(message);
    }

    return (*itr)[field].GetInt();
}

bool JsonHandler::getBoolValue(const Value::ConstValueIterator& itr, const char* field) const{
    string strField = string (field);
    if(!itr->HasMember(field)){
        string message = "Entry did not contain the field " + strField;
        throw MissingJsonFieldException(message);
    }
    if(!(*itr)[field].IsBool()){
        string message = "expected field " + strField;
        message += " to be bool";
        throw FieldWrongTypeException(message);
    }

    return (*itr)[field].GetBool();
}

Value::ConstArray JsonHandler::getArrayValue(const Value::ConstValueIterator& itr, const char* field) const{
    string strField = string (field);
    if(!itr->HasMember(field)){
        string message = "Entry did not contain the field " + strField;
        throw MissingJsonFieldException(message);
    }
    if(!(*itr)[field].IsArray()){
        string message = "expected field " + strField;
        message += " to be array";
        throw FieldWrongTypeException(message);
    }

    return (*itr)[field].GetArray();
}

Value::ConstArray JsonHandler::getArrayValue(const Value::ConstObject & val, const char* field) const{
    string strField = string (field);
    if(!val.HasMember(field)){
        string message = "Entry did not contain the field " + strField;
        throw MissingJsonFieldException(message);
    }
    if(!val[field].IsArray()){
        string message = "expected field " + strField;
        message += " to be array";
        throw FieldWrongTypeException(message);
    }

    return val[field].GetArray();
}

Value::ConstObject JsonHandler::getObectValue(const Value::ConstValueIterator& itr, const char* field) const{
    string strField = string (field);
    if(!itr->HasMember(field)){
        string message = "Entry did not contain the field " + strField;
        throw MissingJsonFieldException(message);
    }
    if(!(*itr)[field].IsObject()){
        string message = "expected field " + strField;
        message += " to be object";
        throw FieldWrongTypeException(message);
    }

    return (*itr)[field].GetObject();
}



