/*
 * MainAuxDefinition.c
 * This module creates an array contains all possible commands from the user, and their properties
 * to define for each command in which modes is valid and what the number and type of
 * parameters it should receive.
 * This properties array is being used by the parser for validation of the input for
 * each of the commands properties
 */

#include "MainAuxDefinitions.h"
#include <stdlib.h>

/* create the commands properties array use for user inputs validations*/
/*void initCommandProperties(){
//	insertCommandProperties();
//}
*/

/* Receives the properties of a command and insert to the commands properties array */
void createCommandsArray(Command command ,char* commName, const Mode* mode, int numOfParams, ParameterType paramType){
	CommandsPropertiesArrr[command].commandName = commName;
	CommandsPropertiesArrr[command].modeAvailability = mode;
	CommandsPropertiesArrr[command].numOfParameters = numOfParams;
	CommandsPropertiesArrr[command].parameterType = paramType;
}


/* insert each command's properties to the commands properties array */
void insertCommandProperties(){
	createCommandsArray(SOLVE,"solve", CommandProperties_mode_all ,1,STRING_T);
	createCommandsArray(EDIT,"edit",CommandProperties_mode_all, 1,STRING_T);
	createCommandsArray(MARK_ERRORS,"mark_errors", CommandProperties_mode_solve , 1, INT_T);
	createCommandsArray(PRINT_BOARD,"print_board",CommandProperties_mode_editSolve, 0,NONE_T);
	createCommandsArray(SET,"set",CommandProperties_mode_editSolve, 3, INT_T );
	createCommandsArray(VALIDATE,"validate",CommandProperties_mode_editSolve, 0, NONE_T);
	createCommandsArray(GUESS, "guess",CommandProperties_mode_solve, 1, FLOAT_T);
	createCommandsArray(GENERATE,"generate", CommandProperties_mode_edit, 2, INT_T);
	createCommandsArray(UNDO,"undo", CommandProperties_mode_editSolve, 0, NONE_T);
	createCommandsArray(REDO,"redo", CommandProperties_mode_editSolve, 0, NONE_T);
	createCommandsArray(SAVE,"save", CommandProperties_mode_editSolve, 1, STRING_T);
	createCommandsArray(HINT,"hint", CommandProperties_mode_solve, 2, INT_T);
	createCommandsArray(GUESS_HINT, "guess_hint", CommandProperties_mode_solve, 2, INT_T);
	createCommandsArray(NUM_SOLUTIONS, "num_solutions", CommandProperties_mode_editSolve, 0,NONE_T);
	createCommandsArray(AUTOFILL, "autofill", CommandProperties_mode_solve, 0, NONE_T);
	createCommandsArray(RESET, "reset", CommandProperties_mode_editSolve, 0, NONE_T);
	createCommandsArray(EXIT, "exit", CommandProperties_mode_all, 0, NONE_T);
}





