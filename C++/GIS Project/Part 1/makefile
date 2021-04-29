COMP = g++-10

GOOGLE_TEST_INCLUDE = /usr/src/googletest/googletest/include/
GOOGLE_LIB = /usr/src/googletest/googletest/gcc-10-build/lib/ 

#put all your object files here
OBJS = main.o Entity.o Circle.o Line.o Point.o GIS.o JsonHandler.o Logger.o POI.o Way.o GridMath.o Grid.o GridCell.o GridCoordinates.o GridEntity.o SpiralCoordinatesTraverser.o Navigation.o NavigationValidator.o Restrictions.o RouteFinder.o tests.o#other objects should be placed here!
#The executabel filename DON'T CHANGE
EXEC = ex2
CPP_COMP_FLAG = -std=c++20 -Werror -Wall -Wextra -Werror -pedantic-errors -DNDEBUG -I $(GOOGLE_TEST_INCLUDE)
CPP_LINK_FLAG = -L $(GOOGLE_LIB) -lgtest -lgtest_main -pthread

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) $(CPP_LINK_FLAG) -o $@

#a rule for building a simple c++ source file
#use g++ -MM main.cpp to see dependencies
main.o:  #put dependencies here!
	$(COMP) $(CPP_COMP_FLAG) -c main.cpp

Entity.o: MapEntities/Entity.cpp MapEntities/Entity.h MapEntities/../GISNamedTypes.h MapEntities/../NamedType.h MapEntities/../Double.h MapEntities/../Geometry/Geometry.h MapEntities/../Geometry/../CoordinatesMath.h MapEntities/../Geometry/../Grid/GridMath.h MapEntities/../Geometry/../Grid/GridCoordinates.h MapEntities/../Geometry/../Grid/SpiralCoordinatesTraverser.h
	$(COMP) $(CPP_COMP_FLAG) -c MapEntities/Entity.cpp

Circle.o: Geometry/Circle.cpp Geometry/Geometry.h Geometry/../GISNamedTypes.h Geometry/../NamedType.h Geometry/../Double.h Geometry/../CoordinatesMath.h Geometry/../Grid/GridMath.h Geometry/../Grid/GridCoordinates.h Geometry/../Grid/SpiralCoordinatesTraverser.h
	$(COMP) $(CPP_COMP_FLAG) -c Geometry/Circle.cpp

Line.o: Geometry/Line.cpp Geometry/Geometry.h Geometry/../GISNamedTypes.h Geometry/../NamedType.h Geometry/../Double.h Geometry/../CoordinatesMath.h Geometry/../Grid/GridMath.h Geometry/../Grid/GridCoordinates.h Geometry/../Grid/SpiralCoordinatesTraverser.h
	$(COMP) $(CPP_COMP_FLAG) -c Geometry/Line.cpp

Point.o: Geometry/Point.cpp Geometry/Geometry.h Geometry/../GISNamedTypes.h Geometry/../NamedType.h Geometry/../Double.h Geometry/../CoordinatesMath.h Geometry/../Grid/GridMath.h Geometry/../Grid/GridCoordinates.h Geometry/../Grid/SpiralCoordinatesTraverser.h
	$(COMP) $(CPP_COMP_FLAG) -c Geometry/Point.cpp

GIS.o: GIS.cpp GIS.h GISNamedTypes.h NamedType.h Double.h Grid/Grid.h Grid/GridCell.h Grid/GridEntity.h Grid/../MapEntities/Entity.h Grid/../MapEntities/../Geometry/Geometry.h Grid/../MapEntities/../Geometry/../CoordinatesMath.h Grid/../MapEntities/../Geometry/../Grid/GridMath.h Grid/../MapEntities/../Geometry/../Grid/GridCoordinates.h Grid/../MapEntities/../Geometry/../Grid/SpiralCoordinatesTraverser.h JsonHandler.h Logger.h BasicEntityInfo.h MapEntities/Junction.h MapEntities/POI.h MapEntities/Way.h BasicWayInfo.h Navigation/Restrictions.h GISExceptions.h
	$(COMP) $(CPP_COMP_FLAG) -c GIS.cpp

JsonHandler.o: JsonHandler.cpp JsonHandler.h MapEntities/Entity.h MapEntities/../GISNamedTypes.h MapEntities/../NamedType.h MapEntities/../Double.h MapEntities/../Geometry/Geometry.h MapEntities/../Geometry/../CoordinatesMath.h MapEntities/../Geometry/../Grid/GridMath.h MapEntities/../Geometry/../Grid/GridCoordinates.h MapEntities/../Geometry/../Grid/SpiralCoordinatesTraverser.h Logger.h BasicEntityInfo.h MapEntities/Junction.h MapEntities/POI.h MapEntities/Way.h Grid/Grid.h Grid/GridCell.h Grid/GridEntity.h
	$(COMP) $(CPP_COMP_FLAG) -c JsonHandler.cpp

Logger.o: Logger.cpp Logger.h
	$(COMP) $(CPP_COMP_FLAG) -c Logger.cpp

POI.o: MapEntities/POI.cpp MapEntities/POI.h MapEntities/../GISNamedTypes.h MapEntities/../NamedType.h MapEntities/../Double.h MapEntities/../Geometry/Geometry.h MapEntities/../Geometry/../CoordinatesMath.h MapEntities/../Geometry/../Grid/GridMath.h MapEntities/../Geometry/../Grid/GridCoordinates.h MapEntities/../Geometry/../Grid/SpiralCoordinatesTraverser.h MapEntities/Entity.h
	$(COMP) $(CPP_COMP_FLAG) -c MapEntities/POI.cpp

Way.o: MapEntities/Way.cpp MapEntities/Way.h MapEntities/Entity.h MapEntities/../GISNamedTypes.h MapEntities/../NamedType.h MapEntities/../Double.h MapEntities/../Geometry/Geometry.h MapEntities/../Geometry/../CoordinatesMath.h MapEntities/../Geometry/../Grid/GridMath.h MapEntities/../Geometry/../Grid/GridCoordinates.h MapEntities/../Geometry/../Grid/SpiralCoordinatesTraverser.h
	$(COMP) $(CPP_COMP_FLAG) -c MapEntities/Way.cpp

Grid.o: Grid/Grid.cpp Grid/Grid.h Grid/../GISNamedTypes.h Grid/../NamedType.h Grid/../Double.h Grid/GridCell.h Grid/GridEntity.h Grid/../MapEntities/Entity.h Grid/../MapEntities/../Geometry/Geometry.h Grid/../MapEntities/../Geometry/../CoordinatesMath.h Grid/../MapEntities/../Geometry/../Grid/GridMath.h Grid/../MapEntities/../Geometry/../Grid/GridCoordinates.h Grid/../MapEntities/../Geometry/../Grid/SpiralCoordinatesTraverser.h
	$(COMP) $(CPP_COMP_FLAG) -c Grid/Grid.cpp

GridMath.o: Grid/GridMath.cpp Grid/GridMath.h Grid/../CoordinatesMath.h Grid/../GISNamedTypes.h Grid/../NamedType.h Grid/../Double.h Grid/../Geometry/Geometry.h Grid/../Geometry/../Grid/SpiralCoordinatesTraverser.h Grid/../Geometry/../Grid/GridCoordinates.h
	$(COMP) $(CPP_COMP_FLAG) -c Grid/GridMath.cpp

GridCoordinates.o: Grid/GridCoordinates.cpp Grid/GridCoordinates.h Grid/../GISNamedTypes.h Grid/../NamedType.h Grid/../Double.h
	$(COMP) $(CPP_COMP_FLAG) -c Grid/GridCoordinates.cpp

GridCell.o: Grid/GridCell.cpp Grid/GridCell.h Grid/GridEntity.h Grid/../MapEntities/Entity.h Grid/../MapEntities/../GISNamedTypes.h Grid/../MapEntities/../NamedType.h Grid/../MapEntities/../Double.h Grid/../MapEntities/../Geometry/Geometry.h Grid/../MapEntities/../Geometry/../CoordinatesMath.h Grid/../MapEntities/../Geometry/../Grid/GridMath.h Grid/../MapEntities/../Geometry/../Grid/GridCoordinates.h Grid/../MapEntities/../Geometry/../Grid/SpiralCoordinatesTraverser.h
	$(COMP) $(CPP_COMP_FLAG) -c Grid/GridCell.cpp

GridEntity.o: Grid/GridEntity.cpp Grid/GridEntity.h Grid/../MapEntities/Entity.h Grid/../MapEntities/../GISNamedTypes.h Grid/../MapEntities/../NamedType.h Grid/../MapEntities/../Double.h Grid/../MapEntities/../Geometry/Geometry.h Grid/../MapEntities/../Geometry/../CoordinatesMath.h Grid/../MapEntities/../Geometry/../Grid/GridMath.h Grid/../MapEntities/../Geometry/../Grid/GridCoordinates.h Grid/../MapEntities/../Geometry/../Grid/SpiralCoordinatesTraverser.h
	$(COMP) $(CPP_COMP_FLAG) -c Grid/GridEntity.cpp

SpiralCoordinatesTraverser.o: Grid/SpiralCoordinatesTraverser.cpp Grid/SpiralCoordinatesTraverser.h Grid/../GISNamedTypes.h Grid/../NamedType.h Grid/../Double.h Grid/GridCoordinates.h Grid/GridMath.h Grid/../CoordinatesMath.h Grid/../Geometry/Geometry.h
	$(COMP) $(CPP_COMP_FLAG) -c Grid/SpiralCoordinatesTraverser.cpp

Navigation.o: Navigation/Navigation.cpp Navigation/Navigation.h Navigation/NavigationGIS.h Navigation/../GIS.h Navigation/../GISNamedTypes.h Navigation/../NamedType.h Navigation/../Double.h Navigation/../Grid/Grid.h Navigation/../Grid/GridCell.h Navigation/../Grid/GridEntity.h Navigation/../Grid/../MapEntities/Entity.h Navigation/../Grid/../MapEntities/../Geometry/Geometry.h Navigation/../Grid/../MapEntities/../Geometry/../CoordinatesMath.h Navigation/../Grid/../MapEntities/../Geometry/../Grid/GridMath.h Navigation/../Grid/../MapEntities/../Geometry/../Grid/GridCoordinates.h Navigation/../Grid/../MapEntities/../Geometry/../Grid/SpiralCoordinatesTraverser.h Navigation/../JsonHandler.h Navigation/../Logger.h Navigation/../BasicEntityInfo.h Navigation/../MapEntities/Junction.h Navigation/../MapEntities/POI.h Navigation/../MapEntities/Way.h Navigation/../BasicWayInfo.h Navigation/../Navigation/Restrictions.h Navigation/../GISExceptions.h Navigation/Routes.h Navigation/Route.h Navigation/RouteSection.h Navigation/RouteFinder.h
	$(COMP) $(CPP_COMP_FLAG) -c Navigation/Navigation.cpp

NavigationValidator.o: tests/NavigationValidator.cpp tests/NavigationValidator.h tests/../GIS.h tests/../GISNamedTypes.h tests/../NamedType.h tests/../Double.h tests/../Grid/Grid.h tests/../Grid/GridCell.h tests/../Grid/GridEntity.h tests/../Grid/../MapEntities/Entity.h tests/../Grid/../MapEntities/../Geometry/Geometry.h tests/../Grid/../MapEntities/../Geometry/../CoordinatesMath.h tests/../Grid/../MapEntities/../Geometry/../Grid/GridMath.h tests/../Grid/../MapEntities/../Geometry/../Grid/GridCoordinates.h tests/../Grid/../MapEntities/../Geometry/../Grid/SpiralCoordinatesTraverser.h tests/../JsonHandler.h tests/../Logger.h tests/../BasicEntityInfo.h tests/../MapEntities/Junction.h tests/../MapEntities/POI.h tests/../MapEntities/Way.h tests/../BasicWayInfo.h tests/../Navigation/Restrictions.h tests/../GISExceptions.h tests/../Navigation/Route.h
	$(COMP) $(CPP_COMP_FLAG) -c  tests/NavigationValidator.cpp

Restrictions.o: Navigation/Restrictions.cpp Navigation/Restrictions.h Navigation/../MapEntities/Way.h Navigation/../MapEntities/Entity.h Navigation/../MapEntities/../GISNamedTypes.h Navigation/../MapEntities/../NamedType.h Navigation/../MapEntities/../Double.h Navigation/../MapEntities/../Geometry/Geometry.h Navigation/../MapEntities/../Geometry/../CoordinatesMath.h Navigation/../MapEntities/../Geometry/../Grid/GridMath.h Navigation/../MapEntities/../Geometry/../Grid/GridCoordinates.h Navigation/../MapEntities/../Geometry/../Grid/SpiralCoordinatesTraverser.h
	$(COMP) $(CPP_COMP_FLAG) -c  Navigation/Restrictions.cpp

RouteFinder.o: Navigation/RouteFinder.cpp Navigation/PriorityQueue.h Navigation/RouteFinder.h Navigation/Route.h Navigation/../GISNamedTypes.h Navigation/../NamedType.h Navigation/../Double.h Navigation/../MapEntities/Way.h Navigation/../MapEntities/Entity.h Navigation/../MapEntities/../Geometry/Geometry.h Navigation/../MapEntities/../Geometry/../CoordinatesMath.h Navigation/../MapEntities/../Geometry/../Grid/GridMath.h Navigation/../MapEntities/../Geometry/../Grid/GridCoordinates.h Navigation/../MapEntities/../Geometry/../Grid/SpiralCoordinatesTraverser.h Navigation/NavigationGIS.h Navigation/../GIS.h Navigation/../Grid/Grid.h Navigation/../Grid/GridCell.h Navigation/../Grid/GridEntity.h Navigation/../JsonHandler.h Navigation/../Logger.h Navigation/../BasicEntityInfo.h Navigation/../MapEntities/Junction.h Navigation/../MapEntities/POI.h Navigation/../BasicWayInfo.h Navigation/../Navigation/Restrictions.h Navigation/../GISExceptions.h Navigation/RouteSection.h
	$(COMP) $(CPP_COMP_FLAG) -c  Navigation/RouteFinder.cpp

tests.o: tests.cpp GISNamedTypes.h NamedType.h Double.h CoordinatesMath.h Grid/Grid.h Grid/GridCell.h Grid/GridEntity.h Grid/../MapEntities/Entity.h Grid/../MapEntities/../Geometry/Geometry.h Grid/../MapEntities/../Geometry/../Grid/GridMath.h Grid/../MapEntities/../Geometry/../Grid/GridCoordinates.h Grid/../MapEntities/../Geometry/../Grid/SpiralCoordinatesTraverser.h GIS.h JsonHandler.h Logger.h BasicEntityInfo.h MapEntities/Junction.h MapEntities/POI.h MapEntities/Way.h BasicWayInfo.h Navigation/Restrictions.h GISExceptions.h GISProvider.h tests/NavigationValidator.h tests/../Navigation/Route.h Navigation/Navigation.h Navigation/NavigationGIS.h Navigation/Routes.h Navigation/RouteSection.h
	$(COMP) $(CPP_COMP_FLAG) -c tests.cpp

clean:
	rm -f $(OBJS) $(EXEC)
