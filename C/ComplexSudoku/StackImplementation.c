/*
 * StackImplementation.c
 * This module contains implementations of a Stack data structure, for the use of the exhaustive
 * backtracking algorithm, for implementing the num_solutions command and function.
 * The definition of the stuck structure is in the .h file.
 * it contains essential function - create stuck, pop, push, check if the stuck is full and if empty.
 * Pop and push also handled changes in the stack size (pop and push also includes adjustments to the
 * stack changing size).
 */
#include "StackImplementation.h"
#include "DoublyLinkedList.h"
#include "MainAux.h"
#include "stdio.h"
#include "stdlib.h"


/* create and initialize a new stack */
Stack* createStack(int stackSize){
	Stack *stack = (Stack*)malloc(sizeof(Stack));
	stack->current= -1;
	stack->maxSize = stackSize;
	stack->elementsArr = (Element**)malloc(sizeof(Element *)* stackSize);
	return stack;
}

void freeStack(Stack* stack){
	free(stack->elementsArr);
	free(stack);
}

/* duplicates the board, insert the given value to the given cell if it is not fixed, and returns
 * a new element that contains this board
 */
Element* initElemment_fromExistingBoard(int row, int col, GameBoard* existBoard, int newVal, int oldRow, int oldCol){
	Element* new_element = initElemment(row, col, existBoard);
	if(new_element->stackBoard.Board[oldRow][oldCol].sym != '.'){
		new_element->stackBoard.Board[oldRow][oldCol].value = newVal;
	}
	return new_element;

}

/* initiate an element need to be added to the stack */
Element* initElemment(int row, int col, GameBoard* board){
	Element* element = (Element *)malloc(sizeof(Element));
	element->row = row;
	element->col = col;
	element->stackBoard = *board;
	/* at this point, element->stackBoard->Board == board->board */
	element->stackBoard.Board = duplicateCellMatrixStack(board->Board, board->BoardSize);
	return element;
}

/* check and return 1 if the stack is empty , else 0 */
int isEmptyStack(Stack* stack)
{
	return (stack->current <= -1);
}

/* check and return 1 if the stack is full , else 0 */
int isFull(Stack* stack)
{
    return stack->current +1 >= stack->maxSize;
}

/* remove the last element that was insert to the stack and return it
 * if the stack became small - shrank its memory allocation */
Element* pop(Stack* stack){ /*remove the last element in the stack without returning it*/
	if(4*(stack->current+1) <= stack->maxSize){ /*reached 1/4 of the maximum size so shrink to half the size*/
		stack->maxSize = 0.5*(stack->maxSize);
		stack->elementsArr = (Element**)realloc (stack->elementsArr,sizeof(Element*)*stack->maxSize);
	}
	stack->current -=1;
	return stack->elementsArr[stack->current +1];
}

/* push an element to the stack, updates it's pointer. if half of the stack is full - double its size */
void push(Stack* stack, Element* element){
	if(isFull(stack)){
		stack->maxSize = 2*(stack->maxSize); /*reached the maximum size so double the size*/
		stack->elementsArr = (Element**)realloc (stack->elementsArr,sizeof(Element*)*stack->maxSize);
	}
	stack->current +=1;
	stack->elementsArr[stack->current] = element;
}

/* free an element of the stack */
void freeElement(Element* element){
	/*freeGameBoard(element->stackBoard);*/
	freeCellMatrixStack(element->stackBoard.Board, element->stackBoard.BoardSize);
	free(element);
}



