#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "user_interface.h"
#include "game_logic.h"
#include "aux_main.h"
#include "move_list.h"
#include "file_manipulation.h"



sudokuCommands 
str2enum(const char *str)
{
	int j;
	for (j = 0; j < sizeof(conversion) / sizeof(conversion[0]); ++j)
		if (!strcmp(str, conversion[j].str))
			return conversion[j].val;
	return error_command;
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
	case solve_command:break;
	case edit_command:break;
	case mark_errors_command: break;
	case print_board_command: break;
	case set_command:

		break;
	case validate_command:break; 
	case generate_command:break;
	case undo_command:break;
	case redo_command:break;
	case save_command:break;
	case hint_command:break;
	case num_solutions_command:break;
	case autofill_command:break;
	case reset_command:break;
	case exit_command:break;
	case error_command:break;
	default:
		return 0;
		break;
	}
}
	

int set(int row_index, int col_index, int value) {

	int prev_val, updated_val, board_len;

	board_len = sudoku.block_row_length * sudoku.block_col_length; 
	prev_val = sudoku.board[row_index][col_index].value;
	updated_val = value;

	/* check if (i,j) is a fixed cell */
	if (sudoku.board[row_index][col_index].is_fixed) { /* it is fixed.*/
		printf("Error: cell is fixed\n");
		return false;
	}

	// TODO: Check if row_index && col_index are valid ? (or mabye in the parser ?)

	/* check if the value is legal */
	if (value < 0 || value > board_len) {
		printf("Error: value not in range 0-N\n");
		return false;
	}
	
	sudoku.board[row_index][col_index].value = value;
	if (add_new_node(row_index, col_index, prev_val, updated_val) == EXIT_FAILURE) {
		return false;
	}
	update_errors(row_index, col_index); /* update all the relevant cells */
	print_board();
	
	return true;
	// counter, check last cell

}

void print_board() {

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
			else if (sudoku.board[i][j].is_fixed) {				/* If fixed number */
				printf(" "); /* DOT for fixed number */
				printf("%2d.", sudoku.board[i][j].value); /* DOT for fixed number*/
			}
			else if (!sudoku.board[i][j].is_fixed) { /* Non-fixed number that the user inputed */
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


int autofill() {
	cell **prev_board, **updated_board;

	/* create a copy of the board before the autofill function */
	if( !(prev_board = copy_current_board()) ){
		return EXIT_FAILURE;
	}

	autofill_board(0, 0);

	/* create a copy of the newly board after the autofill function */
	if ( !(updated_board = copy_current_board()) ) {
		free(prev_board);
		return EXIT_FAILURE;
	}

	if (add_new_node_autofill(prev_board, updated_board) == EXIT_FAILURE) {
		printf("adding a new node to the list has failed\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

void autofill_board(int row_index, int col_index) {
	int board_length, value;
	board_length = sudoku.block_col_length*sudoku.block_row_length;
	if (row_index >= board_length) { /* end of the board */
		/* do nothing */
	}
	else if (col_index >= board_length) { /* end of a line */
		autofill_board(row_index + 1, 0);
	}
	else {
		if (!sudoku.board[row_index][col_index].value) {
			value = one_possible_value(row_index, col_index);  /* checks that there is only 1 valid value */
		}
		autofill_board(row_index, col_index + 1); /* next cell */
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


int Solve(char* filepath) {
	FILE* fd;
	int ret_val;
	int block_rows, block_cols;

	/* Change the game mode */
	game_mode = solve;

	/* Open the file*/
	fd = fopen(filepath, "r");
	if (!fd) {
		printf("Error: File doesn't exist or cannot be opened\n");
		return EXIT_SUCCESS;
	}

	/* Reset basic game utilities */
	delete_list_full();
	free_board(sudoku.board, sudoku.block_col_length, sudoku.block_row_length);
	sudoku.board = NULL;

	/* Read from the file and initialize the board and sudoku's block_col/row lengths */
	if (read_from_file(fd, &block_rows, &block_cols) == EXIT_FAILURE) {
		free_board(sudoku.board, block_cols, block_rows);
		return EXIT_FAILURE;
	}

	/* Set basic sudoku utilities */
	sudoku.block_row_length = block_rows;
	sudoku.block_col_length = block_cols;

	print_board();

	return EXIT_SUCCESS;
}


int Edit(char* filepath) {
	FILE* fd;
	int ret_val;
	int block_rows, block_cols;

	/* Change the game mode */
	game_mode = edit;

	/* Reset basic game utilities */
	delete_list_full();
	free_board(sudoku.board, sudoku.block_col_length, sudoku.block_row_length);
	sudoku.board = NULL;

	if (filepath != NULL) {
		/* Open the file*/
		fd = fopen(filepath, "r");
		if (!fd) {
			printf("Error: File cannot be opened\n");
			return EXIT_SUCCESS;
		}

		/* Read from the file and initialize the board and sudoku's block_col/row lengths */
		if (read_from_file(fd, &block_rows, &block_cols) == EXIT_FAILURE) {
			free_board(sudoku.board, block_cols, block_rows);
			return EXIT_FAILURE;
		}

		/* Set basic sudoku utilities */
		sudoku.block_row_length = block_rows;
		sudoku.block_col_length = block_cols;
	}
	else {
		if (initialize_new_board(sudoku.board, DEFAULT_BLOCK_LEN, DEFAULT_BLOCK_LEN) == EXIT_FAILURE) {
			return EXIT_FAILURE;
		}

		sudoku.block_row_length = DEFAULT_BLOCK_LEN;
		sudoku.block_col_length = DEFAULT_BLOCK_LEN;

	}

	print_board();

	return EXIT_SUCCESS;
}

int Save(char* filepath) {
	FILE* fd;
	int ret_val;
	int block_rows, block_cols;

	if (game_mode == edit) {

		// TODO: check if board has errors, and if so --> exit and print:
		//		printf("Error: board contains erroneous values\n");
		
		// TODO: validate. if validation fails, --> exit and print:
		//		printf("Error: board validation failed\n");


	}

	/* Open the file*/
	fd = fopen(filepath, "w");
	if (!fd) {
		printf("Error: File cannot be created or modified\n");
		return EXIT_SUCCESS;
	}

	if (save_to_file(fd) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}
	
	printf("Saved to %s\n", filepath);



}