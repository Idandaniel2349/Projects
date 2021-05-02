/*
 * CommandsImplementation.h
 *
 *  This module takes care of all the possible commands by the user called from the parser module.
 *  This header file only takes care of functions definitions.
 */

#ifndef COMMANDSIMPLEMENTATION_H_
#define COMMANDSIMPLEMENTATION_H_

#include "MainAuxDefinitions.h"
#include "DoublyLinkedList.h"


void undo(DoublyLinkedList *dolist,Mode mode,GameBoard *gameboard, int print,int MarkE);
void redo(DoublyLinkedList *dolist,Mode mode,GameBoard *gameboard,int MarkE);
void autoFill(Mode mode,int MarkE,GameBoard *gameboard,DoublyLinkedList *dolist, GameStatus* status);
int findLegalValue(GameBoard *gameboard, int row, int col);
void save(GameBoard *gameboard,Mode mode,char *x, GameStatus* status);
void reset(GameBoard *gameboard,Mode mode, DoublyLinkedList *dolist,int MarkE);
void print_board(GameBoard *gameboard, Mode mode, int MarkE);
void mark_errors(int *MarkE, int x);
void solve(GameBoard *gameboard,Mode *mode, char *x,int MarkE, int isEdit,DoublyLinkedList *dolist, GameStatus* status);
int cheack_natural( char *s);
int cheack_valid( char *s,int blockSize);
void placeCell(Cell** Board,int row,int col,char *data,int isEdit);
int checkFixedLegal(GameBoard *game);
void edit(GameBoard *gameboard,Mode *mode, char *x, int isX, int MarkE,DoublyLinkedList *dolist, GameStatus* status);
void make9X9Board(GameBoard *gameboard,GameStatus* status);

void hint(GameBoard* gameboard, int x, int y,GameStatus* status);
void guess_hint(GameBoard* gameboard, int x, int y);
void guess(GameBoard* gameboard, float x,Mode mode, int MarkE, DoublyLinkedList *dolist, GameStatus* status);
void num_solutions(GameBoard* gameboard);
void set(GameBoard *gameboard, int row, int col, int val, Mode mode, int MarkE, GameStatus* status, DoublyLinkedList *dolist);
void generate(GameBoard* gameboard, int x, int y,Mode mode, int MarkE, DoublyLinkedList *dolist,GameStatus* status);
int number_of_empty_cells_in_board(GameBoard *gameboard);
int generate_board(GameBoard* gameboard, int x, int y);
void fix_random_cell(GameBoard* gameboard);
int fill_random_cell(GameBoard* gameboard);

GameStatus exitFunc();

int validate(GameBoard *gameboard,GameStatus* status,int isSave);
int validateBoard(GameBoard *gameboard);
int number_of_empty_cells_in_board(GameBoard *gameboard);


#endif /* COMMANDSIMPLEMENTATION_H_ */
