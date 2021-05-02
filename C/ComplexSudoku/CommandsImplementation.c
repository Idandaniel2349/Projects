/*
 * CommandsImplementation.c
*  This module takes care of all the possible commands by the user called from the parser module.
*  Specific explanations of every functions where given in the code.
 */

#include "CommandsImplementation.h"
#include "Gurobi.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "Errors.h"
#include "MainAuxDefinitions.h"
#include "MainAux.h"
#include "DoublyLinkedList.h"
#include "StackImplementation.h"
#include "exhaustiveBacktracking.h"
#include "string.h"


/*undo the changes of the current node in the dolist, print for knowing if we need to print the changes*/
void undo(DoublyLinkedList *dolist,Mode mode,GameBoard *gameboard,int print,int MarkE){
	int i;
	Change *data;
	if(isEmpty(dolist) || dolist->current==dolist->sentinel){/*np more undo available*/
		undoLimit();
		return;
	}
	data=dolist->current->data;
	for(i=0;i<dolist->current->changelen;i++){/*for every change return the previous data in cell*/
		gameboard->Board[data[i].row][data[i].col]=data[i].prev;
		if (print==1)
			printf("changed cell [%d,%d] from %d to %d\n", data[i].col+1, data[i].row+1, data[i].new.value,data[i].prev.value);
	}
	moveCurrentleft(dolist);
	if(print==1){
		updateBoardErrors(gameboard);
		print_board(gameboard,mode,MarkE);
	}
}

/*redo the changes of the current node in the dolist*/
void redo(DoublyLinkedList *dolist,Mode mode,GameBoard *gameboard,int MarkE){
	int i;
	Change *data;
	if(isEmpty(dolist) || dolist->current->next==NULL){/*np more redo available*/
		redoLimit();
		return;
	}
	moveCurrentRight(dolist);
	data=dolist->current->data;
	for(i=0;i<dolist->current->changelen;i++){/*for every change return the new data in cell*/
		gameboard->Board[data[i].row][data[i].col]=data[i].new;
		printf("changed cell [%d,%d] from %d to %d\n", data[i].col+1, data[i].row+1, data[i].prev.value,data[i].new.value);
	}
	updateBoardErrors(gameboard);
	print_board(gameboard,mode,MarkE);
}

/*fills all empty cells with only 1 possible value*/
void autoFill(Mode mode,int MarkE,GameBoard *gameboard,DoublyLinkedList *dolist, GameStatus* status){
	int i,j;
	int value;
	int cha=0;/*number of changes*/
	Cell ** board;
	Change *data;
	data=(Change*)malloc(sizeof(Change)*gameboard->BoardSize*gameboard->BoardSize);
	/*Init new board*/
	/*allocate memory for new gameboard*/
	board=(Cell**) malloc(sizeof(Cell*)*(gameboard->BoardSize));
	if(data==NULL || board==NULL)/*malloc allocation failed*/
		return mallocError(status);

	for (i = 0; i < gameboard->BoardSize; i++) {
		board[i] = (Cell *) malloc(sizeof(Cell) * gameboard->BoardSize);
		if(board[i]==NULL)/*malloc allocation failed*/
			return mallocError(status);
	}
	/*check conditions for command*/
	if(gameboard->erroneous==1){
		printf("can't autofill an erroneous board\n");
		free(data);
		for (i = 0; i < gameboard->BoardSize; i++) {
			free(board[i]);
		}
		free(board);
		return;
	}
	/*copy the board values to new board*/
	for (i = 0; i < gameboard->BoardSize; i++) {
			for (j = 0; j < gameboard->BoardSize; j++) {
				board[i][j]=gameboard->Board[i][j];
		     }
	 }
	/*fill obvious values in new board*/
	for (i = 0; i < gameboard->BoardSize; i++) {/*for every row*/
			for (j = 0; j < gameboard->BoardSize; j++) {/*for every col*/
				if(gameboard->Board[i][j].value==0){/*if an empty cell*/
					value=findLegalValue(gameboard,i,j);
					if(value!=-1){/*there is only one possible value for the cell*/
						board[i][j].value=value;/*change value in new board*/
						/*add the change to the node*/
						data[cha].row=i;
						data[cha].col=j;
						data[cha].prev.value=0;
						data[cha].prev.sym=' ';
						data[cha].new.value=value;;
						data[cha].new.sym=' ';
						cha++;
						printf("board in Cell (%d,%d) changed from value 0 to %d\n",j+1,i+1,value);
					}
				}
	         }
    }
	addAfterCurrent(dolist,data,cha);/*add the move to the move list*/

   /*free previous board and set new board*/
	for (i = 0; i < gameboard->BoardSize; i++) {/*for every row*/
		for (j = 0; j < gameboard->BoardSize; j++) {/*for every col*/
			if(gameboard->Board[i][j].value==0){/*if an empty cell*/
				gameboard->Board[i][j].value=board[i][j].value;
			}
		}
	}
	/*free needed memory*/
	for (i = 0; i < gameboard->BoardSize; i++) {
			free(board[i]);
	}
	free(board);
	/*update for errors in new board*/
	updateBoardErrors(gameboard);
	/*print the updated board*/
	print_board(gameboard,mode,MarkE);
	/*check if the function solve the board*/
	if(mode == SOLVE_M){
		isBoardSolvedSuccessfully(gameboard, &mode, status);/*changes the status to SOLVED_S if it does*/
	}
}

/*finding the one legal value for the cell i,j , if more then 1 available or cell not empty return -1*/
int findLegalValue(GameBoard *gameboard, int row, int col){
	int i=1;
	int index=0;
	int val;
	while(index<2 && i<gameboard->BoardSize+1){/*while there is only one or zero legal values for cell and running on all possible values i*/
		if(isLegalVal(row,col,i,gameboard)){/*if i is a legal value for the cell*/
					val=i;
					index++;
		}
		i++;
	}
	if(index!=1)/*means the number of legal values is diffrent then 1*/
		return -1;
	else/*only one legal value, return it*/
		return val;
}

/*save the current board to file in path x*/
void save(GameBoard *gameboard,Mode mode, char* x,GameStatus* status){
	FILE * fPtr = NULL;
	int i;
	int j;

	if(mode==EDIT_M){
		if(gameboard->erroneous==1){/*can't save an erronous board in edit mode*/
			invalidSave("erroneous");
			return;
		}
		i=validate(gameboard,status,1);
		if(i==-1){/*there was a malloc failure in validateBoard*/
			return;
		}
		if(i==0){/*can't save an unsolvable board in edit mode*/
			invalidSave("unsolvable");
			return;
		}
	}

	fPtr=fopen(x,"w");
	if (fPtr==NULL){/*failed to open file*/
		fileOpenFailure(x);
		return;
	}
	/*write gameboard sizes to file*/
	fprintf(fPtr,"%d ",gameboard->BlockHeight);
    fprintf(fPtr,"%d\n",gameboard->BlockWidth);
	/*write gameboard values to file*/
	for(i=0;i<gameboard->BoardSize;i++){/*for every row*/
		for(j=0;j<gameboard->BoardSize;j++){/*for every col*/
			if(gameboard->Board[i][j].value==0)/*if empty cell*/
				fprintf(fPtr,"0 ");
			else{
				if(mode==EDIT_M)
					fprintf(fPtr,"%d. ",gameboard->Board[i][j].value);
				if(mode==SOLVE_M){
					if(gameboard->Board[i][j].sym=='*' || gameboard->Board[i][j].sym==' ')
						fprintf(fPtr,"%d ",gameboard->Board[i][j].value);
					if(gameboard->Board[i][j].sym=='.')
						fprintf(fPtr,"%d. ",gameboard->Board[i][j].value);
				}
			}
		 }
		 fprintf(fPtr,"\n");/*end of row*/
	}
	fclose(fPtr);
}

/*reset the board to initial state*/
void reset(GameBoard *gameboard,Mode mode, DoublyLinkedList *dolist,int MarkE){

	while(dolist->current!=dolist->sentinel){/*while there are moves to undo, undo them*/
		undo(dolist,mode,gameboard,0,MarkE);
	}
	updateBoardErrors(gameboard);
	print_board(gameboard,mode,MarkE);
}

/*printing the game board in the needed format*/
void print_board(GameBoard *gameboard, Mode mode, int MarkE){
	int i;
	int j;
	int t;
	int N=gameboard->BlockHeight*gameboard->BlockWidth;
	int m=gameboard->BlockHeight;
	int n=gameboard->BlockWidth;


	for(i=1;i<N+1;i++){/*for every row*/
		if(i%m==1 || m==1){/*if the first row for blocks*/
		   for(t=0;t<4*N+m+1;t++){
			   printf("-");
		   }
		   printf("\n");
		}
		for(j=1;j<N+1;j++){/*for every column*/
			if(j%n==1 || n==1)/*if the first column for blocks*/
				printf("|");

			if(gameboard->Board[i-1][j-1].value==0){
				printf(" ");
				printf(" ");
			}
			else if(gameboard->Board[i-1][j-1].value<10){/*if the cell value is one character*/
				printf("%c",' ');
				printf("%d",gameboard->Board[i-1][j-1].value);
			}
			else/*if the cell value is two character*/
				printf("%2d",gameboard->Board[i-1][j-1].value);

			if(gameboard->Board[i-1][j-1].sym=='*'){/*if cell s is  asterisk- cell value is an error*/
				if(mode==EDIT_M || MarkE==1)/*if we need to show there is an error*/
					printf("%c ",gameboard->Board[i-1][j-1].sym);
				else{
					printf("%c ",' ');
				}
			}
			else{
				printf("%c ",gameboard->Board[i-1][j-1].sym);
			}
		}
		printf("|\n");
	}
	for(t=0;t<4*N+m+1;t++){
	    printf("-");
    }
    printf("\n");
}

/*change MarkE value to x*/
void mark_errors(int *MarkE, int x){
	*MarkE=x;
}

/*init a board from file in path x, isEdit marks whatever this was called from function edit or not*/
void solve(GameBoard *gameboard,Mode *mode,char *x,int MarkE, int isEdit,DoublyLinkedList *dolist, GameStatus* status){
	FILE * fPtr = NULL;
	char * s;
	char** data;
	int index=0;
	int c;
	int height;
	int width;
	int blockSize;
	int i,j;
	Cell** Board;
	GameBoard *game;

	s=(char*)malloc(sizeof(char*));/*to avoid init errors*/
	if(s==NULL)/*malloc allocation failed*/
		 return mallocError(status);


	fPtr=fopen(x,"r");
	if (fPtr==NULL){/*failed to open file*/
			fileOpenFailure(x);
			free(s);
			return;
	}
	/*get block height*/
	if((c=fscanf(fPtr,"%s",s))==EOF){/*get first value form file, if EOF - no more values available */
		fileTooShort(x);
		fclose(fPtr);
		free(s);
		return;
	}
	if(c==0){/*cheacking if file read failed*/
		fileReadFailure(x);
		fclose(fPtr);
		free(s);
		return;
	}
	if((c=cheack_natural(s))==-1)/*malloc failure in cheack_natural function*/
		return mallocError(status);
	if(c==0){/*if result from isNatural is -1, the charcter is not a natural number*/
		notNatural(s);
		fclose(fPtr);
		free(s);
		return;
	}
	sscanf(s,"%d",&height);

	/*get block width*/
	if((c=fscanf(fPtr,"%s",s))==EOF){/*get second value form file, if EOF - no more values available */
		fileTooShort(x);
		fclose(fPtr);
		free(s);
		return;
	}
	if(c==0){/*cheacking if file read failed*/
		fileReadFailure(x);
		fclose(fPtr);
		free(s);
		return;
	}
	if((c=cheack_natural(s))==-1)/*malloc failure in cheack_natural function*/
		return mallocError(status);
	if(c==0){/*if result from isNatural is -1, the charcter is not a natural number*/
		notNatural(s);
		fclose(fPtr);
		free(s);
		return;
	}
	sscanf(s,"%d",&width);
	blockSize=height*width;

	data=(char**)malloc(sizeof(char*)*blockSize*blockSize);/*allocate the needed memory for data array*/
	if(data==NULL)/*malloc allocation failed*/
		 return mallocError(status);
	for (i = 0; i < blockSize*blockSize; i++) {
		data[i] = (char *) malloc(sizeof(char)*3);
		if(data[i]==NULL)/*malloc allocation failed*/
			return mallocError(status);
    }


	/*get board values*/
	while((c=fscanf(fPtr,"%s",s))!=EOF){/*while there are more values to read and there is less then */
		if(c==0){/*if read failed*/
			fileReadFailure(x);
			fclose(fPtr);
			for (i = 0; i < blockSize*blockSize; i++) {
				free(data[i]);
		    }
			free(data);
			free(s);
			return;
		}
		if(index>(blockSize*blockSize)-1){/*if we read too many values*/
			index++;
			break;
		}
		if(cheack_valid(s,blockSize)==0){/*if value isn't valid*/
			invalidCellValue(blockSize);
			fclose(fPtr);
			for (i = 0; i < blockSize*blockSize; i++) {
				free(data[i]);
		    }
			free(data);
			free(s);
			return;
		}
		sprintf(data[index],"%s",s);
		index++;
	}

	if(index!=(blockSize*blockSize)){/*if number of values(for the board) read are too few or many*/
		valNum(blockSize);/*Appropriate error*/
		fclose(fPtr);
		for (i = 0; i < blockSize*blockSize; i++) {
			free(data[i]);
	    }
		free(data);
		free(s);
		return;
	}

	/*else, got the right number of values we needed and they are valid values(not necessary legal)*/
	index=0;/*reuse it since we now know the data array length is blockSize*blockSize*/
	Board=(Cell**) malloc(sizeof(Cell*)*(blockSize));/*allocate memory for the board*/
	if(Board==NULL)/*malloc allocation failed*/
		 return mallocError(status);
	for (i = 0; i < blockSize; i++) {
		 Board[i] = (Cell *) malloc(sizeof(Cell) * blockSize);
		 if(Board[i]==NULL)/*malloc allocation failed*/
		 	 return mallocError(status);
	 }
	game=(GameBoard*) malloc(sizeof(GameBoard));/*allocate memory for the new GameBoard and init values*/
	if(game==NULL)/*malloc allocation failed*/
		 return mallocError(status);
	game->BlockHeight=height;
	game->BlockWidth=width;
	game->BoardSize=blockSize;
	game->erroneous=0;
	game->Board=Board;
	/*place the cell values in the game Board cells*/
	for(i=0;i<blockSize;i++){
		for(j=0;j<blockSize;j++){
			placeCell(game->Board,i,j, data[index],isEdit);
			index++;
		}
	}

	if(isEdit==0){/*check if fixed cells are legal only if not in the function edit*/
		if(checkFixedLegal(game)==0){/*check if all fixed values are legal, if not handles error printing inside method and free memory*/
			for (i = 0; i < blockSize; i++) {
				 free(Board[i]);
		    }
		    free(Board);
		    free(game);
		    fclose(fPtr);
		    for (i = 0; i < blockSize*blockSize; i++) {
		    	free(data[i]);
		    }
		    free(data);
		    free(s);
		    return;
	    }
	}

	/*now make the new game board and other needed changes*/
	/*first free previous allocated memory*/
	for (i = 0; i < gameboard->BoardSize; i++) {
			free(gameboard->Board[i]);
	}
	/*new set the new board*/
	gameboard->BlockHeight=game->BlockHeight;
	gameboard->BlockWidth=game->BlockWidth;
	gameboard->BoardSize=game->BoardSize;
	gameboard->erroneous=0;
	gameboard->Board=(Cell**) realloc(gameboard->Board,sizeof(Cell*)*(blockSize));/*reallocate memory for the board*/
	if(gameboard->Board==NULL)/*malloc allocation failed*/
		 return mallocError(status);
	for (i = 0; i < gameboard->BoardSize; i++) {
		gameboard->Board[i] = (Cell *) malloc(sizeof(Cell) * blockSize);
		if(gameboard->Board[i]==NULL)/*malloc allocation failed*/
				 return mallocError(status);
	}
	for(i=0; i<gameboard->BoardSize;i++){/*assign Cells data*/
		for(j=0; j<gameboard->BoardSize;j++){
			gameboard->Board[i][j].value=game->Board[i][j].value;
			gameboard->Board[i][j].sym=game->Board[i][j].sym;
		}
	}

    /*clean the dolist*/
    clean(dolist);

	updateBoardErrors(gameboard);
	if(isEdit==0){
		*mode=SOLVE_M;
		print_board(gameboard,*mode,MarkE);
	}
	fclose(fPtr);

	/*check if the function solve the board*/
	if(*mode == SOLVE_M){
		isBoardSolvedSuccessfully(gameboard, mode, status);/*changes the status to SOLVED_S if it does*/
	}

	/*free all memory assigned in function*/
	for (i = 0; i < blockSize*blockSize; i++) {
		free(data[i]);
    }
	free(data);
	free(s);
	for(i = 0; i < blockSize; i++){
		free(Board[i]);
	}
	free(Board);
	free(game);
}

/*checks whatever s is a natural number*/
int cheack_natural(char *s){
	int num;/*number part*/
	char *str;/*string part*/
	char *snum=(char*)malloc(sizeof(char)*2);
	if(snum==NULL)
		return -1;
	num=strtol(s,&str,10);/*seperate num and string part from s*/
	sprintf(snum,"%d",num);/*get the number part as string*/
	if(strcmp(s,snum)!=0 || num<1){/*means s is not a Natural number*/
		free(snum);
		return 0;
	}
	free(snum);
	return 1;/*if we got here s is a string of a natural number*/
}

/*checks whatever s is a valid cell data*/
int cheack_valid(char *s,int blockSize){
	int num;/*number part*/
	char *str;/*string part*/
	num=strtol(s,&str,10);/*seperate num and string part from s*/
	if(num<0 || num>blockSize)/*if num part is not in the correct range then invalid*/
		return 0;
	if(strcmp(str,"")!=0 && strcmp(str,".")!=0)/*if string part is not empty string or . then invalid*/
		return 0;
	if(num==0 && strcmp(str,".")==0)/*0 can't be fixed*/
		return 0;
	return 1;
}

/*places cell data in cell [col,row]*/
void placeCell(Cell** Board,int row,int col,char *data,int isEdit){
	int num;/*number part*/
	char *str;/*string part*/
	num=strtol(data,&str,10);/*seperate num and string part from data*/
	Board[row][col].value=num;
	if(strcmp(str,".")==0 && isEdit==0)/*if fixed and not in the function edit mark as such, if in the function edit don't mark since edit mode doesn't mark fixed cells*/
		Board[row][col].sym='.';
	else
		Board[row][col].sym=' ';
}

int checkFixedLegal(GameBoard *game){/*check if all fixed cells are legal*/
	int i;
	int j;

	for(i=0; i<game->BoardSize;i++){/*for every row*/
		for(j=0; j<game->BoardSize;j++){/*for every col*/
			if(game->Board[i][j].sym=='.'){/*if a fixed cell check whatever the cell value is legal*/
				if(isLegalVal(i,j,game->Board[i][j].value,game)==0){
					printf("Fixed cell [%d,%d] is illegal(only showing first illegal fixed cell",i+1,j+1);
					return 0;
				}
			}
		}
	}
	return 1;/*if we got her all fixed cells values are legal so return 1*/
}

/*load a board to edit mode, if isX=0 we didn't get a path, else we did */
void edit(GameBoard *gameboard,Mode *mode, char *x, int isX, int MarkE,DoublyLinkedList *dolist, GameStatus* status){

	if(isX==0){/*means the function works without path to file so make 9X9 empty board */
		make9X9Board(gameboard,status);
		if(*status==EXIT_S){/*malloc failure in function make9X9Board*/
			return;
		}
		*mode=EDIT_M;
		print_board(gameboard,*mode,MarkE);
		clean(dolist);
		return;
	}
	/*else we got a path so work with it so just use solve with isEdit=1 for edit special cases*/
	solve(gameboard,mode,x,MarkE,1,dolist, status);
	*mode=EDIT_M;
	print_board(gameboard,*mode,MarkE);
}

/*init to an empty 9X9 board*/
void make9X9Board(GameBoard *gameboard,GameStatus* status){
	int i;
	int j;
	for (i = 0; i < gameboard->BoardSize; i++) {
			 free(gameboard->Board[i]);
	 }

    gameboard->BlockHeight=3;
   	gameboard->BlockWidth=3;
    gameboard->BoardSize=9;
    gameboard->erroneous=0;
    gameboard->Board=(Cell**) realloc(gameboard->Board,sizeof(Cell*)*9);/*allocate memory for the board*/
    if(gameboard->Board==NULL)/*malloc failure*/
    		return mallocError(status);

	for (i = 0; i < 9; i++) {
		gameboard->Board[i] = (Cell *) malloc(sizeof(Cell) * 9);
		if(gameboard->Board[i]==NULL)/*malloc failure*/
		    	return mallocError(status);
	 }

	/*make an empty board*/
	for(i=0;i<9;i++){
		for(j=0;j<9;j++){
			gameboard->Board[i][j].value=0;
			gameboard->Board[i][j].sym=' ';
		}
	}
}


/* set a given value to the given cell if the cell is not fixed (0 means the cell is cleared) */
void set(GameBoard *gameboard, int row, int col, int val, Mode mode, int MarkE, GameStatus* status, DoublyLinkedList *dolist){
	col = col - 1; /*column given by the user is 1 based so decrease by 1*/
	row = row - 1;/*row given by the user is 1 based so decrease by 1*/

	if(mode == SOLVE_M){ /*can't set a value to a fixed cell in solve mode*/
		if(gameboard->Board[row][col].sym == '.'){
			setInFixedCellError();
			return;
		}
	}
	if(val == 0){ /*clear the cell*/
		addAfterCurrentWithCellChange(dolist, row, col, gameboard->Board[row][col].value, val, gameboard->Board[row][col].sym, ' ');
		gameboard->Board[row][col].value = 0;
		gameboard->Board[row][col].sym = ' ';
		updateBoardErrors(gameboard);
	}
	else{
		addAfterCurrentWithCellChange(dolist, row, col, gameboard->Board[row][col].value, val, gameboard->Board[row][col].sym, gameboard->Board[row][col].sym); /*add the change to the dolist which uses for undo and redo commands*/
		gameboard->Board[row][col].value = val;
		updateBoardErrors(gameboard);
		}
	print_board(gameboard,mode,MarkE);
	/*check if the set solve the board*/
	if(mode == SOLVE_M){
		isBoardSolvedSuccessfully(gameboard, &mode, status);/*changes the status to SOLVED_S if it does*/
	}
	return;
}

/* exit the game*/
GameStatus exitFunc(GameStatus status){
	printf("Exiting game\n");
	return EXIT_S; /*to terminate the program from main*/
}

/* check if the given board has a solution using ILP, if so return 1, else 0 */
int validateBoard(GameBoard *gameboard){
	int hasSolution;
	GameBoard* newboard;
	newboard = (GameBoard*) malloc(sizeof(GameBoard));
	if(newboard==NULL){
		return -2;
	}
	initGameBoard(newboard,gameboard);/*init newboard values from gameboard values(not including the board)*/
	duplicate_board(newboard,gameboard);/* allocate and copy the board matrix of the game*/
	hasSolution = ILP(newboard);
	freeGameBoard(newboard);
	return hasSolution;
}

/* check and informs the user if the given board has a solution, return 1 if so and 0 if not. isSave=1 means we called from save */
int validate(GameBoard *gameboard,GameStatus* status, int isSave){
	int hasSolution;
	if(gameboard->erroneous && isSave==0){
			erroneousBoardInValidate();
			return 0;
	}
	hasSolution = validateBoard(gameboard);
	if(hasSolution == -2){/*malloc failure in function validateBoard*/
		mallocError(status);
		return -1;
	}
	if(hasSolution == 1 && isSave==0){
		printf("The board is solvable.\n");
		return 1;
	}
	else if(hasSolution == 0 && isSave==0){
		printf("The board is not solvable.\n");
		return 0;
	}
	else if(hasSolution == 1 && isSave==1){
			return 1;
	}
	return 0;
}

/* calculate and informs the number of solutions the given board have, using exhaustive backtracking algorithm */
void num_solutions(GameBoard *gameboard){
	int solutionsNum;
	if(gameboard->erroneous){
		erroneousBoardNumSolutions();
		return;
	}
	solutionsNum = 0;
	solutionsNum = numberOfSolutions(gameboard);
	printf("The number of solutions for the current board is %d.\n", solutionsNum);
	return;
}

/* return the number of empty cells in game board */
int number_of_empty_cells_in_board(GameBoard *gameboard){
	int i, j, counter = 0;
	for (i=0; i<gameboard->BoardSize; i+=1){
		for(j=0; j<gameboard->BoardSize; j+=1){
			if(gameboard->Board[i][j].value == 0){
				counter += 1;
			}
		}
	}
	return counter;
}

/* try generate board using ILP, returns 1 for success, 0 for failure */
int generate_board(GameBoard* gameboard, int x, int y){
	int i, j;
	for(i=0; i<x; i++){
		if(!fill_random_cell(gameboard)){ /* failed filling an empty cell */
			return 0;
		}
	}
	if (ILP(gameboard) != 1){
		return 0;
	}
	for(i=0; i<gameboard->BoardSize; i++){
		for(j=0; j<gameboard->BoardSize; j++){
			gameboard->Board[i][j].sym = ' ';
		}
	}
	/* make sure fix_random_cell will not run forever */
	if (y > gameboard->BoardSize*gameboard->BoardSize){
		return 0;
	}
	for(i=0; i<y; i++){
		fix_random_cell(gameboard);
	}
	for(i=0; i<gameboard->BoardSize; i++){
		for(j=0; j<gameboard->BoardSize; j++){
			if(gameboard->Board[i][j].sym != '.'){
				gameboard->Board[i][j].value = 0;
			}
		}
	}
	for(i=0; i<gameboard->BoardSize; i++){
		for(j=0; j<gameboard->BoardSize; j++){
				gameboard->Board[i][j].sym = ' ';
		}
	}
	return 1;
}

/* change random cell in the gameboard to be fixed */
void fix_random_cell(GameBoard* gameboard){
	int row, col;
	row = rand()%gameboard->BoardSize;
	col = rand()%gameboard->BoardSize;
	while(gameboard->Board[row][col].sym == '.') {
		row = rand()%gameboard->BoardSize;
		col = rand()%gameboard->BoardSize;
	}
	gameboard->Board[row][col].sym = '.';
	return;
}

/* randomly finds an empty cell in the board and fill it with random legal value
 * if no legal value found = return 0, else return 1 */
int fill_random_cell(GameBoard* gameboard){
	int row, col, val;
	int i, count;
	count=0;
	/* Randomizing row,col until the spot is empty*/
	row = rand()%gameboard->BoardSize;
	col = rand()%gameboard->BoardSize;
	while(gameboard->Board[row][col].value != 0) {
		row = rand()%gameboard->BoardSize;
		col = rand()%gameboard->BoardSize;
	}

	for(i =0; i<gameboard->BoardSize; i++){
		if (isLegalVal(row, col, i, gameboard)){
			count++;
		}
	}
	/* the empty chosen cell has no valid value, the board is corrupted */
	if (count == 0){
		return 0;
	}
	/* Randomizing number until it is valid */
	val = rand()%gameboard->BoardSize;
	while(!isLegalVal(row, col, val, gameboard)){
		val = rand()%gameboard->BoardSize;
	}
	gameboard->Board[row][col].value = val;
	return 1;
}



/* Generate board by randomly filling x empty cells with legal values,
 * running ILP to solve, and clear all but Y cells */
void generate(GameBoard* gameboard, int x, int y,Mode mode, int MarkE, DoublyLinkedList *dolist,GameStatus* status){
	time_t t;
	int i, generate_board_result;
	GameBoard* newboard;
	newboard = (GameBoard*) malloc(sizeof(GameBoard));
	if(newboard==NULL){
		return mallocError(status);
	}
	initGameBoard(newboard,gameboard);/*init newboard values from gameboard values(not including the board)*/

	/* Initializes random number generator */
	srand((unsigned) time(&t));
	if (x < 0 || y <= 0 || y > gameboard->BoardSize*gameboard->BoardSize){
		printf("Error: Invalid argument for generate\n");
		free(newboard);
		return;
	}
	if (number_of_empty_cells_in_board(gameboard) < x){
		printf("Error: The board doesn't contain enough empty cells\n");
		free(newboard);
		return;
	}
	/* trying the algorithm 1000 times as described in generate command */
	for(i=0; i<1000; i++){
		duplicate_board(newboard,gameboard);/* allocate and copy the board matrix of the game*/
		generate_board_result = generate_board(newboard, x, y);
		if(generate_board_result){
			break;
		}
		freeCellMatrix(newboard);
	}
	if(!generate_board_result){
		printf("Error in the puzzle generator\n");
		free(newboard);
		return;
	}
	/* placing the changes at the dolist */
	addAfterCurrentWithBoardsDifferenece(dolist, gameboard, newboard);
	duplicate_board_values(gameboard,newboard);
	print_board(gameboard,mode,MarkE);
	freeGameBoard(newboard);
	return;
}

/* Guess a solution to the current board using LP with threshold x*/
void guess(GameBoard* gameboard, float x,Mode mode, int MarkE, DoublyLinkedList *dolist, GameStatus* status){
	int lp_result = -1;
	GameBoard* newboard;
	newboard = (GameBoard*) malloc(sizeof(GameBoard));
	if(newboard==NULL){
		return mallocError(status);
	}
	initGameBoard(newboard,gameboard);/*init newboard values from gameboard values(not including the board)*/
	if(gameboard->erroneous){
		printf("Can't guess using LP for erroneous board\n");
		free(newboard);
		return;
	}
	duplicate_board(newboard,gameboard);/* allocate and copy the board matrix of the game*/
	/* threshold = x , x,y are not used in this scenario, isGuessBoard = 1 */
	lp_result = LP(newboard, x, 0, 0, 1);
	if (lp_result != 1){
		printf("Failed running LP\n");
		if(lp_result == 0){
			printf("The board is unsolvable\n");
		}
		freeGameBoard(newboard);
		return;
	}
	/* placing the changes at the dolist */
	addAfterCurrentWithBoardsDifferenece(dolist, gameboard, newboard);
	duplicate_board_values(gameboard,newboard);
	print_board(gameboard,mode,MarkE);
	/*check if the function solve the board*/
	if(mode == SOLVE_M){
		isBoardSolvedSuccessfully(gameboard, &mode, status);/*changes the status to SOLVED_S if it does*/
	}
	freeGameBoard(newboard);
	return;
}

/* Give a hint to the user by showing the solution of single cell [x,y] */
void hint(GameBoard* gameboard, int x, int y,GameStatus* status){
	int ilp_result = -1;
	GameBoard* newboard;
	newboard = (GameBoard*) malloc(sizeof(GameBoard));
	if(newboard==NULL){
			return mallocError(status);
	}
	initGameBoard(newboard,gameboard);/*init newboard values from gameboard values(not including the board)*/

	if(gameboard->erroneous){
		printf("Can't generate hint for erroneous board\n");
		free(newboard);
		return;
	}
	if(gameboard->Board[x-1][y-1].sym == '*' || gameboard->Board[x-1][y-1].value != 0){
		printf("Can't generate hint, the spot already taken\n");
		free(newboard);
		return;
	}
	duplicate_board(newboard,gameboard);/* allocate and copy the board matrix of the game*/
	ilp_result = ILP(newboard);
	if (ilp_result != 1){
		printf("Failed running ILP\n");
		if(ilp_result == 0){
			printf("The board is unsolvable\n");
		}
		freeGameBoard(newboard);
		return;
	}
	printf("Hint: set cell [%d,%d] to %d\n",y,x, newboard->Board[x-1][y-1].value);
	freeGameBoard(newboard);
	return;
}

/* Show a guess to the user for a single cell x,y */
void guess_hint(GameBoard* gameboard, int x, int y){
	int lp_result = -1;

	if(gameboard->erroneous){
		printf("Can't guess a hint for erroneous board\n");
		return;
	}
	if(gameboard->Board[x-1][y-1].sym == '*' || gameboard->Board[x-1][y-1].value != 0){
		printf("Can't guess a hint, the spot already taken\n");
		return;
	}
	/*	threshold =0 , is not used in this scenario, isGuessBoard = 0 */
	lp_result = LP(gameboard, 0, x, y, 0);
	if (lp_result != 1){
		printf("Failed running LP\n");
		if(lp_result == 0){
			printf("The board is unsolvable\n");
		}
		return;
	}
	return;
}





