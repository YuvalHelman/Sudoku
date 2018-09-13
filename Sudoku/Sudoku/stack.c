/*
* The module implements stack operations.
*/
#include <stdio.h>
#include <stdlib.h>
#include "aux_main.h"
#include "game_logic.h"
#include "user_interface.h"
#include "stack.h"


struct node_stack *top_node = NULL;


/* Private function decleration */
/* to insert elements in stack*/
void push(int row_index, int col_index, int value);
/* to delete access elements from stack */
void pop(int *row_index, int *col_index, int *value);
/* to check if stack is empty*/
int empty();
/* to empty and destroy the stack*/
void destroy();
/*	checks which cell is the most 'advenced' one that is still empty and 
*	saves the values in last_input_row, int, last_input_col.
*
*	last_input_col: the cell's column.
*	last_input_row: the cell's row.
*
*   returns: ture if there was un empty cell, false otherwise.
*/
int last_input_index(int *last_input_row, int *last_input_col);

int set_reset_save_the_value(int value, int row_index, int col_index);

int number_of_solutions();

/* Public functions */

int numberOfSolutions() {
	/* variables declarations */
	int row_index, col_index, count, board_length, **temp_matrice_values;
	count = 0;
	board_length = sudoku.block_col_length * sudoku.block_row_length;

	if (is_board_erronous()) {
		printf("Error: board contains erroneous values\n");
		return 0;
	}

	temp_matrice_values = initialize_integer_board();

	/* Save the board's values in a temporary board */
	for (col_index = 0; col_index < board_length; col_index++) {
		for (row_index = 0; row_index < board_length; row_index++) {
			temp_matrice_values[row_index][col_index] = sudoku.board[row_index][col_index].value;
		}
	}
	count = number_of_solutions();

	/* Copy the original values back to the sudoku.board */
	for (col_index = 0; col_index < board_length; col_index++) {
		for (row_index = 0; row_index < board_length; row_index++) {
			sudoku.board[row_index][col_index].value = temp_matrice_values[row_index][col_index];
		}
	}

	free_int_matrix(temp_matrice_values, sudoku.block_col_length, sudoku.block_row_length);

	if (count != 0) {
		printf("Number of solutions: %d\n", count);
		if (count == 1)
			printf("This is a good board!\n");
		else printf("The puzzle has more than 1 solution, try to edit it further\n");
	}
	else printf("The puzzle has 0 solutions.");

	return count;
}
/* Private functions */



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


int empty()
{
	if (top_node == NULL)
		return true;
	else
		return false;
}




void destroy()
{
	struct node_stack *temp;
	int *row_index, *col_index, *value;
	value = col_index = row_index = 0;
	temp = top_node;
	while (temp!= NULL)
	{
		pop(row_index, col_index, value);
		temp = temp->link;
	}
	printf("stack destroyed\n");
}

int last_input_index(int *last_input_row, int *last_input_col) {
	/* variables declarations */
	int row_index, col_index, board_length;
	board_length = sudoku.block_col_length * sudoku.block_row_length;
	for (row_index = board_length - 1; row_index >= 0; row_index--) {
		for (col_index = board_length - 1; col_index >= 0; col_index--) {
			if (sudoku.board[row_index][col_index].value == 0) {
				*last_input_row = row_index;
				*last_input_col = col_index;
				return true;
			}
		}
	}
	return false;
}



int set_reset_save_the_value(int value, int row_index, int col_index) {
	if (valid_value(row_index, col_index, value)) {
		push(row_index, col_index, value);
		sudoku.board[row_index][col_index].value = value;
		return true;
	}
	return false;
}

int number_of_solutions() {
	/* variables declarations */
	int row_index, col_index, count, value, board_length, last_input_row, last_input_col;
	count = row_index = col_index = 0;
	value = 1;
	board_length = sudoku.block_col_length * sudoku.block_row_length;
	if (last_input_index(&last_input_row, &last_input_col)) {
		return count;
	}
	while (row_index < board_length) {
		col_index = col_index == board_length ? 0 : col_index;
		while (col_index < board_length) {
			if (sudoku.board[row_index][col_index].value == 0) {
				while (value <= board_length) {
					if (set_reset_save_the_value(value, row_index, col_index)) {
						value = 1;
						if (row_index == last_input_row && col_index == last_input_col) {
							pop(&row_index, &col_index, &value);
							sudoku.board[row_index][col_index].value = 0;
							count++;
							col_index--;
							value++;
						}
						break;
					}
					value++;
				}
				if (value == board_length + 1) {
					if (!empty()) {
						sudoku.board[row_index][col_index].value = 0;
						pop(&row_index, &col_index, &value);
						value++;
						sudoku.board[row_index][col_index].value = 0;
						col_index--;
					}
					else return count;
				}
			}
			col_index++;
		}
		row_index++;
	}
	return count;
}

