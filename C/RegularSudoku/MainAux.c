/*
 * MainAux.c
 */

#include "MainAux.h"
#include "stdio.h"
# include "stdlib.h"
# include <time.h>
#include "Game.h"
#include "Solver.h"
#include "Parser.h"


void printGameBoard(char gameboard[9][9][2]){
	int i;
	int j;

	for(i=1;i<10;i++){/*for every row*/
		if(i%3==1)/*if the first row for blocks*/
		   printf("----------------------------------\n");
		for(j=1;j<10;j++){/*for every column*/
			if(j%3==1)/*if the first column for blocks*/
				printf("| ");
			printf("%c",gameboard[i-1][j-1][0]);
			if(gameboard[i-1][j-1][1]!=' ')
				printf("%d ",gameboard[i-1][j-1][1]);
			else
				printf("%c ",' ');
		}
		printf("|\n");
	}
	printf("----------------------------------\n");








}
