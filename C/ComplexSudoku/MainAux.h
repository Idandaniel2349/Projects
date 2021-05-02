/*
 * MainAux.h
 * This module is responsible to hold functions used multiple times in the code and the main struct definitions for the gameboard.
 * Cell struct define the board cell as a value and symbol.
 * The gameboard struct is the struct that defines the gane informations related to the board needed to be saveed:
 * block width and height, and boardSize(that is the blockSize, row length and col length that are all equal).
 * At the end there are all the needed function definitions.
 */


#ifndef MAINAUX_H_
#define MAINAUX_H_
#include "MainAuxDefinitions.h"

/* define a structure for a cell in the sudoku board */
typedef struct{
	int value;
	char sym;

}Cell;

/* define a structure for the sudoku board and it's properties*/
typedef struct{
	int BoardSize;
	int BlockWidth;
	int BlockHeight;
	int erroneous;/*0- no error, 1- error*/
	Cell** Board;
}GameBoard;

void printGameBoard(GameBoard *gameboard, Mode mode, int MarkE);
void updateBoardErrors(GameBoard *gameboard);
void duplicate_board(GameBoard* newBoard,GameBoard* oldBoard);
void initGameBoard(GameBoard* newBoard,GameBoard* oldBoards);
void initCellMatrix(GameBoard* newBoard);
void freeGameBoard(GameBoard* board);
void freeCellMatrix(GameBoard* board);
void duplicate_board_values(GameBoard* to,GameBoard* from);
void isBoardSolvedSuccessfully(GameBoard* gameboard, Mode* mode, GameStatus* status);

void freeCellMatrixStack(Cell** matrix, int size);
GameBoard* duplicate_boardStack(GameBoard* oldBoard);
GameBoard* initGameBoardStack(int BoardSize, int BlockWidth, int BlockHeight, int erroneous);
Cell** initCellMatrixStack(int size);
Cell** allocateCellMatrix(int size);
Cell** duplicateCellMatrixStack(Cell ** matrix, int size);


#endif /* MAINAUX_H_ */
