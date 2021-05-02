/*
 * StackImplementation.h
 * This module defines the stack being used by the exhaustive backtracking algorithm.
 * In this file there is a definition for a Stack structure which contains the max size of the stack,
 * the "stack pointer" and an array of elements.
 * The element structure is also defined in this file and it contains a board to represent a possible
 * board in the process of finding a solution to the board, and the row and column of the cell in
 * which the backtrack algorithm is inserted a value.

 */

#ifndef STACKIMPLEMENTATION_H_
#define STACKIMPLEMENTATION_H_
#include "DoublyLinkedList.h"

typedef struct Element{
	GameBoard stackBoard;
	int row; /*board element attributes*/
	int col;/*board element attributes*/
}Element;

typedef struct{
	int current;
	int maxSize;
	Element** elementsArr;
}Stack;


void freeElement(Element* element);
Stack* createStack(int N);
Element* initElemment(int row, int col, GameBoard* board);
Element* initElemment_fromExistingBoard(int row, int col, GameBoard* existBoard, int newVal, int oldRow, int oldCol);
void createElemment(Element element, int row, int col, GameBoard* board);
int isEmptyStack(Stack* stack);
void push(Stack* stack, Element* element);
Element* pop(Stack* stack);
void createElemment(Element element, int row, int col, GameBoard* board);
void freeStack(Stack* stack);
#endif /* STACKIMPLEMENTATION_H_ */
