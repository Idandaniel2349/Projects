/*
 * exhaustiveBacktracking.h
*  This module takes care of the exhaustive backtracking algorithm(called stackSolve here), numOfSolutions functions for use in num_solutions
*  command in CommandsImplementation module.
*  This header file only takes care of functions definitions.
 */

#ifndef EXHAUSTIVEBACKTRACKING_H_
#define EXHAUSTIVEBACKTRACKING_H_
#include "MainAux.h"

int numberOfSolutions(GameBoard *gameboard);
int solveBoard(GameBoard *gameboard, int cnt);
int isBoardSolved(GameBoard *board);
int solveBoardstack(GameBoard *gameboard, int cnt);
void getNextEmpty(GameBoard *gameboard,int* inRow, int* inCol);
void getNextEmptyFromLocation(GameBoard *gameboard,int* inRow, int* inCol, int rowSearchStart, int colSearchStart);
int isLegalVal(int row, int col, int num, GameBoard *gameboard);
int isBoxLegal(int row, int col, int num, GameBoard *gameboard);

#endif /* EXHAUSTIVEBACKTRACKING_H_ */
