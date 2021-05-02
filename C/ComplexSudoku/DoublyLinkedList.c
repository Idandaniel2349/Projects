/*
 * DoublyLinkedList.c
 * This module implements the Doubly linked list used as the dolist in main. information about the struct definitions in h file.
 * Here we create the functions that make changes to the list, taking into the account the needed information needed for saving the changes of the dolist.
 * Except the basis functions we also have functions to delete all nodes after current(for when adding in the middle of a list and needing to delete all afterwards)
 * Functions to find the changes needed to be done. Functions to move the current pointer(for undo and redo), and functions to clean the list(in solve and edit).
 * Also freelist functions to free all needed memory in main before exiting.
 */
#include "DoublyLinkedList.h"
#include "MainAux.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void createList(DoublyLinkedList *list){/*creats an empty list with sentinel*/
	/*set memory for sentinel and then set its default values for empty list*/
	Node *sentinel=malloc(sizeof(Node));
	sentinel->changelen=-1;
	sentinel->data=NULL;
	sentinel->next=NULL;
	sentinel->prev=NULL;
	list->sentinel=sentinel;
	/*set default values for empty list*/
	list->head=list->sentinel;
	list->tail=list->sentinel;
	list->current=list->sentinel;
	list->len=0;
}

int isEmpty(DoublyLinkedList *list){/*check if the list is empty*/
	return list->len==0;
}

void addAtStart(DoublyLinkedList *list, Change* data, int changelen){/*add a node at the start of the list(as head)*/
    Node *node=malloc(sizeof(Node));/*adding memory for the new node*/
    /*adding the necessary data to the node*/
    node->data=data;
    node->changelen=changelen;
    node->prev=list->sentinel;
    if(isEmpty(list)){/*if the list is empty add node as head, current and tail*/
    	node->next=NULL;
    	list->head= node;
        list->tail= node;
        list->current= node;
        list->sentinel->next=node;
    	list->len++;
    	return;
    }
    /*if the list is not empty add node as head*/
    node->next = list->head;
    list->head->prev = node;
    list->head=node;
    list->len++;
}

void addAfterCurrent(DoublyLinkedList *list, Change* data, int changelen){/*add a node after the current one as tail*/
	deleteAllAfterCurrent(list);
	addAtEnd(list,data,changelen);
}

void addAfterCurrentWithCellChange(DoublyLinkedList *list, int row, int col, int old_val, int new_val, char old_sym, char new_sym){
	Change* data = (Change*)malloc(sizeof(Change));
	data[0].col = col;
	data[0].row = row;
	data[0].prev.value = old_val;
	data[0].new.value = new_val;
	data[0].prev.sym = old_sym;
	data[0].new.sym = new_sym;
	addAfterCurrent(list, data, 1);
}

/* generate new node for the dolist, with differences between two steps
 */
void addAfterCurrentWithBoardsDifferenece(DoublyLinkedList *list, GameBoard* old_board, GameBoard* new_board){
	Change* data;
	int changelen;
	generateDifferenceBetweenTwoBoards(&data, &changelen, old_board, new_board);
	addAfterCurrent(list, data, changelen);
}

/* create Changes array of differences between two game-boards
 * data and changelen are only pointer to store the result of the calculation
 */
void generateDifferenceBetweenTwoBoards(Change** data, int* changelen, GameBoard* old_board, GameBoard* new_board){
	int i, j;
	/* These errors should not happen at any time ! */
	if(new_board->BlockHeight != old_board->BlockHeight){
		printf("Fatal error when generating difference between two boards");
		return;
	}
	if(new_board->BlockWidth != old_board->BlockWidth){
		printf("Fatal error when generating difference between two boards");
		return;
	}
	if(new_board->BoardSize != old_board->BoardSize){
		printf("Fatal error when generating difference between two boards");
		return;
	}
	*data = (Change*)malloc(sizeof(Change)*old_board->BoardSize*old_board->BoardSize);
	*changelen = 0;
	for(i=0; i<old_board->BoardSize; i++){
		for(j=0; j<old_board->BoardSize; j++){
			if(old_board->Board[i][j].value != new_board->Board[i][j].value || old_board->Board[i][j].sym != new_board->Board[i][j].sym){
				(*data)[*changelen].row = i;
				(*data)[*changelen].col = j;
				(*data)[*changelen].prev = old_board->Board[i][j];
				(*data)[*changelen].new = new_board->Board[i][j];
				(*changelen)++;
			}
		}
	}
	*data = (Change*)realloc(*data, sizeof(Change)*(*changelen));
}

void addAtEnd(DoublyLinkedList *list, Change* data, int changelen){/*add a node as the list tail*/
	    Node *node=malloc(sizeof(Node));/*adding memory for the new node*/
	    /*adding the necessary data to the node*/
	    node->data=data;
	    node->changelen=changelen;
	    node->next=NULL;
	    if(isEmpty(list)){/*if the list is empty add node as head, current and tail*/
	    	node->prev=list->sentinel;
	    	list->head= node;
	    	list->tail= node;
	    	list->current= node;
	    	list->sentinel->next=node;
	    	list->len++;
	    	return;
	    }
	    /*if the list is not empty add node as tail*/
	    node->prev = list->tail;
	    list->tail->next = node;
	    list->tail=node;
	    list->current= list->tail;
	    list->len++;
}

void deleteFirst(DoublyLinkedList *list){/*delete first node in list*/
	Node *temp;
	if(isEmpty(list)){/*if the list is empty do nothing*/
		return;
	}
	if(list->len==1){/*if the list has one item change to an empty list*/
		createList(list);
		return;
	}
	/*else, delete the first node by disconnecting it and free its memory*/
	temp=list->head;
	list->head = list->head->next;
	list->head->prev = list->sentinel;
	list->sentinel->next=list->head;
	temp->next=NULL;
	temp->prev=NULL;
	list->len--;
	free(temp);
}

void deleteLast(DoublyLinkedList *list){/*delete last node in list*/
	Node *temp;
	if(list->len==0){/*if the list is empty do nothing*/
		return;
	}
	if(list->len==1){/*if the list has one item change to an empty list*/
		createList(list);
		return;
	}
	/*else, delete the last node by disconnecting it and free its memory*/
	temp=list->tail;
	list->tail = list->tail->prev;
	list->tail->next=NULL;
	list->current= list->tail;
	temp->prev=NULL;
	list->len--;
	free(temp);
}

void deleteAllAfterCurrent(DoublyLinkedList *list){/*delete all nodes after current*/
	Node *temp = list->tail;
	if (isEmpty(list) || list->current->next==NULL)/*if the list is  empty or or current is tail do nothing*/
		return;
	while(temp!=list->current){/*delete and free all nodes after current from the list*/
		list->tail = list->tail->prev;
		list->len--;
		free(temp->data);
		free(temp);
		temp=list->tail;
	}
	list->current->next=NULL;
}

void moveCurrentleft(DoublyLinkedList *list){/*moves current node to be the next one*/
	if(list->current->prev==NULL)
		return;
	list->current=list->current->prev;
}
void moveCurrentRight(DoublyLinkedList *list){/*moves current node to be the previous one*/
	if(list->current->next==NULL)
			return;
	list->current=list->current->next;

}

void clean(DoublyLinkedList *list){/*clean all moves from the list*/
	list->current=list->sentinel;
	deleteAllAfterCurrent(list);
	free(list->sentinel);
	createList(list);
}

void freelist(DoublyLinkedList *list){/*free all list data that was malloced*/
	if(list->len>0){
		list->current=list->sentinel;
		deleteAllAfterCurrent(list);
	}
	free(list->sentinel);
	free(list);
}

