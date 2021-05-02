/*
 * exhaustiveBacktracking.c
*  This module takes care of the exhaustive backtracking algorithm(called stackSolve here), numOfSolutions functions for use in num_solutions
*  command in CommandsImplementation module.
*  There is also implementation for IsLegal value commands and other commands it uses for use in stackSolve and also used in a few places for
*  commands in CommandsImplementation module.
 */
#include "exhaustiveBacktracking.h"
#include "MainAux.h"
#include "MainAuxDefinitions.h"
#include "StackImplementation.h"
#include "stdio.h"
#include "stdlib.h"
#include "CommandsImplementation.h"


/* implements the exhaustive backtracking algorithm by checking all possible full boards */
void stackSolve(Stack* stack, int* cntSolutions){
	int nextRow = -1;
	int nextCol = -1;
	int val;
	Element* currentElement = pop(stack);
	if(currentElement->col == -1 && currentElement->row == -1){
		*cntSolutions += 1;
		freeElement(currentElement);
		return;
	}

	getNextEmptyFromLocation(&currentElement->stackBoard, &nextRow, &nextCol, currentElement->row, currentElement->col+1);
	for (val = 1; val <= currentElement->stackBoard.BoardSize ; val += 1){
		if (isLegalVal(currentElement->row, currentElement->col, val, &currentElement->stackBoard)){
			Element* e = initElemment_fromExistingBoard(nextRow, nextCol, &currentElement->stackBoard, val, currentElement->row, currentElement->col);
			push(stack, e);
		}
	}
	freeElement(currentElement);
}

/*find next empty cell in the board */
void getNextEmptyFromLocation(GameBoard *gameboard,int* inRow, int* inCol, int rowSearchStart, int colSearchStart){
	int col, row = 0;
	if (rowSearchStart > 0){
		row = rowSearchStart;
	}
	for(; row < gameboard->BoardSize; row++){
		for(col = 0 ; col < gameboard->BoardSize; col++){
			if(row == rowSearchStart && col < colSearchStart){
				continue;
			}
			if(gameboard->Board[row][col].value == 0){/*if the cell is empty return it*/
				*inRow = row;
				*inCol = col;
				return;
			}
		}
	}
}

/*find next empty cell in the board*/
void getNextEmpty(GameBoard *gameboard,int* inRow, int* inCol){
	getNextEmptyFromLocation(gameboard, inRow, inCol, 0, 0);
}

/*check if the value num is legal in cell rowXcol in board*/
int isLegalVal(int row, int col, int num, GameBoard *gameboard){
	int i;
	for(i=0; i< gameboard->BoardSize; i++){
		if (i!=row && (gameboard->Board[i][col].value == num)){ /*check if the value already exist in that row*/
			return 0;/*return 0 if exists*/
		}
		if ((gameboard->Board[row][i].value == num) && i!=col){ /*check if the value already exist in that col*/
			return 0;/*return 0 if exists*/
		}
	}
	if (!isBoxLegal(row, col, num, gameboard)){/*check if the value num is legal in the cell box*/
		return 0;/*return 0 if not legal*/
	}
	return 1;/*return 1 if the value num is legal for the cell*/
}

/*check if the value num is legal in the cell box*/
int isBoxLegal(int row, int col, int num, GameBoard *gameboard){
	int i,j;
	for(i = (row-(row%gameboard->BlockHeight)); i<(row-(row%gameboard->BlockHeight))+ gameboard->BlockHeight; i++){/*for the rows of the current box*/
		for(j = (col-(col%gameboard->BlockWidth)); j< (col-(col%gameboard->BlockWidth))+ gameboard->BlockWidth; j++){/*for the columns of the current box*/
			if(i==row && j==col){
			}
			else if(gameboard->Board[i][j].value == num){/*check if num in the cell iXj*/
				return 0;/*if num already in box return 0*/
			}
		}
	}
	return 1;/*if the value num is legal in the box return 1*/
}

int isBoardSolved(GameBoard *board){
	int row = -1;
	int col = -1;
	getNextEmpty(board, &row, &col);
	if(row == -1 && col == -1){
		return 1;
	}
	return 0;
}

/* calculates and returns the number of solutions of the given board by calling using exhaustive
 * backtracking algorithm */
int numberOfSolutions(GameBoard* gameboard){
	int cntSolutions;
	int startRow;
	int startCol;
	int stackSize;
	Stack* stack;
	Element* firstElement;

	cntSolutions = 0;
	startRow = -1;
	startCol = -1;
	stackSize = gameboard->BoardSize * gameboard->BoardSize;
	stack = createStack(stackSize); /*initiate a stack for the algorithm use*/
	getNextEmpty(gameboard, &startRow, &startCol); /* get the next empty cell in the board*/

	if(number_of_empty_cells_in_board(gameboard) == 0){/*the board is full and not erroneous so there is 1 solution*/
		freeStack(stack);
		return 1;
	}
	firstElement = initElemment(startRow, startCol, gameboard);
	push(stack, firstElement);
	while(!isEmptyStack(stack)){ /* runs while the stack is not empty, meaning didnt finish check all options*/
		stackSolve(stack, &cntSolutions);
	}
	freeStack(stack);
	return cntSolutions;
}






