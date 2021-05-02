/*
 * main.c
 * This module is responsible for running the sudoku game, the first part of the module is allocation of needed memory throughout the game,
 * then repeatedly getting user commands and executing them until exit status is achieved. Then freeing all needed memory and ending the game.
 * Here we have the main parameters of the game: gameboard, mode, MarkE, status, dolist.
 */
#include "MainAuxDefinitions.h"
#include "MainAux.h"
#include "DoublyLinkedList.h"
#include "Parser.h"
#include "stdio.h"
#include "stdlib.h"
/*#include "CommandsImplementation.h"*/
/*#include "Gurobi.h"*/


int main(){
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
    Mode mode;
    GameBoard* board;
	int MarkE;
	GameStatus status;
	DoublyLinkedList *dolist;
	int inputLen = 0;
	int i = 0;
	dolist = (DoublyLinkedList*)(malloc(sizeof(DoublyLinkedList)));
	createList(dolist);
	board=(GameBoard*) malloc(sizeof(GameBoard));/*further initialization when loading a certain board*/
	if(dolist==NULL || board==NULL){
		mallocError(&status);
		return 0;
	}

    status = PLAY_S;
    insertCommandProperties(); /*insert each command properties
    into the Commands Properties Array later being used for commands input validations*/


    /*initiate a board: initiating ot firstly here to avoid problems when loading new boards*/
    board->BlockHeight=3;
	board->BlockWidth=3;
	board->BoardSize=9;
	board->erroneous=0;
	board->Board=(Cell**) malloc(sizeof(Cell*)*(9));
	if(board->Board==NULL){
		mallocError(&status);
		return 0;
	}
	for (i = 0; i < 9; i++) {
		board->Board[i] = (Cell *) malloc(sizeof(Cell) * 9);
		if(board->Board[i]==NULL){
			mallocError(&status);
			return 0;
		}
	 }

	/* getting input from the user, will run as long as no exit occurred */
    while(status != EXIT_S){ /*initializations for Init mode*/
    	mode = INIT_M;
    	MarkE = 1;
    	status = PLAY_S;
    	printf("Welcome to the Sudoku game!\n"
    			"In order to edit an existing puzzle, use the 'edit' command and provide the path to the puzzle.\n"
    			"In order to create a new puzzle, use the 'edit' command without any parameter.\n"
    			"In order to solve a pre-existing puzzle, use the 'solve' command and provide the path to the puzzle.\n\n");
    	while(status == PLAY_S || status == INVALID_S){ /*while puzzle haven't been solved or exited*/
    		printf("Please enter a desired command:\n");
    		status = getUserCommands(&mode, board, &MarkE,dolist,&inputLen);
    		while((status == PLAY_S || status == INVALID_S) && (inputLen == 0)){
    			status = getUserCommands(&mode, board, &MarkE,dolist,&inputLen);
    		}
    	}
    }


    /*free board created:*/
	for(i = 0; i < board->BoardSize; i++) {
		free(board->Board[i]);
	}
	free(board->Board);
    free(board);
    freelist(dolist);/*free the dolist, command in DoublyLinkedList module */
    /*freeCommandsProperties(); added to exitfunc instead */
    return 0;
}
