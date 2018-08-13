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
	sudokuCommands sudoku_command;
	char *xchar, *ychar, *zchar, *command;
	command = strtok(buffer, " \t\r\n");
	xchar = strtok(NULL, " \t\r\n");
	ychar = strtok(NULL, " \t\r\n");
	zchar = strtok(NULL, " \t\r\n");
	if (command == NULL) {
		return 0;
	}
	sudoku_command = str2enum(command);
	switch (sudoku_command)
	{
	case solve_command:
		Solve(xchar);	
		break;
	case edit_command:
		Edit(xchar);
		break;
	case mark_errors_command:
		if (game_mode != solve) 
			goto Error_Command;
		mark_error(atoi(xchar));
		break;
	case print_board_command:
		if (game_mode == init)
			goto Error_Command;
		else print_board();
		break;
	case set_command:
		if (game_mode == init)
			goto Error_Command;
		else set(atoi(xchar), atoi(ychar), atoi(zchar));
		break;
	case validate_command:
		if (game_mode == init)
			goto Error_Command;
		else validate();
		break;
	case generate_command:
		if (game_mode != edit)
			goto Error_Command;
		else generate(atoi(xchar),atoi(ychar));
		break;
	case undo_command:
		if (game_mode == init)
			goto Error_Command;
		else undo();
		break;
	case redo_command:
		if (game_mode == init)
			goto Error_Command;
		else redo();
		break;
	case save_command:
		if (game_mode == init)
			goto Error_Command;
		else Save(xchar);
		break;
	case hint_command:
		if (game_mode != solve)
			goto Error_Command;
		hint(atoi(xchar), atoi(ychar));
		break;
	case num_solutions_command:
		if (game_mode == init)
			goto Error_Command;
		else num_solutions();
		break;
	case autofill_command:
		if (game_mode != solve)
			goto Error_Command;
		autofill();
		break;
	case reset_command:
		if (game_mode == init)
			goto Error_Command;
		else reset();
		break;
	case exit_command:
		Exit();
		break;
Error_Command:
	case error_command:
		printf("ERROR: invalid command\n");
		break;
	default:
		return 0;
		break;
	}
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

void mark_errors(int value) {
	if (value != 0 && value != 1) {
		printf("Error: the value should be 0 or 1\n");
	}
	else sudoku.mark_errors = value;
}

int set(int col_index, int row_index, int value) {

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
	if (value < 0 || value > board_len || 
		row_index < 0 || row_index > board_len ||
		col_index < 0 || col_index > board_len) {
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
	board_length = sudoku.block_col_length * sudoku.block_row_length;

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
	int row_index, col_index, board_length, value, **new_values, i;
	board_length = sudoku.block_col_length*sudoku.block_row_length;


	if ((new_values = (int **)malloc(sizeof(int *) * board_length)) == NULL) {
		printf("Error: Malloc has failed allocating the board\n");
		return EXIT_FAILURE;
	}
	for (i = 0; i < board_length; i++) {
		if ((new_values[i] = (int *)calloc(board_length, sizeof(int)) == NULL)) {
			printf("Error: Malloc has failed allocating the board\n");
			return EXIT_FAILURE;
		}
	}

	/* create a copy of the board before the autofill function */
	if (!(prev_board = copy_current_board())) {
		return EXIT_FAILURE;
	}

	for (col_index = 0; col_index < board_length; col_index++) {
		for (row_index = 0; row_index < board_length; row_index++) {
			if (!sudoku.board[row_index][col_index].value) {
				value = one_possible_value(row_index, col_index);  /* checks that there is only 1 valid value */
				new_values[row_index][col_index] = value;
			}
		}
	}

	for (col_index = 0; col_index < board_length; col_index++) {
		for (row_index = 0; row_index < board_length; row_index++) {
			if (!sudoku.board[row_index][col_index].value) {
				sudoku.board[row_index][col_index].value = new_values[row_index][col_index];
			}
		}
	}
	free_int_matrix(new_values, sudoku.block_col_length, sudoku.block_row_length);
	/* create a copy of the newly board after the autofill function */
	if ( !(updated_board = copy_current_board()) ) {
		free_board(prev_board, sudoku.block_col_length, sudoku.block_row_length);
		return EXIT_FAILURE;
	}

	if (add_new_node_autofill(prev_board, updated_board) == EXIT_FAILURE) {
		printf("adding a new node to the list has failed\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
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
				return 0;
			}
		}
	}
	return value;
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
	return EXIT_SUCCESS;
}

void print_board_solution() {

	/* variables declarations */
	int i, j, board_length;
	board_length = sudoku.block_col_length * sudoku.block_row_length;

	/* Print 4N+n+1 dashes for the start*/
	separator_row();

	/* Go over the columns */
	for (i = 0; i < board_length; i++) {
		printf("|"); /* Opening pipe */

					 /* Go over Columns*/
		for (j = 0; j < board_length; j++) {

			if (sudoku.board[i][j].solution == 0) /* blank */
			{
				printf("    ");
			}
			else if (sudoku.board[i][j].is_fixed) {				/* If fixed number */
				printf(" "); /* DOT for fixed number */
				printf("%2d.", sudoku.board[i][j].solution); /* DOT for fixed number*/
			}
			else if (!sudoku.board[i][j].is_fixed) { /* Non-fixed number that the user inputed */
				printf(" "); /* space for normal number */
				printf("%2d ", sudoku.board[i][j].solution);
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

