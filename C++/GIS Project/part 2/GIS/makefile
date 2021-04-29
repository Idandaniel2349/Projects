COMP = g++-10

SOURCE_FILES := $(notdir $(shell find . -name '*.cpp'))
OBJS_FILES := $(SOURCE_FILES:%.cpp=%.o)
BIN_DIR := bin
OBJS := $(addprefix $(BIN_DIR)/, $(OBJS_FILES))
SHARED_OBJ := GIS_316497817.so
OUTPUT_DIR := ../../OUTPUT/GIS
OUTPUT_SHARED_OBJ := $(OUTPUT_DIR)/$(SHARED_OBJ)

CPP_COMP_FLAG := -std=c++20 -Werror -Wall -Wextra -pedantic-errors -DNDEBUG -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
CPP_LINK_FLAG := -shared 

all: $(BIN_DIR) $(OUTPUT_DIR) $(OUTPUT_SHARED_OBJ)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

$(OUTPUT_SHARED_OBJ): $(OBJS)
	$(COMP) $(CPP_LINK_FLAG) -o $@ $^ #$(addprefix $(BIN_DIR)/, $^)

# Make sure to have all your dependencies for each .o file creation rule

$(BIN_DIR)/GIS_316497817.o: GIS_316497817.cpp GIS_316497817.h \
            ../Common/GISNamedTypes.h ../Common/NamedType.h ../Common/Double.h \
            Grid/Grid.h Grid/GridCell.h Grid/GridEntity.h \
            Grid/../MapEntities/Entity.h Grid/../MapEntities/../Geometry/Geometry.h \
            Grid/../MapEntities/../Geometry/../../Common/CoordinatesMath.h \
            Grid/../MapEntities/../Geometry/../Grid/GridMath.h \
            Grid/../MapEntities/../Geometry/../Grid/GridCoordinates.h \
            Grid/../MapEntities/../Geometry/../Grid/SpiralCoordinatesTraverser.h \
            JsonHandler.h Logger.h BasicEntityInfo.h MapEntities/Junction.h \
            MapEntities/POI.h Way.h ../Common/AbstractWay.h BasicWayInfo.h \
            ../Common/Restrictions.h GISExceptions.h ../Common/AbstractGIS.h \
            ../Common/GISRegistration.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

$(BIN_DIR)/JsonHandler.o: JsonHandler.cpp JsonHandler.h MapEntities/Entity.h \
 MapEntities/../../Common/GISNamedTypes.h \
 MapEntities/../../Common/NamedType.h MapEntities/../../Common/Double.h \
 MapEntities/../Geometry/Geometry.h \
 MapEntities/../Geometry/../../Common/CoordinatesMath.h \
 MapEntities/../Geometry/../Grid/GridMath.h \
 MapEntities/../Geometry/../Grid/GridCoordinates.h \
 MapEntities/../Geometry/../Grid/SpiralCoordinatesTraverser.h Logger.h \
 BasicEntityInfo.h MapEntities/Junction.h MapEntities/POI.h Way.h \
 ../Common/AbstractWay.h Grid/Grid.h Grid/GridCell.h Grid/GridEntity.h \
 BasicWayInfo.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

$(BIN_DIR)/Logger.o: Logger.cpp Logger.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

$(BIN_DIR)/Way.o: Way.cpp Way.h MapEntities/Entity.h \
            MapEntities/../../Common/GISNamedTypes.h \
            MapEntities/../../Common/NamedType.h MapEntities/../../Common/Double.h \
            MapEntities/../Geometry/Geometry.h \
            MapEntities/../Geometry/../../Common/CoordinatesMath.h \
            MapEntities/../Geometry/../Grid/GridMath.h \
            MapEntities/../Geometry/../Grid/GridCoordinates.h \
            MapEntities/../Geometry/../Grid/SpiralCoordinatesTraverser.h \
            ../Common/AbstractWay.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

$(BIN_DIR)/Circle.o: Geometry/Circle.cpp Geometry/Geometry.h \
 Geometry/../../Common/GISNamedTypes.h Geometry/../../Common/NamedType.h \
 Geometry/../../Common/Double.h Geometry/../../Common/CoordinatesMath.h \
 Geometry/../Grid/GridMath.h Geometry/../Grid/GridCoordinates.h \
 Geometry/../Grid/SpiralCoordinatesTraverser.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

$(BIN_DIR)/Line.o: Geometry/Line.cpp Geometry/Geometry.h \
 Geometry/../../Common/GISNamedTypes.h Geometry/../../Common/NamedType.h \
 Geometry/../../Common/Double.h Geometry/../../Common/CoordinatesMath.h \
 Geometry/../Grid/GridMath.h Geometry/../Grid/GridCoordinates.h \
 Geometry/../Grid/SpiralCoordinatesTraverser.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

$(BIN_DIR)/Point.o: Geometry/Point.cpp Geometry/Geometry.h \
 Geometry/../../Common/GISNamedTypes.h Geometry/../../Common/NamedType.h \
 Geometry/../../Common/Double.h Geometry/../../Common/CoordinatesMath.h \
 Geometry/../Grid/GridMath.h Geometry/../Grid/GridCoordinates.h \
 Geometry/../Grid/SpiralCoordinatesTraverser.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

$(BIN_DIR)/Grid.o: Grid/Grid.cpp Grid/Grid.h Grid/../../Common/GISNamedTypes.h \
 Grid/../../Common/NamedType.h Grid/../../Common/Double.h Grid/GridCell.h \
 Grid/GridEntity.h Grid/../MapEntities/Entity.h \
 Grid/../MapEntities/../Geometry/Geometry.h \
 Grid/../MapEntities/../Geometry/../../Common/CoordinatesMath.h \
 Grid/../MapEntities/../Geometry/../Grid/GridMath.h \
 Grid/../MapEntities/../Geometry/../Grid/GridCoordinates.h \
 Grid/../MapEntities/../Geometry/../Grid/SpiralCoordinatesTraverser.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

$(BIN_DIR)/GridCell.o: Grid/GridCell.cpp Grid/GridCell.h Grid/GridEntity.h \
 Grid/../MapEntities/Entity.h \
 Grid/../MapEntities/../../Common/GISNamedTypes.h \
 Grid/../MapEntities/../../Common/NamedType.h \
 Grid/../MapEntities/../../Common/Double.h \
 Grid/../MapEntities/../Geometry/Geometry.h \
 Grid/../MapEntities/../Geometry/../../Common/CoordinatesMath.h \
 Grid/../MapEntities/../Geometry/../Grid/GridMath.h \
 Grid/../MapEntities/../Geometry/../Grid/GridCoordinates.h \
 Grid/../MapEntities/../Geometry/../Grid/SpiralCoordinatesTraverser.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

$(BIN_DIR)/GridCoordinates.o: Grid/GridCoordinates.cpp Grid/GridCoordinates.h \
 Grid/../../Common/GISNamedTypes.h Grid/../../Common/NamedType.h \
 Grid/../../Common/Double.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

$(BIN_DIR)/GridEntity.o: Grid/GridEntity.cpp Grid/GridEntity.h \
 Grid/../MapEntities/Entity.h \
 Grid/../MapEntities/../../Common/GISNamedTypes.h \
 Grid/../MapEntities/../../Common/NamedType.h \
 Grid/../MapEntities/../../Common/Double.h \
 Grid/../MapEntities/../Geometry/Geometry.h \
 Grid/../MapEntities/../Geometry/../../Common/CoordinatesMath.h \
 Grid/../MapEntities/../Geometry/../Grid/GridMath.h \
 Grid/../MapEntities/../Geometry/../Grid/GridCoordinates.h \
 Grid/../MapEntities/../Geometry/../Grid/SpiralCoordinatesTraverser.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

$(BIN_DIR)/GridMath.o: Grid/GridMath.cpp Grid/GridMath.h \
 Grid/../../Common/CoordinatesMath.h Grid/../../Common/GISNamedTypes.h \
 Grid/../../Common/NamedType.h Grid/../../Common/Double.h \
 Grid/../Geometry/Geometry.h \
 Grid/../Geometry/../Grid/SpiralCoordinatesTraverser.h \
 Grid/../Geometry/../Grid/GridCoordinates.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

$(BIN_DIR)/SpiralCoordinatesTraverser.o: Grid/SpiralCoordinatesTraverser.cpp \
 Grid/SpiralCoordinatesTraverser.h Grid/../../Common/GISNamedTypes.h \
 Grid/../../Common/NamedType.h Grid/../../Common/Double.h \
 Grid/GridCoordinates.h Grid/GridMath.h \
 Grid/../../Common/CoordinatesMath.h Grid/../Geometry/Geometry.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

$(BIN_DIR)/Entity.o: MapEntities/Entity.cpp MapEntities/Entity.h \
 MapEntities/../../Common/GISNamedTypes.h \
 MapEntities/../../Common/NamedType.h MapEntities/../../Common/Double.h \
 MapEntities/../Geometry/Geometry.h \
 MapEntities/../Geometry/../../Common/CoordinatesMath.h \
 MapEntities/../Geometry/../Grid/GridMath.h \
 MapEntities/../Geometry/../Grid/GridCoordinates.h \
 MapEntities/../Geometry/../Grid/SpiralCoordinatesTraverser.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

$(BIN_DIR)/POI.o: MapEntities/POI.cpp MapEntities/POI.h \
 MapEntities/../../Common/GISNamedTypes.h \
 MapEntities/../../Common/NamedType.h MapEntities/../../Common/Double.h \
 MapEntities/../Geometry/Geometry.h \
 MapEntities/../Geometry/../../Common/CoordinatesMath.h \
 MapEntities/../Geometry/../Grid/GridMath.h \
 MapEntities/../Geometry/../Grid/GridCoordinates.h \
 MapEntities/../Geometry/../Grid/SpiralCoordinatesTraverser.h \
 MapEntities/Entity.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

clean:
	rm -rf $(BIN_DIR) $(OUTPUT_SHARED_OBJ)
