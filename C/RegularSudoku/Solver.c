/*
 * Solver.c
 *
 */
#include "Solver.h"
#include "stdio.h"
# include "stdlib.h"
# include <time.h>
#include "Game.h"
#include "MainAux.h"
#include "Parser.h"

int solveBoard(char board[9][9][2]){
	int cell[2] = {-1,-1};
	int n;
	getNextEmpty(board,cell);/*find next empty cell in the board*/
	if(cell[0] == -1){/*no more empty cells in the board*/
		return 1;/*means the board solved successfully */
	}
	for(n = 1; n < 10; n++){/*run on order from 1 to 9*/
		if(isLegalVal(cell[0],cell[1],n,board)){/*check if the current value n is legal*/
			board[cell[0]][cell[1]][1] = n;/*place the legal value*/
			if(solveBoard(board)){/*keep solving the board with the changes we made, if returns 1 then there is a valid solution for the board*/
				return 1;/*there is a solution so return 1*/
			}
			board[cell[0]][cell[1]][1] = ' ';/*empty the cell since we will return to the previous cell because the solution isn't valid*/
		}
	}
	return 0;/*no valid solution*/
}

void getNextEmpty(char board[9][9][2],int cell[2]){/*find next empty cell in the board*/
	int row, col;
	for(row = 0; row<9; row++){
		for(col = 0 ; col < 9; col++){
			if(board[row][col][1] == ' '){/*if the cell is empty return it*/
				cell[0]= row;
				cell[1]= col;
				return;
			}
			}
		}
	}

int isLegalVal(int row, int col, int num, char board[9][9][2]){/*check if the value num is legal in cell rowXcol in board*/
	int i;
	for(i=0; i<9; i++){
		if ((board[row][i][1] ==num) || (board[i][col][1] ==num)){ /*check if the value is already exist in that row or column*/
			return 0;/*return 0 if exists*/
		}
	}
	if (!isBoxLegal(row, col, num, board)){/*check if the value num is legal in the cell box*/
		return 0;/*return 0 if not legal*/

	}
	return 1;/*return 1 if the value num is legal for the cell*/
}

int isBoxLegal(int row, int col, int num, char board[9][9][2]){/*check if the value num is legal in the cell box*/
	int i,j;
	for(i = (row-(row%3)); i<(row-(row%3))+3; i++){/*for the rows of the current box*/
		for(j = (col-(col%3)); j< (col-(col%3))+3; j++){/*for the columns of the current box*/
			if(board[i][j][1]==num){/*check if num in the cell iXj*/
				return 0;/*if num already in box return 0*/
			}
		}
	}
	return 1;/*if the value num is legal in the box return 1*/
}


int generateBoard(char board[9][9][2]){
	int cell[2] = {-1,-1};
	int n;
	int legalValues[9];
	int len;
	int indexRe;
	int cnt = 0;

	getNextEmpty(board,cell);/*find next empty cell in the board*/
	if(cell[0] == -1){/*no more empty cells in the board*/
		return 1;/*means the board solved successfully */
	}
	for(n = 1; n < 10; n++){/*fill the array with all legal values and save the length in cnt*/
		if(isLegalVal(cell[0],cell[1],n,board)){
			legalValues[cnt] = n;
			cnt++;
		}
	}
	len=cnt;
	while(len!=0){/*while there are available legal values for the cell in legalValues*/
		if(len==1){/*if only one legal value place it in the cell*/
			board[cell[0]][cell[1]][1] = legalValues[0];
			len--;
		}
		else{
		indexRe=rand()%len;/*choose a random index in the array legalValues from index 0 to len-1*/
		board[cell[0]][cell[1]][1] = legalValues[indexRe];/*place the value in the index we choose in the cell*/
		for(n=indexRe;n<len-1;n++){/*shift left all values after the one we removed*/
			legalValues[n]=legalValues[n+1];
	    }
		len--;

		}

		if(generateBoard(board)){/*keep regenerating the board with the changes we made, if returns 1 then there is a valid solution for the board*/
			return 1;/*there is a solution so return 1*/
		}
		board[cell[0]][cell[1]][1] = ' ';/*empty the cell since we will return to the previous cell because the solution isn't valid*/
	}
	return 0;/*no valid solution*/
}

