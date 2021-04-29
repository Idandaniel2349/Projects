#include "gtest/gtest.h"
#include "GISNamedTypes.h"
#include "CoordinatesMath.h"
#include "Grid/Grid.h"
#include "GIS.h"
#include "string"
#include "GISProvider.h"
#include "tests/NavigationValidator.h"
#include "Navigation/Navigation.h"


////////////////TestEX1////////////////////////

TEST(GISBasic, loadMapFile) {
    GIS gis = GISProvider::getGIS();
    std::vector<EntityId> loaded_entities = gis.loadMapFile("nyc2.json");
    std::vector<EntityId> expected_entities({EntityId("P1001"), EntityId("P1002"), EntityId("J1001"), EntityId("J1002"), EntityId("J1010"), EntityId("P1010"), EntityId("W2001"), EntityId("W2002")});
    EXPECT_EQ(loaded_entities, expected_entities);
}

TEST(GISBasic, getEntities) {
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("nyc2.json");
    std::vector<EntityId> entities = gis.getEntities("Washington Square Park");
    std::sort(entities.begin(), entities.end());
    std::vector<EntityId> expected_entities({EntityId("P1001")});
    EXPECT_EQ(entities, expected_entities);
}

TEST(GISBasic, clearReturnValue) {
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("nyc2.json");
    std::size_t number_of_cleared_entities = gis.clear();
    std::size_t expected_number_of_cleared_entities = 8;
    EXPECT_EQ(number_of_cleared_entities, expected_number_of_cleared_entities);
}

TEST(GISBasic, saveMapFileReturnValue) {
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("nyc2.json");
    std::size_t number_of_saved_entities = gis.saveMapFile("nyc_updated.json");
    std::size_t expected_number_of_saved_entities = 8;
    EXPECT_EQ(number_of_saved_entities, expected_number_of_saved_entities);
}


TEST(GIS, filesTest) {


    string searchName="Washington Square Park";

    GIS gis1=GIS();

    int sameEntities=1;

    //check load
    gis1.loadMapFile("nyc.json");

    gis1.saveMapFile("output.json");

    GIS gis2=GIS();

    gis2.loadMapFile("output.json");

    vector<EntityId> vector1=gis1.getEntities(searchName);

    vector<EntityId> vector2=gis2.getEntities(searchName);

    for(const auto& it:vector1){
        if(std::find(vector2.begin(), vector2.end(), it) == vector2.end()){
            sameEntities=0;
        }
    }

    EXPECT_EQ(sameEntities,1);
}


TEST(GIS, getEntityClosestPointTest) {

    GIS gis=GIS();

    gis.loadMapFile("nyc.json");

    //check correctness for nonexistant searchName
    Coordinates cord0=Coordinates((Longitude)43.5656,(Latitude)76.45454);
    std::optional<Coordinates> closestEntity0 = gis.getEntityClosestPoint((EntityId)"NONE",cord0);

    std::optional<Coordinates> optionalValue;

    EXPECT_EQ(closestEntity0,optionalValue);



    //check correctness for Junction
    Coordinates cord1=Coordinates((Longitude)43.5656,(Latitude)76.45454);
    std::optional<Coordinates> closestEntity1 = gis.getEntityClosestPoint((EntityId)"J2",cord1);


    EXPECT_EQ(closestEntity1,Coordinates((Longitude)1.05,(Latitude)1.05));

}


TEST(GIS, getWayClosestPoint) {

    GIS gis=GIS();

    gis.loadMapFile("nyc.json");

    //test 1 - when not on a way
    Coordinates cord1=Coordinates((Longitude)1,(Latitude)1);
    std::pair<Coordinates, EntityId> closestWay = gis.getWayClosestPoint(cord1);

    EXPECT_EQ(closestWay.first,Coordinates((Longitude)0.99,(Latitude)0.99));

    //test2 - when on a way
    Coordinates cord2=Coordinates((Longitude)0.99,(Latitude)0.96);
    std::pair<Coordinates, EntityId> closestWay2 = gis.getWayClosestPoint(cord2);

    EXPECT_EQ(closestWay2.first,cord2);
}

//clear

TEST(Clear,DontExist){
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("DontExist.json");

    std::size_t result = gis.clear();

    EXPECT_EQ(result,(std::size_t)0);
}

TEST(Clear,empty){
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("empty.json");

    std::size_t result = gis.clear();

    EXPECT_EQ(result,(std::size_t)0);
}

TEST(Clear,result){
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("nyc.json");

    std::size_t result = gis.clear();

    EXPECT_EQ(result,(std::size_t)11);
}

//load  tests

TEST(Load, NoFile) {
    GIS gis = GISProvider::getGIS();
    std::vector<EntityId> result=gis.loadMapFile("DontExist.json");

    EXPECT_TRUE(result.empty());
}

TEST(Load, emptyArray) {
    GIS gis = GISProvider::getGIS();
    std::vector<EntityId> result=gis.loadMapFile("emptyArray.json");

    EXPECT_TRUE(result.empty());
}

TEST(Load, empty) {
    GIS gis = GISProvider::getGIS();
    std::vector<EntityId> result=gis.loadMapFile("empty.json");

    EXPECT_TRUE(result.empty());
}

TEST(Load,EntitesRecieved){
    GIS gis = GISProvider::getGIS();
    std::vector<EntityId> result=gis.loadMapFile("nyc.json");

    EXPECT_EQ((int)result.size(),11);

    std::vector<EntityId> result2=gis.loadMapFile("sameName.json");

    EXPECT_EQ((int)result2.size(),8);
}

TEST(Load,CoordinatesRejected){
    GIS gis = GISProvider::getGIS();
    std::vector<EntityId> result=gis.loadMapFile("cords.json");

    EXPECT_EQ(result.size(),(std::size_t)0);
}

TEST(Load, loadNotSchema){
    GIS g = GIS();
    string testMap = "wrong.json";

    auto loadedIds = g.loadMapFile(testMap);
    EXPECT_TRUE(loadedIds.empty());
}

TEST(Load, loadExistingId){
    GIS g = GIS();
    string testMap = "nyc2.json";
    string usedIdMap = "id.json";

    g.loadMapFile(testMap);
    auto loadedIds = g.loadMapFile(usedIdMap);
    EXPECT_TRUE(loadedIds.empty());
}

TEST(Load, loadMixedIdIdFirst){
    GIS g = GIS();
    string testMap = "mixedIdsIdFirst.json";

    auto loadedIds = g.loadMapFile(testMap);
    EXPECT_EQ((int)loadedIds.size(), 6);
    unordered_set<EntityId> expectedIds = {EntityId("P1001"), EntityId("P1002"), EntityId("J1001"), EntityId("J1010"),
                                           EntityId("J1002"), EntityId("W2001")};
    for(auto& id : loadedIds){
        EXPECT_TRUE(expectedIds.contains(id));
    }
}

TEST(Load, loadMixedIdNoIdFirst){
    GIS g = GIS();
    string testMap = "mixedIdsNoIdFirst.json";

    auto loadedIds = g.loadMapFile(testMap);
    EXPECT_EQ((int)loadedIds.size(), 4);
    unordered_set<EntityId> expectedIds = {EntityId("1"), EntityId("2"), EntityId("3"), EntityId("4")};
    for(auto& id : loadedIds){
        EXPECT_TRUE(expectedIds.contains(id));
    }
}

TEST(Load, LoadWayNoJuctionsInFile){
    GIS g = GIS();
    string testMap1 = "nyc2.json";
    string testMap2 = "justWay.json";
    g.loadMapFile(testMap1);
    auto loadedIds = g.loadMapFile(testMap2);

    EXPECT_EQ(1, (int)loadedIds.size());
    if((int)loadedIds.size() == 1) {
        EXPECT_EQ(loadedIds[0], EntityId("W2003"));
    }
}

//save

TEST(Save,SaveResult){
    GIS gis = GISProvider::getGIS();

    std::size_t size = gis.saveMapFile("output.json");

    EXPECT_EQ(size,(std::size_t)0);

    gis.loadMapFile("nyc.json");

    std::size_t size2 = gis.saveMapFile("output.json");

    EXPECT_EQ(size2,(std::size_t)11);
}

TEST(Save,saveAfterClear){
    GIS gis = GISProvider::getGIS();


    gis.loadMapFile("noWays.json");

    gis.saveMapFile("output.json");

    gis.clear();

    gis.loadMapFile("output.json");

    std::size_t size = gis.saveMapFile("output2.json");


    EXPECT_EQ(size,(std::size_t)9);
}



//getEntities

TEST(getEntities,result){
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("nyc.json");

    std::vector<EntityId> result=gis.getEntities("Washington Square Park");

    EXPECT_EQ((int)result.size(),4);
}

TEST(getEntities,fromDiffrentFiles){
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("nyc.json");
    gis.loadMapFile("sameName.json");

    std::vector<EntityId> result=gis.getEntities("name");

    EXPECT_EQ((int)result.size(),9);
}

TEST(getEntities,sameName){
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("sameName.json");

    std::vector<EntityId> result=gis.getEntities("name");

    EXPECT_EQ((int)result.size(),8);
}

TEST(getEntities,onACircle){
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("sameName.json");

    Coordinates cord =Coordinates((Longitude)40.731051, (Latitude)-73.999611);

    Meters radius = Meters(3);

    std::vector<EntityId> result=gis.getEntities("name",cord,radius);

    EXPECT_EQ((int)result.size(),1);
}

TEST(getEntities, getClosestEntitesClosestPointTest){
    GIS g = GIS();
    Logger testLogger = Logger("test.log");
    unordered_map<EntityId,shared_ptr<Entity>> entityByIdTest;
    string testMap = "nyc2.json";
    g.loadMapFile(testMap);
    EntityId idToSearch = EntityId("P1002");
    Coordinates closestPoint((Longitude)0,(Latitude)0);
    Coordinates c = Coordinates(Longitude(50), Latitude(50));

    JsonHandler loader = JsonHandler(&testLogger, &entityByIdTest);
    auto entities = loader.loadEntities(testMap);
    for(const auto& entity : entities){
        if(entity->getId() == idToSearch){
            closestPoint = entity->closestPoint(c);
        }
    }

    EXPECT_EQ(closestPoint, g.getEntityClosestPoint(idToSearch, c));
}

TEST(getEntities, getEntitesByName){
    GIS g = GIS();
    Logger testLogger = Logger("test.log");
    unordered_map<EntityId,shared_ptr<Entity>> entityByIdTest;
    string testMap = "ignoreDiffNames.json";
    string searchName = "name";
    unordered_set<EntityId> expectedIds;

    g.loadMapFile(testMap);

    JsonHandler loader = JsonHandler(&testLogger, &entityByIdTest);
    auto entities = loader.loadEntities(testMap);
    for(const auto& entity : entities){
        if(entity->getName() == searchName){
            expectedIds.insert(entity->getId());
        }
    }

    auto returnedIds = g.getEntities(searchName);

    EXPECT_EQ(returnedIds.size(), expectedIds.size());
    for(const auto& id : returnedIds){
        EXPECT_TRUE(expectedIds.contains(id));
    }
}

TEST(getEntities, getClosestEntitesRadiusTest){
    GIS g = GIS();
    Logger testLogger = Logger("test.log");
    unordered_map<EntityId,shared_ptr<Entity>> entityByIdTest;
    string testMap = "sameName.json";
    string searchName = "name";
    Meters radius = Meters(282);
    Coordinates searchPoint = Coordinates(Longitude(-73.999446), Latitude(40.733566));
    unordered_set<EntityId> expectedIds;

    g.loadMapFile(testMap);

    JsonHandler loader = JsonHandler(&testLogger, &entityByIdTest);
    auto entities = loader.loadEntities(testMap);
    for(const auto& entity : entities){
        Coordinates closestPoint = entity->closestPoint(searchPoint);
        if(CoordinatesMath::calculateDistance(closestPoint, searchPoint) <= radius){
            expectedIds.insert(entity->getId());
        }
    }

    auto returnedIds = g.getEntities(searchName, searchPoint, radius);

    EXPECT_EQ(returnedIds.size(), expectedIds.size());
    for(const auto& id : returnedIds){
        EXPECT_TRUE(expectedIds.contains(id));
    }
}

TEST(getEntities, getClosestEntitesRadiusAntiMerdianTest){
    GIS g = GIS();
    Logger testLogger = Logger("test.log");
    unordered_map<EntityId,shared_ptr<Entity>> entityByIdTest;
    string testMap = "antimeridianRadius.json";
    string searchName = "name";
    Meters radius = Meters(55);
    Coordinates searchPoint = Coordinates(Longitude(179.999999), Latitude(0));
    unordered_set<EntityId> expectedIds;

    g.loadMapFile(testMap);

    JsonHandler loader = JsonHandler(&testLogger, &entityByIdTest);
    auto entities = loader.loadEntities(testMap);
    for(const auto& entity : entities){
        Coordinates closestPoint = entity->closestPoint(searchPoint);
        if(CoordinatesMath::calculateDistance(closestPoint, searchPoint) <= radius){
            expectedIds.insert(entity->getId());
        }
    }

    auto returnedIds = g.getEntities(searchName, searchPoint, radius);

    EXPECT_EQ(returnedIds.size(), expectedIds.size());
    for(const auto& id : returnedIds){
        EXPECT_TRUE(expectedIds.contains(id));
    }
}

TEST(getEntities, getClosestEntitesByNameIgnoresDiffNamesTest){
    GIS g = GIS();
    Logger testLogger = Logger("test.log");
    unordered_map<EntityId,shared_ptr<Entity>> entityByIdTest;
    string testMap = "ignoreDiffNames.json";
    string searchName = "name";
    //should find all entities
    Meters radius = Meters(3000);

    Coordinates searchPoint = Coordinates(Longitude(40.733566), Latitude(-73.999446));
    unordered_set<EntityId> expectedIds;

    g.loadMapFile(testMap);

    JsonHandler loader = JsonHandler(&testLogger, &entityByIdTest);
    auto entities = loader.loadEntities(testMap);
    for(const auto& entity : entities){
        Coordinates closestPoint = entity->closestPoint(searchPoint);
        if(CoordinatesMath::calculateDistance(closestPoint, searchPoint) <= radius &&
           entity->getName() == searchName){
            expectedIds.insert(entity->getId());
        }
    }

    auto returnedIds = g.getEntities(searchName, searchPoint, radius);

    EXPECT_EQ(returnedIds.size(), expectedIds.size());
    for(const auto& id : returnedIds){
        EXPECT_TRUE(expectedIds.contains(id));
    }
}


//get way closest point
TEST(ClosestWay, GetWayClosestPoint1Test){
    GIS g = GIS();
    Logger testLogger = Logger("test.log");
    unordered_map<EntityId,shared_ptr<Entity>> entityByIdTest;
    string testMap = "lotsOfWays.json";

    Coordinates searchPoint = Coordinates(Longitude(40.734897), Latitude(-73.991643));
    Coordinates closestPoint((Longitude)0,(Latitude)0);
    bool minInit = false;
    Meters minDistance= Meters(0);
    EntityId closestWay = EntityId("");
    g.loadMapFile(testMap);

    JsonHandler loader = JsonHandler(&testLogger, &entityByIdTest);
    auto entities = loader.loadEntities(testMap);
    for(const auto& entity : entities){
        if(entity->getType() == EntityType::way) {
            Coordinates wayClosestPoint = entity->closestPoint(searchPoint);
            auto distance = CoordinatesMath::calculateDistance(wayClosestPoint, searchPoint);
            if (distance <=  minDistance || !minInit) {
                minDistance = distance;
                closestPoint = wayClosestPoint;
                closestWay = entity->getId();
                minInit = true;
            }
        }
    }

    auto result = g.getWayClosestPoint(searchPoint);
    EXPECT_EQ(closestWay, result.second);
    EXPECT_EQ(closestPoint, result.first);
}


TEST(ClosestWay, GetWayClosestPoint2Test){
    GIS g = GIS();
    Logger testLogger = Logger("test.log");
    unordered_map<EntityId,shared_ptr<Entity>> entityByIdTest;
    string testMap = "lotsOfWays.json";
    Coordinates searchPoint = Coordinates(Longitude(40.731499), Latitude(-73.996712));
    Coordinates closestPoint((Longitude)0,(Latitude)0);
    bool minInit = false;
    Meters minDistance= Meters(0);
    EntityId closestWay = EntityId("");
    g.loadMapFile(testMap);

    JsonHandler loader = JsonHandler(&testLogger, &entityByIdTest);
    auto entities = loader.loadEntities(testMap);
    for(const auto& entity : entities){
        if(entity->getType() == EntityType::way) {
            Coordinates wayClosestPoint = entity->closestPoint(searchPoint);
            auto distance = CoordinatesMath::calculateDistance(wayClosestPoint, searchPoint);
            if (distance <=  minDistance || !minInit) {
                minDistance = distance;
                closestPoint = wayClosestPoint;
                closestWay = entity->getId();
                minInit = true;
            }
        }
    }

    auto result = g.getWayClosestPoint(searchPoint);
    EXPECT_EQ(closestWay, result.second);
    EXPECT_EQ(closestPoint, result.first);
}

TEST(ClosestWay, GetWayClosestPoint3Test){
    GIS g = GIS();
    Logger testLogger = Logger("test.log");
    unordered_map<EntityId,shared_ptr<Entity>> entityByIdTest;
    string testMap = "lotsOfWays.json";
    Coordinates searchPoint = Coordinates(Longitude(40.7328576), Latitude(-73.996428));
    Coordinates closestPoint((Longitude)0,(Latitude)0);
    bool minInit = false;
    Meters minDistance= Meters(0);
    EntityId closestWay = EntityId("");
    g.loadMapFile(testMap);

    JsonHandler loader = JsonHandler(&testLogger, &entityByIdTest);
    auto entities = loader.loadEntities(testMap);
    for(const auto& entity : entities){
        if(entity->getType() == EntityType::way) {
            Coordinates wayClosestPoint = entity->closestPoint(searchPoint);
            auto distance = CoordinatesMath::calculateDistance(wayClosestPoint, searchPoint);
            if (distance <=  minDistance || !minInit) {
                minDistance = distance;
                closestPoint = wayClosestPoint;
                closestWay = entity->getId();
                minInit = true;
            }
        }
    }

    auto result = g.getWayClosestPoint(searchPoint);
    EXPECT_EQ(closestWay, result.second);
    EXPECT_EQ(closestPoint, result.first);
}

TEST(ClosestWay, GetWayClosestPointAntiMerdianTest){
    GIS g = GIS();
    Logger testLogger = Logger("test.log");
    unordered_map<EntityId,shared_ptr<Entity>> entityByIdTest;
    string testMap = "antimerdianWays.json";
    Coordinates searchPoint = Coordinates(Longitude(179.999999), Latitude(0));
    Coordinates closestPoint((Longitude)0,(Latitude)0);
    bool minInit = false;
    Meters minDistance= Meters(0);
    EntityId closestWay = EntityId("");
    g.loadMapFile(testMap);

    JsonHandler loader = JsonHandler(&testLogger, &entityByIdTest);
    auto entities = loader.loadEntities(testMap);
    for(const auto& entity : entities){
        if(entity->getType() == EntityType::way) {
            Coordinates wayClosestPoint = entity->closestPoint(searchPoint);
            auto distance = CoordinatesMath::calculateDistance(wayClosestPoint, searchPoint);
            if (distance <=  minDistance || !minInit) {
                minDistance = distance;
                closestPoint = wayClosestPoint;
                closestWay = entity->getId();
                minInit = true;
            }
        }
    }

    auto result = g.getWayClosestPoint(searchPoint);
    EXPECT_EQ(closestWay, result.second);
    EXPECT_EQ(closestPoint, result.first);
}

TEST(ClosestWay, wayClosestPointNoLoadTest){
    GIS g = GIS();
    auto result = g.getWayClosestPoint(Coordinates((Longitude)0,(Latitude)0));

    EXPECT_EQ(EntityId(""), result.second);
    EXPECT_EQ(Coordinates(Longitude(404), Latitude(404)), result.first);
}


TEST(ClosestWay, wayClosestPointLoadNoWaysTest){
    GIS g = GIS();
    string testMap = "noWays.json";
    g.loadMapFile(testMap);
    auto result = g.getWayClosestPoint(Coordinates((Longitude)0,(Latitude)0));

    EXPECT_EQ(EntityId(""), result.second);
    EXPECT_EQ(Coordinates(Longitude(404), Latitude(404)), result.first);
}

TEST(ClosestWay, wayClosestPointAfterClearTest){
    GIS g = GIS();
    string testMap = "lotsOfWays.json";
    g.loadMapFile(testMap);
    g.clear();
    auto result = g.getWayClosestPoint(Coordinates(Longitude(0), Latitude(0)));

    EXPECT_EQ(EntityId(""), result.second);
    EXPECT_EQ(Coordinates(Longitude(404), Latitude(404)), result.first);
}

//general

TEST(general,justRunThings){
    GIS gis = GISProvider::getGIS();

    std::vector<EntityId> result=gis.loadMapFile("nyc.json");

    EXPECT_EQ((int)result.size(),11);

    std::size_t size = gis.saveMapFile("output.json");

    EXPECT_EQ((int)size,11);

    std::size_t size2 = gis.clear();

    EXPECT_EQ((int)size2,11);

    std::vector<EntityId> result2=gis.loadMapFile("nyc.json");

    EXPECT_EQ((int)result2.size(),11);

    gis.loadMapFile("sameName.json");

    std::vector<EntityId> result3=gis.getEntities("name");

    EXPECT_EQ((int)result3.size(),9);

    gis.clear();

    gis.loadMapFile("sameName.json");

    Coordinates cord =Coordinates((Longitude)40.731051, (Latitude)-73.999611);

    Meters radius = Meters(3);

    std::vector<EntityId> result4=gis.getEntities("name",cord,radius);

    EXPECT_EQ((int)result4.size(),1);
}

TEST(NavigationValidator, notValidWays){
    Meters currLength(0);
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("simpleRoute.json");

    NavigationValidator validator=NavigationValidator(gis);

    Coordinates cord1=Coordinates(Longitude(10),Latitude(10));
    Coordinates cord2=Coordinates(Longitude(20),Latitude(20));

    std::vector<std::pair<EntityId, Direction>> ways;
    Meters length(0);
    Minutes time(0);
    ways.push_back(std::pair<EntityId, Direction>("W1",Direction::A_to_B));
    ways.push_back(std::pair<EntityId, Direction>("W2",Direction::A_to_B));
    ways.push_back(std::pair<EntityId, Direction>("W3",Direction::A_to_B));
    ways.push_back(std::pair<EntityId, Direction>("W4",Direction::A_to_B));
    ways.push_back(std::pair<EntityId, Direction>("W5",Direction::A_to_B));


    for(size_t i=0; i<ways.size();i++){
        currLength=CoordinatesMath::calculateDistance(gis.getWay(ways[i].first).getFromJunctionCoordinates(),gis.getWay(ways[i].first).getToJunctionCoordinates());
        length+=currLength;
        time+=(Minutes)currLength/gis.getWay(ways[i].first).getSpeedLimit();
    }

    std::vector<std::pair<EntityId, Direction>> ways2;

    Route route = Route(ways2,cord1,cord2,length,time*60);


    EXPECT_EQ(validator.validateRoute(cord1,cord2,route),false);

}

TEST(NavigationValidator, notValidPoints){
    Meters currLength(0);
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("simpleRoute.json");

    NavigationValidator validator=NavigationValidator(gis);

    Coordinates cord1=Coordinates(Longitude(10),Latitude(10));
    Coordinates cord2=Coordinates(Longitude(20),Latitude(20));

    std::vector<std::pair<EntityId, Direction>> ways;
    Meters length(0);
    Minutes time(0);
    ways.push_back(std::pair<EntityId, Direction>("W1",Direction::A_to_B));
    ways.push_back(std::pair<EntityId, Direction>("W2",Direction::A_to_B));
    ways.push_back(std::pair<EntityId, Direction>("W3",Direction::A_to_B));
    ways.push_back(std::pair<EntityId, Direction>("W4",Direction::A_to_B));
    ways.push_back(std::pair<EntityId, Direction>("W5",Direction::A_to_B));


    for(size_t i=0; i<ways.size();i++){
        currLength=CoordinatesMath::calculateDistance(gis.getWay(ways[i].first).getFromJunctionCoordinates(),gis.getWay(ways[i].first).getToJunctionCoordinates());
        length+=currLength;
        time+=(Minutes)currLength/gis.getWay(ways[i].first).getSpeedLimit();
    }

    Route route = Route(ways,cord1,cord2,length,time*60);

    EXPECT_EQ(validator.validateRoute(cord1,cord2,route),false);

}

TEST(NavigationValidator, notValidLengthOrTime){
    Meters currLength(0);
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("simpleRoute.json");

    NavigationValidator validator=NavigationValidator(gis);

    Coordinates cord1=Coordinates(Longitude(1),Latitude(0));
    Coordinates cord2=Coordinates(Longitude(4),Latitude(0));

    std::vector<std::pair<EntityId, Direction>> ways;
    Meters length(0);
    Minutes time(0);
    ways.push_back(std::pair<EntityId, Direction>("W1",Direction::A_to_B));
    ways.push_back(std::pair<EntityId, Direction>("W2",Direction::A_to_B));
    ways.push_back(std::pair<EntityId, Direction>("W3",Direction::A_to_B));
    ways.push_back(std::pair<EntityId, Direction>("W4",Direction::A_to_B));
    ways.push_back(std::pair<EntityId, Direction>("W5",Direction::A_to_B));


    for(size_t i=0; i<ways.size();i++){
        currLength=CoordinatesMath::calculateDistance(gis.getWay(ways[i].first).getFromJunctionCoordinates(),gis.getWay(ways[i].first).getToJunctionCoordinates());
        length+=currLength;
        time+=(Minutes)currLength/gis.getWay(ways[i].first).getSpeedLimit();
    }

    Route route1 = Route(ways,cord1,cord2,Meters(2),time*60);
    Route route2 = Route(ways,cord1,cord2,length,Minutes(22));

    EXPECT_EQ(validator.validateRoute(cord1,cord2,route1),false);
    EXPECT_EQ(validator.validateRoute(cord1,cord2,route1),false);

}

TEST(NavigationValidator, simpleRoute){
    Meters currLength(0);
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("simpleRoute.json");

    NavigationValidator validator=NavigationValidator(gis);

    Coordinates cord1=Coordinates(Longitude(1),Latitude(0));
    Coordinates cord2=Coordinates(Longitude(4),Latitude(0));

    std::vector<std::pair<EntityId, Direction>> ways;
    Meters length(0);
    Minutes time(0);
    ways.push_back(std::pair<EntityId, Direction>("W1",Direction::A_to_B));
    ways.push_back(std::pair<EntityId, Direction>("W2",Direction::A_to_B));
    ways.push_back(std::pair<EntityId, Direction>("W3",Direction::A_to_B));
    ways.push_back(std::pair<EntityId, Direction>("W4",Direction::A_to_B));
    ways.push_back(std::pair<EntityId, Direction>("W5",Direction::A_to_B));


    for(size_t i=0; i<ways.size();i++){
        currLength=CoordinatesMath::calculateDistance(gis.getWay(ways[i].first).getFromJunctionCoordinates(),gis.getWay(ways[i].first).getToJunctionCoordinates());
        length+=currLength;
        time+=(Minutes)currLength/gis.getWay(ways[i].first).getSpeedLimit();
    }

    Route route = Route(ways,cord1,cord2,length,time*60);

    EXPECT_EQ(validator.validateRoute(cord1,cord2,route),true);
}


TEST(getRoutes,empty){
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("empty.json");

    Navigation navigation = Navigation(gis);

    Coordinates cord1=Coordinates(Longitude(1),Latitude(0));
    Coordinates cord2=Coordinates(Longitude(4),Latitude(0));

    Routes routes =navigation.getRoutes(cord1,cord2);

    Route invalidRoute = Route(std::vector<std::pair<EntityId, Direction>>(),cord1, cord2,
                               Meters(0), Minutes(0));
    EXPECT_EQ(routes.shortestDistance(),invalidRoute);
    EXPECT_EQ(routes.shortestTime(),invalidRoute);

}

TEST(getRoutes,simpleRoute){
    Meters currLength(0);

    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("simpleRoute.json");

    Navigation navigation = Navigation(gis);

    Coordinates cord1=Coordinates(Longitude(1),Latitude(0));
    Coordinates cord2=Coordinates(Longitude(4),Latitude(0));

    std::vector<std::pair<EntityId, Direction>> ways;
    Meters length(0);
    Minutes time(0);
    ways.push_back(std::pair<EntityId, Direction>("W1",Direction::A_to_B));
    ways.push_back(std::pair<EntityId, Direction>("W2",Direction::A_to_B));
    ways.push_back(std::pair<EntityId, Direction>("W3",Direction::A_to_B));
    ways.push_back(std::pair<EntityId, Direction>("W4",Direction::A_to_B));
    ways.push_back(std::pair<EntityId, Direction>("W5",Direction::A_to_B));


    for(size_t i=0; i<ways.size();i++) {
        currLength = CoordinatesMath::calculateDistance(gis.getWay(ways[i].first).getFromJunctionCoordinates(),
                                                        gis.getWay(ways[i].first).getToJunctionCoordinates());
        length += currLength;
        time += (Minutes) (currLength / gis.getWay(ways[i].first).getSpeedLimit());
    }

    Route route = Route(ways,cord1,cord2,length,time*60);

    Routes routes =navigation.getRoutes(cord1,cord2);

    EXPECT_EQ(routes.shortestDistance(),route);
    EXPECT_EQ(routes.shortestTime(),route);
    NavigationValidator validator(gis);
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, routes.shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, routes.shortestTime()));
}

TEST(getRoutes,simpleRouteMiddleEdges){
    Meters currLength(0);

    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("simpleRoute.json");

    Navigation navigation = Navigation(gis);

    Coordinates cord1=Coordinates(Longitude(1.5),Latitude(0));
    Coordinates cord2=Coordinates(Longitude(3.5),Latitude(0));

    std::vector<std::pair<EntityId, Direction>> ways;
    Meters length(0);
    Minutes time(0);
    ways.push_back(std::pair<EntityId, Direction>("W1",Direction::A_to_B));
    ways.push_back(std::pair<EntityId, Direction>("W2",Direction::A_to_B));
    ways.push_back(std::pair<EntityId, Direction>("W3",Direction::A_to_B));
    ways.push_back(std::pair<EntityId, Direction>("W4",Direction::A_to_B));
    ways.push_back(std::pair<EntityId, Direction>("W5",Direction::A_to_B));

    currLength=gis.getWay(ways[0].first).getLength() - CoordinatesMath::calculateDistance(cord1,Coordinates(Longitude(1),Latitude(0)));
    length += currLength;
    time += (Minutes) currLength / gis.getWay(ways[0].first).getSpeedLimit();
    for(size_t i=1; i<ways.size()-1;i++) {
        currLength = CoordinatesMath::calculateDistance(gis.getWay(ways[i].first).getFromJunctionCoordinates(),
                                                        gis.getWay(ways[i].first).getToJunctionCoordinates());
        length += currLength;
        time += (Minutes) currLength / gis.getWay(ways[i].first).getSpeedLimit();
    }
    currLength=gis.getWay(ways[4].first).getLength() - CoordinatesMath::calculateDistance(cord2,Coordinates(Longitude(4),Latitude(0)));
    length += currLength;
    time += (Minutes) currLength / gis.getWay(ways[4].first).getSpeedLimit();

    Route route = Route(ways,cord1,cord2,length,time*60);

    Routes routes =navigation.getRoutes(cord1,cord2);


    EXPECT_EQ(routes.shortestDistance(),route);
    EXPECT_EQ(routes.shortestTime(),route);
    NavigationValidator validator(gis);
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, routes.shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, routes.shortestTime()));

}

TEST(getRoutes,simpleRouteOffRoad){
    Meters currLength(0);

    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("simpleRoute.json");

    Navigation navigation = Navigation(gis);

    Coordinates cord1=Coordinates(Longitude(1),Latitude(0));
    Coordinates cord2=Coordinates(Longitude(4),Latitude(0));

    Coordinates cord3=Coordinates(Longitude(1),Latitude(0.5));
    Coordinates cord4=Coordinates(Longitude(4),Latitude(0.5));

    std::vector<std::pair<EntityId, Direction>> ways;
    Meters length(0);
    Minutes time(0);
    ways.push_back(std::pair<EntityId, Direction>("W1",Direction::A_to_B));
    ways.push_back(std::pair<EntityId, Direction>("W2",Direction::A_to_B));
    ways.push_back(std::pair<EntityId, Direction>("W3",Direction::A_to_B));
    ways.push_back(std::pair<EntityId, Direction>("W4",Direction::A_to_B));
    ways.push_back(std::pair<EntityId, Direction>("W5",Direction::A_to_B));

    for(size_t i=0; i<ways.size();i++) {
        currLength = CoordinatesMath::calculateDistance(gis.getWay(ways[i].first).getFromJunctionCoordinates(),
                                                        gis.getWay(ways[i].first).getToJunctionCoordinates());
        length += currLength;
        time += (Minutes) currLength / gis.getWay(ways[i].first).getSpeedLimit();
    }

    Route route = Route(ways,cord1,cord2,length,time*60);

    Routes routes =navigation.getRoutes(cord3,cord4);

    EXPECT_EQ(routes.shortestDistance(),route);
    EXPECT_EQ(routes.shortestTime(),route);
    NavigationValidator validator(gis);
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, routes.shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, routes.shortestTime()));

}

TEST(getRoutes,diffShortFast){
    Meters currLength(0);

    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("diffShortFast.json");

    Navigation navigation = Navigation(gis);

    Coordinates cord1=Coordinates(Longitude(1),Latitude(0));
    Coordinates cord2=Coordinates(Longitude(4),Latitude(0));

    std::vector<std::pair<EntityId, Direction>> ways1;
    Meters length1(0);
    Minutes time1(0);
    ways1.push_back(std::pair<EntityId, Direction>("W1",Direction::A_to_B));
    ways1.push_back(std::pair<EntityId, Direction>("W2",Direction::A_to_B));
    ways1.push_back(std::pair<EntityId, Direction>("W3",Direction::A_to_B));
    ways1.push_back(std::pair<EntityId, Direction>("W4",Direction::A_to_B));
    ways1.push_back(std::pair<EntityId, Direction>("W5",Direction::A_to_B));


    for(size_t i=0; i<ways1.size();i++) {
        currLength = CoordinatesMath::calculateDistance(gis.getWay(ways1[i].first).getFromJunctionCoordinates(),
                                                        gis.getWay(ways1[i].first).getToJunctionCoordinates());
        length1 += currLength;
        time1 += (Minutes) currLength / gis.getWay(ways1[i].first).getSpeedLimit();
    }

    Route route1 = Route(ways1,cord1,cord2,length1,time1*60);

    std::vector<std::pair<EntityId, Direction>> ways2;
    Meters length2(0);
    Minutes time2(0);
    ways2.push_back(std::pair<EntityId, Direction>("W6",Direction::A_to_B));
    ways2.push_back(std::pair<EntityId, Direction>("W7",Direction::A_to_B));
    ways2.push_back(std::pair<EntityId, Direction>("W8",Direction::A_to_B));

    for(size_t i=0; i<ways2.size();i++) {
        currLength = CoordinatesMath::calculateDistance(gis.getWay(ways2[i].first).getFromJunctionCoordinates(),
                                                        gis.getWay(ways2[i].first).getToJunctionCoordinates());
        length2 += currLength;
        time2 += (Minutes) currLength / gis.getWay(ways2[i].first).getSpeedLimit();
    }

    Route route2 = Route(ways2,cord1,cord2,length2,time2*60);

    Routes routes =navigation.getRoutes(cord1,cord2);

    EXPECT_EQ(routes.shortestDistance(),route1);
    EXPECT_EQ(routes.shortestTime(),route2);
    NavigationValidator validator(gis);
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, routes.shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, routes.shortestTime()));
}

TEST(getRoutes,partillyDiffShortFast){
    Meters currLength(0);

    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("partillyDiffShortFast.json");

    Navigation navigation = Navigation(gis);

    Coordinates cord1=Coordinates(Longitude(1),Latitude(0));
    Coordinates cord2=Coordinates(Longitude(4),Latitude(0));

    std::vector<std::pair<EntityId, Direction>> ways1;
    Meters length1(0);
    Minutes time1(0);
    ways1.push_back(std::pair<EntityId, Direction>("W1",Direction::A_to_B));
    ways1.push_back(std::pair<EntityId, Direction>("W2",Direction::A_to_B));
    ways1.push_back(std::pair<EntityId, Direction>("W3",Direction::A_to_B));
    ways1.push_back(std::pair<EntityId, Direction>("W4",Direction::A_to_B));
    ways1.push_back(std::pair<EntityId, Direction>("W5",Direction::A_to_B));

    for(size_t i=0; i<ways1.size();i++) {
        currLength = CoordinatesMath::calculateDistance(gis.getWay(ways1[i].first).getFromJunctionCoordinates(),
                                                        gis.getWay(ways1[i].first).getToJunctionCoordinates());
        length1 += currLength;
        time1 += (Minutes) currLength / gis.getWay(ways1[i].first).getSpeedLimit();
    }

    Route route1 = Route(ways1,cord1,cord2,length1,time1*60);

    std::vector<std::pair<EntityId, Direction>> ways2;
    Meters length2(0);
    Minutes time2(0);
    ways2.push_back(std::pair<EntityId, Direction>("W1",Direction::A_to_B));
    ways2.push_back(std::pair<EntityId, Direction>("W6",Direction::A_to_B));
    ways2.push_back(std::pair<EntityId, Direction>("W7",Direction::A_to_B));
    ways2.push_back(std::pair<EntityId, Direction>("W8",Direction::A_to_B));
    ways2.push_back(std::pair<EntityId, Direction>("W5",Direction::A_to_B));

    for(size_t i=0; i<ways2.size();i++) {
        currLength = CoordinatesMath::calculateDistance(gis.getWay(ways2[i].first).getFromJunctionCoordinates(),
                                                        gis.getWay(ways2[i].first).getToJunctionCoordinates());
        length2 += currLength;
        time2 += (Minutes) currLength / gis.getWay(ways2[i].first).getSpeedLimit();
    }

    Route route2 = Route(ways2,cord1,cord2,length2,time2*60);

    Routes routes =navigation.getRoutes(cord1,cord2);

    EXPECT_EQ(routes.shortestDistance(),route1);
    EXPECT_EQ(routes.shortestTime(),route2);
    NavigationValidator validator(gis);
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, routes.shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, routes.shortestTime()));

}

TEST(getRoutes,sameDistanceDiffTimes){
    Meters currLength(0);

    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("sameDistanceDiffTimes.json");

    Navigation navigation = Navigation(gis);

    Coordinates cord1=Coordinates(Longitude(1),Latitude(0));
    Coordinates cord2=Coordinates(Longitude(50),Latitude(0));

    std::vector<std::pair<EntityId, Direction>> ways1;
    Meters length1(0);
    Minutes time1(0);

    ways1.push_back(std::pair<EntityId, Direction>("W4",Direction::A_to_B));
    ways1.push_back(std::pair<EntityId, Direction>("W5",Direction::A_to_B));
    ways1.push_back(std::pair<EntityId, Direction>("W6",Direction::A_to_B));

    for(size_t i=0; i<ways1.size();i++) {
        currLength = CoordinatesMath::calculateDistance(gis.getWay(ways1[i].first).getFromJunctionCoordinates(),
                                                        gis.getWay(ways1[i].first).getToJunctionCoordinates());
        length1 += currLength;
        time1 += (Minutes) currLength / gis.getWay(ways1[i].first).getSpeedLimit();
    }

    Route route1 = Route(ways1,cord1,cord2,length1,time1*60);

    Routes routes =navigation.getRoutes(cord1,cord2);

    EXPECT_EQ(routes.shortestDistance(),route1);
    EXPECT_EQ(routes.shortestTime(),route1);
    NavigationValidator validator(gis);
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, routes.shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, routes.shortestTime()));
}

TEST(getRoutes,sameDistanceSameTimeMoreWays){
    Meters currLength(0);

    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("sameDistanceSameTimeMoreWays.json");

    Navigation navigation = Navigation(gis);

    Coordinates cord1=Coordinates(Longitude(5),Latitude(5));
    Coordinates cord2=Coordinates(Longitude(15),Latitude(5));

    std::vector<std::pair<EntityId, Direction>> ways1;
    Meters length1(0);
    Minutes time1(0);

    ways1.push_back(std::pair<EntityId, Direction>("W1",Direction::A_to_B));
    ways1.push_back(std::pair<EntityId, Direction>("W2",Direction::A_to_B));
    ways1.push_back(std::pair<EntityId, Direction>("W3",Direction::A_to_B));

    for(size_t i=0; i<ways1.size();i++) {
        currLength = CoordinatesMath::calculateDistance(gis.getWay(ways1[i].first).getFromJunctionCoordinates(),
                                                        gis.getWay(ways1[i].first).getToJunctionCoordinates());
        length1 += currLength;
        time1 += (Minutes) currLength / gis.getWay(ways1[i].first).getSpeedLimit();
    }

    Route route1 = Route(ways1,cord1,cord2,length1,time1*60);

    std::vector<std::pair<EntityId, Direction>> ways2;
    Meters length2(0);
    Minutes time2(0);

    ways2.push_back(std::pair<EntityId, Direction>("W4",Direction::A_to_B));
    ways2.push_back(std::pair<EntityId, Direction>("W5",Direction::A_to_B));
    ways2.push_back(std::pair<EntityId, Direction>("W6",Direction::A_to_B));
    ways2.push_back(std::pair<EntityId, Direction>("W7",Direction::A_to_B));
    ways2.push_back(std::pair<EntityId, Direction>("W8",Direction::A_to_B));

    Routes routes =navigation.getRoutes(cord1,cord2);

    EXPECT_EQ(routes.shortestDistance(),route1);
    EXPECT_EQ(routes.shortestTime(),route1);
    NavigationValidator validator(gis);
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, routes.shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, routes.shortestTime()));
}

TEST(getRoutes,complicatedMapTest1){
    Meters currLength(0);

    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("complicatedMap.json");

    Navigation navigation = Navigation(gis);

    Coordinates cord1=Coordinates(Longitude(-0.01),Latitude(0.01));
    Coordinates cord2=Coordinates(Longitude(1.1),Latitude(-0.011));

    vector<std::pair<EntityId, Direction>> possibleRoutes[3];

    vector<Route> routes;

    possibleRoutes[0].push_back(std::pair<EntityId, Direction>("W1", Direction::B_to_A));
    possibleRoutes[0].push_back(std::pair<EntityId, Direction>("W3", Direction::A_to_B));
    possibleRoutes[0].push_back(std::pair<EntityId, Direction>("W9", Direction::A_to_B));
    possibleRoutes[0].push_back(std::pair<EntityId, Direction>("W13", Direction::A_to_B));
    possibleRoutes[0].push_back(std::pair<EntityId, Direction>("W14", Direction::B_to_A));

    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W1", Direction::B_to_A));
    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W3", Direction::A_to_B));
    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W9", Direction::A_to_B));
    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W10", Direction::A_to_B));
    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W11", Direction::A_to_B));
    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W12", Direction::A_to_B));
    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W14", Direction::B_to_A));

    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W1", Direction::B_to_A));
    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W3", Direction::A_to_B));
    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W15", Direction::A_to_B));
    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W16", Direction::A_to_B));
    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W17", Direction::A_to_B));
    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W18", Direction::A_to_B));
    for(int i=0; i<3; ++i) {
        Meters length(0);
        Minutes time(0);
        for (size_t j = 0; j < possibleRoutes[i].size(); j++) {
            currLength = gis.getWay(possibleRoutes[i][j].first).getLength();
            length += currLength;
            time += (Minutes) currLength / gis.getWay(possibleRoutes[i][j].first).getSpeedLimit();
        }
        routes.push_back(Route(possibleRoutes[i], cord1, cord2, length, time * 60));
    }

    Routes foundRoutes =navigation.getRoutes(cord1,cord2);
    EXPECT_EQ(foundRoutes.shortestDistance(), *std::min_element(routes.begin(), routes.end(), [](Route a, Route b){
        return a.totalLength() < b.totalLength();
    }));

    EXPECT_EQ(foundRoutes.shortestTime(), *std::min_element(routes.begin(), routes.end(), [](Route a, Route b){
        return a.estimatedDuration() < b.estimatedDuration();
    }));
    NavigationValidator validator(gis);
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, foundRoutes.shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, foundRoutes.shortestTime()));
}

TEST(getRoutes,complicatedMapTest2){
    Meters currLength(0);

    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("complicatedMap.json");

    Navigation navigation = Navigation(gis);

    Coordinates cord1=Coordinates(Longitude(-0.01),Latitude(1.58));
    Coordinates cord2=Coordinates(Longitude(0.8),Latitude(-0.617));

    vector<std::pair<EntityId, Direction>> possibleRoutes[3];

    vector<Route> routes;

    possibleRoutes[0].push_back(std::pair<EntityId, Direction>("W19", Direction::A_to_B));
    possibleRoutes[0].push_back(std::pair<EntityId, Direction>("W9", Direction::A_to_B));
    possibleRoutes[0].push_back(std::pair<EntityId, Direction>("W13", Direction::A_to_B));

    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W6", Direction::A_to_B));
    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W7", Direction::A_to_B));
    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W8", Direction::A_to_B));
    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W4", Direction::B_to_A));
    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W9", Direction::A_to_B));
    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W10", Direction::A_to_B));
    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W11", Direction::A_to_B));
    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W12", Direction::A_to_B));


    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W6", Direction::A_to_B));
    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W7", Direction::A_to_B));
    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W8", Direction::A_to_B));
    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W4", Direction::B_to_A));
    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W15", Direction::A_to_B));
    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W16", Direction::A_to_B));
    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W17", Direction::A_to_B));
    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W18", Direction::A_to_B));
    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W14", Direction::A_to_B));
    for(int i=0; i<3; ++i) {
        Meters length(0);
        Minutes time(0);
        for (size_t j = 0; j < possibleRoutes[i].size(); j++) {
            currLength = gis.getWay(possibleRoutes[i][j].first).getLength();
            length += currLength;
            time += (Minutes) currLength / gis.getWay(possibleRoutes[i][j].first).getSpeedLimit();
        }
        routes.push_back(Route(possibleRoutes[i], cord1, cord2, length, time * 60));
    }

    Routes foundRoutes =navigation.getRoutes(cord1,cord2);

    EXPECT_EQ(foundRoutes.shortestDistance(), *std::min_element(routes.begin(), routes.end(), [](Route a, Route b){
        return a.totalLength() < b.totalLength();
    }));

    EXPECT_EQ(foundRoutes.shortestTime(), *std::min_element(routes.begin(), routes.end(), [](Route a, Route b){
        return a.estimatedDuration() < b.estimatedDuration();
    }));
    NavigationValidator validator(gis);
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, foundRoutes.shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, foundRoutes.shortestTime()));
}

TEST(getRoutes,complicatedMapEdgesNotJunctionTest1){
    Meters currLength(0);

    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("complicatedMap.json");

    Navigation navigation = Navigation(gis);

    Coordinates cord1=Coordinates(Longitude(-0.0075),Latitude(0.0076));
    Coordinates cord2=Coordinates(Longitude(1.082),Latitude(-0.0118));

    vector<std::pair<EntityId, Direction>> possibleRoutes[3];

    vector<Route> routes;

    possibleRoutes[0].push_back(std::pair<EntityId, Direction>("W1", Direction::B_to_A));
    possibleRoutes[0].push_back(std::pair<EntityId, Direction>("W3", Direction::A_to_B));
    possibleRoutes[0].push_back(std::pair<EntityId, Direction>("W9", Direction::A_to_B));
    possibleRoutes[0].push_back(std::pair<EntityId, Direction>("W13", Direction::A_to_B));
    possibleRoutes[0].push_back(std::pair<EntityId, Direction>("W14", Direction::B_to_A));

    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W1", Direction::B_to_A));
    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W3", Direction::A_to_B));
    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W9", Direction::A_to_B));
    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W10", Direction::A_to_B));
    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W11", Direction::A_to_B));
    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W12", Direction::A_to_B));
    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W14", Direction::B_to_A));

    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W1", Direction::B_to_A));
    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W3", Direction::A_to_B));
    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W15", Direction::A_to_B));
    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W16", Direction::A_to_B));
    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W17", Direction::A_to_B));
    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W18", Direction::A_to_B));
    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W14", Direction::A_to_B));
    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W14", Direction::B_to_A));
    for(int i=0; i<3; ++i) {
        Meters length(0);
        Minutes time(0);
        for (size_t j = 0; j < possibleRoutes[i].size(); j++) {
            currLength = gis.getWay(possibleRoutes[i][j].first).getLength();
            length += currLength;
            time += (Minutes) currLength / gis.getWay(possibleRoutes[i][j].first).getSpeedLimit();
        }
        routes.push_back(Route(possibleRoutes[i], cord1, cord2, length, time * 60));
    }

    Routes foundRoutes =navigation.getRoutes(cord1,cord2);
    EXPECT_EQ(foundRoutes.shortestDistance().getWays(), (*std::min_element(routes.begin(), routes.end(), [](Route a, Route b){
        return a.totalLength() < b.totalLength();
    })).getWays());

    EXPECT_EQ(foundRoutes.shortestTime().getWays(), (*std::min_element(routes.begin(), routes.end(), [](Route a, Route b){
        return a.estimatedDuration() < b.estimatedDuration();
    })).getWays());
    NavigationValidator validator(gis);
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, foundRoutes.shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, foundRoutes.shortestTime()));
}

TEST(getRoutes,complicatedMapEdgesNotJunctionTest2){
    Meters currLength(0);

    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("complicatedMap.json");

    Navigation navigation = Navigation(gis);

    Coordinates cord1=Coordinates(Longitude(-0.01),Latitude(1.4));
    Coordinates cord2=Coordinates(Longitude(0.83),Latitude(-0.517));

    vector<std::pair<EntityId, Direction>> possibleRoutes[3];

    vector<Route> routes;

    possibleRoutes[0].push_back(std::pair<EntityId, Direction>("W5", Direction::A_to_B));
    possibleRoutes[0].push_back(std::pair<EntityId, Direction>("W19", Direction::A_to_B));
    possibleRoutes[0].push_back(std::pair<EntityId, Direction>("W9", Direction::A_to_B));
    possibleRoutes[0].push_back(std::pair<EntityId, Direction>("W13", Direction::A_to_B));
    possibleRoutes[0].push_back(std::pair<EntityId, Direction>("W14", Direction::B_to_A));
    possibleRoutes[0].push_back(std::pair<EntityId, Direction>("W14", Direction::A_to_B));

    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W5", Direction::A_to_B));
    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W6", Direction::A_to_B));
    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W7", Direction::A_to_B));
    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W8", Direction::A_to_B));
    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W4", Direction::B_to_A));
    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W9", Direction::A_to_B));
    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W10", Direction::A_to_B));
    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W11", Direction::A_to_B));
    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W12", Direction::A_to_B));
    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W14", Direction::B_to_A));
    possibleRoutes[1].push_back(std::pair<EntityId, Direction>("W14", Direction::A_to_B));

    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W5", Direction::A_to_B));
    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W6", Direction::A_to_B));
    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W7", Direction::A_to_B));
    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W8", Direction::A_to_B));
    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W4", Direction::B_to_A));
    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W15", Direction::A_to_B));
    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W16", Direction::A_to_B));
    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W17", Direction::A_to_B));
    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W18", Direction::A_to_B));
    possibleRoutes[2].push_back(std::pair<EntityId, Direction>("W14", Direction::A_to_B));

    for(int i=0; i<3; ++i) {
        Meters length(0);
        Minutes time(0);
        for (size_t j = 0; j < possibleRoutes[i].size(); j++) {
            currLength = gis.getWay(possibleRoutes[i][j].first).getLength();
            length += currLength;
            time += (Minutes) currLength / gis.getWay(possibleRoutes[i][j].first).getSpeedLimit();
        }
        routes.push_back(Route(possibleRoutes[i], cord1, cord2, length, time * 60));
    }

    Routes foundRoutes =navigation.getRoutes(cord1,cord2);

    EXPECT_EQ(foundRoutes.shortestDistance().getWays(), (*std::min_element(routes.begin(), routes.end(), [](Route a, Route b){
        return a.totalLength() < b.totalLength();
    })).getWays());
    auto a = (*std::min_element(routes.begin(), routes.end(), [](Route a, Route b){
        return a.totalLength() < b.totalLength();
    })).getWays();
    EXPECT_EQ(foundRoutes.shortestTime().getWays(), (*std::min_element(routes.begin(), routes.end(), [](Route a, Route b){
        return a.estimatedDuration() < b.estimatedDuration();
    })).getWays());
    NavigationValidator validator(gis);
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, foundRoutes.shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, foundRoutes.shortestDistance()));
}



TEST(getRoutes, wayIsHighway){
    Meters currLength(0);

    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("wayIsHighway.json");

    Navigation navigation = Navigation(gis);

    Coordinates cord1=Coordinates(Longitude(1),Latitude(0.85));
    Coordinates cord2=Coordinates(Longitude(4),Latitude(-0.000002));
    Coordinates cord3 = CoordinatesMath::closestPointOnSegment(cord1, Coordinates(Longitude(2), Latitude(0)), Coordinates(Longitude(2), Latitude(1)));
    Coordinates cord4=Coordinates(Longitude(4),Latitude(0));

    std::vector<std::pair<EntityId, Direction>> ways;
    Meters length(0);
    Minutes time(0);
    ways.push_back(std::pair<EntityId, Direction>("W2",Direction::A_to_B));
    ways.push_back(std::pair<EntityId, Direction>("W3",Direction::A_to_B));
    ways.push_back(std::pair<EntityId, Direction>("W4",Direction::A_to_B));
    ways.push_back(std::pair<EntityId, Direction>("W5",Direction::A_to_B));

    Routes routes =navigation.getRoutes(cord1,cord2);



    currLength=gis.getWay(ways[0].first).getLength() - CoordinatesMath::calculateDistance(cord3,Coordinates(Longitude(2),Latitude(0)));
    length += currLength;
    time += (Minutes) currLength / gis.getWay(ways[0].first).getSpeedLimit();
    for(size_t i=1; i<ways.size();i++) {
        currLength = CoordinatesMath::calculateDistance(gis.getWay(ways[i].first).getFromJunctionCoordinates(),
                                                        gis.getWay(ways[i].first).getToJunctionCoordinates());
        length += currLength;
        time += (Minutes) (currLength / gis.getWay(ways[i].first).getSpeedLimit());
    }

    Route route = Route(ways,cord3,cord4,length,time*60);

    EXPECT_EQ(routes.shortestDistance(),route);
    EXPECT_EQ(routes.shortestTime(),route);
    NavigationValidator validator(gis);
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, routes.shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, routes.shortestTime()));
}

TEST(getRoutes, notConnectedRoute) {
    GIS gis = GISProvider::getGIS();
    gis.loadMapFile("antimerdianWays.json");

    Navigation navigation = Navigation(gis);

    Coordinates cord1=Coordinates(Longitude(179.99700),Latitude(0.002));

    Coordinates cord2=Coordinates(Longitude(-179.999500),Latitude(0.002));
    auto routes = navigation.getRoutes(cord1, cord2);

    EXPECT_FALSE(routes.isValid());
    EXPECT_EQ(routes.getErrorMessage(), "No ways connecting start and end point");
}










