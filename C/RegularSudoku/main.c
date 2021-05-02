/*
 * main.c
 *
 */
#include "stdio.h"
# include "stdlib.h"
# include <time.h>
#include "Game.h"
#include "Solver.h"
#include "MainAux.h"
#include "Parser.h"


int main(int argc , char *argv[]){
	char board [9][9][2];/*our gameBoard*/
	char solvedBoard [9][9][2];/*a current solution for the gameBoard*/
	int move;
	int seed;
	char clean;
	int solved=0;

	/*get seed and use it*/
	if(argc==1)
		seed=5;
	if(argc>1)
		sscanf(argv[1],"%d",&seed);
	srand(seed);

	/*create game board*/
	if(!createBoard(board,solvedBoard)){
		return 0;
	}
	/* empty input buffer before main loop*/
	while((clean = getchar())!=EOF && clean!='\n'){};

	while(1){/*run the game*/
		move = userMove(board,solvedBoard,solved);/*get move from user and execute it*/
		if(move == 0){/*exit*/
			return 0;
		}

		if(move==2){/*restart*/
			if(!createBoard(board,solvedBoard)){/*create game board*/
				return 0;
			}
			solved=0;
			/* empty input buffer before main loop*/
			while((clean = getchar())!=EOF && clean!='\n'){};
		}
		if(move==3)
			solved=1;

	}
}

