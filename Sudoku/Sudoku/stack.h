

#ifndef STACK_H
#define STACK_H


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
* The function uses a exhaustive backtracking algorithm to solve the sudoku, and checks how many solutions there is.
* @param board - matrice with the cells inforamtion
* @return - the number of possible solutions.
*/
int numberOfSolutions();


#endif