# Grid-ex4

#General:
-Only accepts latitude between -90 and 90 and longtitude
between -180 and 180, else throws exceptions and terminates.
-cell have deafult move copy and assigment operators,
Grid blocks them.
-NumCols can't throw exception so instead it will print a message 
 and return -1 when latitude isn't in range;

#Cell:
- Used an unordered_map from type to vector of entities
+vector of entities for easier access and better time 
complexity in some fucntions.
-Have a constructor that sets the location of the Cell as Coordinates.
-Using the deafult copy,move and assigment operators since
I need them to be able to init rows;
-No need for destructor since we use smart pointers.

#Row: A new private inner class representing a row on the grid:
- Has members: vector of cells in the row and rowWidth.

#iterator: A public iterator for grid: 
-Uses vector and row iterators to run on all grid cells.

#Grid:
-Represented by a vector of Rows.
-block copy,move and assigment operators. Each time a new
Grid need to be made it will be made with the regular 
constructor(which makes an empty grid with all the cells and rows needed).
That way we will have to specifically write each insertion to each grid and not copy.
-No need for destructor since we use smart pointers.
 
