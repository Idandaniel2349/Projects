/*
 * DoublyLinkedList.h
 * This module takes care of the Doubly linked list used as the dolist in main.
 * The change struct defined here is a cell change done to our board holding the row, col and previous and new cell information.
 * The node struct is the struct used in the doubly linked list and holds change data(as a pointer and len to know how many chnages were done),
 * and pointers to prev and next nodes.
 * DoublyLinkedList holds the list sentinel, head tail and len + current position to know where we are when traversing the dolist.
 * At the end there are all the needed function definitions.
 *
 */


#ifndef DOUBLYLINKEDLIST_H_
#define DOUBLYLINKEDLIST_H_
#include "MainAux.h"

typedef struct{/*a change done to a cell*/
	int row;
	int col;
	Cell prev;
	Cell new;
}Change;

struct node{/*a node containing a list of changes for use in doublyLinkedList*/
	Change* data;
	int changelen;
	struct node *prev,*next;
};
typedef struct node Node;

typedef struct {/*a doubly linked list*/
	int len;
	Node *tail;
	Node *head;
	Node *current;
	Node *sentinel;
}DoublyLinkedList;


void createList(DoublyLinkedList *list);
int isEmpty(DoublyLinkedList *list);
void addAtStart(DoublyLinkedList *list, Change* data, int changelen);
void addAtEnd(DoublyLinkedList *list, Change* data, int changelen);
void addAfterCurrent(DoublyLinkedList *list, Change* data, int changelen);
void deleteFirst(DoublyLinkedList *list);
void deleteLast(DoublyLinkedList *list);
void deleteAllAfterCurrent(DoublyLinkedList *list);
void addAfterCurrentWithBoardsDifferenece(DoublyLinkedList *list, GameBoard* old_board, GameBoard* new_board);
void generateDifferenceBetweenTwoBoards(Change** data, int* changelen, GameBoard* old_board, GameBoard* new_board);
void addAfterCurrentWithCellChange(DoublyLinkedList *list, int row, int col, int old_val, int new_val, char old_sym, char new_sym);


void moveCurrentleft(DoublyLinkedList *list);
void moveCurrentRight(DoublyLinkedList *list);

void clean(DoublyLinkedList *list);
void freelist(DoublyLinkedList *list);



#endif /* DOUBLYLINKEDLIST_H_ */
