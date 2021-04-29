COMP = g++-10

GOOGLE_TEST_INCLUDE = /usr/src/googletest/googletest/include/
GOOGLE_LIB = /usr/src/googletest/googletest/gcc-10-build/lib/

#put all your object files here
OBJS = main.o Entity.o Circle.o Line.o Point.o GIS_316497817.o JsonHandler.o Logger.o POI.o Way.o GridMath.o Grid.o GridCell.o GridCoordinates.o GridEntity.o SpiralCoordinatesTraverser.o Navigation_316497817.o NavigationValidator.o RouteFinder.o tests.o#other objects should be placed here!
#The executabel filename DON'T CHANGE
EXEC = testrunner
CPP_COMP_FLAG = -std=c++20 -Werror -Wall -Wextra -Werror -pedantic-errors -DNDEBUG -I $(GOOGLE_TEST_INCLUDE)
CPP_LINK_FLAG = -L $(GOOGLE_LIB) -lgtest -lgtest_main -pthread

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) $(CPP_LINK_FLAG) -o $@

#a rule for building a simple c++ source file
#use g++ -MM main.cpp to see dependencies
main.o:  #put dependencies here!
	$(COMP) $(CPP_COMP_FLAG) -c main.cpp

Entity.o: ../GIS/MapEntities/Entity.cpp ../GIS/MapEntities/Entity.h \
 ../GIS/MapEntities/../../Common/GISNamedTypes.h \
 ../GIS/MapEntities/../../Common/NamedType.h \
 ../GIS/MapEntities/../../Common/Double.h \
 ../GIS/MapEntities/../Geometry/Geometry.h \
 ../GIS/MapEntities/../Geometry/../../Common/CoordinatesMath.h \
 ../GIS/MapEntities/../Geometry/../Grid/GridMath.h \
 ../GIS/MapEntities/../Geometry/../Grid/GridCoordinates.h \
 ../GIS/MapEntities/../Geometry/../Grid/SpiralCoordinatesTraverser.h
	$(COMP) $(CPP_COMP_FLAG) -c ../GIS/MapEntities/Entity.cpp

Circle.o: ../GIS/Geometry/Circle.cpp ../GIS/Geometry/Geometry.h \
 ../GIS/Geometry/../../Common/GISNamedTypes.h \
 ../GIS/Geometry/../../Common/NamedType.h \
 ../GIS/Geometry/../../Common/Double.h \
 ../GIS/Geometry/../../Common/CoordinatesMath.h \
 ../GIS/Geometry/../Grid/GridMath.h \
 ../GIS/Geometry/../Grid/GridCoordinates.h \
 ../GIS/Geometry/../Grid/SpiralCoordinatesTraverser.h
	$(COMP) $(CPP_COMP_FLAG) -c ../GIS/Geometry/Circle.cpp

Line.o: ../GIS/Geometry/Line.cpp ../GIS/Geometry/Geometry.h \
 ../GIS/Geometry/../../Common/GISNamedTypes.h \
 ../GIS/Geometry/../../Common/NamedType.h \
 ../GIS/Geometry/../../Common/Double.h \
 ../GIS/Geometry/../../Common/CoordinatesMath.h \
 ../GIS/Geometry/../Grid/GridMath.h \
 ../GIS/Geometry/../Grid/GridCoordinates.h \
 ../GIS/Geometry/../Grid/SpiralCoordinatesTraverser.h
	$(COMP) $(CPP_COMP_FLAG) -c ../GIS/Geometry/Line.cpp

Point.o: ../GIS/Geometry/Point.cpp ../GIS/Geometry/Geometry.h \
 ../GIS/Geometry/../../Common/GISNamedTypes.h \
 ../GIS/Geometry/../../Common/NamedType.h \
 ../GIS/Geometry/../../Common/Double.h \
 ../GIS/Geometry/../../Common/CoordinatesMath.h \
 ../GIS/Geometry/../Grid/GridMath.h \
 ../GIS/Geometry/../Grid/GridCoordinates.h \
 ../GIS/Geometry/../Grid/SpiralCoordinatesTraverser.h
	$(COMP) $(CPP_COMP_FLAG) -c ../GIS/Geometry/Point.cpp

GIS_316497817.o: ../GIS/GIS_316497817.cpp ../GIS/GIS_316497817.h \
 ../GIS/../Common/GISNamedTypes.h ../GIS/../Common/NamedType.h \
 ../GIS/../Common/Double.h ../GIS/Grid/Grid.h ../GIS/Grid/GridCell.h \
 ../GIS/Grid/GridEntity.h ../GIS/Grid/../MapEntities/Entity.h \
 ../GIS/Grid/../MapEntities/../Geometry/Geometry.h \
 ../GIS/Grid/../MapEntities/../Geometry/../../Common/CoordinatesMath.h \
 ../GIS/Grid/../MapEntities/../Geometry/../Grid/GridMath.h \
 ../GIS/Grid/../MapEntities/../Geometry/../Grid/GridCoordinates.h \
 ../GIS/Grid/../MapEntities/../Geometry/../Grid/SpiralCoordinatesTraverser.h \
 ../GIS/JsonHandler.h ../GIS/Logger.h ../GIS/BasicEntityInfo.h \
 ../GIS/MapEntities/Junction.h ../GIS/MapEntities/POI.h ../GIS/Way.h \
 ../GIS/../Common/AbstractWay.h ../GIS/BasicWayInfo.h \
 ../GIS/../Common/Restrictions.h ../GIS/GISExceptions.h \
 ../GIS/../Common/AbstractGIS.h ../GIS/../Common/GISRegistration.h
	$(COMP) $(CPP_COMP_FLAG) -c ../GIS/GIS_316497817.cpp

JsonHandler.o: ../GIS/JsonHandler.cpp ../GIS/JsonHandler.h \
 ../GIS/MapEntities/Entity.h \
 ../GIS/MapEntities/../../Common/GISNamedTypes.h \
 ../GIS/MapEntities/../../Common/NamedType.h \
 ../GIS/MapEntities/../../Common/Double.h \
 ../GIS/MapEntities/../Geometry/Geometry.h \
 ../GIS/MapEntities/../Geometry/../../Common/CoordinatesMath.h \
 ../GIS/MapEntities/../Geometry/../Grid/GridMath.h \
 ../GIS/MapEntities/../Geometry/../Grid/GridCoordinates.h \
 ../GIS/MapEntities/../Geometry/../Grid/SpiralCoordinatesTraverser.h \
 ../GIS/Logger.h ../GIS/BasicEntityInfo.h ../GIS/MapEntities/Junction.h \
 ../GIS/MapEntities/POI.h ../GIS/Way.h ../GIS/../Common/AbstractWay.h \
 ../GIS/Grid/Grid.h ../GIS/Grid/GridCell.h ../GIS/Grid/GridEntity.h \
 ../GIS/BasicWayInfo.h
	$(COMP) $(CPP_COMP_FLAG) -c ../GIS/JsonHandler.cpp

Logger.o: ../GIS/Logger.cpp ../GIS/Logger.h
	$(COMP) $(CPP_COMP_FLAG) -c ../GIS/Logger.cpp

POI.o: ../GIS/MapEntities/POI.cpp ../GIS/MapEntities/POI.h \
 ../GIS/MapEntities/../../Common/GISNamedTypes.h \
 ../GIS/MapEntities/../../Common/NamedType.h \
 ../GIS/MapEntities/../../Common/Double.h \
 ../GIS/MapEntities/../Geometry/Geometry.h \
 ../GIS/MapEntities/../Geometry/../../Common/CoordinatesMath.h \
 ../GIS/MapEntities/../Geometry/../Grid/GridMath.h \
 ../GIS/MapEntities/../Geometry/../Grid/GridCoordinates.h \
 ../GIS/MapEntities/../Geometry/../Grid/SpiralCoordinatesTraverser.h \
 ../GIS/MapEntities/Entity.h
	$(COMP) $(CPP_COMP_FLAG) -c ../GIS/MapEntities/POI.cpp

Way.o: ../GIS/Way.cpp ../GIS/Way.h ../GIS/MapEntities/Entity.h \
 ../GIS/MapEntities/../../Common/GISNamedTypes.h \
 ../GIS/MapEntities/../../Common/NamedType.h \
 ../GIS/MapEntities/../../Common/Double.h \
 ../GIS/MapEntities/../Geometry/Geometry.h \
 ../GIS/MapEntities/../Geometry/../../Common/CoordinatesMath.h \
 ../GIS/MapEntities/../Geometry/../Grid/GridMath.h \
 ../GIS/MapEntities/../Geometry/../Grid/GridCoordinates.h \
 ../GIS/MapEntities/../Geometry/../Grid/SpiralCoordinatesTraverser.h \
 ../GIS/../Common/AbstractWay.h
	$(COMP) $(CPP_COMP_FLAG) -c ../GIS/Way.cpp

Grid.o: ../GIS/Grid/Grid.cpp ../GIS/Grid/Grid.h \
 ../GIS/Grid/../../Common/GISNamedTypes.h \
 ../GIS/Grid/../../Common/NamedType.h ../GIS/Grid/../../Common/Double.h \
 ../GIS/Grid/GridCell.h ../GIS/Grid/GridEntity.h \
 ../GIS/Grid/../MapEntities/Entity.h \
 ../GIS/Grid/../MapEntities/../Geometry/Geometry.h \
 ../GIS/Grid/../MapEntities/../Geometry/../../Common/CoordinatesMath.h \
 ../GIS/Grid/../MapEntities/../Geometry/../Grid/GridMath.h \
 ../GIS/Grid/../MapEntities/../Geometry/../Grid/GridCoordinates.h \
 ../GIS/Grid/../MapEntities/../Geometry/../Grid/SpiralCoordinatesTraverser.h
	$(COMP) $(CPP_COMP_FLAG) -c ../GIS/Grid/Grid.cpp

GridMath.o: ../GIS/Grid/GridMath.cpp ../GIS/Grid/GridMath.h \
 ../GIS/Grid/../../Common/CoordinatesMath.h \
 ../GIS/Grid/../../Common/GISNamedTypes.h \
 ../GIS/Grid/../../Common/NamedType.h ../GIS/Grid/../../Common/Double.h \
 ../GIS/Grid/../Geometry/Geometry.h \
 ../GIS/Grid/../Geometry/../Grid/SpiralCoordinatesTraverser.h \
 ../GIS/Grid/../Geometry/../Grid/GridCoordinates.h
	$(COMP) $(CPP_COMP_FLAG) -c ../GIS/Grid/GridMath.cpp

GridCoordinates.o: ../GIS/Grid/GridCoordinates.cpp \
 ../GIS/Grid/GridCoordinates.h ../GIS/Grid/../../Common/GISNamedTypes.h \
 ../GIS/Grid/../../Common/NamedType.h ../GIS/Grid/../../Common/Double.h
	$(COMP) $(CPP_COMP_FLAG) -c ../GIS/Grid/GridCoordinates.cpp

GridCell.o: ../GIS/Grid/GridCell.cpp ../GIS/Grid/GridCell.h \
 ../GIS/Grid/GridEntity.h ../GIS/Grid/../MapEntities/Entity.h \
 ../GIS/Grid/../MapEntities/../../Common/GISNamedTypes.h \
 ../GIS/Grid/../MapEntities/../../Common/NamedType.h \
 ../GIS/Grid/../MapEntities/../../Common/Double.h \
 ../GIS/Grid/../MapEntities/../Geometry/Geometry.h \
 ../GIS/Grid/../MapEntities/../Geometry/../../Common/CoordinatesMath.h \
 ../GIS/Grid/../MapEntities/../Geometry/../Grid/GridMath.h \
 ../GIS/Grid/../MapEntities/../Geometry/../Grid/GridCoordinates.h \
 ../GIS/Grid/../MapEntities/../Geometry/../Grid/SpiralCoordinatesTraverser.h
	$(COMP) $(CPP_COMP_FLAG) -c ../GIS/Grid/GridCell.cpp

GridEntity.o: ../GIS/Grid/GridEntity.cpp ../GIS/Grid/GridEntity.h \
 ../GIS/Grid/../MapEntities/Entity.h \
 ../GIS/Grid/../MapEntities/../../Common/GISNamedTypes.h \
 ../GIS/Grid/../MapEntities/../../Common/NamedType.h \
 ../GIS/Grid/../MapEntities/../../Common/Double.h \
 ../GIS/Grid/../MapEntities/../Geometry/Geometry.h \
 ../GIS/Grid/../MapEntities/../Geometry/../../Common/CoordinatesMath.h \
 ../GIS/Grid/../MapEntities/../Geometry/../Grid/GridMath.h \
 ../GIS/Grid/../MapEntities/../Geometry/../Grid/GridCoordinates.h \
 ../GIS/Grid/../MapEntities/../Geometry/../Grid/SpiralCoordinatesTraverser.h
	$(COMP) $(CPP_COMP_FLAG) -c ../GIS/Grid/GridEntity.cpp

SpiralCoordinatesTraverser.o: ../GIS/Grid/SpiralCoordinatesTraverser.cpp \
 ../GIS/Grid/SpiralCoordinatesTraverser.h \
 ../GIS/Grid/../../Common/GISNamedTypes.h \
 ../GIS/Grid/../../Common/NamedType.h ../GIS/Grid/../../Common/Double.h \
 ../GIS/Grid/GridCoordinates.h ../GIS/Grid/GridMath.h \
 ../GIS/Grid/../../Common/CoordinatesMath.h \
 ../GIS/Grid/../Geometry/Geometry.h
	$(COMP) $(CPP_COMP_FLAG) -c ../GIS/Grid/SpiralCoordinatesTraverser.cpp

Navigation_316497817.o: ../Navigation/Navigation_316497817.cpp \
 ../Navigation/Navigation_316497817.h \
 ../Navigation/../Common/NavigationGIS.h \
 ../Navigation/../Common/AbstractGIS.h \
 ../Navigation/../Common/GISNamedTypes.h \
 ../Navigation/../Common/NamedType.h ../Navigation/../Common/Double.h \
 ../Navigation/../Common/Restrictions.h \
 ../Navigation/../Common/AbstractWay.h ../Navigation/Routes.h \
 ../Navigation/Route.h ../Navigation/../Common/AbstractRoute.h \
 ../Navigation/../Common/AbstractRoutes.h ../Navigation/RouteSection.h \
 ../Navigation/../Common/AbstractNavigation.h ../Navigation/RouteFinder.h \
 ../Navigation/../Common/CoordinatesMath.h \
 ../Navigation/../Common/NavigationRegistration.h
	$(COMP) $(CPP_COMP_FLAG) -c ../Navigation/Navigation_316497817.cpp

NavigationValidator.o: ../Simulator/NavigationValidator.cpp \
 ../Simulator/NavigationValidator.h \
 ../Simulator/../Common/GISNamedTypes.h \
 ../Simulator/../Common/NamedType.h ../Simulator/../Common/Double.h \
 ../Simulator/../Common/AbstractGIS.h \
 ../Simulator/../Common/Restrictions.h \
 ../Simulator/../Common/AbstractWay.h \
 ../Simulator/../Common/AbstractRoute.h \
 ../Simulator/../Common/CoordinatesMath.h
	$(COMP) $(CPP_COMP_FLAG) -c  ../Simulator/NavigationValidator.cpp

RouteFinder.o: ../Navigation/RouteFinder.cpp \
 ../Navigation/PriorityQueue.h ../Navigation/RouteFinder.h \
 ../Navigation/Route.h ../Navigation/../Common/GISNamedTypes.h \
 ../Navigation/../Common/NamedType.h ../Navigation/../Common/Double.h \
 ../Navigation/../Common/AbstractRoute.h \
 ../Navigation/../Common/NavigationGIS.h \
 ../Navigation/../Common/AbstractGIS.h \
 ../Navigation/../Common/Restrictions.h \
 ../Navigation/../Common/AbstractWay.h ../Navigation/RouteSection.h \
 ../Navigation/../Common/CoordinatesMath.h
	$(COMP) $(CPP_COMP_FLAG) -c  ../Navigation/RouteFinder.cpp

tests.o: tests.cpp ../Common/GISNamedTypes.h ../Common/NamedType.h \
 ../Common/Double.h ../Common/CoordinatesMath.h ../GIS/Grid/Grid.h \
 ../GIS/Grid/GridCell.h ../GIS/Grid/GridEntity.h \
 ../GIS/Grid/../MapEntities/Entity.h \
 ../GIS/Grid/../MapEntities/../Geometry/Geometry.h \
 ../GIS/Grid/../MapEntities/../Geometry/../Grid/GridMath.h \
 ../GIS/Grid/../MapEntities/../Geometry/../Grid/GridCoordinates.h \
 ../GIS/Grid/../MapEntities/../Geometry/../Grid/SpiralCoordinatesTraverser.h \
 ../GIS/GIS_316497817.h ../GIS/JsonHandler.h ../GIS/Logger.h \
 ../GIS/BasicEntityInfo.h ../GIS/MapEntities/Junction.h \
 ../GIS/MapEntities/POI.h ../GIS/Way.h ../GIS/../Common/AbstractWay.h \
 ../GIS/BasicWayInfo.h ../GIS/../Common/Restrictions.h \
 ../GIS/GISExceptions.h ../GIS/../Common/AbstractGIS.h \
 ../Simulator/NavigationValidator.h \
 ../Simulator/../Common/AbstractRoute.h \
 ../Navigation/Navigation_316497817.h \
 ../Navigation/../Common/NavigationGIS.h ../Navigation/Routes.h \
 ../Navigation/Route.h ../Navigation/../Common/AbstractRoutes.h \
 ../Navigation/RouteSection.h \
 ../Navigation/../Common/AbstractNavigation.h mocks.h
	$(COMP) $(CPP_COMP_FLAG) -c tests.cpp

clean:
	rm -f $(OBJS) $(EXEC)