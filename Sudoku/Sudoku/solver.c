
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "solver.h"
#include "gurobi_c.h"
#include "aux_main.h"


int gurobi_initializer() {

	GRBenv   *env;
	GRBmodel *model;
	int       error;
	double    *sol;
	int       *ind;
	double    *val;
	double    obj[3];
	char      vtype[3];
	int       optimstatus;
	double    objval;
	int DIM;


	DIM = sudoku.block_col_length * sudoku.block_row_length;

	ind = malloc(sizeof(int)*DIM);
	val = malloc(sizeof(int)*DIM);
	double    lb[DIM*DIM*DIM];
	char      vtype[DIM*DIM*DIM];
	char     *names[DIM*DIM*DIM];


	env = NULL;
	model = NULL;
	error = 0;

	



	/* Create environment - log file is mip1.log */
	error = GRBloadenv(&env, "mip1.log");
	if (error) {
		printf("ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(env));
		return EXIT_FAILURE;
	}

	/* Create an empty model named "mip1" */
	error = GRBnewmodel(env, &model, "mip1", 0, NULL, NULL, NULL, NULL, NULL);
	if (error) {
		printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
		return -1;
	}


}


int ILP_solver() {


	GRBenv   *env = NULL;
	GRBmodel *model = NULL;
	int       error = 0;
	double    sol[3];
	int       ind[3];
	double    val[3];
	double    obj[3];
	char      vtype[3];
	int       optimstatus;
	double    objval;


	/* Create environment - log file is mip1.log */
	error = GRBloadenv(&env, "mip1.log");
	if (error) {
		printf("ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(env));
		return -1;
	}
}


