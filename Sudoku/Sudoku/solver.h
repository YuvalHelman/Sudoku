#ifndef SOLVER_H
#define SOLVER_H

#include "aux_main.h"

int gurobi_initializer(); /* todo: ERASE that later */

/*
*	The function checks if there is a solution to the current board.
*	If there is a solution, it returns true. else it returns false.
*	If the "matrice" argument is NULL, the solution is put in the sudoku.board_solution.
*	otherwise, the solution is put in the "matrice" argument.
*
*	matrice: an optional integer matrice to put in the solution.
*
*   returns: true(1) when there is a solution to the current board.
*			 false(0) when there isn't a solution.
*/
int is_solvable(int **matrice);



#endif