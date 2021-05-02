/*
 * Game.c
 *
 */
#include "Game.h"
#include "stdio.h"
#include "stdlib.h"
#include <time.h>
#include "Solver.h"
#include "MainAux.h"
#include "Parser.h"
#include <string.h>

/*clear all cells excepts randomly chosen cells, their amount is according to the user's choice */
void fillFixedCells(char board [9][9][2], int fixedCells){
	int i,j,x,y;
	for(i = 0; i < fixedCells; i++){
		x = rand()%9;/*choose a number from 1 to 9*/
		y = rand()%9;/*choose a number from 1 to 9*/
		while(board[y][x][0]== '.'){/*while on a fixed cell keep choosing a new cell*/
			x = rand()%9;
			y = rand()%9;
		}
		board[y][x][0] = '.';
	}
	for(i = 0; i<9; i++){/*empty all unfixed cells*/
		for(j = 0; j<9; j++){
		if (board[i][j][0] != '.'){
			board[i][j][1] = ' ';
		}
		}
	}
}

/*creates empty 9*9*2 board, solve it and then clear all cells excepts
 * amount of cells needs to be fixed, finally prints the generated board */
int createBoard(char board [9][9][2], char solvedBoard [9][9][2]){
	int fixedCells;
	int i;
	int j;
	fixedCells = cellsToFill();
	if(fixedCells == -1){
		return 0;
	}
	for(i=0;i<9;i++){/*create empty board*/
		for(j=0;j<9;j++){
			board[i][j][0]=' ';
			board[i][j][1]=' ';
		}
	}
	generateBoard(board);
	memcpy(solvedBoard, board, sizeof(char)*9*9*2);
	fillFixedCells(board, fixedCells);
	printGameBoard(board);
	return 1;
}
/*check if the set command from the user is valid, and that the chosen cell is not fixed
 * and if so sets the value provided by the user to the cell, finally prints the updated board */
int set(char board[9][9][2]){
	char * str_tok_null_handler;
	int col;
	int row;
	int value;

	str_tok_null_handler = strtok(NULL, " \t\r\n");
	if (str_tok_null_handler == NULL ) {
		    printf("Error: invalid command\n");
			return 0;
	}
	col = atoi(str_tok_null_handler);
	str_tok_null_handler = strtok(NULL, " \t\r\n");
	if (str_tok_null_handler == NULL ) {
	    	printf("Error: invalid command\n");
			return 0;
	}
	row = atoi(str_tok_null_handler);
	str_tok_null_handler = strtok(NULL, " \t\r\n");
	if (str_tok_null_handler == NULL ) {
		    printf("Error: invalid command\n");
			return 0;
	}
	value = atoi(str_tok_null_handler);

	if(board[row-1][col-1][0] == '.'){ /*which means this cell is fixed*/
	    	printf("Error: cell is fixed\n");
			fflush(stdout);
			return 0;
	}
	else if(value == 0){/*clear cell*/
			board[row-1][col-1][1] = ' ';
			printGameBoard(board);
			return 0;
	}
	else if(!isLegalVal(row-1,col-1,value,board)){/*not legal value for cell*/
			printf("Error: value is invalid\n");
			fflush(stdout);
			return 0;
	}
	else{
				board[row-1][col-1][1] = value;/*value is valid so place it in cell*/
	}
	printGameBoard(board);
	if(isLastEmptyCell(col-1,row-1,board)&& (value!='0')){/*if we got here then the value is valid, so if it is the last cell the puzzle solved successfully*/
		    printf("Puzzle solved successfully\n");
			fflush(stdout);
			return 1;
	}
	return 0;


}

/*checks if the specified cell is the last unfilled cell in the board */
int isLastEmptyCell(int col,int row,char board[9][9][2]){
 int i;
 int j;
 for(i=0;i<9;i++){
	 for(j=0;j<9;j++){
		 if(board[i][j][1]==' ' && (i!=row || j!=col))
			 return 0;
	 }
 }
 return 1;
}

/*prints the value of the cell in the specified indexes according to the saved solved board*/
void hint(char solvedBoard[9][9][2]) {
	int col;
	int row;
	char * str_tok_null_handler;
	str_tok_null_handler = strtok(NULL, " \t\r\n");
	if (str_tok_null_handler == NULL ) {
		printf("Error: invalid command\n");
		return;
	}
	col = atoi(str_tok_null_handler);
	str_tok_null_handler = strtok(NULL, " \t\r\n");
	if (str_tok_null_handler == NULL ) {
		printf("Error: invalid command\n");
		return;
	}
	row = atoi(str_tok_null_handler);
	printf("Hint: set cell to %d\n", solvedBoard[row - 1][col - 1][1]);
}


/*checks and prints if the current board is solvable and if so
 * updates the saved solved board with the solution*/
void validate(char board[9][9][2], char solvedBoard[9][9][2]){
	int solvable;
	char copyBoard [9][9][2];
	memcpy(copyBoard ,board, sizeof(char)*9*9*2);
	solvable = solveBoard(copyBoard);
	if(solvable && (solvedBoard[0][0][0] == '.' || solvedBoard[0][0][0] ==' ')){
		solvedBoard = copyBoard;
		printf("Validation passed: board is solvable\n");
		fflush(stdout);
	}
	else{
		printf("Validation failed: board is unsolvable\n");
		fflush(stdout);
	}
}





