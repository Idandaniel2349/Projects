GIS_DIR := GIS
NAVIGATION_DIR := Navigation
SIMULATOR_DIR := Simulator

MAKE_ALL := MAKE_CMD -C $(GIS_DIR) && MAKE_CMD -C $(NAVIGATION_DIR) && MAKE_CMD -C $(SIMULATOR_DIR)   

all: 
	$(subst MAKE_CMD, make, $(MAKE_ALL))

clean:
	$(subst MAKE_CMD, make clean, $(MAKE_ALL))
