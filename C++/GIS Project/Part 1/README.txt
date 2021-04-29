This part has 2 main parts: The GIS and Navigation:

The GIS:

- To see template of JSON file go to tests folder text file.
- To see the entites types classes go to "MapEntities","Geometry" folder.
- JsonHandler class is responsible for loading and saving files.
- As we load entites we put them on the Grid, See Grid folder for details.
- The main functions the project supports are in the GIS class
See GIS.h for detailed documentation of the functions.
This is our "Master" class which all the info we load and make is 
saved into, and the functions in it use the rest of our classes of fucntions
- See further documentation in the code and in the "Extra- info" file
  for more information on specific classes or functions.

- The functions The GIS supports are: load info from JSON file, 
save info to JSON file, get entities by name, get entities in a circle around a point,
get closest point on a way that is closest to the given Coordinates, 
getting ways starting(ending) at a certain Junction(and also ways under certain restrictions),
return a way by its Id.

The Navigation:

The Navigation part of the project gets a GIS and is responsible for 
Navigation on the Map

- The Navigation main function is getRoutes(in Navigation class), 
the function gets start and end coordinates and need to find the
shortest and fastest routes between them. See code for more
documentation, and tests for use.

More:
- Extra information can be found in the "Extra- info" file and the code itself,
the best way to look at the code is through the two "main" classes GIS and 
Navigation and work through there to the classes it use.
- Errors will be logged to errors.log file (only made if there is errors).
- Only JSON files with the correct template can be loaded.
- To run the tests you must have google tests installed, or put in the right place( depends on where the code is run)
- Uses spiral algorithem on the grid and A* algorhitem to find shortest, fastest path.

To Run: run the makefile then run the file created and it will run the
googletests.

See part 2 folder for additions





