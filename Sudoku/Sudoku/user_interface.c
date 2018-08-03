#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "user_interface.h"
#include "game_logic.h"



sudokuCommands 
str2enum(const char *str)
{
	int j;
	for (j = 0; j < sizeof(conversion) / sizeof(conversion[0]); ++j)
		if (!strcmp(str, conversion[j].str))
			return conversion[j].val;
	error_message("no such string");
}



int user_command(char* buffer) {
	/* */
	int x, y, z;
	sudokuCommands sudoku_command;
	char *xchar, *ychar, *zchar, *command;
	command = strtok(buffer, " \t\r\n");
	xchar = strtok(NULL, " \t\r\n");
	ychar = strtok(NULL, " \t\r\n");
	if (command == NULL) {
		return 0;
	}
	sudoku_command = str2enum(command);
	switch (sudoku_command)
	{
	solve:break;
	edit:break;
	mark_errors: break;
	print_board:break;
	set:

		break;
	validate:break;
	generate:break;
	undo:break;
	redo:break;
	save:break;
	hint:break;
	num_solutions:break;
	autofill:break;
	reset:break;
	exit:break;
	default:
		return 0;
		break;
	}
}
	

int set(int row_index, int col_index, int value) {

	/* check if (i,j) is a fixed cell */
	if (sudoku.board[row_index][col_index].fixed) { /* it is fixed.*/
		printf("Error: cell is fixed\n");
		return false;
	}
	/* check if the value is ligall*/
	if (value < 0 || value > sudoku.block_row_length*sudoku.block_col_length) {
		printf("Error: value not in range 0-N\n");
		return false;
	}
	else {
		sudoku.board[row_index][col_index].value = value;
		update_errors(row_index, col_index); /* update all the relevant cells */
		print_board();
		return true;
		// counter, check last cell

	}


}

int print_board() {

	/* variables declarations */
	int i, j, board_length;
	board_length = sudoku.block_col_length*sudoku.block_row_length;

	/* Print 4N+n+1 dashes for the start*/
	separator_row();

	/* Go over the columns */
	for (i = 0; i < board_length; i++) {
		printf("|"); /* Opening pipe */

					  /* Go over Columns*/
		for (j = 0; j < board_length; j++) {

			if (sudoku.board[i][j].value == 0) /* blank */
			{
				printf("    ");
			}
			else if (sudoku.board[i][j].fixed) {				/* If fixed number */
				printf(" "); /* DOT for fixed number */
				printf("%2d.", sudoku.board[i][j].value); /* DOT for fixed number*/
			}
			else if (!sudoku.board[i][j].fixed) { /* Non-fixed number that the user inputed */
				printf(" "); /* space for normal number */
				printf("%2d ", sudoku.board[i][j].value);
				if (sudoku.mark_errors && sudoku.board[i][j].error) /* check if we need to mark an error */
					printf("*");
				else printf(" ");
			}

			/* after every m numbers , print a pipe*/
			if (j != board_length - 1) {
				if (j % sudoku.block_col_length == sudoku.block_col_length - 1)
					printf("|");
			}
			else printf("|");

		}
		printf("\n"); /*  Next line*/

					  /*Print dashes every 3 lines*/
		if (i % sudoku.block_row_length == sudoku.block_row_length - 1) {
			separator_row();
		}
	}
}


void separator_row() {
	int i;
	for (i = 0; i <= sudoku.block_col_length*sudoku.block_row_length + sudoku.block_row_length; i++)
		printf("-");
	printf("\n");
}


void autofill() {
	autofill(0, 0);
}

void autofill(int row_index, int col_index) {
	int i, j, board_length, value;
	board_length = sudoku.block_col_length*sudoku.block_row_length;
	if (row_index >= board_length) { /* end of the board */
		return;
	}
	else if (col_index >= board_length) { /* end of a line */
		return autofill(row_index + 1, 0);
	}
	else {
		if (!sudoku.board[row_index][col_index].value) {
			value = one_possible_value(row_index, col_index);  /* checks that there is only 1 valid value */
		}
		autofill(row_index, col_index + 1); /* next cell */
		sudoku.board[row_index][col_index].value = value; /* change the value after we checked all the cells */

	}
}


int one_possible_value(int row_index, int col_index) {
	int i, count, board_length, value;
	count = 0;
	board_length = sudoku.block_col_length*sudoku.block_row_length;
	for (i = 1; i <= board_length; i++) {
		if (valid_value(col_index, row_index, i)) {
			count++;
			value = i;
			if (count > 1) {
				return false;
			}
		}
	}
	return value;
}