
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gurobi_c.h"
#include "solver.h"

/*
		This module implements the external library -> Gurobi.
		The module implements methods for solving (or finding out there isn't any solution) for any sudoku board in any size.
		all Arrays are refferenced as 3-dimensions with [(i * DIM*DIM) + (j * DIM) + v] refferences.
*/

/*
*	
*
*/


/* Private function decleration */

/*
*   This function initialize the sudoku.solution with the ILP solution.
*/
void update_board_solution(double *sol, int DIM);

/*
*   This function initialize the sudoku.board with the ILP solution.
*/
void update_board_values(double *sol, int DIM);

/*
*   This function initialize the matrice argument that was given with the ILP solution.
*/
void update_arg_matrice(int **matrice, double *sol, int DIM);

/* TODO: document
TODO: break to little functions
*/
int gurobi_initializer(int **matrice, int fill_values_and_not_solution_flag);

/* Public functions */

int is_there_a_solution(int **matrice, int fill_values_and_not_solution_flag) {

	if (gurobi_initializer(matrice, fill_values_and_not_solution_flag) == true) {
		return true;
	}

	return false;
}

/* Private functions */

void update_board_solution(double *sol, int DIM) {
	int i, j, v;

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

void update_board_values(double *sol, int DIM) {
	int i, j, v;

	/* Update the board.solution from the given solution array */
	for (i = 0; i < DIM; i++) {
		for (j = 0; j < DIM; j++) {
			for (v = 0; v < DIM; v++) {
				if (sol[i*DIM*DIM + j * DIM + v] == 1.0) {
					sudoku.board[i][j].value = (v + 1);
				}
			}
		}
	}
}

void update_arg_matrice(int **matrice, double *sol, int DIM) {
	int i, j, v;

	/* Update the board.solution from the given solution array */
	for (i = 0; i < DIM; i++) {
		for (j = 0; j < DIM; j++) {
			for (v = 0; v < DIM; v++) {
				if (sol[i*DIM*DIM + j * DIM + v] == 1.0) {
					matrice[i][j] = (v + 1);
				}
			}
		}
	}
}


int gurobi_initializer(int **matrice, int fill_values_and_not_solution_flag) {

	GRBenv   *env;
	GRBmodel *model;
	int       error;
	int is_solvable;
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
			for (v = 0; v < DIM; v++) { /* v = Fixed Value.*/
				if (matrice) {
					if (matrice[i][j] == (v + 1))
						lb[positionGur(i, j, v, DIM)] = 1.0;
					else
						lb[positionGur(i, j, v, DIM)] = 0.0;

					vtype[positionGur(i, j, v, DIM)] = GRB_BINARY;
				}
				else {
					if (sudoku.board[i][j].value == (v + 1))
						lb[positionGur(i, j, v, DIM)] = 1.0;
					else
						lb[positionGur(i, j, v, DIM)] = 0.0;

					vtype[positionGur(i, j, v, DIM)] = GRB_BINARY;
				}
			}
		}
	}



	/* Create environment */
	error = GRBloadenv(&env, "sudokuGurobi.log"); 
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

	/* Each cell gets only one value.
	a constraint is conducted on each cell to have only one value chosen.
	*/
	for (i = 0; i < DIM; i++) {
		for (j = 0; j < DIM; j++) {
			for (v = 0; v < DIM; v++) {
				ind[v] = positionGur(i, j, v, DIM);
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
				ind[i] = positionGur(i, j, v, DIM);
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
				ind[j] = positionGur(i, j, v, DIM);
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
				for (i = jg * b_row_l; i < (jg + 1)*b_row_l; i++) {
					for (j = ig * b_col_l; j < (ig + 1)*b_col_l ; j++) {
						ind[count] = positionGur(i, j, v, DIM);
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

	if (optimstatus == GRB_OPTIMAL) {

		error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, DIM*DIM*DIM, sol);
		if (error) {
			printf("Error: GRBgetdblattrarray failed.\n");
			printf("ERROR: %s\n", GRBgeterrormsg(env));
			exit(EXIT_FAILURE);
		}
		/* Update the board.solution from the given solution array */
		if (matrice) {
			update_arg_matrice(matrice, sol, DIM);
		}
		else {
			if (fill_values_and_not_solution_flag == true) {
				update_board_values(sol, DIM);
			}
			else {
				update_board_solution(sol, DIM);
			}
		}

		is_solvable = true;
	}
	else if (optimstatus == GRB_INF_OR_UNBD || optimstatus == GRB_INFEASIBLE) {
		is_solvable = false;
	}
	else {
		printf("Optimization has encountered an error\n");
		is_solvable = false;
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

	return is_solvable;
}



