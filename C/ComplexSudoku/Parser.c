/*
 * Parser.c
 * This module handles the user input. it receive a line of input, break it into the command name and
 * parameters. then, the command properties are being validate in the needed order (command name,
 * mode availability, parameters number, type and range).
 * At any point, if one of the parameters of the input is invalid, a relevant error message that
 * instruct the user of the required command properties is presented. then it discard the
 * invalid input and waits for a new input.
 * If all the input command properties are valid, the relevant command function is being called.
 * For each property validation there is a function in this file.
 *
 */
#include "MainAuxDefinitions.h"
#include "MainAux.h"
#include "Errors.h"
#include "stdio.h"
#include <string.h>
#include "stdlib.h"
#include "CommandsImplementation.h"
#define MAX_INPUT_CHARS 256
#define MAX_PARAMETERS_NUM 3

/*get line of input from the user,  insert into the input array,
validates its not more than 256 chars and updates the game status accordingly*/
GameStatus getInput(char* inputArr, int* cnt){
	char ch = 0;
	while(1){
		ch = getchar();
		if (ch == EOF){
			fflush(stdin);
			return EXIT_S;
		}
		else if(ch == '\n'){
			fflush(stdin);
			return PLAY_S;
		}
		if (*cnt == MAX_INPUT_CHARS){
			fflush(stdin);
			return INVALID_S;
		}
		inputArr[*cnt] = (char)ch;
		(*cnt)++;

	}
    fflush(stdin);
    inputArr[*cnt]='\0';
    return PLAY_S;
}

/* validates the name of the input command is valid (such command exists) */
int validateName(char* inputArr){
	char* commName;
	char* temp;
	int i;

	commName = strtok(inputArr," \t\n\r\f\v");
	if(commName == (void*)0){ /*equals Null, which means no command was given*/
		return (int)INVALID;
	}

	for(i = 1; i <= NUMBER_OF_COMMANDS; i++){
		temp = CommandsPropertiesArrr[i].commandName;
		if(strcmp(commName,temp) == 0){ /*equals 0 if strings are equals*/
			return i;
		}
	}
	return (int)INVALID;
}

/* validates that the input command is valid for the current game mode. */
GameStatus validateMode(Mode mode, int commType){
	int i;
	for(i=1; i< (int)(CommandsPropertiesArrr[commType].modeAvailability[0]+1); i++){
		if(mode == CommandsPropertiesArrr[commType].modeAvailability[i]){
			return PLAY_S;
		}
	}
	return INVALID_S;
}

/* extract the parameters in the user's input snd inset into parameters array */
int extractParameters(char* inputArr, char* parametersArr[]) {
    int cnt;
    char* parameter;
    cnt=0;
    parameter=NULL;

    while (cnt < MAX_PARAMETERS_NUM +1) {
    	parameter = strtok(NULL, " \t\r\n\v\f");
        if (parameter == NULL) {
            break;
        }
        parametersArr[cnt] = parameter;
        cnt++;

        if (inputArr == '\0') {
            break;
        }
    }
    return cnt;
}

/* validates that the number of parameters given by the user is valid for the command */
GameStatus validateNumOfParameters(int commType, int numOfParameters){
	if(commType == EDIT){
		if(numOfParameters <= CommandsPropertiesArrr[commType].numOfParameters){ /*the edit command parameter is optional*/
			return PLAY_S;
		}
		else{
			return INVALID_S;
		}
	}
	else{
		if(numOfParameters != CommandsPropertiesArrr[commType].numOfParameters){/*other commands must get the exact number of parameters*/
			return INVALID_S;
		}
	}
	return PLAY_S;
}

/* check the type of parameters given by the user */
ParameterType checkParameterType(char* parameter){
	int i;
	char c;
	int n;

	if ((sscanf(parameter, "%d%c", &i, &c) == 1) && (sscanf(parameter, "%d", &i) == 1)) {/*the only type is integer*/
		return INT_T;
	}
	else if (sscanf(parameter, "%d%c%d", &i, &c, &n) == 3 && (c == '.')) {/*the format is of Float parameter*/
		return FLOAT_T;
	}
	return STRING_T;
}

/* validates that the type of the parameters provided by the user is valid for the command.
 * if not, informs the user of the right parameters type */
GameStatus validateParametersType(int commType, int numOfParameters, char* parametersArr[]){
	int i;
	if((commType == SOLVE)||(commType == SAVE) || (commType == EDIT)){ /*commands required one string parameters.
	(already handled edit command without a path) */
		if(checkParameterType(parametersArr[0]) != STRING_T){
			printf("Error: the type of parameter provided is invalid for this command.\n"
					"This command requires a string parameter (file path)\n");
			return INVALID_S;
		}
		else{
			return PLAY_S;
		}
	}
	else if((commType == GUESS)){
		if(checkParameterType(parametersArr[0]) == FLOAT_T){
			return PLAY_S;
		}
		else if(checkParameterType(parametersArr[0]) == INT_T){
				if((atoi(parametersArr[0]) == 0)||(atoi(parametersArr[0]) == 1)){
					return PLAY_S;
				}
		}
		else{
			printf("Error: the type of parameter provided is invalid for this command.\n"
					"This command requires a Float parameter\n");
			return INVALID_S;
		}

	}
	else{ /* all other command are required parameters of type int */
		for(i = 0; i < numOfParameters; i++){
			if(checkParameterType(parametersArr[i]) != INT_T){
				printf("Error: the type of parameters provided is invalid for this command.\n"
						"This command requires Int parameters\n");
				return INVALID_S;
			}
		}
	}
	return PLAY_S;
}

/* validates that the range of the parameters provided by the user is valid for the command.
 * if not, informs the user of the right parameters range */
GameStatus validateParametersRange(int commType, int numOfParameters, char* parametersArr[], int boardSize){
	if((commType == SOLVE)||(commType == SAVE) || (commType == EDIT)){ /*commands required a string parameter */
		return PLAY_S;
	}
	else if((commType == GUESS)){
		if(checkParameterType(parametersArr[0]) == FLOAT_T){
			if(atof(parametersArr[0]) < 0 || atof(parametersArr[0]) > 1){
				printf("Error: the range of parameter provided is invalid for this command.\n"
						"This command requires a Float in the range 0 to 1 (including)\n");
				return INVALID_S;
			}
		}
		else if(checkParameterType(parametersArr[0]) == INT_T){
			if(atoi(parametersArr[0]) != 0 && atoi(parametersArr[0]) != 1){
				printf("Error: the range of parameter provided is invalid for this command.\n"
						"This command requires a Float in the range 0 to 1 (including)\n");
				return INVALID_S;
			}
		}
	}
	else if((commType == MARK_ERRORS)){
		if((atoi(parametersArr[0]) != 0) && (atoi(parametersArr[0]) != 1)){
			printf("Error: this command only receives 0 or 1 as a parameter.\n");
			return INVALID_S;
		}
	}
	else if((commType == SET)){
		if((atoi(parametersArr[0]) < 1 ) || (atoi(parametersArr[0]) > boardSize)) {
			printf("Error: the range of the first parameter is invalid.\nIt's value should be in the range 1 - %d\n",boardSize);
			return INVALID_S;
		}
		else if((atoi(parametersArr[1]) < 1 ) || (atoi(parametersArr[1]) > boardSize)) {
			printf("Error: the range of the second parameter is invalid.\nIt's value should be in the range 1 - %d\n",boardSize);
			return INVALID_S;
		}
		else if((atoi(parametersArr[2]) < 0 ) || (atoi(parametersArr[2]) > boardSize)) {
			printf("Error: the range of the third parameter is invalid.\n"
					"It's value should be in the range 0 - %d. (0 for clearing the cell)\n",boardSize);
			return INVALID_S;
		}
	}
	else if((commType == GENERATE)){
		if(atoi(parametersArr[0]) < 0 || atoi(parametersArr[0]) > (boardSize*boardSize) ){/*the first parameter
				of this command needs to be equal or greater than 0, and not bigger than number of cells in the board*/
			printf("Error: the range of the first parameter is invalid.\n"
					"It's value should be in the range 0 - %d.\n",boardSize*boardSize);
			return INVALID_S;
		}
		if(atoi(parametersArr[1]) < 1 || atoi(parametersArr[1]) > (boardSize*boardSize)){/*the second parameter
			of this command needs to be greater than 0, and not bigger than number of cells in the board*/
			printf("Error: the range of the second parameter is invalid.\n"
					"It's value should be in the range 1 - %d.\n",boardSize*boardSize);
			return INVALID_S;
		}
	}
	else if((commType == HINT) || (commType == GUESS_HINT)){
		if((atoi(parametersArr[0]) < 1)|| (atoi(parametersArr[0]) > boardSize)){
			printf("Error: the range of the first parameter is invalid.\n"
					"It's value should be in the range 1 - %d.\n",boardSize);
			return INVALID_S;
		}
		if((atoi(parametersArr[1]) < 1)|| (atoi(parametersArr[1]) > boardSize)){
			printf("Error: the range of the second parameter is invalid.\n"
					"It's value should be in the range 1 - %d.\n",boardSize);
			return INVALID_S;
		}
	}
	return PLAY_S;
}



/* Repeatedly get user's input, parse and analyze it.
 *  If the input is valid, will call the relevant function that executes the command.
 *  else, the relevant error will be presented.
 */
GameStatus getUserCommands(Mode* mode, GameBoard* boardPtr, int* MarkE,DoublyLinkedList *dolist, int* inputLen){
	GameStatus currStatus;
	char inputArr[MAX_INPUT_CHARS] = {0}; /* initiate the parameters Array to zeros */
	char* inputPtr = inputArr;
	int commType;
	int numOfParameters;
	char* parametersArr[MAX_PARAMETERS_NUM + 1];
	int boardSize = boardPtr->BoardSize;
	*inputLen = 0;

	currStatus = getInput(inputPtr,inputLen);
	if(*inputLen == 0){ /*if the input was an enter or blank lines*/
		return currStatus;
	}

	if(currStatus ==  EXIT_S){ /*got an EOF from user*/
		return exitFunc();
	}
	if(currStatus == INVALID_S){/*the input contains more than 256 chars*/
		return InvalidInputLength();
	}

    commType = validateName(inputPtr);
	if (commType == INVALID){
		return InvalidCommandName();
	}

	currStatus = validateMode(*mode, commType);
	if (currStatus == INVALID_S){
		return InvalidCommandMode(commType);
	}

	numOfParameters = extractParameters(inputPtr, parametersArr);
	if (numOfParameters > MAX_PARAMETERS_NUM){ /*none of the commands can get more than the maximum number of parameters*/
		return TooMuchParameters(commType);
	}

	currStatus = validateNumOfParameters(commType,numOfParameters);
	if(currStatus == INVALID_S){
		return InvalidNumberOfParameters(commType,numOfParameters);
	}

	if(numOfParameters > 0){
		currStatus = validateParametersType(commType, numOfParameters, parametersArr);
		if(currStatus == INVALID_S){
			return INVALID_S;
		}
		currStatus = validateParametersRange(commType, numOfParameters, parametersArr,boardSize);
		if(currStatus == INVALID_S){
			return INVALID_S;
		}
	}

	switch(commType){
	case SOLVE:
		solve(boardPtr, mode, parametersArr[0] ,*MarkE, 0, dolist, &currStatus);
		return currStatus;
		break;

	case EDIT:
		if(numOfParameters > 0){
			edit(boardPtr, mode, parametersArr[0], 1, *MarkE, dolist, &currStatus); /*filePath = parametersArr[0] hasPath = 1*/
		}
		else{
			edit(boardPtr, mode, (char*)0, 0, *MarkE, dolist, &currStatus);
		}
		return currStatus;
		break;

	case MARK_ERRORS:
		mark_errors(MarkE, atoi(parametersArr[0]));
		return currStatus;
		break;

	case PRINT_BOARD:
		print_board(boardPtr, *mode, *MarkE);
		return currStatus;
		break;

	case SET:
		set(boardPtr, atoi(parametersArr[1]),atoi(parametersArr[0]),atoi(parametersArr[2]), *mode, *MarkE, &currStatus, dolist);
		return currStatus;
		break;

	case VALIDATE:
		validate(boardPtr,&currStatus,0);
		return currStatus;
		break;

	case GUESS:
		guess(boardPtr, atof(parametersArr[0]), *mode, *MarkE ,dolist, &currStatus);
		return currStatus;
		break;

	case GENERATE:
		generate(boardPtr, atoi(parametersArr[0]),atoi(parametersArr[1]), *mode, *MarkE, dolist,&currStatus);
		return currStatus;
		break;

	case UNDO:
		undo(dolist, *mode, boardPtr, 1, *MarkE);
		return currStatus;
		break;

	case REDO:
		redo(dolist, *mode,boardPtr, *MarkE);
		return currStatus;
		break;

	case SAVE:
		save(boardPtr, *mode, parametersArr[0],&currStatus);
		return currStatus;
		break;

	case HINT:
		hint(boardPtr, atoi(parametersArr[1]), atoi(parametersArr[0]),&currStatus);
		return currStatus;
		break;

	case GUESS_HINT:
		guess_hint(boardPtr, atoi(parametersArr[1]), atoi(parametersArr[0]));
		return currStatus;
		break;

	case NUM_SOLUTIONS:
		num_solutions(boardPtr);
		return currStatus;
		break;

	case AUTOFILL:
		autoFill(*mode, *MarkE, boardPtr, dolist, &currStatus);
		return currStatus;
		break;

	case RESET:
		reset(boardPtr, *mode, dolist, *MarkE);
		return currStatus;
		break;

	case EXIT:
		return(exitFunc());
		break;
	}
	return PLAY_S;
}




