/*
 * Parser.c
 *
 *  Created on: Dec 26, 2019
 *      Author: adifi
 */
#include "Parser.h"
#include "stdio.h"
# include "stdlib.h"
# include <time.h>
#include "Game.h"
#include "Solver.h"
#include "MainAux.h"
#include <string.h>

int cellsToFill(){/*get the number of cells to fill from user*/
	int cells;
	printf("Please enter the number of cells to fill [0-80]:\n");
	fflush(stdout);
	scanf("%d",&cells);/*get the number of cells to fill from user*/
	if(feof(stdin)!=0){/*check for EOF*/
		printf("Exiting...\n");
		fflush(stdout);
		return -1;
	}
	while( cells < 0 || cells > 80 ){/*while value from user isn't valid*/
		printf("Error: invalid number of cells to fill (should be between 0 and 80)\n");
		fflush(stdout);
		scanf(" %d",&cells);/*get the number of cells to fill from user*/

	}
	return cells;
}

int userMove (char board[9][9][2], char solvedBoard[9][9][2],int solved){
	char move_input[1024];/*everything the user inputs*/
	char* com;/*the command the user makes first inputs*/
	/*get inputs line from user*/
	if(fgets(move_input,1024,stdin) == NULL){
		printf("Exiting...\n");
		fflush(stdout);
		return 0;
	}
	if(feof(stdin)!=0){/*check for EOF*/
		printf("Exiting...\n");
		fflush(stdout);
		return 0;
	}

	if (move_input[0] == '\n' || move_input[0] == ' ') {
		fflush(stdout);
	    return 1;
	}
	com = strtok(move_input," \t\r\n");/*get the command from the user to com and save back all other inputs to move*/

	/*check for command entered and execute it*/
	if(strcmp(com,"set")==0 && !solved){
		if(set(board)){
			fflush(stdout);
			return 3;
		}
	}
	else if(strcmp(com,"hint")==0 && !solved){
		hint(solvedBoard);
	}
	else if(strcmp(com,"validate")==0 && !solved){
		validate(board, solvedBoard);
	}
	else if(strcmp(com,"restart")==0){
		return 2;
	}
	else if(strcmp(com,"exit")==0){
		printf("Exiting...\n");
		fflush(stdout);
		return 0;
	}
	else{/*no valid command entered*/
		printf("Error: invalid command\n");
		fflush(stdout);
	}
	return 1;
}



