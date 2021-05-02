/*
 * Errors.c
 * This module is responsible to hold errors commands call throughout the game.
 * Here we define Errors such as invalid values, command, mode. parameters errors for parser, file errors and more.
 * Those Errors are then used where needed for shorter and clearer code and to keep the same errors working the same way.
 */

#include "MainAuxDefinitions.h"
#include "stdio.h"
#include "stdlib.h"
#include "CommandsImplementation.h"

void mallocError(GameStatus* status){
	printf("Fatal Error: failed to allocate needed memory, exiting the program\n");
	*status=exitFunc();

}
void invalidSave(char *s){
	printf("Error: can't save an %s board in edit mode\n",s);
}

void invalidCellValue(int blockSize){
	printf("Error: Cell values should be numbers between 0 to %d, or numbers between 1 to %d with . afterwards\n",blockSize,blockSize);
}

void valNum(int blockSize){
	printf("Error: numbers of cell values for board incorrect, number of values should be %d\n",blockSize*blockSize);
}

void notNatural(char*s){
	printf("Error: %s is not a natural positive number, height and width must be natural positive numbers\n",s);
}


void fileTooShort(char * x){
	printf("Error: not enough values at file %s\n",x);
}

void fileReadFailure(char * x){
	printf("Error: failed to read from file %s\n",x);
}

void fileOpenFailure(char * x){
	printf("Error: unable to open file %s\n",x);
}

void InvalidCommand(){
	printf("Error: Invalid command\n");
}


void InvalidFilePath(){
	printf("Error: Invalid file path\n");
}

void InvalidFileFormat(){
	printf("Error: Invalid file format\n");
}

void undoLimit(){
	printf("Error: Reached undo limit, no further undo available\n");
}

void redoLimit(){
	printf("Error: Reached redo limit, no further redo available\n");
}


GameStatus InvalidCommandName(){
	printf("Error: Invalid command name\n");
	return INVALID_S;
}

/*prints error message if the command is not valid in the current mode
 * and informs the user in which modes the command is available*/
GameStatus InvalidCommandMode(int commType){
	int i;
	printf("Error: Invalid command for the current mode.\nThis command available in the following modes:\n");
	for(i = 1; i <=CommandsPropertiesArrr[commType].modeAvailability[0]; i++){
		if((int)CommandsPropertiesArrr[commType].modeAvailability[i] == 1){
			printf("init\n");
		}
		else if((int)CommandsPropertiesArrr[commType].modeAvailability[i] == 2){
			printf("edit\n");
		}
		else if((int)CommandsPropertiesArrr[commType].modeAvailability[i] == 3){
			printf("solve\n");
		}
	}
	printf("\n");
	return INVALID_S;
}


GameStatus InvalidInputLength(){
	printf("Error: the input length exceeds the limitation. Please enter a command with at most 256 chars\n");
	return INVALID_S;
}


GameStatus TooMuchParameters(int commType){
	printf("Error: the number of parameters given exceeds the limitation.\n"
			"For this command please enter %d parameters\n",CommandsPropertiesArrr[commType].numOfParameters);
	return INVALID_S;
}

GameStatus InvalidNumberOfParameters(int commType,int NumOfParameters){
	if(NumOfParameters > CommandsPropertiesArrr[commType].numOfParameters){
		printf("Error: too much parameters provided.\n"
				"For this command please enter %d parameters\n",CommandsPropertiesArrr[commType].numOfParameters);

	}
	else{
		printf("Error: not enough parameters provided.\n"
				"For this command please enter %d parameters\n",CommandsPropertiesArrr[commType].numOfParameters);
	}
	return INVALID_S;
}


void setInFixedCellError(){
	printf("Error: Can't set a value to a fixed cell\n");
	return;
}

void erroneousBoardInValidate(){
	printf("Error: the board is erroneous and therefore not solvable.\n");
}

void erroneousBoardNumSolutions(){
	printf("Error: the board is erroneous and therefore num_solutions command is not applicable.\n");
}














