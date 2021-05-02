/*
 * MainAux.c
 * This module is responsible to hold functions used multiple times in the code.
 * Functions used to update information,and functions for allocating and freeing certain temporary memory for the commands.
 */

#include "MainAux.h"
#include "MainAuxDefinitions.h"
#include "exhaustiveBacktracking.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CommandsImplementation.h"

/*Check board cells for error values and update as necessary*/
void updateBoardErrors(GameBoard *gameboard){
	int i;
	int j;
	int isEro=0;
	for(i=0; i<gameboard->BoardSize; i++){/*for every row*/
		for(j=0; j<gameboard->BoardSize; j++){/*for every col*/
			if(gameboard->Board[i][j].sym!='.' && gameboard->Board[i][j].value!=0){/*if not a fixed cell*/
				if(isLegalVal(i,j,gameboard->Board[i][j].value,gameboard))/*if a legal cell mark as such*/
					gameboard->Board[i][j].sym=' ';
				else{/*not a legal cell, mark as such and light the erroneous bit*/
					gameboard->Board[i][j].sym='*';
					gameboard->erroneous=1;
					isEro=1;
				}
			}
		}
	}
	if(isEro==0)/*no errors in board*/
		gameboard->erroneous=0;
}


void freeCellMatrix(GameBoard* board){
	int i=0;
	for (i = 0; i < board->BoardSize; i++) {
		free(board->Board[i]);
	}
	free(board->Board);
}

void initCellMatrix(GameBoard* newBoard){
	int i;
	/*int j;*/
	newBoard->Board =(Cell**) malloc(sizeof(Cell*)*newBoard->BoardSize);/*allocate memory for the board*/
	for (i = 0; i < newBoard->BoardSize; i++) {
		newBoard->Board[i] = (Cell *) malloc(sizeof(Cell) * newBoard->BoardSize);
	}
}

void freeGameBoard(GameBoard* board){
	freeCellMatrix(board);
	free(board);
}

/* creates a new duplicated board from an existing board */
void duplicate_board(GameBoard* newBoard,GameBoard* oldBoard){
	/*int i,j*/;
	initCellMatrix(newBoard);
	duplicate_board_values(newBoard,oldBoard);
}

void initGameBoard(GameBoard* newBoard,GameBoard* oldBoard){
	newBoard->BlockHeight = oldBoard->BlockHeight;
	newBoard->BlockWidth = oldBoard->BlockWidth;
	newBoard->BoardSize = oldBoard->BoardSize;
	newBoard->erroneous = oldBoard->erroneous;
}

/* copy the cell values of a destination board to a target board */
void duplicate_board_values(GameBoard* to,GameBoard* from){
	int i,j;
	for(i=0;i<to->BoardSize;i++){
		for(j=0;j<to->BoardSize;j++){
			to->Board[i][j].value=from->Board[i][j].value;
			to->Board[i][j].sym=from->Board[i][j].sym;
		}
	}

}

/* check if the given board is full and if so, if the solution is valid */
void isBoardSolvedSuccessfully(GameBoard* gameboard, Mode* mode, GameStatus* status){
	/*check if all cells in the board were set*/
	if(number_of_empty_cells_in_board(gameboard) != 0){
		return;
	}
	/*if(solvedSuccessfully(gameboard)){*/
	if(gameboard->erroneous == 0){/*means if it is a valid solution*/
		printf("The board is solved successfully!\nthe mode is now set to init\n");
		*mode = INIT_M;
		*status = SOLVED_S;
	}else{
		printf("All cells in the board were set but the board is erroneous\n");
	}
	return;
}

/*free memory allocated for matrix of board cells */
void freeCellMatrixStack(Cell** matrix, int size){
	int i=0;
	for (i = 0; i < size; i++) {
		free(matrix[i]);
	}
	free(matrix);
}

/*copies the cell's values of the given board to a new board and return it */
GameBoard* duplicate_boardStack(GameBoard* oldBoard){
	int i,j;
	GameBoard* newBoard = initGameBoardStack(oldBoard->BoardSize, oldBoard->BlockWidth, oldBoard->BlockHeight, oldBoard->erroneous);
	for(i=0;i<newBoard->BoardSize;i++){
		for(j=0;j<newBoard->BoardSize;j++){
			newBoard->Board[i][j].value = oldBoard->Board[i][j].value;
			newBoard->Board[i][j].sym = oldBoard->Board[i][j].sym;
		}
	}
	return newBoard;
}
/* create and allocate duplicate of a given matrix (and it's size) */
Cell** duplicateCellMatrixStack(Cell ** matrix, int size){
	int i,j;
	Cell** new_matrix = allocateCellMatrix(size);
	for(i=0;i<size;i++){
		for(j=0;j<size;j++){
			new_matrix[i][j].value = matrix[i][j].value;
			new_matrix[i][j].sym = matrix[i][j].sym;
		}
	}
	return new_matrix;
}

GameBoard* initGameBoardStack(int BoardSize, int BlockWidth, int BlockHeight, int erroneous){
	GameBoard *game = (GameBoard*) malloc(sizeof(GameBoard));
	game->BlockHeight = BlockHeight;
	game->BlockWidth = BlockWidth;
	game->BoardSize = BoardSize;
	game->erroneous = erroneous;
	game->Board = initCellMatrixStack(game->BoardSize);
	return game;
}

Cell** allocateCellMatrix(int size){
	int i;
	Cell** matrix =(Cell**) malloc(sizeof(Cell*)*size);/*allocate memory for the board*/
	for (i = 0; i < size; i++) {
		matrix[i] = (Cell *) malloc(sizeof(Cell) * size);
	}
	return matrix;
}

Cell** initCellMatrixStack(int size){
	int i, j;
	Cell** matrix = allocateCellMatrix(size);
	for(i=0; i<size; i++){
		for(j=0; j<size; j++){
			matrix[i][j].value = 0;
			matrix[i][j].sym = ' ';
		}
	}
	return matrix;
}

