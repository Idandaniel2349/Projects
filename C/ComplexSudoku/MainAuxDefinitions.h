/*
 * MainAuxDefinitions.h
 * This module contains some enums and structured being used in the program.
 * Mode enum - defines the different modes of the game.
 * Command enum - defined all possible commands the user can insert.
 * GameStatus enum - defines different statuses for the program. it is being used by main to
 * decide rather execute the command or not and continue with the program flow and receive input
 * from user (in PLAY and INVALID statuses), or to start over from init mode
 * (in SOLVED status, which means the board was solved successfully) or to exit the game (EXIT status).
 * ParameterType enum - defines the types of parameters possibles by some of the commands.
 * CommandProperties structure - defines the structure of each element in the commands properties array,
 * each element contains the properties of a certain command (in which modes is valid and what the
 * number and type parameters it receives).
 */

#ifndef STATUSENUM_H_
#define STATUSENUM_H_

#define NUMBER_OF_COMMANDS 17

typedef enum{
    INIT_M = 1,EDIT_M = 2, SOLVE_M = 3
}Mode;

typedef enum {
    INVALID = 0,SOLVE = 1,EDIT = 2 ,MARK_ERRORS = 3, PRINT_BOARD = 4, SET = 5, VALIDATE = 6, GUESS = 7, GENERATE = 8, UNDO = 9,
    REDO = 10, SAVE = 11, HINT = 12, GUESS_HINT = 13, NUM_SOLUTIONS = 14, AUTOFILL = 15, RESET = 16, EXIT = 17
}Command;


typedef enum{
	SOLVED_S = 0,
	PLAY_S = 1,
	INVALID_S = 2,
	EXIT_S = -1
}GameStatus;

typedef enum{
    STRING_T = 1, INT_T = 2, FLOAT_T = 3, NONE_T = 4
}ParameterType;

/* define a structure for the commands and their properties */
typedef struct{
	char* commandName;
	const Mode* modeAvailability;
	int numOfParameters;
	ParameterType parameterType;
}CommandProperties;

CommandProperties CommandsPropertiesArrr[NUMBER_OF_COMMANDS];
/*creates mode combinations arrays, first element in the array declares the number of modes in it*/
static const Mode CommandProperties_mode_all[4] = {3, INIT_M, EDIT_M,SOLVE_M};
static const Mode CommandProperties_mode_solve[2] = {1, SOLVE_M};
static const Mode CommandProperties_mode_edit[2] ={1, EDIT_M};
static const Mode CommandProperties_mode_editSolve[3] = {2, EDIT_M, SOLVE_M};


void insertCommandProperties();
void createCommandsArray(Command command ,char* commName, const Mode* mode, int numOfParams, ParameterType paramType);

#endif /* STATUSENUM_H_ */


