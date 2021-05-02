/*
 * Solver.h
 *
 */

#ifndef SOLVER_H_
#define SOLVER_H_

int solveBoard(char board[9][9][2]);
void getNextEmpty(char board[9][9][2],int cell[2]);
int isLegalVal(int row, int col, int num, char board[9][9][2]);
int isBoxLegal(int row, int col, int num, char board[9][9][2]);
int generateBoard(char board[9][9][2]);




#endif /* SOLVER_H_ */
