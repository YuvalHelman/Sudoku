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

int get_command_and_parse() {
	
	int ret_command;
	char command[MAX_COMMAND_SIZE];
	unsigned int seed_arg;
	char* fgets_ret; /* for EOF checking */


	/* Get Commands and Play*/
	ret_command = 0;
	
	do {
		/* Get commands */
		if (feof(stdin)) { /* EOF reached. exit. */
			printf("Exiting...");
			return EXIT_SUCCESS;
		}

		printf("Enter your command:\n");
		fgets_ret = fgets(command, MAX_COMMAND_SIZE, stdin);
		ret_command = user_command(command);

		if (fgets_ret == NULL && ferror(stdin)) { /* fgets ancountered some error */
			perror("Error: fgets has failed.");
			return EXIT_FAILURE;
		}

		
	} while (fgets_ret != NULL);


	
}


int user_command(char* buffer) {
	/* */
	sudokuCommands sudoku_command;
	char *xchar, *ychar, *zchar, *command;
	int xchar_asInt, ychar_asInt, zchar_asInt;
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
		if (sudoku.game_mode != solve) { 
			printf("ERROR: invalid command\n"); /* case b */
		} else {
			errno = 0;
			xchar_asInt = (int)(strtol(xchar, NULL, BASE10));
			if (errno != 0) {
				perror("strtol function failed.");
				return EXIT_FAILURE;
			}
			mark_error(xchar_asInt);
		}
	break;
	case print_board_command:
		if (sudoku.game_mode == init) {
			printf("ERROR: invalid command\n"); /* case b */
		}
		else {
			print_board();
		}
	break;
	case set_command:
		if (sudoku.game_mode == init) {
			printf("ERROR: invalid command\n"); /* case b */
		}
		else {
			errno = 0;
			xchar_asInt = (int)(strtol(xchar, NULL, BASE10));
			if (errno == ERANGE || errno == EINVAL) {
				perror("strtol function failed.");
				return EXIT_FAILURE;
			}
			errno = 0;
			ychar_asInt = (int)(strtol(ychar, NULL, BASE10));
			if (errno != 0) {
				perror("strtol function failed.");
				return EXIT_FAILURE;
			}
			errno = 0;
			zchar_asInt = (int)(strtol(zchar, NULL, BASE10));
			if (errno != 0) {
				perror("strtol function failed.");
				return EXIT_FAILURE;
			}

			set(xchar_asInt, ychar_asInt, zchar_asInt);
		}
	break;
	case validate_command:
		if (sudoku.game_mode == init)
			printf("ERROR: invalid command\n"); /* case b */
		else validate();
	break;
	case generate_command:
		if (sudoku.game_mode != edit)
			printf("ERROR: invalid command\n"); /* case b */
		else generate(atoi(xchar),atoi(ychar));
	break;
	case undo_command:
		if (sudoku.game_mode == init)
			printf("ERROR: invalid command\n"); /* case b */
		else undo();
	break;
	case redo_command:
		if (sudoku.game_mode == init)
			printf("ERROR: invalid command\n"); /* case b */
		else redo();
	break;
	case save_command:
		if (sudoku.game_mode == init)
			printf("ERROR: invalid command\n"); /* case b */
		else Save(xchar);
	break;
	case hint_command:
		if (sudoku.game_mode != solve)
			printf("ERROR: invalid command\n"); /* case b */
		hint(atoi(xchar), atoi(ychar));
	break;
	case num_solutions_command:
		if (sudoku.game_mode == init)
			printf("ERROR: invalid command\n"); /* case b */
		else num_solutions();
	break;
	case autofill_command:
		if (sudoku.game_mode != solve)
			printf("ERROR: invalid command\n"); /* case b */
		autofill();
	break;
	case reset_command:
		if (sudoku.game_mode == init) {
			printf("ERROR: invalid command\n");
			return EXIT_SUCCESS;
		}
		else {
			reset();
		}
	break;
	case exit_command:
		Exit();
	break;
	default:
		printf("ERROR: invalid command\n");
	break;
	}

	return EXIT_SUCCESS;
}
	
int Solve(char* filepath) {
	FILE* fd;
	int ret_val;
	int block_rows, block_cols;

	/* Change the game mode */
	sudoku.game_mode = solve;

	/* Open the file*/
	fd = fopen(filepath, "r");
	if (!fd) {
		printf("Error: File doesn't exist or cannot be opened\n"); /* case b */
		return EXIT_SUCCESS;
	}

	/* Reset basic game utilities (case d) */
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
	sudoku.game_mode = edit;

	/* Reset basic game utilities (case f) */
	delete_list_full();
	free_board(sudoku.board, sudoku.block_col_length, sudoku.block_row_length);
	sudoku.board = NULL;

	if (filepath != NULL) {
		/* Open the file*/
		fd = fopen(filepath, "r");
		if (!fd) {
			printf("Error: File cannot be opened\n"); /* case b */
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

	/* restart Sudoku's essential variables */
	sudoku.mark_errors = 0;

	print_board();

	return EXIT_SUCCESS;
}

void mark_errors(int value) {
	if (value != 0 && value != 1) {
		printf("Error: the value should be 0 or 1\n");
	}
	else sudoku.mark_errors = value;
}

//add nodes to the move list
// check if return values could be changed to EXIT_SUCCESS \ EXIT_FAILURE.
int set(int col_index, int row_index, int value) {

	int prev_val, updated_val, board_len;
	
	/* check if the values are legal */
	if (value < 0 || value > board_len ||
		row_index < 0 || row_index > board_len ||
		col_index < 0 || col_index > board_len) {
		printf("Error: value not in range 0-N\n");
		return false;
	}

	board_len = sudoku.block_row_length * sudoku.block_col_length;
	prev_val = sudoku.board[row_index][col_index].value;
	updated_val = value;


	/* check if (i,j) is a fixed cell (case e) */
	if (sudoku.board[row_index][col_index].is_fixed) { /* it is fixed.*/
		printf("Error: cell is fixed\n");
		return false;
	}

	/* Update the value in the board itself and the number of filled cells */
	sudoku.board[row_index][col_index].value = value;
	update_num_of_filled_cells(prev_val, updated_val);

	/* Update the move_list */
	if (add_new_node(row_index, col_index, prev_val, updated_val) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}
	
	/* update errors for all relevant cells */
	update_errors(row_index, col_index); 

	print_board();
	
	return EXIT_SUCCESS;
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


//add nodes to the move list
int autofill() {
	cell **prev_board, **updated_board;
	int row_index, col_index, board_length, value, **new_values;
	board_length = sudoku.block_col_length*sudoku.block_row_length;

	new_values = NULL;

	if (initialize_integer_board(new_values, sudoku.block_col_length, sudoku.block_row_length) 
		== EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	/* create a copy of the board before the autofill function */
	if (!(prev_board = copy_current_board())) {
		return EXIT_FAILURE;
	}

	for (col_index = 0; col_index < board_length; col_index++) {
		for (row_index = 0; row_index < board_length; row_index++) {
			if (!sudoku.board[row_index][col_index].value) { /* check if the cell is unfilled (value=0) */
				value = one_possible_value(row_index, col_index);  /* checks that there is only 1 valid value */
				new_values[row_index][col_index] = value;
			}
		}
	}

	/* Copy value from the temp matrix to the board */
	for (col_index = 0; col_index < board_length; col_index++) {
		for (row_index = 0; row_index < board_length; row_index++) {
			if (!sudoku.board[row_index][col_index].value) {
				sudoku.board[row_index][col_index].value = new_values[row_index][col_index];
				update_num_of_filled_cells(ZERO, NON_ZERO); // ???
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

	if (sudoku.game_mode == edit) {

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



/*
*  This function recieves a pointer to an integer matrix that needs to be initialized.
*	The initialized matrix has all 0's in its cells.
*
*   returns: EXIT_SUCCESS(0) on success.
*			 on any error returns EXIT_FAILURE(1) and prints the error.
*/
int initialize_integer_board(int** board, int block_col_len, int block_row_len) {
	int board_size, i;

	if (board) {
		printf("board should be NULL in order to be initialized. might contain needed data\n");
		return EXIT_FAILURE;
	}

	board_size = block_col_len * block_row_len;

	if ((board = (int **)malloc(board_size * sizeof(int *))) == NULL) {
		printf("Error: Malloc has failed allocating the board\n");
		return EXIT_FAILURE;
	}
	for (i = 0; i < board_size; i++) {
		if ((board[i] = (int *)calloc(board_size, sizeof(int)) == NULL)) {
			printf("Error: Malloc has failed allocating the board\n");
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}