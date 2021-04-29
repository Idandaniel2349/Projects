#include "gtest/gtest.h"
#include "../Common/GISNamedTypes.h"
#include "../Common/CoordinatesMath.h"
#include "../GIS/Grid/Grid.h"
#include "../GIS/GIS_316497817.h"
#include "string"
#include "../GIS/GIS_316497817.h"
#include "../Simulator/NavigationValidator.h"
#include "../Navigation/Navigation_316497817.h"
#include "mocks.h"


using std::get;

////////////////TestEX1////////////////////////

TEST(GISBasic, loadMapFile) {
    GIS_316497817 gis = GIS_316497817();
    std::vector<EntityId> loaded_entities = gis.loadMapFile("nyc2.json");
    std::vector<EntityId> expected_entities({EntityId("P1001"), EntityId("P1002"), EntityId("J1001"), EntityId("J1002"), EntityId("J1010"), EntityId("P1010"), EntityId("W2001"), EntityId("W2002")});
    EXPECT_EQ(loaded_entities, expected_entities);
}

TEST(GISBasic, getEntities) {
    GIS_316497817 gis = GIS_316497817();
    gis.loadMapFile("nyc2.json");
    std::vector<EntityId> entities = gis.getEntities("Washington Square Park");
    std::sort(entities.begin(), entities.end());
    std::vector<EntityId> expected_entities({EntityId("P1001")});
    EXPECT_EQ(entities, expected_entities);
}

TEST(GISBasic, clearReturnValue) {
    GIS_316497817 gis = GIS_316497817();
    gis.loadMapFile("nyc2.json");
    std::size_t number_of_cleared_entities = gis.clear();
    std::size_t expected_number_of_cleared_entities = 8;
    EXPECT_EQ(number_of_cleared_entities, expected_number_of_cleared_entities);
}

TEST(GISBasic, saveMapFileReturnValue) {
    GIS_316497817 gis = GIS_316497817();
    gis.loadMapFile("nyc2.json");
    std::size_t number_of_saved_entities = gis.saveMapFile("nyc_updated.json");
    std::size_t expected_number_of_saved_entities = 8;
    EXPECT_EQ(number_of_saved_entities, expected_number_of_saved_entities);
}


TEST(GIS, filesTest) {


    string searchName="Washington Square Park";

    GIS_316497817 gis1 = GIS_316497817();

    int sameEntities=1;

    //check load
    gis1.loadMapFile("nyc.json");

    gis1.saveMapFile("output.json");

    GIS_316497817 gis2 = GIS_316497817();

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

    GIS_316497817 gis = GIS_316497817();

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

    GIS_316497817 gis = GIS_316497817();

    gis.loadMapFile("nyc.json");

    //test 1 - when not on a way
    Coordinates cord1=Coordinates((Longitude)1,(Latitude)1);
    auto closestWay = gis.getWayClosestPoint(cord1);

    EXPECT_EQ(get<0>(closestWay),Coordinates((Longitude)0.99,(Latitude)0.99));

    //test2 - when on a way
    Coordinates cord2=Coordinates((Longitude)0.99,(Latitude)0.96);
    auto closestWay2 = gis.getWayClosestPoint(cord2);

    EXPECT_EQ(get<0>(closestWay2),cord2);
}

//clear

TEST(Clear,DontExist){
    GIS_316497817 gis = GIS_316497817();
    gis.loadMapFile("DontExist.json");

    std::size_t result = gis.clear();

    EXPECT_EQ(result,(std::size_t)0);
}

TEST(Clear,empty){
    GIS_316497817 gis = GIS_316497817();
    gis.loadMapFile("empty.json");

    std::size_t result = gis.clear();

    EXPECT_EQ(result,(std::size_t)0);
}

TEST(Clear,result){
    GIS_316497817 gis = GIS_316497817();
    gis.loadMapFile("nyc.json");

    std::size_t result = gis.clear();

    EXPECT_EQ(result,(std::size_t)11);
}

//load  tests

TEST(Load, NoFile) {
    GIS_316497817 gis = GIS_316497817();
    std::vector<EntityId> result=gis.loadMapFile("DontExist.json");

    EXPECT_TRUE(result.empty());
}

TEST(Load, emptyArray) {
    GIS_316497817 gis = GIS_316497817();
    std::vector<EntityId> result=gis.loadMapFile("emptyArray.json");

    EXPECT_TRUE(result.empty());
}

TEST(Load, empty) {
    GIS_316497817 gis = GIS_316497817();
    std::vector<EntityId> result=gis.loadMapFile("empty.json");

    EXPECT_TRUE(result.empty());
}

TEST(Load,EntitesRecieved){
    GIS_316497817 gis = GIS_316497817();
    std::vector<EntityId> result=gis.loadMapFile("nyc.json");

    EXPECT_EQ((int)result.size(),11);

    std::vector<EntityId> result2=gis.loadMapFile("sameName.json");

    EXPECT_EQ((int)result2.size(),8);
}

TEST(Load,CoordinatesRejected){
    GIS_316497817 gis = GIS_316497817();
    std::vector<EntityId> result=gis.loadMapFile("cords.json");

    EXPECT_EQ(result.size(),(std::size_t)0);
}

TEST(Load, loadNotSchema){
    GIS_316497817 g = GIS_316497817();
    string testMap = "wrong.json";

    auto loadedIds = g.loadMapFile(testMap);
    EXPECT_TRUE(loadedIds.empty());
}

TEST(Load, loadExistingId){
    GIS_316497817 g = GIS_316497817();
    string testMap = "nyc2.json";
    string usedIdMap = "id.json";

    g.loadMapFile(testMap);
    auto loadedIds = g.loadMapFile(usedIdMap);
    EXPECT_TRUE(loadedIds.empty());
}

TEST(Load, loadMixedIdIdFirst){
    GIS_316497817 g = GIS_316497817();
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
    GIS_316497817 g = GIS_316497817();
    string testMap = "mixedIdsNoIdFirst.json";

    auto loadedIds = g.loadMapFile(testMap);
    EXPECT_EQ((int)loadedIds.size(), 4);
    unordered_set<EntityId> expectedIds = {EntityId("1"), EntityId("2"), EntityId("3"), EntityId("4")};
    for(auto& id : loadedIds){
        EXPECT_TRUE(expectedIds.contains(id));
    }
}

TEST(Load, LoadWayNoJuctionsInFile){
    GIS_316497817 g = GIS_316497817();
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
    GIS_316497817 gis = GIS_316497817();

    std::size_t size = gis.saveMapFile("output.json");

    EXPECT_EQ(size,(std::size_t)0);

    gis.loadMapFile("nyc.json");

    std::size_t size2 = gis.saveMapFile("output.json");

    EXPECT_EQ(size2,(std::size_t)11);
}

TEST(Save,saveAfterClear){
    GIS_316497817 gis = GIS_316497817();

    gis.loadMapFile("noWays.json");

    gis.saveMapFile("output.json");

    gis.clear();

    gis.loadMapFile("output.json");

    std::size_t size = gis.saveMapFile("output2.json");


    EXPECT_EQ(size,(std::size_t)9);
}



//getEntities

TEST(getEntities,result){
    GIS_316497817 gis = GIS_316497817();
    gis.loadMapFile("nyc.json");

    std::vector<EntityId> result=gis.getEntities("Washington Square Park");

    EXPECT_EQ((int)result.size(),4);
}

TEST(getEntities,fromDiffrentFiles){
    GIS_316497817 gis = GIS_316497817();
    gis.loadMapFile("nyc.json");
    gis.loadMapFile("sameName.json");

    std::vector<EntityId> result=gis.getEntities("name");

    EXPECT_EQ((int)result.size(),9);
}

TEST(getEntities,sameName){
    GIS_316497817 gis = GIS_316497817();
    gis.loadMapFile("sameName.json");

    std::vector<EntityId> result=gis.getEntities("name");

    EXPECT_EQ((int)result.size(),8);
}

TEST(getEntities,onACircle){
    GIS_316497817 gis = GIS_316497817();
    gis.loadMapFile("sameName.json");

    Coordinates cord =Coordinates((Longitude)40.731051, (Latitude)-73.999611);

    Meters radius = Meters(3);

    std::vector<EntityId> result=gis.getEntities("name",cord,radius);

    EXPECT_EQ((int)result.size(),1);
}

TEST(getEntities, getClosestEntitesClosestPointTest){
    GIS_316497817 g = GIS_316497817();
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
        if(entity->getEntityId() == idToSearch){
            closestPoint = entity->closestPoint(c);
        }
    }

    EXPECT_EQ(closestPoint, g.getEntityClosestPoint(idToSearch, c));
}

TEST(getEntities, getEntitesByName){
    GIS_316497817 g = GIS_316497817();
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
            expectedIds.insert(entity->getEntityId());
        }
    }

    auto returnedIds = g.getEntities(searchName);

    EXPECT_EQ(returnedIds.size(), expectedIds.size());
    for(const auto& id : returnedIds){
        EXPECT_TRUE(expectedIds.contains(id));
    }
}

TEST(getEntities, getClosestEntitesRadiusTest){
    GIS_316497817 g = GIS_316497817();
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
            expectedIds.insert(entity->getEntityId());
        }
    }

    auto returnedIds = g.getEntities(searchName, searchPoint, radius);

    EXPECT_EQ(returnedIds.size(), expectedIds.size());
    for(const auto& id : returnedIds){
        EXPECT_TRUE(expectedIds.contains(id));
    }
}

TEST(getEntities, getClosestEntitesRadiusAntiMerdianTest){
    GIS_316497817 g = GIS_316497817();
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
            expectedIds.insert(entity->getEntityId());
        }
    }

    auto returnedIds = g.getEntities(searchName, searchPoint, radius);

    EXPECT_EQ(returnedIds.size(), expectedIds.size());
    for(const auto& id : returnedIds){
        EXPECT_TRUE(expectedIds.contains(id));
    }
}

TEST(getEntities, getClosestEntitesByNameIgnoresDiffNamesTest){
    GIS_316497817 g = GIS_316497817();
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
            expectedIds.insert(entity->getEntityId());
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
    GIS_316497817 g = GIS_316497817();
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
                closestWay = entity->getEntityId();
                minInit = true;
            }
        }
    }

    auto result = g.getWayClosestPoint(searchPoint);
    EXPECT_EQ(closestWay, get<1>(result));
    EXPECT_EQ(closestPoint, get<0>(result));
}


TEST(ClosestWay, GetWayClosestPoint2Test){
    GIS_316497817 g = GIS_316497817();
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
                closestWay = entity->getEntityId();
                minInit = true;
            }
        }
    }

    auto result = g.getWayClosestPoint(searchPoint);
    EXPECT_EQ(closestWay, get<1>(result));
    EXPECT_EQ(closestPoint, get<0>(result));
}

TEST(ClosestWay, GetWayClosestPoint3Test){
    GIS_316497817 g = GIS_316497817();
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
                closestWay = entity->getEntityId();
                minInit = true;
            }
        }
    }

    auto result = g.getWayClosestPoint(searchPoint);
    EXPECT_EQ(closestWay, get<1>(result));
    EXPECT_EQ(closestPoint, get<0>(result));
}

TEST(ClosestWay, GetWayClosestPointAntiMerdianTest){
    GIS_316497817 g = GIS_316497817();
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
                closestWay = entity->getEntityId();
                minInit = true;
            }
        }
    }

    auto result = g.getWayClosestPoint(searchPoint);
    EXPECT_EQ(closestWay, get<1>(result));
    EXPECT_EQ(closestPoint, get<0>(result));
}

TEST(ClosestWay, wayClosestPointNoLoadTest){
    GIS_316497817 g = GIS_316497817();
    auto result = g.getWayClosestPoint(Coordinates((Longitude)0,(Latitude)0));

    EXPECT_EQ(EntityId(""), get<1>(result));
    EXPECT_EQ(Coordinates(Longitude(404), Latitude(404)), get<0>(result));
}


TEST(ClosestWay, wayClosestPointLoadNoWaysTest){
    GIS_316497817 g = GIS_316497817();
    string testMap = "noWays.json";
    g.loadMapFile(testMap);
    auto result = g.getWayClosestPoint(Coordinates((Longitude)0,(Latitude)0));

    EXPECT_EQ(EntityId(""), get<1>(result));
    EXPECT_EQ(Coordinates(Longitude(404), Latitude(404)), get<0>(result));
}

TEST(ClosestWay, wayClosestPointAfterClearTest){
    GIS_316497817 g = GIS_316497817();
    string testMap = "lotsOfWays.json";
    g.loadMapFile(testMap);
    g.clear();
    auto result = g.getWayClosestPoint(Coordinates(Longitude(0), Latitude(0)));

    EXPECT_EQ(EntityId(""), get<1>(result));
    EXPECT_EQ(Coordinates(Longitude(404), Latitude(404)), get<0>(result));
}

//general

TEST(general,justRunThings){
    GIS_316497817 gis = GIS_316497817();

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
    GIS_316497817 gis = GIS_316497817();
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
    GIS_316497817 gis = GIS_316497817();
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
    GIS_316497817 gis = GIS_316497817();
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
    GIS_316497817 gis = GIS_316497817();
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
    GIS_316497817 gis1 = GIS_316497817();
    gis1.loadMapFile("empty.json");
    NavigationGIS gis(gis1);

    Navigation_316497817 navigation = Navigation_316497817(gis);
    Coordinates cord1=Coordinates(Longitude(1),Latitude(0));
    Coordinates cord2=Coordinates(Longitude(4),Latitude(0));
    std::unique_ptr<AbstractRoutes> routes =navigation.getRoutes(cord1, cord2);
    std::cout<<4<<std::endl;
    Route invalidRoute = Route(std::vector<std::pair<EntityId, Direction>>(),cord1, cord2,
                               Meters(0), Minutes(0));
    std::cout<<5<<std::endl;
    EXPECT_EQ( (Route&&)routes->shortestDistance(),invalidRoute);
    EXPECT_EQ((Route&&)routes->shortestTime(),invalidRoute);

}

TEST(getRoutes,simpleRoute){
    Meters currLength(0);

    GIS_316497817 gis1 = GIS_316497817();
    gis1.loadMapFile("simpleRoute.json");
    NavigationGIS gis(gis1);
    Navigation_316497817 navigation = Navigation_316497817(gis);

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

    std::unique_ptr<AbstractRoutes> routes =navigation.getRoutes(cord1,cord2);

    EXPECT_EQ((Route&&)routes->shortestDistance(),route);
    EXPECT_EQ((Route&&)routes->shortestTime(),route);
    NavigationValidator validator(gis1);
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, (Route&&)routes->shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, (Route&&)routes->shortestTime()));
}

TEST(getRoutes,simpleRouteMiddleEdges){
    Meters currLength(0);

    GIS_316497817 gis1 = GIS_316497817();
    gis1.loadMapFile("simpleRoute.json");
    NavigationGIS gis(gis1);
    Navigation_316497817 navigation = Navigation_316497817(gis);

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

    std::unique_ptr<AbstractRoutes> routes =navigation.getRoutes(cord1, cord2);


    EXPECT_EQ((Route&&)routes->shortestDistance(),route);
    EXPECT_EQ((Route&&)routes->shortestTime(),route);
    NavigationValidator validator(gis1);
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, (Route&&)routes->shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, (Route&&)routes->shortestTime()));

}

TEST(getRoutes,simpleRouteOffRoad){
    Meters currLength(0);

    GIS_316497817 gis1 = GIS_316497817();
    gis1.loadMapFile("simpleRoute.json");
    NavigationGIS gis(gis1);
    Navigation_316497817 navigation = Navigation_316497817(gis);

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

    std::unique_ptr<AbstractRoutes> routes =navigation.getRoutes(cord3,cord4);

    EXPECT_EQ((Route&&)routes->shortestDistance(),route);
    EXPECT_EQ((Route&&)routes->shortestTime(),route);
    NavigationValidator validator(gis1);
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, (Route&&)routes->shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, (Route&&)routes->shortestTime()));

}

TEST(getRoutes,diffShortFast){
    Meters currLength(0);

    GIS_316497817 gis1 = GIS_316497817();
    gis1.loadMapFile("diffShortFast.json");
    NavigationGIS gis(gis1);
    Navigation_316497817 navigation = Navigation_316497817(gis);

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

    std::unique_ptr<AbstractRoutes> routes =navigation.getRoutes(cord1,cord2);

    EXPECT_EQ((Route&&)routes->shortestDistance(),route1);
    EXPECT_EQ((Route&&)routes->shortestTime(),route2);
    NavigationValidator validator(gis1);
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, (Route&&)routes->shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, (Route&&)routes->shortestTime()));
}

TEST(getRoutes,partillyDiffShortFast){
    Meters currLength(0);

    GIS_316497817 gis1 = GIS_316497817();
    gis1.loadMapFile("partillyDiffShortFast.json");
    NavigationGIS gis(gis1);
    Navigation_316497817 navigation = Navigation_316497817(gis);

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

    std::unique_ptr<AbstractRoutes> routes =navigation.getRoutes(cord1,cord2);

    EXPECT_EQ((Route&&)routes->shortestDistance(),route1);
    EXPECT_EQ((Route&&)routes->shortestTime(),route2);
    NavigationValidator validator(gis1);
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, (Route&&)routes->shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, (Route&&)routes->shortestTime()));

}

TEST(getRoutes,sameDistanceDiffTimes){
    Meters currLength(0);

    GIS_316497817 gis1 = GIS_316497817();
    gis1.loadMapFile("sameDistanceDiffTimes.json");
    NavigationGIS gis(gis1);
    Navigation_316497817 navigation = Navigation_316497817(gis);

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

    std::unique_ptr<AbstractRoutes> routes =navigation.getRoutes(cord1,cord2);

    EXPECT_EQ((Route&&)routes->shortestDistance(),route1);
    EXPECT_EQ((Route&&)routes->shortestTime(),route1);
    NavigationValidator validator(gis1);
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, (Route&&)routes->shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, (Route&&)routes->shortestTime()));
}

TEST(getRoutes,sameDistanceSameTimeMoreWays){
    Meters currLength(0);

    GIS_316497817 gis1 = GIS_316497817();
    gis1.loadMapFile("sameDistanceSameTimeMoreWays.json");
    NavigationGIS gis(gis1);
    Navigation_316497817 navigation = Navigation_316497817(gis);

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

    std::unique_ptr<AbstractRoutes> routes =navigation.getRoutes(cord1,cord2);

    EXPECT_EQ((Route&&)routes->shortestDistance(),route1);
    EXPECT_EQ((Route&&)routes->shortestTime(),route1);
    NavigationValidator validator(gis1);
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, (Route&&)routes->shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, (Route&&)routes->shortestTime()));
}

TEST(getRoutes,complicatedMapTest1){
    Meters currLength(0);

    GIS_316497817 gis1 = GIS_316497817();
    gis1.loadMapFile("complicatedMap.json");
    NavigationGIS gis(gis1);
    Navigation_316497817 navigation = Navigation_316497817(gis);

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

    std::unique_ptr<AbstractRoutes> foundRoutes =navigation.getRoutes(cord1,cord2);
    EXPECT_EQ((Route&&)foundRoutes->shortestDistance(), *std::min_element(routes.begin(), routes.end(), [](Route a, Route b){
        return a.totalLength() < b.totalLength();
    }));

    EXPECT_EQ((Route&&)foundRoutes->shortestTime(), *std::min_element(routes.begin(), routes.end(), [](Route a, Route b){
        return a.estimatedDuration() < b.estimatedDuration();
    }));
    NavigationValidator validator(gis1);
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, foundRoutes->shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, foundRoutes->shortestTime()));
}

TEST(getRoutes,complicatedMapTest2){
    Meters currLength(0);

    GIS_316497817 gis1 = GIS_316497817();
    gis1.loadMapFile("complicatedMap.json");
    NavigationGIS gis(gis1);
    Navigation_316497817 navigation = Navigation_316497817(gis);

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

    std::unique_ptr<AbstractRoutes> foundRoutes =navigation.getRoutes(cord1,cord2);

    EXPECT_EQ((Route&&)foundRoutes->shortestDistance(), *std::min_element(routes.begin(), routes.end(), [](Route a, Route b){
        return a.totalLength() < b.totalLength();
    }));

    EXPECT_EQ((Route&&)foundRoutes->shortestTime(), *std::min_element(routes.begin(), routes.end(), [](Route a, Route b){
        return a.estimatedDuration() < b.estimatedDuration();
    }));
    NavigationValidator validator(gis1);
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, foundRoutes->shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, foundRoutes->shortestTime()));
}

TEST(getRoutes,complicatedMapEdgesNotJunctionTest1){
    Meters currLength(0);

    GIS_316497817 gis1 = GIS_316497817();
    gis1.loadMapFile("complicatedMap.json");
    NavigationGIS gis(gis1);
    Navigation_316497817 navigation = Navigation_316497817(gis);

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

    std::unique_ptr<AbstractRoutes> foundRoutes =navigation.getRoutes(cord1,cord2);
    EXPECT_EQ(foundRoutes->shortestDistance().getWays(), (*std::min_element(routes.begin(), routes.end(), [](Route a, Route b){
        return a.totalLength() < b.totalLength();
    })).getWays());

    EXPECT_EQ(foundRoutes->shortestTime().getWays(), (*std::min_element(routes.begin(), routes.end(), [](Route a, Route b){
        return a.estimatedDuration() < b.estimatedDuration();
    })).getWays());
    NavigationValidator validator(gis1);
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, foundRoutes->shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, foundRoutes->shortestTime()));
}

TEST(getRoutes,complicatedMapEdgesNotJunctionTest2){
    Meters currLength(0);

    GIS_316497817 gis1 = GIS_316497817();
    gis1.loadMapFile("complicatedMap.json");
    NavigationGIS gis(gis1);
    Navigation_316497817 navigation = Navigation_316497817(gis);

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

    std::unique_ptr<AbstractRoutes> foundRoutes =navigation.getRoutes(cord1,cord2);

    EXPECT_EQ(foundRoutes->shortestDistance().getWays(), (*std::min_element(routes.begin(), routes.end(), [](Route a, Route b){
        return a.totalLength() < b.totalLength();
    })).getWays());
    auto a = (*std::min_element(routes.begin(), routes.end(), [](Route a, Route b){
        return a.totalLength() < b.totalLength();
    })).getWays();
    EXPECT_EQ(foundRoutes->shortestTime().getWays(), (*std::min_element(routes.begin(), routes.end(), [](Route a, Route b){
        return a.estimatedDuration() < b.estimatedDuration();
    })).getWays());
    NavigationValidator validator(gis1);
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, foundRoutes->shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, foundRoutes->shortestDistance()));
}



TEST(getRoutes, wayIsHighway){
    Meters currLength(0);

    GIS_316497817 gis1 = GIS_316497817();
    gis1.loadMapFile("wayIsHighway.json");
    NavigationGIS gis(gis1);
    Navigation_316497817 navigation = Navigation_316497817(gis);

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

    std::unique_ptr<AbstractRoutes> routes =navigation.getRoutes(cord1,cord2);



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

    EXPECT_EQ((Route&&)routes->shortestDistance(),route);
    EXPECT_EQ((Route&&)routes->shortestTime(),route);
    NavigationValidator validator(gis1);
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, (Route&&)routes->shortestDistance()));
    EXPECT_TRUE(validator.validateRoute(cord1, cord2, (Route&&)routes->shortestTime()));
}

TEST(getRoutes, notConnectedRoute) {
    GIS_316497817 gis1 = GIS_316497817();
    gis1.loadMapFile("antimerdianWays.json");
    NavigationGIS gis(gis1);
    Navigation_316497817 navigation = Navigation_316497817(gis);

    Coordinates cord1=Coordinates(Longitude(179.99700),Latitude(0.002));

    Coordinates cord2=Coordinates(Longitude(-179.999500),Latitude(0.002));
    auto routes = navigation.getRoutes(cord1, cord2);

    EXPECT_FALSE(routes->isValid());
    EXPECT_EQ(routes->getErrorMessage(), "No ways connecting start and end point");
}






TEST(GISPre, loadMapFile){
    GIS_316497817 gis  = GIS_316497817();
    std::string input_file{"nyc2.json"};
    std::vector<EntityId> loaded_entities = gis.loadMapFile(input_file);
    EXPECT_EQ(loaded_entities, getInputFileEntities(input_file));
}

TEST(GISPre, getEntities) {
    GIS_316497817 gis  = GIS_316497817();
    std::string input_file{"nyc2.json"};
    gis.loadMapFile(input_file);
    std::vector<EntityId> entities = gis.getEntities("Washington Square Park");
    std::sort(entities.begin(), entities.end());
    std::vector<EntityId> expected_entities({EntityId("P1001")});
    EXPECT_EQ(entities, expected_entities);
}

TEST(GISPRe, clearReturnValue) {
    GIS_316497817 gis  = GIS_316497817();
    std::string input_file{"nyc2.json"};
    gis.loadMapFile(input_file);
    std::size_t number_of_cleared_entities = gis.clear();
    EXPECT_EQ(number_of_cleared_entities, getInputFileEntities(input_file).size());
}

TEST(GISPre, saveMapFileReturnValue) {
    GIS_316497817 gis  = GIS_316497817();
    std::string input_file{"nyc2.json"};
    std::string output_file{"out1."+input_file};
    gis.loadMapFile(input_file);
    std::size_t number_of_saved_entities = gis.saveMapFile(output_file);
    EXPECT_EQ(number_of_saved_entities, getInputFileEntities(input_file).size());
    std::remove(output_file.c_str());
}

// Ex1 Tests

TEST(GISEx1, saveMapFileLoadSavedMap) {
    GIS_316497817 gis  = GIS_316497817();
    std::string input_file{"nyc2.json"};
    gis.loadMapFile(input_file);
    std::string output_file{"out2."+input_file};
    std::size_t number_of_saved_entities = gis.saveMapFile(output_file);
    EXPECT_EQ(number_of_saved_entities, getInputFileEntities(input_file).size());
    gis.clear();
    auto loaded_entities = gis.loadMapFile(input_file);
    std::sort(loaded_entities.begin(), loaded_entities.end());
    std::vector<EntityId> expected_entites = getInputFileEntities(input_file);
    std::sort(expected_entites.begin(), expected_entites.end());
    EXPECT_EQ(loaded_entities, expected_entites);
    std::remove(output_file.c_str());
}

TEST(GISEx1, clearLoad) {
    GIS_316497817 gis  = GIS_316497817();
    std::string input_file{"nyc2.json"};
    gis.loadMapFile(input_file);
    gis.clear();
    std::vector<EntityId> loaded_entities = gis.loadMapFile("nyc2.json");
    EXPECT_EQ(loaded_entities, getInputFileEntities(input_file));
}

TEST(GISEx1, loadMapFileMap1) {
    GIS_316497817 gis  = GIS_316497817();
    std::string input_file{"ex1_map1.json"};
    std::vector<EntityId> loaded_entities = gis.loadMapFile(input_file);
    EXPECT_EQ(loaded_entities, getInputFileEntities(input_file));
}

TEST(GISEx1, loadMapFileMap2) {
    GIS_316497817 gis  = GIS_316497817();
    std::string input_file{"ex1_map2.json"};
    std::vector<EntityId> loaded_entities = gis.loadMapFile(input_file);
    EXPECT_EQ(loaded_entities, getInputFileEntities(input_file));
}

TEST(GISEx1, loadMapFileMap1NoIds) {
    GIS_316497817 gis  = GIS_316497817();
    std::vector<EntityId> loaded_entities = gis.loadMapFile("ex1_map1.no_ids.json");
    std::size_t number_of_loaded_entities = loaded_entities.size();
    std::size_t expected_number_of_loaded_entities = 5;
    EXPECT_EQ(number_of_loaded_entities, expected_number_of_loaded_entities);
}

TEST(GISEx1, loadMapFileMap1MixedIds) {
    GIS_316497817 gis  = GIS_316497817();
    std::vector<EntityId> loaded_entities = gis.loadMapFile("ex1_map1.mixed_ids_file.json");
    std::size_t number_of_loaded_entities = loaded_entities.size();
    std::set<std::size_t> expected_number_of_loaded_entities({1, 6, 7});
    EXPECT_TRUE(expected_number_of_loaded_entities.count(number_of_loaded_entities));
}

TEST(GISEx1, twoLoadsSameFile) {
    GIS_316497817 gis  = GIS_316497817();
    gis.loadMapFile("ex1_map1.json");
    gis.loadMapFile("ex1_map1.json");
    std::vector<EntityId> entities = gis.getEntities("LWS");
    std::vector<EntityId> expected_entities({EntityId("Ju02")});
    EXPECT_EQ(entities, expected_entities);
}

TEST(GISEx1, twoLoadsMap1Map2) {
    GIS_316497817 gis  = GIS_316497817();
    std::string input_file1{"ex1_map1.json"};
    std::vector<EntityId> loaded_entities1 = gis.loadMapFile(input_file1);
    EXPECT_EQ(loaded_entities1, getInputFileEntities(input_file1));
    std::string input_file2{"ex1_map2.json"};
    std::vector<EntityId> loaded_entities2 = gis.loadMapFile(input_file2);
    EXPECT_EQ(loaded_entities2, getInputFileEntities(input_file2));
}

TEST(GISEx1, twoLoadsMap1Map2AndGetEntities) {
    GIS_316497817 gis  = GIS_316497817();
    std::string input_file1{"ex1_map1.json"};
    gis.loadMapFile(input_file1);
    std::string input_file2{"ex1_map2.json"};
    gis.loadMapFile(input_file2);
    std::vector<EntityId> entities = gis.getEntities("LWS");
    std::vector<EntityId> expected_entities({EntityId("J2"), EntityId("Ju02")});
    std::sort(entities.begin(), entities.end());
    EXPECT_EQ(entities, expected_entities);
}

constexpr double ex1_epsilon = 1;
template <typename LL> //TODO: Use LatLong in ex2 
bool testLatLongEqualityInEx1(LL tested, LL golden) {
    return std::fabs(static_cast<double>(tested) - static_cast<double>(golden)) < ex1_epsilon;
}

bool testCoordinatsEqualityInEx1(const Coordinates tested, const Coordinates& golden) {
    return testLatLongEqualityInEx1(tested.longitude(), golden.longitude()) &&
           testLatLongEqualityInEx1(tested.latitude(), golden.latitude());
}

TEST(GISEx1, getEntityClosestPointNotKnown) {
    // [1] If the Id is not known returns an empty optional
    GIS_316497817 gis  = GIS_316497817();
    gis.loadMapFile("ex1_map1.json");
    Coordinates point{Longitude{40.0}, Latitude{10.0}};
    Coordinates pnt_center{Longitude{20.0}, Latitude{70.0}};
    Meters pnt_radius{1001.96};
    std::optional<Coordinates> closest_point = gis.getEntityClosestPoint(EntityId{"non-exixtant"}, point);
    ASSERT_FALSE(closest_point);
}

TEST(GISEx1, getEntityClosestPoint1) {
    GIS_316497817 gis  = GIS_316497817();
    gis.loadMapFile("ex1_map1.json");
    Coordinates point{Longitude{40.0}, Latitude{10.0}};
    Coordinates pnt_center{Longitude{20.0}, Latitude{70.0}};
    Meters pnt_radius{1001.96};
    std::optional<Coordinates> closest_point = gis.getEntityClosestPoint(EntityId{"Pnt1"}, point);
    Coordinates expected_point{Longitude{20.0101094228863}, Latitude{69.9916792225197}};
    ASSERT_TRUE(closest_point);
    EXPECT_TRUE(testCoordinatsEqualityInEx1(*closest_point, expected_point));
}

TEST(GISEx1, getEntityClosestPoint2) {
    GIS_316497817 gis  = GIS_316497817();
    gis.loadMapFile("ex1_map2.json");
    Coordinates point{Longitude{-1}, Latitude{-1}};
    Coordinates pnt_center{Longitude{20.730811}, Latitude{10.997455}};
    Meters pnt_radius{100.96};
    std::optional<Coordinates> closest_point = gis.getEntityClosestPoint(EntityId{"P1"}, point);
    Coordinates expected_point{Longitude{20.7299920271604}, Latitude{10.9970330126177}};
    ASSERT_TRUE(closest_point);
    EXPECT_TRUE(testCoordinatsEqualityInEx1(*closest_point, expected_point));
}

TEST(GISEx1, getWayClosestPoint) {
    GIS_316497817 gis  = GIS_316497817();
    gis.loadMapFile("ex1_map1.json");
    Coordinates east_point{Longitude{15}, Latitude{-50}};
    auto closest = gis.getWayClosestPoint(east_point);
    EXPECT_EQ(std::get<1>(closest), EntityId{"East1"});
    Coordinates ju_04{Longitude{20}, Latitude{-20.0}};
    EXPECT_EQ(std::get<0>(closest), ju_04);
}

TEST(GISEx1, getWayClosestPointNorth) {
    GIS_316497817 gis  = GIS_316497817();
    gis.loadMapFile("ex1_map2.json");
    Coordinates northern_point{Longitude{-1}, Latitude{60}};
    auto closest_from_gis = gis.getWayClosestPoint(northern_point);
    EXPECT_EQ(get<1>(closest_from_gis), EntityId{"W1"});
    Coordinates closest_to_northern_way{Longitude{-3.55086}, Latitude{81.3014}};
    EXPECT_TRUE(testCoordinatsEqualityInEx1(get<0>(closest_from_gis), closest_to_northern_way));
}

TEST(GISEx1, getWayClosestPointSouth) {
    GIS_316497817 gis  = GIS_316497817();
    gis.loadMapFile("ex1_map2.json");
    Coordinates southern_point{Longitude{21}, Latitude{-1}};
    auto closest_from_gis = gis.getWayClosestPoint(southern_point);
    Coordinates closest_to_southern_way{Longitude{21.7155}, Latitude{-10.7113}};
    EXPECT_TRUE(testCoordinatsEqualityInEx1(get<0>(closest_from_gis), closest_to_southern_way));
}

TEST(GISEx1, getEntities) {
    GIS_316497817 gis  = GIS_316497817();
    gis.loadMapFile("ex1_map2.json");
    std::vector<EntityId> entities = gis.getEntities("A_way");
    std::vector<EntityId> expected_entities({EntityId("W1"), EntityId("W2")});
    std::sort(entities.begin(), entities.end());
    EXPECT_EQ(entities, expected_entities);
}

TEST(GISEx1, getEntitiesRadius) {
    GIS_316497817 gis  = GIS_316497817();
    gis.loadMapFile("ex1_map2.json");
    Coordinates point{Longitude{-20}, Latitude{-9.94}};
    Meters circle_radius{100'000};
    std::vector<EntityId> entities = gis.getEntities("A_way", point, circle_radius);
    auto distance_from_way = CoordinatesMath::distanceFromSegment(point, ex1_map2_W2.from, ex1_map2_W2.to);
    EXPECT_TRUE(distance_from_way <= circle_radius); // Sanity check
    std::vector<EntityId> expected_entities({EntityId("W2")});
    EXPECT_EQ(entities, expected_entities);
}

TEST(GISEx1, loadMap1SaveClearAndLoad) {
    GIS_316497817 gis  = GIS_316497817();
    std::string input_file{"ex1_map1.json"};
    auto loaded_input = gis.loadMapFile(input_file);
    std::sort(loaded_input.begin(), loaded_input.end());
    std::string output_file{"out."+input_file};
    std::size_t saved = gis.saveMapFile(output_file);
    std::size_t number_of_cleared_entities = gis.clear();
    EXPECT_TRUE(number_of_cleared_entities == saved && saved == getInputFileEntities(input_file).size());
    auto loaded_output = gis.loadMapFile(output_file);
    std::sort(loaded_output.begin(), loaded_output.end());
    EXPECT_EQ(loaded_input, loaded_output);
    std::remove(output_file.c_str());
}

TEST(GISEx1, loadMap2SaveClearAndLoad) {
    GIS_316497817 gis  = GIS_316497817();
    std::string input_file{"ex1_map2.json"};
    auto loaded_input = gis.loadMapFile(input_file);
    std::sort(loaded_input.begin(), loaded_input.end());
    std::string output_file{"out."+input_file};
    std::size_t saved = gis.saveMapFile(output_file);
    std::size_t number_of_cleared_entities = gis.clear();
    EXPECT_TRUE(number_of_cleared_entities == saved && saved == getInputFileEntities(input_file).size());
    auto loaded_output = gis.loadMapFile(output_file);
    std::sort(loaded_output.begin(), loaded_output.end());
    EXPECT_EQ(loaded_input, loaded_output);
    std::remove(output_file.c_str());
}

TEST(GISEx1, twoLoadsMap1Map2ClearSaveAndLoad) {
    GIS_316497817 gis  = GIS_316497817();
    std::string input_file1{"ex1_map1.json"};
    std::vector<EntityId> loaded_entities1 = gis.loadMapFile(input_file1);
    std::string input_file2{"ex1_map2.json"};
    std::vector<EntityId> loaded_entities2 = gis.loadMapFile(input_file2);
    std::string output_file{"out_ex1_map1map2.json"};
    std::size_t number_of_saved_entities = gis.saveMapFile(output_file);
    std::size_t number_of_cleared_entities = gis.clear();
    EXPECT_TRUE(getInputFileEntities(input_file1).size() + getInputFileEntities(input_file2).size() == number_of_saved_entities
                && number_of_saved_entities == number_of_cleared_entities);
    std::vector<EntityId> loaded_entities = gis.loadMapFile(output_file);
    std::vector<EntityId> combined_entities = getCombinedFilesEntities(input_file1, input_file2);
    std::sort(combined_entities.begin(), combined_entities.end());
    std::sort(loaded_entities.begin(), loaded_entities.end());
    EXPECT_EQ(loaded_entities, combined_entities);
    std::remove(output_file.c_str());
}

TEST(GISEx1, loadNonExistantFile) {
    GIS_316497817 gis  = GIS_316497817();
    std::string input_file{"___non_existant_file___"};
    std::vector<EntityId> loaded_entities = gis.loadMapFile(input_file);
    std::size_t expected_number_of_loaded_entities = 0;
    EXPECT_EQ(loaded_entities.size(), expected_number_of_loaded_entities);
}

TEST(GISEx1, loadMapFileTwoWays) {
    GIS_316497817 gis  = GIS_316497817();
    std::string input_file{"ex1_two_ways.json"};
    auto loaded_entities = gis.loadMapFile(input_file);
    EXPECT_EQ(loaded_entities, getInputFileEntities(input_file));
}

TEST(GISEx1, getWayClosestPointTwoWays1) {
    GIS_316497817 gis  = GIS_316497817();
    std::string input_file{"ex1_two_ways.json"};
    gis.loadMapFile(input_file);
    Coordinates point{Longitude{47}, Latitude{30.312}};
    auto closest_way = gis.getWayClosestPoint(point);
    auto closest_point = CoordinatesMath::closestPointOnSegment(point, ex1_two_ways_LeftWay.from, ex1_two_ways_LeftWay.to);
    EXPECT_EQ(get<0>(closest_way), closest_point);
    EXPECT_EQ(get<1>(closest_way), EntityId{"LeftWay"});
}

TEST(GISEx1, loadMapFileTwoWays180thMeridian) {
    GIS_316497817 gis  = GIS_316497817();
    std::string input_file{"ex1_two_ways_180th_meridian.json"};
    auto loaded_entities = gis.loadMapFile(input_file);
    EXPECT_EQ(loaded_entities, getInputFileEntities(input_file));
}

TEST(GISEx2, getEntityClosestPoint1) {
    GIS_316497817 gis  = GIS_316497817();
    gis.loadMapFile("ex1_map1.json");
    Coordinates point{Longitude{40.0}, Latitude{10.0}};
    Coordinates pnt_center{Longitude{20.0}, Latitude{70.0}};
    Meters pnt_radius{1001.96};
    std::optional<Coordinates> closest_point = gis.getEntityClosestPoint(EntityId{"Pnt1"}, point);
    Coordinates expected_point = CoordinatesMath::closestPointOnCircle(point, pnt_center, pnt_radius);
    ASSERT_TRUE(closest_point);
    EXPECT_EQ(*closest_point, expected_point);
}

TEST(GISEx2, getEntityClosestPoint2) {
    GIS_316497817 gis  = GIS_316497817();
    gis.loadMapFile("ex1_map2.json");
    Coordinates point{Longitude{-1}, Latitude{-1}};
    Coordinates pnt_center{Longitude{20.730811}, Latitude{10.997455}};
    Meters pnt_radius{100.96};
    std::optional<Coordinates> closest_point = gis.getEntityClosestPoint(EntityId{"P1"}, point);
    Coordinates expected_point = CoordinatesMath::closestPointOnCircle(point, pnt_center, pnt_radius);
    ASSERT_TRUE(closest_point);
    EXPECT_EQ(*closest_point, expected_point);
}





