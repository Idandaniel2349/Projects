COMP = g++-10

SOURCE_FILES := $(notdir $(shell find . -name '*.cpp'))
OBJS := $(SOURCE_FILES:%.cpp=%.o)
BIN_DIR := bin
EXEC := simulator_316497817
OUTPUT_DIR := ../../OUTPUT
OUTPUT_EXEC := $(OUTPUT_DIR)/$(EXEC) 

CPP_COMP_FLAG = -std=c++20 -Werror -Wall -Wextra -pedantic-errors -DNDEBUG
CPP_LINK_FLAG = -rdynamic -pthread -ldl

all: $(BIN_DIR) $(OUTPUT_DIR) $(OUTPUT_EXEC)

$(BIN_DIR): 
	mkdir -p $(BIN_DIR)

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

$(OUTPUT_EXEC): $(addprefix $(BIN_DIR)/, $(OBJS))
	$(COMP) $^ $(CPP_LINK_FLAG) -o $@ 


# Make sure to have all your dependencies for each .o file creation rule

$(BIN_DIR)/main.o: main.cpp ../Common/Restrictions.h SOLoader.h \
            ../Common/AbstractGIS.h ../Common/GISNamedTypes.h ../Common/NamedType.h \
            ../Common/Double.h ../Common/AbstractWay.h \
            ../Common/AbstractNavigation.h ../Common/NavigationGIS.h \
            ../Common/AbstractRoutes.h ../Common/AbstractRoute.h CachedGIS.h \
            SafeQueue.h NavigationRunner.h NavigationResult.h ConsensusCalculator.h \
            RouteSummary.h RouteResult.h ArgParser.h ScoreCalculator.h CSVIO.h SimulatorExceptions.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

$(BIN_DIR)/GISRegistration.o: GISRegistration.cpp ../Common/*.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

$(BIN_DIR)/NavigationRegistration.o: NavigationRegistration.cpp ../Common/*.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

$(BIN_DIR)/NavigationValidator.o: NavigationValidator.cpp ../Common/*.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

$(BIN_DIR)/SOLoader.o: SOLoader.cpp SOLoader.h ../Common/AbstractGIS.h \
 ../Common/GISNamedTypes.h ../Common/NamedType.h ../Common/Double.h \
 ../Common/Restrictions.h ../Common/AbstractWay.h \
 ../Common/AbstractNavigation.h ../Common/NavigationGIS.h \
 ../Common/AbstractRoutes.h ../Common/AbstractRoute.h CachedGIS.h SimulatorExceptions.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

$(BIN_DIR)/ScoreCalculator.o: ScoreCalculator.cpp ScoreCalculator.h RouteResult.h \
 ../Common/GISNamedTypes.h ../Common/NamedType.h ../Common/Double.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

$(BIN_DIR)/NavigationRunner.o: NavigationRunner.cpp NavigationRunner.h \
            ../Common/AbstractGIS.h ../Common/GISNamedTypes.h ../Common/NamedType.h \
            ../Common/Double.h ../Common/Restrictions.h ../Common/AbstractWay.h \
            ../Common/AbstractNavigation.h ../Common/NavigationGIS.h \
            ../Common/AbstractRoutes.h ../Common/AbstractRoute.h NavigationResult.h \
            ConsensusCalculator.h RouteSummary.h RouteResult.h SOLoader.h \
            CachedGIS.h NavigationValidator.h ../Common/CoordinatesMath.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

$(BIN_DIR)/CSVIO.o: CSVIO.cpp CSVIO.h RouteResult.h ../Common/GISNamedTypes.h \
 ../Common/NamedType.h ../Common/Double.h SimulatorExceptions.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

$(BIN_DIR)/ConsensusCalculator.o: ConsensusCalculator.cpp ConsensusCalculator.h \
            ../Common/GISNamedTypes.h ../Common/NamedType.h ../Common/Double.h \
            NavigationResult.h ../Common/AbstractRoutes.h ../Common/AbstractRoute.h \
            RouteSummary.h RouteResult.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

$(BIN_DIR)/ArgParser.o: ArgParser.cpp ArgParser.h SimulatorExceptions.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

clean:
	rm -rf $(BIN_DIR) $(OUTPUT_EXEC)
