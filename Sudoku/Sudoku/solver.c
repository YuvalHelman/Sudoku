
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gurobi_c.h"
#include "solver.h"
#include "aux_main.h"
#include "game_logic.h"

/*
*	all Arrays are refferenced as 3-dimensions with [(i * DIM*DIM) + (j * DIM) + v] reffrences. 
*
*/



/* Delete this one if the other ones work. just copied it to different functions */
int gurobi_initializer() {

	GRBenv   *env;
	GRBmodel *model;
	int       error;
	double    *sol; /* An array that holds the solution for (i,j,k) tuples */
	int       *ind; /* used for refferencing the variables.*/
	double    *val; /* an Array of possible value ceoficients */
	double	  *lb; /* Lower bounds for the values */
	char      *vtype; /* Variable types : BINARY for all of them */
	int       optimstatus;
	int		  DIM; /* The board dimensions */
	int       i; /* rows */
	int		  j; /* Columns */
	int		  v, ig, jg, count;
	int b_col_l, b_row_l;

	b_col_l = sudoku.block_col_length;
	b_row_l = sudoku.block_row_length;
	DIM = sudoku.block_col_length * sudoku.block_row_length;

	ind = malloc(sizeof(int)*DIM);
	val = malloc(sizeof(double)*DIM);
	lb = malloc(sizeof(double)*DIM*DIM*DIM);
	vtype = malloc(sizeof(char)*DIM*DIM*DIM);
	sol = malloc(sizeof(double)*DIM*DIM*DIM);

	if (!ind || !val || !lb || !val || !vtype || !sol) {
		printf("Error: malloc has failed on ILP solver\n");
		exit(EXIT_FAILURE);
	}

	env = NULL;
	model = NULL;
	error = 0;

	/* Fill the lower bound to 1 for any cell that already has a value (non-zero) */
	for (i = 0; i < DIM; i++) {
		for (j = 0; j < DIM; j++) {
			/*TODO: Added the below row v to (v+1). check if its ok */
			for (v = 0; v < DIM; v++) { /* v = Fixed Value.*/
				if (sudoku.board[i][j].value == (v + 1))
					lb[i*DIM*DIM + j * DIM + v] = 1.0;
				else
					lb[i*DIM*DIM + j * DIM + v] = 0.0;

				vtype[i*DIM*DIM + j * DIM + v] = GRB_BINARY;

			}
		}
	}
	/* DEBUG
	fflush(stdin); fflush(stdout);
	printf("error printing 1\n");
	/* Debug */

	/* Create environment */
	error = GRBloadenv(&env, "sudokuGurobi.log"); /* TODO: change 2nd argument to NULL when no need for log anymore */
	if (error || env == NULL) {
		printf("Error: could not create environment.\n");
		printf("ERROR: %s\n", GRBgeterrormsg(env));
		return EXIT_FAILURE;
	}

	error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
	if (error) {
		printf("Error: gurobi removing output has failed.");
		printf("ERROR: %s\n", GRBgeterrormsg(env));
		return EXIT_FAILURE;
	}

	/* Create an empty model */
	error = GRBnewmodel(env, &model, "sudoku", DIM*DIM*DIM, NULL, lb, NULL,
		vtype, NULL);
	if (error) {
		perror("Error: gurobi new model failed");
		printf("ERROR: %s\n", GRBgeterrormsg(env));
		return EXIT_FAILURE;
	}

	/* Each cell gets only one value.
	a constraint is conducted on each cell to have only one value chosen.
	*/
	for (i = 0; i < DIM; i++) {
		for (j = 0; j < DIM; j++) {
			for (v = 0; v < DIM; v++) {
				ind[v] = (i * DIM*DIM) + (j * DIM) + v;
				val[v] = 1.0;
			}

			error = GRBaddconstr(model, DIM, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				printf("Error: GRBaddconstr failed.\n");
				printf("ERROR: %s\n", GRBgeterrormsg(env));
				return EXIT_FAILURE;
			}
		}
	}

	/* Each value must appear once in each row.
	a constraint is conducted on each value to be valid in only one row.
	*/
	for (v = 0; v < DIM; v++) {
		for (j = 0; j < DIM; j++) {
			for (i = 0; i < DIM; i++) {
				ind[i] = (i * DIM*DIM) + (j * DIM) + v;
				val[i] = 1.0;
			}

			error = GRBaddconstr(model, DIM, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				printf("Error: GRBaddconstr failed.\n");
				printf("ERROR: %s\n", GRBgeterrormsg(env));
				return EXIT_FAILURE;
			}
		}
	}

	/* Each value must appear once in each column
	a constraint is conducted on each value to be valid in only one column.
	*/
	for (v = 0; v < DIM; v++) {
		for (i = 0; i < DIM; i++) {
			for (j = 0; j < DIM; j++) {
				ind[j] = (i * DIM*DIM) + (j * DIM) + v;
				val[j] = 1.0;
			}

			error = GRBaddconstr(model, DIM, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				printf("Error: GRBaddconstr failed.\n");
				printf("ERROR: %s\n", GRBgeterrormsg(env));
				return EXIT_FAILURE;
			}
		}
	}

	/* Each value must appear once in each subgrid */
	for (v = 0; v < DIM; v++) {
		/* Iterate Subgrids */
		for (ig = 0; ig < b_row_l; ig++) {
			for (jg = 0; jg < b_col_l; jg++) {
				count = 0;
				/* Iterate the inside of the subgrid */
				for (i = ig * b_col_l; i < (ig + 1)*b_col_l; i++) {
					for (j = jg * b_row_l; j < (jg + 1)*b_row_l; j++) {
						ind[count] = (i * DIM*DIM) + (j * DIM) + v;
						val[count] = 1.0;
						count++;
					}
				}

				error = GRBaddconstr(model, DIM, ind, val, GRB_EQUAL, 1.0, NULL);
				if (error) {
					printf("Error: GRBaddconstr failed.\n");
					printf("ERROR: %s\n", GRBgeterrormsg(env));
					return EXIT_FAILURE;
				}
			}
		}
	}

	/* Optimize model for a solution */
	error = GRBoptimize(model);
	if (error) {
		printf("Error: GRBoptimize failed.\n");
		printf("ERROR: %s\n", GRBgeterrormsg(env));
		return EXIT_FAILURE;
	}

	/* Capture solution information */
	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	if (error) {
		printf("Error: GRBgetintattr failed.\n");
		printf("ERROR: %s\n", GRBgeterrormsg(env));
		return EXIT_FAILURE;
	}

	if (optimstatus == GRB_OPTIMAL) {

		error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, DIM*DIM*DIM, sol);
		if (error) {
			printf("Error: GRBgetdblattrarray failed.\n");
			printf("ERROR: %s\n", GRBgeterrormsg(env));
			return EXIT_FAILURE;
		}
		/* Update the board.solution from the given solution array */
		for (i = 0; i < DIM; i++) {
			for (j = 0; j < DIM; j++) {
				for (v = 0; v < DIM; v++) {
					if (sol[i*DIM*DIM + j * DIM + v] == 1.0) {
						sudoku.board[i][j].solution = (v + 1);
					}
				}
			}
		}

		return EXIT_SUCCESS;
	}
	else if (optimstatus == GRB_INF_OR_UNBD) {
		printf("Model is infeasible or unbounded\n");
		return EXIT_FAILURE;
	}
	else {
		printf("Optimization has encountered an error\n");
		return EXIT_FAILURE;
	}


	/* Free arrays */
	free(ind);
	free(val);
	free(sol);
	free(vtype);
	free(lb);
	/* Free model */
	GRBfreemodel(model);
	/* Free environment */
	GRBfreeenv(env);

	return 0;


}


/*
*					Private (Static) functions - not available outside this source file.
*/

/*
*	Initiates upper bounds of 1 to any variable Xi,j,v that applies board[i][j] = v   in the current board.
*	This makes restrictions for the values that exist on the board before initiating constraints.
*/
void initiate_upper_bounds(int DIM, int *lb, int *vtype) {
	int i, v, j;

	/* Fill the lower bound to 1 for any cell that already has a value (non-zero) */
	for (i = 0; i < DIM; i++) {
		for (j = 0; j < DIM; j++) {
			/*TODO: Added the below row v to (v+1). check if its ok */
			for (v = 0; v < DIM; v++) { /* v = Fixed Value.*/
				if (sudoku.board[i][j].value == (v + 1))
					lb[i*DIM*DIM + j * DIM + v] = 1.0;
				else
					lb[i*DIM*DIM + j * DIM + v] = 0.0;

				vtype[i*DIM*DIM + j * DIM + v] = GRB_BINARY;

			}
		}
	}
}

/*
*   This function initializes the environment and model in which the gurobi initializer runs on.
*	In addition, we cancel the gurobi logs printing on the console window.
*	
*   returns: EXIT_SUCCESS(0) on success.
*			 on any error returns EXIT_FAILURE(1) and prints the error.
*/
int create_env_model(GRBenv *env, GRBmodel *model, int DIM, int *lb, int *vtype) {
	int error;


	error = GRBloadenv(&env, "sudokuGurobi.log"); /* TODO: change 2nd argument to NULL when no need for log anymore */
	if (error || env == NULL) {
		printf("Error: could not create environment.\n");
		printf("ERROR: %s\n", GRBgeterrormsg(env));
		exit(EXIT_FAILURE);
	}

	error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
	if (error) {
		printf("Error: gurobi removing output has failed.");
		printf("ERROR: %s\n", GRBgeterrormsg(env));
		exit(EXIT_FAILURE);
	}

	/* Create an empty model */
	error = GRBnewmodel(env, &model, "sudoku", DIM*DIM*DIM, NULL, lb, NULL,
		vtype, NULL);
	if (error) {
		perror("Error: gurobi new model failed");
		printf("ERROR: %s\n", GRBgeterrormsg(env));
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}

/*
*   This function initialize the sudoku-related constraints for the ILP solver
*	The constraints are:
*	- Each Cell can only accept one value.
*	- Each Value must appear only once in each row.
*	- Each Value must appear only once in each column.
*	- Each Value must appear only once in each Block.
*
*   returns: EXIT_SUCCESS(0) on success.
*			 on any error returns EXIT_FAILURE(1) and prints the error.
*/
int initiate_constraints(GRBenv *env, GRBmodel *model, int DIM, int *ind, int *val, int b_row_l, int b_col_l) {
	int error, i, j, v, ig, jg, count;


	/* Each cell gets only one value.
	a constraint is conducted on each cell to have only one value chosen.
	*/
	for (i = 0; i < DIM; i++) {
		for (j = 0; j < DIM; j++) {
			for (v = 0; v < DIM; v++) {
				ind[v] = (i * DIM*DIM) + (j * DIM) + v;
				val[v] = 1.0;
			}

			error = GRBaddconstr(model, DIM, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				printf("Error: GRBaddconstr failed.\n");
				printf("ERROR: %s\n", GRBgeterrormsg(env));
				exit(EXIT_FAILURE);
			}
		}
	}

	/* Each value must appear once in each row.
	a constraint is conducted on each value to be valid in only one row.
	*/
	for (v = 0; v < DIM; v++) {
		for (j = 0; j < DIM; j++) {
			for (i = 0; i < DIM; i++) {
				ind[i] = (i * DIM*DIM) + (j * DIM) + v;
				val[i] = 1.0;
			}

			error = GRBaddconstr(model, DIM, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				printf("Error: GRBaddconstr failed.\n");
				printf("ERROR: %s\n", GRBgeterrormsg(env));
				exit(EXIT_FAILURE);
			}
		}
	}

	/* Each value must appear once in each column
	a constraint is conducted on each value to be valid in only one column.
	*/
	for (v = 0; v < DIM; v++) {
		for (i = 0; i < DIM; i++) {
			for (j = 0; j < DIM; j++) {
				ind[j] = (i * DIM*DIM) + (j * DIM) + v;
				val[j] = 1.0;
			}

			error = GRBaddconstr(model, DIM, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				printf("Error: GRBaddconstr failed.\n");
				printf("ERROR: %s\n", GRBgeterrormsg(env));
				exit(EXIT_FAILURE);
			}
		}
	}

	/* Each value must appear once in each subgrid */
	for (v = 0; v < DIM; v++) {
		/* Iterate Subgrids */
		for (ig = 0; ig < b_row_l; ig++) {
			for (jg = 0; jg < b_col_l; jg++) {
				count = 0;
				/* Iterate the inside of the subgrid */
				for (i = ig * b_col_l; i < (ig + 1)*b_col_l; i++) {
					for (j = jg * b_row_l; j < (jg + 1)*b_row_l; j++) {
						ind[count] = (i * DIM*DIM) + (j * DIM) + v;
						val[count] = 1.0;
						count++;
					}
				}

				error = GRBaddconstr(model, DIM, ind, val, GRB_EQUAL, 1.0, NULL);
				if (error) {
					printf("Error: GRBaddconstr failed.\n");
					printf("ERROR: %s\n", GRBgeterrormsg(env));
					exit(EXIT_FAILURE);
				}
			}
		}
	}

	return EXIT_SUCCESS;
}

/*
*   This function initialize the sudoku.board
*	The initialized board has all 0's in its cells.
*	should be free'd with free_board() function when the board isn't needed anymore.
*
*   returns: EXIT_SUCCESS(0) on success.
*			 on any error returns EXIT_FAILURE(1) and prints the error.
*/
bool optimize_and_get_sol(GRBenv *env, GRBmodel *model, int DIM, int *sol) {
	int error, i, j, v;
	int optimstatus;

	/* Optimize model for a solution */
	error = GRBoptimize(model);
	if (error) {
		printf("Error: GRBoptimize failed.\n");
		printf("ERROR: %s\n", GRBgeterrormsg(env));
		exit(EXIT_FAILURE);
	}

	/* Capture solution information */
	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	if (error) {
		printf("Error: GRBgetintattr failed.\n");
		printf("ERROR: %s\n", GRBgeterrormsg(env));
		exit(EXIT_FAILURE);
	}

	if (optimstatus == GRB_OPTIMAL) { /* Model has been solved and there is a solution to the board */

		error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, DIM*DIM*DIM, sol);
		if (error) {
			printf("Error: GRBgetdblattrarray failed.\n");
			printf("ERROR: %s\n", GRBgeterrormsg(env));
			exit(EXIT_FAILURE);
		}
		return TRUE;
	}
	else if (optimstatus == GRB_INF_OR_UNBD) { /* Model is infeasible or unbounded */
		return FALSE;
	}
	else {
		printf("Optimization has encountered an error. Program is terminating.\n");
		exit(EXIT_FAILURE);
	}


}

/*
*   This function initialize the sudoku.board
*	The initialized board has all 0's in its cells.
*	should be free'd with free_board() function when the board isn't needed anymore.
*
*   returns: EXIT_SUCCESS(0) on success.
*			 on any error returns EXIT_FAILURE(1) and prints the error.
*/
void update_solution(int *sol) {
	int i, j, v, DIM;

	/* Update the board.solution from the given solution array */
	for (i = 0; i < DIM; i++) {
		for (j = 0; j < DIM; j++) {
			for (v = 0; v < DIM; v++) {
				if (sol[i*DIM*DIM + j * DIM + v] == 1.0) {
					sudoku.board[i][j].solution = (v + 1);
				}
			}
		}
	}
}

bool is_solvable() {

	GRBenv   *env;
	GRBmodel *model;
	int       error;
	double    *sol; /* An array that holds the solution for (i,j,k) tuples */
	int       *ind; /* used for refferencing the variables.*/
	double    *val; /* an Array of possible value ceoficients */
	double	  *lb; /* Lower bounds for the values */
	char      *vtype; /* Variable types : BINARY for all of them */
	int		  DIM; /* The board dimensions */
	int       i; /* rows */
	int		  j; /* Columns */
	int		  v, ig, jg, count;
	int b_col_l, b_row_l;
	bool is_there_a_solution;

	b_col_l = sudoku.block_col_length;
	b_row_l = sudoku.block_row_length;
	DIM = sudoku.block_col_length * sudoku.block_row_length;
	env = NULL;
	model = NULL;
	error = 0;

	ind = malloc(sizeof(int)*DIM);
	val = malloc(sizeof(double)*DIM);
	lb = malloc(sizeof(double)*DIM*DIM*DIM);
	vtype = malloc(sizeof(char)*DIM*DIM*DIM);
	sol = malloc(sizeof(double)*DIM*DIM*DIM);

	if (!ind || !val || !lb || !val || !vtype || !sol) {
		printf("Error: malloc has failed on ILP solver\n");
		exit(EXIT_FAILURE);
	}

	/* Initiates upper bounds of 1 to any variable Xi,j,v that applies board[i][j] = v   in the current board */
	initiate_upper_bounds(DIM, lb, vtype);

	/* Creates the gurobi's environment and a new model for solving the ILP problem */
	if (create_env_model(env, model, DIM, lb, vtype) == EXIT_FAILURE) {
		exit(EXIT_FAILURE);
	}

	if (initiate_constraints(env, model, DIM, ind, val, b_row_l, b_col_l) == EXIT_FAILURE) {
		exit(EXIT_FAILURE);
	}

	if (optimize_and_get_sol(env, model, DIM, sol) == TRUE) {
		update_solution(sol);
		is_there_a_solution = TRUE;
	}
	else {
		is_there_a_solution = FALSE;
	}

	/* Free arrays */
	free(ind);
	free(val);
	free(sol);
	free(vtype);
	free(lb);
	/* Free model */
	GRBfreemodel(model);
	/* Free environment */
	GRBfreeenv(env);

	return is_there_a_solution;
}

