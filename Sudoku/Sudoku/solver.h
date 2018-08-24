#ifndef SOLVER_H
#define SOLVER_H

#include "aux_main.h"

int gurobi_initializer(); /* todo: ERASE that later */

/*
*	The function checks if there is a solution to the current board.
*	If there is a solution, it returns True. else it returns False.
*	When TRUE is returned, the sudoku.solution matrice contains the solution board.
*
*
*   returns: true(1) when there is a solution to the current board.
			 false(0) when there isn't a solution.
*/
int is_solvable();



#endif