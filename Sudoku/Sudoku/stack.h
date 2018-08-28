

#ifndef STACK_H
#define STACK_H

#include <stdio.h>

#define false 0
#define true 1

struct node_stack
{
	int row_index;
	int col_index;
	int value;
	struct node_stack *link;
} *top_node;


/* function prototypes */
void push(int row_index, int col_index, int value);
void pop(int *row_index, int *col_index, int *value);
int empty();
void destroy();


/*
* The function uses a determenistic algorithm to solve the sudoku.
* The function also updates the "solution matrice" to the new solution if there is one. (if there isn't , the previous solution is kept)
* @param board - matrice with the cells inforamtion
* @param row_index - the row of the cell we are checking
* @param col_index - the column of the cell we are checking
* @return -true(0) if the sudoku is solvebale and store the solution in cell.solution matrix, false(1) otherwise and do nothing(not sure if it work).
*/
int numberOfSolutions();


#endif