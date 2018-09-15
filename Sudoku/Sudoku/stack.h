

#ifndef STACK_H
#define STACK_H


#define false 0
#define true 1

/*
		This module defines stack operations in order to use an exhaustive backtracking algorithm to solve the sudoku, and checks how many solutions are there.
*/

struct node_stack
{
	int row_index;
	int col_index;
	int value;
	struct node_stack *link;
} *top_node;




/*
*	The function uses an exhaustive backtracking algorithm to solve the sudoku, and checks how many solutions are there.
*
*	@param board - matrice with the cells inforamtion
*
*	@returns - the number of possible solutions.
*/
int numberOfSolutions();


#endif