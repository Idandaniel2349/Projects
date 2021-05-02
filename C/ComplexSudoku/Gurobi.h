/*
 * Gurobi.h
 * This functions takes care of the ILP AND LP functions using gurobi functions and variables.
 * This header file only takes care of functions definitions.
 */

#ifndef GUROBI_H_
#define GUROBI_H_
#include "gurobi_c.h"
#include "MainAux.h"

int ILP(GameBoard *gameboard);
int cellCons(GRBenv *env,GRBmodel *model,int *ind,double *val,int *sudokuVar,GameBoard *gameboard,int blockSize);
int rowCons(GRBenv *env,GRBmodel *model,int *ind,double *val,int *sudokuVar,GameBoard *gameboard,int blockSize);
int colCons(GRBenv *env,GRBmodel *model,int *ind,double *val,int *sudokuVar,GameBoard *gameboard,int blockSize);
int blockCons(GRBenv *env,GRBmodel *model,int *ind,double *val,int *sudokuVar,GameBoard *gameboard,int blockSize);

int LP(GameBoard *gameboard,float x,int row,int col, int isGuessBoard);



#endif /* GUROBI_H_ */
