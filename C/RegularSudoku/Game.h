/*
 * Game.h
 *
 */

#ifndef GAME_H_
#define GAME_H_

void fillFixedCells(char board [9][9][2], int fixedCells);
int createBoard(char board [9][9][2], char solvedBoard [9][9][2]);
int set(char board[9][9][2]);
void hint(char solvedBoard[9][9][2]);
void validate(char board[9][9][2], char solvedBoard[9][9][2]);
int isLastEmptyCell(int col,int row,char board[9][9][2]);
#endif /* GAME_H_ */
