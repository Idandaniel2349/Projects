Design considerations:

General:
   # Used unordered maps where both a key and value are 
     needed, for fast searches.
   # Used enums where we had limited values for a field, to
     avoid unwanted values.

Entity:
   # Added geometry vector for easier calling of general functions 
     on every type of entity.
     Junction: Point geometry in index 0 of the Vector
     POI: Circle geometry in index 0 of the Vector
     Way: Geometries of lines making the Way
   # Added a type field for easier work to identity entity types.

Logger:
     # A Logger class to handle the Logger file and Logging 
       messages.

Grid: 
     # Saved the Grid as an unordered map with GridCoordinate 
        key representing the coordinate a GridCell is located
        at, and the GridCell as value. Used unordered map for
        fast searches of a GridCell in the Grid.
        The grid cell contains the geometries of entities 
        so distance queries are only done to the entity
        close geometries and not all of its geometries.

JsonHandler:
     # A class to handle loading and saving Json files.
*Note: when loading files, if a Way is before its junctions the 
            Way will be rejected.
       If first entity has id rejects all the entities without id
       in the file and if the first entity doesnt have id rejects
       all entities with id in the file.
       rejects out of bound coordinates.

Algorithms consideration:
#load duplicate ids:
in case of loading duplicates ids the load will ignore them.

#spiral algorithm:
The spiral algorithm starts from a specific Coordinate on 
the Grid and moves in a spiral ( up, right, down, left, up, right)
around the start coordinate until a specific limit is met.
Used for getCellsOfGeometry for Circle and getEntities by radius and get closestWay.


Efficiency considerations:
The grid is uses an unorderd_map to save the cells which contain entities so the searches on it are fast
and its sparse because it only hold cells which contain entities geometries.
because the grid works with entities geometries and not entities when an entity is found we can qurey the specific
geometry that was in the cell to save time instead of querying all its geometries.
getEntites only by name also uses an unordered_map between names and entities id to conduct fast searches.
getEntities with radius uses the spiral search to query mostly about relevant grid cells around the search
starting point, getWayClosestPoint uses the same method just without the restriction to look within a radius.

#A* algorithm:(calculations in RouteFinder files)
  Used the A* algorithm: to get shortest and fastest routes, 
  the algorithm runs once for fastest and once for Shortest and
  if there is more then one returns accroding to the rules in the assigments)
Shortest:Heuristic: distance from Junction to end Junction
                 Weight function: length of the way
Fastest: Heuristic:distance from Junction to end Junction/max speed limit
               Weight function:length of the way/ speed limit of the way





