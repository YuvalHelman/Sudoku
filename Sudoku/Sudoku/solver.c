
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "solver.h"
#include "gurobi_c.h"
#include "aux_main.h"
#include "game_logic.h"

/*
*	all Arrays are refferenced as 3-dimensions with [(i * DIM*DIM) + (j * DIM) + v] reffrences. 
*
*/
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
	vtype = malloc(sizeof(char)*DIM*DIM*DIM);
	lb = malloc(sizeof(char)*DIM*DIM*DIM);
	sol = malloc(sizeof(char)*DIM*DIM*DIM);


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

		/* Create environment */
		error = GRBloadenv(&env, "sudokuGurobi.log"); /* TODO: change 2nd argument to NULL when no need for log anymore */
		if (error || env == NULL) {
			fprintf(stderr, "Error: could not create environment\n");
			exit(1);
		}

		/* Create an empty model */
		error = GRBnewmodel(env, &model, "sudoku", DIM*DIM*DIM, NULL, lb, NULL,
			vtype, NULL);
		if (error) {
			perror("gurobi new model failed");
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
					perror("gurobi new model failed");
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
					perror("gurobi new model failed");
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
					perror("gurobi new model failed");
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
						perror("gurobi new model failed");
						return EXIT_FAILURE;
					}
				}
			}
		}

		/* Optimize model for a solution */
		error = GRBoptimize(model);
		if (error) {
			perror("gurobi new model failed");
			return EXIT_FAILURE;
		}

		/* Capture solution information */
		error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
		if (error) {
			perror("gurobi new model failed");
			return EXIT_FAILURE;
		}

		if (optimstatus == GRB_OPTIMAL) {

			error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, DIM*DIM*DIM, sol);
			if (error) {
				perror("gurobi new model failed");
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

		/* Error reporting */

		if (error) {
			printf("ERROR: %s\n", GRBgeterrormsg(env));
			exit(1);
		}


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








