/*
 * LP.c
 * Gurobi.c
 * This functions takes care of the ILP AND LP functions using gurobi functions and variables.
 * ILP define the minimal needed gurobi variables as binary and constraints, then finds the solution to the board with the constraints
 * and takes only cells where we get 1 to put in the board.
 * LP define the minimal needed gurobi variables as continuous double numbers between 0 and 1 and constraints + random objective function
 * with those variables, then finds the solution to the board with the constraints. For guess is fills cells with the highest possibility.
 * for Guess hint showes all possible values for cell with possibility higher then x.
 * Constraints are done in functions called from LP/ILP.
 */
#include "Gurobi.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "gurobi_c.h"
#include "MainAux.h"
#include "exhaustiveBacktracking.h"

/*Use ILP to solve the boards. If there is an error, return -1. If there is no solution return 0. If all worked well return 1*/
int ILP(GameBoard *gameboard){
	GRBenv   *env;
	GRBmodel *model;
	int error;
	double *sol;
	int *ind;
	double *val;
	char *vtype;
    double *obj;
	int optimstatus;
    int i,j,k,t;
    int vtypeLen;
    int *sudokuVar;

    int blockSize=gameboard->BoardSize;

    env=NULL;
    model=NULL;
    error=0;
    vtypeLen=0;

    /*allocate needed memory*/
    ind=(int*)malloc(sizeof(int)*blockSize);
    val=(double*)malloc(sizeof(double)*blockSize);
    vtype=(char*)malloc(sizeof(char)*blockSize*blockSize*blockSize);
    sudokuVar=(int*)malloc(sizeof(int)*blockSize*blockSize*blockSize);


	/* Create environment - log file is mip1.log */
    error = GRBloadenv(&env, "mip1.log");
	if (error) {
	     printf("ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(env));
	     free(ind);free(val);free(vtype);free(sudokuVar);
	     return -1;
    }
	error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
	if (error) {
	     printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
	     free(ind);free(val);free(vtype);free(sudokuVar);
		 return -1;
	}

    /* Create an empty model named "mip1" */
     error = GRBnewmodel(env, &model, "mip1", 0, NULL, NULL, NULL, NULL, NULL);
	 if (error) {
		  printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
		  free(ind);free(val);free(vtype);free(sudokuVar);
		  return -1;
	 }

	 /*add variables*/
	 for(i=0;i<blockSize;i++){/*for every row*/
	 	for(j=0;j<blockSize;j++){/*for every col*/
	 	    for(k=1;k<=blockSize;k++){/*for every possible value*/
	 			 if(gameboard->Board[i][j].value==k){/*if the cell is already occupied by value k don't add variable*/
	 				sudokuVar[i*blockSize*blockSize+j*blockSize+(k-1)]=-1;/*mark it as not a gurobi variable*/
	 			 }
	 			 else if(gameboard->Board[i][j].value!=0){/*if the cell is occupied by a value different then k don't add variable*/
	 				sudokuVar[i*blockSize*blockSize+j*blockSize+(k-1)]=-1;/*mark it is not a gurobi variable*/
	 			 }
	 			 else if(isLegalVal(i,j,k,gameboard)==1){/*the cell is empty and k a legal value*/
	 				 vtype[vtypeLen]=GRB_BINARY;
	 				 sudokuVar[i*blockSize*blockSize+j*blockSize+(k-1)]=vtypeLen;
	 				 vtypeLen++;
	 			 }
	 			 else{/*k not a legal value don't add variable*/
	 				sudokuVar[i*blockSize*blockSize+j*blockSize+(k-1)]=-1;/*mark it is not a gurobi variable*/
	 			 }
	 		 }
	 	 }
	 }

	 sol=(double*)malloc(sizeof(double)*vtypeLen);/*allocate memory for solutions*/
	 obj=(double*)malloc(sizeof(double)*vtypeLen);

	 for(t=0;t<vtypeLen;t++){
	 		 obj[t]=1;/*add objective coefficients 1*/
	 }


	 error=GRBaddvars(model,vtypeLen,0,NULL,NULL,NULL,obj,NULL,NULL,vtype,NULL);
	 if (error) {
		printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
		free(ind);free(val);free(vtype);free(sudokuVar);free(sol);free(obj);GRBfreemodel(model);GRBfreeenv(env);
	    return -1;
	 }

	 /* Change objective sense to maximization */
	 error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
	 if (error) {
		printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
		free(ind);free(val);free(vtype);free(sudokuVar);free(sol);free(obj);GRBfreemodel(model);GRBfreeenv(env);
	    return -1;
	 }

	 /* update the model - to integrate new variables */
	 error = GRBupdatemodel(model);
	 if (error) {
	    printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
	    free(ind);free(val);free(vtype);free(sudokuVar);free(sol);free(obj);GRBfreemodel(model);GRBfreeenv(env);
	    return -1;
	 }

	  /*add constraints on each cell*/
	  i=cellCons(env,model,ind,val,sudokuVar,gameboard,blockSize);
	  if(i==-1){/*means an error accord*/
		  free(ind);free(val);free(vtype);free(sudokuVar);free(sol);free(obj);GRBfreemodel(model);GRBfreeenv(env);
		  return -1;
	  }
	  else if(i==0){/*means board is unsolvable*/
		  free(ind);free(val);free(vtype);free(sudokuVar);free(sol);free(obj);GRBfreemodel(model);GRBfreeenv(env);
		  return 0;
	  }

	   /*add constraints on each value for each row*/
	  if(rowCons(env,model,ind,val,sudokuVar,gameboard,blockSize)==-1){
		  free(ind);free(val);free(vtype);free(sudokuVar);free(sol);free(obj);GRBfreemodel(model);GRBfreeenv(env);
	 	  return -1;
	  }

	   /*add constraints on each value for each col*/
	  if(colCons(env,model,ind,val,sudokuVar,gameboard,blockSize)==-1){
		  free(ind);free(val);free(vtype);free(sudokuVar);free(sol);free(obj);GRBfreemodel(model);GRBfreeenv(env);
	  	  return -1;
	  }

	  /*add constraints on each value for each block*/
	  if(blockCons(env,model,ind,val,sudokuVar,gameboard,blockSize)==-1){
		  free(ind);free(val);free(vtype);free(sudokuVar);free(sol);free(obj);GRBfreemodel(model);GRBfreeenv(env);
	 	  return -1;
	  }

	  /* Optimize model - need to call this before calculation */
	  error = GRBoptimize(model);
	  if (error) {
	  	  printf("ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
	      free(ind);free(val);free(vtype);free(sudokuVar);free(sol);free(obj);GRBfreemodel(model);GRBfreeenv(env);
	  	  return -1;
	  }

	  /* Write model to 'mip1.lp'*/
	  error = GRBwrite(model, "mip1.lp");
	  if (error) {
	  	  printf("ERROR %d GRBwrite(): %s\n", error, GRBgeterrormsg(env));
	      free(ind);free(val);free(vtype);free(sudokuVar);free(sol);free(obj);GRBfreemodel(model);GRBfreeenv(env);
	  	  return -1;
	  }

	  /* Get solution information */
	  error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	  if (error) {
	  	  printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
	      free(ind);free(val);free(vtype);free(sudokuVar);free(sol);free(obj);GRBfreemodel(model);GRBfreeenv(env);
	  	  return -1;
	  }

	  if(optimstatus!=GRB_OPTIMAL){/*board is unsolvable*/
		  printf("%d",optimstatus);
		  free(ind);free(val);free(vtype);free(sudokuVar);free(sol);free(obj);GRBfreemodel(model);GRBfreeenv(env);
		  return 0;
	  }

	  /* get the solution - the assignment to each variable */
	  error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, vtypeLen, sol);
	  if (error) {
	  	  printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
	  	  free(ind);free(val);free(vtype);free(sudokuVar);free(sol);free(obj);GRBfreemodel(model);GRBfreeenv(env);
	  	  return -1;
	  }

	  /*assign the needed values to the board*/
	  for(i=0;i<blockSize;i++){
		  for(j=0;j<blockSize;j++){
			  /*for every cell*/
			  if(gameboard->Board[i][j].value==0){ /*only if the cell is empty*/
				  for(k=1;k<=blockSize;k++){
					  if(sudokuVar[i*blockSize*blockSize+j*blockSize+(k-1)]!=-1){
						  if(sol[sudokuVar[i*blockSize*blockSize+j*blockSize+(k-1)]]==1)
							  gameboard->Board[i][j].value=k;
					  }
				  }
			  }
		  }
	  }

	  /*free all used memory*/
	  free(ind);
	  free(val);
	  free(vtype);
	  free(sudokuVar);
	  free(sol);
	  free(obj);
	  GRBfreemodel(model);
	  GRBfreeenv(env);
	  return 1;
}


int cellCons(GRBenv *env,GRBmodel *model,int *ind,double *val,int *sudokuVar,GameBoard *gameboard,int blockSize){ /*add constraints on each cell*/
	int i,j,k,t;
	int error=0;

    for(i=0;i<blockSize;i++){/*for every row*/
		 for(j=0;j<blockSize;j++){/*for every col*/
			 /*for every cell*/
			 t=0;
			 if(gameboard->Board[i][j].value==0){
				 for(k=1;k<=blockSize;k++){/*for every possible value*/
					 if(sudokuVar[i*blockSize*blockSize+j*blockSize+(k-1)]!=-1){/*means cell is empty and k is a legal value*/
						 ind[t]=sudokuVar[i*blockSize*blockSize+j*blockSize+(k-1)];
						 val[t]=1;
						 t++;
					 }
			 	 }
				 if(t==0)/*means there are no legal values for the empty cell so return that the board is unsolvable*/
					 return 0;
				 error = GRBaddconstr(model, t, ind, val, GRB_EQUAL, 1.0, NULL);/*constraint to one possible value for cell(for legal vales only and only if cell isn't empty*/
				 if (error) {
				 	printf("ERROR %d 1st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
				 	return -1;
				 }
			 }
		  }
	 }
	 return 1;
}

int rowCons(GRBenv *env,GRBmodel *model,int *ind,double *val,int *sudokuVar,GameBoard *gameboard,int blockSize){ /*add constraints on each value for each row*/
	int i,j,k,t;
	int add;
	int error=0;

	 for(k=1;k<=blockSize;k++){/*for every possible value*/
		for(i=0;i<blockSize;i++){/*for every row*/
			add=1;
			t=0;
		    for(j=0;j<blockSize;j++){/*for every col*/
		    	if(gameboard->Board[i][j].value==k){/*means there is already value k in the row*/
		    		add=0;
		    	}
			}
		    if(add==1){/*means there is no value k in the row*/
		    	for(j=0;j<blockSize;j++){/*for every col*/
		    		 if(sudokuVar[i*blockSize*blockSize+j*blockSize+(k-1)]!=-1){/*means cell is empty and k is a legal value*/
		    			 ind[t]=sudokuVar[i*blockSize*blockSize+j*blockSize+(k-1)];
		    			 val[t]=1;
		    			 t++;
		    		 }
		       }
			   error = GRBaddconstr(model, t, ind, val, GRB_EQUAL, 1.0, NULL);/*constraint to value once in a row*/
		       if (error) {
			      printf("ERROR %d 1st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
			  	  return -1;
			   }
		    }
		}
	 }
	 return 1;
}

int colCons(GRBenv *env,GRBmodel *model,int *ind,double *val,int *sudokuVar,GameBoard *gameboard,int blockSize){ /*add constraints on each value for each row*/
	int i,j,k;
	int add;
	int t;
	int error=0;

	for(k=1;k<=blockSize;k++){/*for every possible value*/
	     for(j=0;j<blockSize;j++){/*for every col*/
			  add=1;
			  t=0;
		  	  for(i=0;i<blockSize;i++){/*for every row*/
		  		if(gameboard->Board[i][j].value==k){/*means there is already value k in the col*/
		  			add=0;
		  		}
		  	  }
		  	  if(add==1){/*means there is no value k in the row*/
		  		  for(i=0;i<blockSize;i++){/*for every col*/
		  			if(sudokuVar[i*blockSize*blockSize+j*blockSize+(k-1)]!=-1){/*means cell is empty and k is a legal value*/
		  			   ind[t]=sudokuVar[i*blockSize*blockSize+j*blockSize+(k-1)];
		  			   val[t]=1;
		  			   t++;
		  			}
		  		  }
		  		  error = GRBaddconstr(model, t, ind, val, GRB_EQUAL, 1.0, NULL);/*constraint to value once in a col*/
		  		  if (error) {
		  			  printf("ERROR %d 1st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
		  			  return -1;
		  		  }
		  	 }
		 }
	 }
	 return 1;
}

int blockCons(GRBenv *env,GRBmodel *model,int *ind,double *val,int *sudokuVar,GameBoard *gameboard,int blockSize){ /*add constraints on each value for each row*/
	int i,j,k,t,m;
	int count;
	int add;
	int error=0;
	count=0;

	  for(k=1;k<=blockSize;k++){/*for every possible value*/
		  for(i=0;i<gameboard->BlockHeight;i++){
			  for(j=0;j<gameboard->BlockWidth;j++){
				  /*for every block*/
				  count=0;
				  add=1;
				  for(t=i*gameboard->BlockWidth;t<(i+1)*gameboard->BlockWidth;t++){
					  for(m=j*gameboard->BlockHeight;m<(j+1)*gameboard->BlockHeight;m++){
						  if(gameboard->Board[t][m].value==k)
							  add=0;
					  }
				  }
				  if(add==1){
					  for(t=i*gameboard->BlockWidth;t<(i+1)*gameboard->BlockWidth;t++){
					  	  for(m=j*gameboard->BlockHeight;m<(j+1)*gameboard->BlockHeight;m++){
					  		  if(sudokuVar[t*blockSize*blockSize+m*blockSize+(k-1)]!=-1){
					  			  ind[count]=sudokuVar[t*blockSize*blockSize+m*blockSize+(k-1)];
					  			  val[count]=1;
					  			  count++;
					  		  }
					  	  }
					  }
					  error = GRBaddconstr(model, count, ind, val, GRB_EQUAL, 1.0, NULL);/*constraint to value once in a block*/
					  if (error) {
					  	 printf("ERROR %d 1st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
					  	  return -1;
					  }
				  }
			  }
		  }
	 }
	 return 1;
}
/*use LP to solve the board, x is only relevent in guess, row,col only relevent in guess_hint
 * isGuessBoard=1 means we called from guess function.isGuessBoard=0 means we called from guess_hint function.
 * */
int LP(GameBoard *gameboard,float x,int row,int col, int isGuessBoard){
	GRBenv   *env;
	GRBmodel *model;
	int error;
	double *sol;
	int *ind;
	double *val;
	char *vtype;
	double *obj;
	double *lb;
	double *ub;
	int optimstatus;
	int i,j,k,t;
    int vtypeLen;
    int *sudokuVar;
    double score,maxScore;

	int blockSize=gameboard->BoardSize;

    env=NULL;
    model=NULL;
	error=0;
	vtypeLen=0;
    srand(time(0));

	/*allocate needed memory*/
    ind=(int*)malloc(sizeof(int)*blockSize);
	val=(double*)malloc(sizeof(double)*blockSize);
	vtype=(char*)malloc(sizeof(char)*blockSize*blockSize*blockSize);
	sudokuVar=(int*)malloc(sizeof(int)*blockSize*blockSize*blockSize);

    /* Create environment - log file is mip1.log */
     error = GRBloadenv(&env, "mip1.log");
     if (error) {
    	 printf("ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(env));
    	 free(ind);free(val);free(vtype);free(sudokuVar);
    	 return -1;
     }
     error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
     if (error) {
    	 printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
    	 free(ind);free(val);free(vtype);free(sudokuVar);
         return -1;
     }

     /* Create an empty model named "mip1" */
     error = GRBnewmodel(env, &model, "mip1", 0, NULL, NULL, NULL, NULL, NULL);
     if (error) {
    	printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
    	free(ind);free(val);free(vtype);free(sudokuVar);
    	return -1;
     }

     /*add variables*/
	 for(i=0;i<blockSize;i++){/*for every row*/
	 	for(j=0;j<blockSize;j++){/*for every col*/
	 	    for(k=1;k<=blockSize;k++){/*for every possible value*/
	 			 if(gameboard->Board[i][j].value==k){/*if the cell is already occupied by value k don't add variable*/
	 				sudokuVar[i*blockSize*blockSize+j*blockSize+(k-1)]=-1;/*mark it as not a gurobi variable*/
	 			 }
	 			 else if(gameboard->Board[i][j].value!=0){/*if the cell is occupied by a value different then k don't add variable*/
	 				sudokuVar[i*blockSize*blockSize+j*blockSize+(k-1)]=-1;/*mark it is not a gurobi variable*/
	 			 }
	 			 else if(isLegalVal(i,j,k,gameboard)==1){/*the cell is empty and k a legal value*/
	 				 vtype[vtypeLen]=GRB_CONTINUOUS;
	 				 sudokuVar[i*blockSize*blockSize+j*blockSize+(k-1)]=vtypeLen;
	 				 vtypeLen++;
	 			 }
	 			 else{/*k not a legal value don't add variable*/
	 				sudokuVar[i*blockSize*blockSize+j*blockSize+(k-1)]=-1;/*mark it is not a gurobi variable*/
	 			 }

	 		 }
	 	 }
	 }

	 sol=(double*)malloc(sizeof(double)*vtypeLen);/*allocate memory for solutions*/
	 obj=(double*)malloc(sizeof(double)*vtypeLen);/*allocate memory for objective coefficients*/
	 lb=(double*)malloc(sizeof(double)*vtypeLen);/*allocate memory for lower bounds*/
	 ub=(double*)malloc(sizeof(double)*vtypeLen);/*allocate memory for upper bounds*/

     /*for all gurobi variables*/
	 for(t=0;t<vtypeLen;t++){
		 obj[t]=rand()%(blockSize*blockSize*blockSize);
		 lb[t]=0;/*add lower bound*/
		 ub[t]=1;/*add upperbound*/
	 }

	 error=GRBaddvars(model,vtypeLen,0,NULL,NULL,NULL,obj,lb,ub,vtype,NULL);
	 if (error) {
		printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
		free(ind);free(val);free(vtype);free(sudokuVar);free(sol);free(obj);free(lb);free(ub);GRBfreemodel(model);GRBfreeenv(env);
		return -1;
	 }

	 /* Change objective sense to maximization */
	 error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
	 if (error) {
		printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
		free(ind);free(val);free(vtype);free(sudokuVar);free(sol);free(obj);free(lb);free(ub);GRBfreemodel(model);GRBfreeenv(env);
		return -1;
	 }

	 /* update the model - to integrate new variables */
	 error = GRBupdatemodel(model);
	 if (error) {
		 printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
		 free(ind);free(val);free(vtype);free(sudokuVar);free(sol);free(obj);free(lb);free(ub);GRBfreemodel(model);GRBfreeenv(env);
		 return -1;
	 }

	 i=cellCons(env,model,ind,val,sudokuVar,gameboard,blockSize);
	 if(i==-1){/*means an error accord*/
		free(ind);free(val);free(vtype);free(sudokuVar);free(sol);free(obj);free(lb);free(ub);GRBfreemodel(model);GRBfreeenv(env);
		return -1;
	 }
	 else if(i==0){/*means board is unsolvable*/
		 free(ind);free(val);free(vtype);free(sudokuVar);free(sol);free(obj);free(lb);free(ub);GRBfreemodel(model);GRBfreeenv(env);
		 return 0;
	 }

	 /*add constraints on each value for each row*/
	 if(rowCons(env,model,ind,val,sudokuVar,gameboard,blockSize)==-1){
		free(ind);free(val);free(vtype);free(sudokuVar);free(sol);free(obj);free(lb);free(ub);GRBfreemodel(model);GRBfreeenv(env);
		return -1;
	 }

	 /*add constraints on each value for each col*/
	 if(colCons(env,model,ind,val,sudokuVar,gameboard,blockSize)==-1){
		free(ind);free(val);free(vtype);free(sudokuVar);free(sol);free(obj);free(lb);free(ub);GRBfreemodel(model);GRBfreeenv(env);
		return -1;
	 }

	 /*add constraints on each value for each block*/
	 if(blockCons(env,model,ind,val,sudokuVar,gameboard,blockSize)==-1){
		free(ind);free(val);free(vtype);free(sudokuVar);free(sol);free(obj);free(lb);free(ub);GRBfreemodel(model);GRBfreeenv(env);
	    return -1;
	 }

	 /* Optimize model - need to call this before calculation */
	 error = GRBoptimize(model);
	 if (error) {
		printf("ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
		free(ind);free(val);free(vtype);free(sudokuVar);free(sol);free(obj);free(lb);free(ub);GRBfreemodel(model);GRBfreeenv(env);
		return -1;
	 }

     /* Write model to 'mip1.lp'*/
     error = GRBwrite(model, "mip1.lp");
     if (error) {
    	  printf("ERROR %d GRBwrite(): %s\n", error, GRBgeterrormsg(env));
    	  free(ind);free(val);free(vtype);free(sudokuVar);free(sol);free(obj);free(lb);free(ub);GRBfreemodel(model);GRBfreeenv(env);
    	  return -1;
     }

    /* Get solution information */
    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error) {
    	  printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
    	  free(ind);free(val);free(vtype);free(sudokuVar);free(sol);free(obj);free(lb);free(ub);GRBfreemodel(model);GRBfreeenv(env);
    	  return -1;
    }

    if(optimstatus!=GRB_OPTIMAL){/*board is unsolvable*/
    	  return 0;
     }

     /* get the solution - the assignment to each variable */
    error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, vtypeLen, sol);
    if (error) {
    	  printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
    	  free(ind);free(val);free(vtype);free(sudokuVar);free(sol);free(obj);free(lb);free(ub);GRBfreemodel(model);GRBfreeenv(env);
    	  return -1;
    }

    if(isGuessBoard==0){/*means we are in guess for cell command*/
    	printf("Possible values for Cell [%d,%d] of form [value,score]:\n",col,row);
    	for(k=1;k<=blockSize;k++){/*for every possible value*/
    		if(sudokuVar[(row-1)*blockSize*blockSize+(col-1)*blockSize+(k-1)]!=-1){/*if it is a gurobi variable*/
    			if(sol[sudokuVar[(row-1)*blockSize*blockSize+(col-1)*blockSize+(k-1)]]>0){/*if variable with score bigger then 0 print as a possible guess*/
    				printf("[%d,%f]\n",k,sol[sudokuVar[(row-1)*blockSize*blockSize+(col-1)*blockSize+(k-1)]]);
    			}
    		}
    	}

    }
    else{/*means we are in guess for board command*/
    	for(i=0;i<blockSize;i++){
    		for(j=0;j<blockSize;j++){
    			/*for every cell*/
    			maxScore=0;/*keep the max score for the cell in t*/
    			for(k=1;k<=blockSize;k++){/*for every possible value*/
    				if(sudokuVar[i*blockSize*blockSize+j*blockSize+(k-1)]!=-1){/*if it is a gurobi variable*/
    					score=sol[sudokuVar[i*blockSize*blockSize+j*blockSize+(k-1)]];
    					if(score>x && score>maxScore && isLegalVal(i,j,k,gameboard)==1){/*if the variable with score bigger then the threshold x and the max t, and value is legal in cell*/
    						gameboard->Board[i][j].value=k;
    						maxScore=score;
    					}
    				}
    			}
    		}
    	}
    }
    /*free all used memory*/
    free(ind);
    free(val);
    free(vtype);
    free(sudokuVar);
    free(sol);
    free(obj);
    free(lb);
    free(ub);
    GRBfreemodel(model);
    GRBfreeenv(env);
    return 1;
}







