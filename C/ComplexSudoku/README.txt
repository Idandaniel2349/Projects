Complex sudoku project 

In this project we support sudoku boards of diffrent sizes.
Here we use the ILP solver algorithm to solve the board and
Exhaustive backtracking solver for determining the number of different solutions of a board.
We use the ILP(integar linear programming) with Gurobi and as such
to Run the project you will have to install Gurobi on your machine. 

The complex Sudoku project have have the following functionalities:
- Solving Sudoku puzzles
- Editing existing and creating new Sudoku puzzles
- Supporting puzzles of any size
- Validating the board
- Saving and loading puzzles from files
- Randomly generating new puzzles
- Finding the number of solutions for a given board
- Auto-filling option for obvious cells (where only a single value is valid)

The program will have three main modes: Init, Edit and Solve. In the Solve mode, the user
attempts to solve a pre-existing puzzle. 
In the Edit mode, the user edits existing puzzles or
creates new ones, and saves these for opening in the Solve mode later. In the Init mode, the
user loads a file to enter either Edit or Solve mode. 
The program starts in Init mode.

The project consists of 5 main parts:
- Sudoku game logic(CommandsImplementation files)
- Console user interface(main file,Parser files)
- ILP solver algorithm(Gurobi files + gurobi_c for gurobi functions)
- Random puzzle generator(in CommandsImplementation files)
- Exhaustive backtracking solver (for determining the number of different solutions)
(exhaustiveBacktracking files)

*MainAux and MainAuxDefinitions for helper functions and definitions.
*DoublyLinkedList,Errors,StackImplementation as their name says.

The grid consists n x m blocks of size m x n each, for a total of N=mn rows and columns. For
example, a 10x10 grid can consist of 5x2 blocks, each of size 2x5 cells.

The program will keep a "mark errors" parameter with a value of either 1 (True) or 0 (False).
This parameter determines whether errors in the Sudoku puzzle are displayed.
When editing or solving a puzzle, the user is free to enter any input, even erroneous one
(according to the rules of Sudoku). An erroneous board is a board with cells which contain
illegal values, i.e., the same value for two or more neighbors.
When this parameter is set to 1, erroneous values are displayed followed by an asterisk. All
cells that participate in an error should be marked. The default value is 1.
In Edit mode we always mark errors, regardless of the parameter's value.

Throughout a single game (or puzzle) the program maintains a doubly-linked list of the user's
moves, and a pointer marking the current move.
Whenever the user makes a move (via "set,", "autofill", "generate", or "guess"), the redo
part of the list is cleared, i.e., all items beyond the current pointer are removed, the new
move is added to the end of the list and marked as the current move, i.e., the pointer is
updated to point to it.
The user may freely traverse this list (move the pointer) using the "undo" and "redo"
commands, which also update the board according to the move to undo or redo.

For Instruction on commands see Instructions file.

To Run first run the makefile and then the file created.
Remember: Gurobi is a must to run the project
Note: The project allocates and free all memory.

Enjoy