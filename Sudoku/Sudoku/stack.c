/*
* C Program to Implement Stack Operations using Dynamic Memory
* Allocation
*/
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "aux_main.h"


/* to insert elements in stack*/
void push(int row_index, int col_index, int value)
{
	struct node_stack *temp;
	temp = (struct node_stack*)malloc(sizeof(struct node_stack));

	temp->col_index = col_index;
	temp->row_index = row_index;
	temp->value = value;
	temp->link = top_node;
	top_node = temp;

}

/* to delete elements from stack */
void pop(int *row_index, int *col_index, int *value)
{
	struct node_stack *temp;
	if (top_node == NULL)
		printf("**Stack is empty**\n");
	else
	{
		*row_index = top_node->row_index;
		*col_index = top_node->col_index;
		*value = top_node->value;
		temp = top_node;
		top_node = top_node->link;
		free(temp);
	}
}

/* to check if stack is empty*/
int empty()
{
	if (top_node == NULL)
		return true;
	else
		return false;
}




/* to empty and destroy the stack*/
void destroy()
{
	struct node_stack *temp;
	int *row_index, *col_index, *value;
	temp = top_node;
	while (temp!= NULL)
	{
		pop(row_index, col_index, value);
		temp = temp->link;
	}
	printf("stack destroyed\n");
}

/*
* The function uses a determenistic algorithem to solve the sudoku.
* The function also updates the "solution matrice" to the new solution if there is one. (if there isn't , the previous solution is kept)
* @param board - matrice with the cells inforamtion
* @param row_index - the row of the cell we are checking
* @param col_index - the column of the cell we are checking
* @return -true(0) if the sudoku is solvebale and store the solution in cell.solution matrix, false(1) otherwise and do nothing(not sure if it work).
*/
int numberOfSolotions() {
	/* variables declarations */
	int row_index, col_index, count, value, board_length;
	board_length = sudoku.block_col_length * sudoku.block_row_length;

	for (row_index = 0; row_index < board_length; row_index++) {
		for (col_index = 0; col_index < board_length; col_index++) {
			if (sudoku.board[row_index][col_index].is_fixed == false) {
				if (sudoku.board[row_index][col_index].value == 0) {
					for (value = 1; value <= board_length; value++) {
						if (valid_value(row_index, col_index, value)) {
							push(row_index, col_index, value);
							sudoku.board[row_index][col_index].value = value;
							break;
						REC:;
						}
						else if (value == board_length) {
							pop(&row_index, &col_index, &value);
							goto REC;
						}
					}
				}
			}
			if (row_index == board_length - 1 && col_index == board_length - 1) {
				count++;
				pop(&row_index, &col_index, &value);
				goto REC;
			}

		}
	}

	return count; /* Yuval added this. cause there was no 'return' at all*/
}