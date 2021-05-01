COMP = g++-10

SOURCE_FILES := $(notdir $(shell find . -name '*.cpp'))
OBJS_FILES := $(SOURCE_FILES:%.cpp=%.o)
BIN_DIR = bin
OBJS := $(addprefix $(BIN_DIR)/, $(OBJS_FILES))
SHARED_OBJ = Navigation.so
OUTPUT_DIR := ../../OUTPUT/Navigation
OUTPUT_SHARED_OBJ := $(OUTPUT_DIR)/$(SHARED_OBJ)

CPP_COMP_FLAG = -std=c++20 -Werror -Wall -Wextra -pedantic-errors -DNDEBUG -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
CPP_LINK_FLAG = -shared 

all: $(BIN_DIR) $(OUTPUT_DIR) $(OUTPUT_SHARED_OBJ)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

$(OUTPUT_SHARED_OBJ): $(OBJS)
	$(COMP) $(CPP_LINK_FLAG) -o $@ $^

# Make sure to have all your dependencies for each .o file creation rule

$(BIN_DIR)/Navigation.o: Navigation.cpp Navigation7.h \
 ../Common/NavigationGIS.h ../Common/AbstractGIS.h \
 ../Common/GISNamedTypes.h ../Common/NamedType.h ../Common/Double.h \
 ../Common/Restrictions.h ../Common/AbstractWay.h Routes.h Route.h \
 ../Common/AbstractRoute.h ../Common/AbstractRoutes.h RouteSection.h \
 ../Common/AbstractNavigation.h RouteFinder.h ../Common/CoordinatesMath.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

$(BIN_DIR)/RouteFinder.o: RouteFinder.cpp PriorityQueue.h RouteFinder.h Route.h \
 ../Common/GISNamedTypes.h ../Common/NamedType.h ../Common/Double.h \
 ../Common/AbstractRoute.h ../Common/NavigationGIS.h \
 ../Common/AbstractGIS.h ../Common/Restrictions.h ../Common/AbstractWay.h \
 RouteSection.h ../Common/CoordinatesMath.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

clean:
	rm -rf $(BIN_DIR) $(OUTPUT_SHARED_OBJ)
