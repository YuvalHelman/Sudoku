#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "user_interface.h"
#include "game_logic.h"
#include "aux_main.h"
#include "move_list.h"
#include "file_manipulation.h"
#include "solver.h"

void redo_print(int row, int column, int prev_val, int updated_val);
void undo_print(int row, int column, int prev_val, int updated_val);

/* TODO: implement these */
int Exit();
int generate();
int hint();
int mark_error();
int num_solutions();
int reset();
int validate();

int Exit() { return 0; }
int generate() { return 0; }
int hint() { return 0; }
int mark_error() { return 0; }
int num_solutions() { return 0; }
int reset() { return 0; }
int validate() {
	gurobi_initializer();
	
	return 0;
}


/*
*  This function recieves a pointer to an integer matrix that needs to be initialized.
*	The initialized matrix has all 0's in its cells.
*
*   returns: a pointer to the board on success.
*			 on any error returns NULL and prints the error.
*/
int** initialize_integer_board(int block_col_len, int block_row_len) {
	int board_size, i;
	int **board;

	board_size = block_col_len * block_row_len;

	board = (int **)malloc(board_size * sizeof(int *));
	if (!board) {
		printf("Error: Malloc has failed allocating the board\n");
		return NULL;
	}
	for (i = 0; i < board_size; i++) {
		board[i] = (int *)calloc(board_size, sizeof(int));
		if (!(board[i])) {
			printf("Error: Malloc has failed allocating the board\n");
			return NULL;
		}
	}

	return board;
}


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
			printf("Exiting...\n");
			return EXIT_SUCCESS;
		}

		printf("Enter your command:\n");
		fgets_ret = fgets(command, MAX_COMMAND_SIZE, stdin);
		ret_command = user_command(command);

		if (fgets_ret == NULL && ferror(stdin)) { /* fgets ancountered some error */
			perror("Error: fgets has failed.");
			return EXIT_FAILURE;
		}

		printf("board Solution:\n");
		print_board_solution();

		
	} while (fgets_ret != NULL);

	return EXIT_SUCCESS;
	
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
		return Solve(xchar);	
	break;
	case edit_command:
		return Edit(xchar);
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
		else { 
			errno = 0;
			xchar_asInt = (int)(strtol(xchar, NULL, BASE10));
			if (errno == ERANGE || errno == EINVAL) {
				perror("strtol function failed.");
				return EXIT_FAILURE;
			}
			errno = 0;
			ychar_asInt = (int)(strtol(ychar, NULL, BASE10));
			if (errno == ERANGE || errno == EINVAL) {
				perror("strtol function failed.");
				return EXIT_FAILURE;
			}
			generate(xchar_asInt, ychar_asInt);
		}
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
			Save(xchar);
	break;
	case hint_command:
		if (sudoku.game_mode != solve)
			printf("ERROR: invalid command\n"); /* case b */
		else {
			errno = 0;
			xchar_asInt = (int)(strtol(xchar, NULL, BASE10));
			if (errno == ERANGE || errno == EINVAL) {
				perror("strtol function failed.");
				return EXIT_FAILURE;
			}
			errno = 0;
			ychar_asInt = (int)(strtol(ychar, NULL, BASE10));
			if (errno == ERANGE || errno == EINVAL) {
				perror("strtol function failed.");
				return EXIT_FAILURE;
			}
			hint(xchar_asInt, ychar_asInt);
		}
	break;
	case num_solutions_command:
		if (sudoku.game_mode == init)
			printf("ERROR: invalid command\n"); /* case b */
		else num_solutions();
	break;
	case autofill_command:
		if (sudoku.game_mode != solve)
			printf("ERROR: invalid command\n"); /* case b */
		else {
			autofill();
		}
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
	int num_of_filled_cells;
	int block_rows, block_cols;

	/* Change the game mode */
	sudoku.game_mode = solve;
	num_of_filled_cells = 0;

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
	if (read_from_file(fd, &block_rows, &block_cols, &num_of_filled_cells) == EXIT_FAILURE) {
		free_board(sudoku.board, block_cols, block_rows);
		return EXIT_FAILURE;
	}

	/* Set basic sudoku utilities */
	sudoku.block_row_length = block_rows;
	sudoku.block_col_length = block_cols;
	sudoku.num_of_filled_cells = num_of_filled_cells;

	/* restart Sudoku's essential variables */
	sudoku.mark_errors = 0;

	print_board();

	return EXIT_SUCCESS;
}

int Edit(char* filepath) {
	FILE* fd;
	int block_rows, block_cols, num_of_filled_cells;

	/* Change the game mode */
	sudoku.game_mode = edit;
	num_of_filled_cells = 0;

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
		if (read_from_file(fd, &block_rows, &block_cols, &num_of_filled_cells) == EXIT_FAILURE) {
			free_board(sudoku.board, block_cols, block_rows);
			return EXIT_FAILURE;
		}

		/* Set basic sudoku utilities */
		sudoku.block_row_length = block_rows;
		sudoku.block_col_length = block_cols;
		sudoku.num_of_filled_cells = num_of_filled_cells;
	}
	else {
		if (initialize_new_board(DEFAULT_BLOCK_LEN, DEFAULT_BLOCK_LEN) == EXIT_FAILURE) {
			return EXIT_FAILURE;
		}

		sudoku.block_row_length = DEFAULT_BLOCK_LEN;
		sudoku.block_col_length = DEFAULT_BLOCK_LEN;
		sudoku.num_of_filled_cells = 0;
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

	int prev_val, updated_val, board_len, num_of_cells;
	int row_index_board, col_index_board;

	board_len = sudoku.block_row_length * sudoku.block_col_length;
	num_of_cells = board_len * board_len;
	prev_val = sudoku.board[row_index-1][col_index-1].value;
	updated_val = value;
	
	/* check if the values are legal */
	if (value < 0 || value > board_len ||
		row_index < 1 || row_index > board_len ||
		col_index < 1 || col_index > board_len) {
		printf("Error: value not in range 0-%d\n", board_len);
		return false;
	}

	row_index_board = row_index - 1;
	col_index_board = col_index - 1;

	/* check if (i,j) is a fixed cell (case e) */
	if (sudoku.board[row_index_board][col_index_board].is_fixed) { /* it is fixed.*/
		printf("Error: cell is fixed\n");
		return false;
	}

	/* check if (i,j) is a fixed cell (case e) */
	if (sudoku.board[row_index_board][col_index_board].is_fixed) { /* it is fixed.*/
		printf("Error: cell is fixed\n");
		return false;
	}

	/* Update the value in the board itself and the number of filled cells */
	sudoku.board[row_index_board][col_index_board].value = value;
	update_num_of_filled_cells(prev_val, updated_val);

	/* Update the move_list. (Case f,  */
	if (add_new_node(row_index_board, col_index_board, prev_val, updated_val) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	/* update errors for all relevant cells */
	update_errors(row_index_board, col_index_board); 

	print_board();
	
	/* Validate the board, and print this if it's not valid: (case c)*/
	/* TODO TODO */
	if (sudoku.game_mode == solve && sudoku.num_of_filled_cells == num_of_cells) {
		/*
		if( validate() == EXIT_SUCCESS ) {
			printf("Puzzle solved successfully\n");
			sudoku.game_mode = init;
		}
		else {
			printf("Puzzle solution erroneous\n");
		}
		*/
	}
	
	return EXIT_SUCCESS;
}


int undo() {

	int num_of_values, i, row, col, prev, updated;
	node_vals* values_array;

	values_array = undo_list(&num_of_values);

	if (values_array) {
		/* Update the board to the values in the previous turn */
		for (i = 0; i < num_of_values; i++) {
			row = values_array[i].row;
			col = values_array[i].column;
			prev = values_array[i].prev_val;
			updated = values_array[i].updated_val;

			sudoku.board[row][col].value = prev;
		}

		/* Print the board and then the changed cells (case e) */
		print_board();
		for (i = 0; i < num_of_values; i++) {
			row = values_array[i].row;
			col = values_array[i].column;
			prev = values_array[i].prev_val;
			updated = values_array[i].updated_val;

			undo_print(row, col, prev, updated);
		}
	}

	return EXIT_SUCCESS;
}


int redo() {
	int num_of_values, i, row, col, prev, updated;
	node_vals* values_array;

	values_array = redo_list(&num_of_values);

	if (values_array) {
		/* Update the board to the values in future turn */
		for (i = 0; i < num_of_values; i++) {
			row = values_array[i].row;
			col = values_array[i].column;
			prev = values_array[i].prev_val;
			updated = values_array[i].updated_val;

			sudoku.board[row][col].value = updated;
		}

		print_board();
		/* Print the changed cells (case e) */
		for (i = 0; i < num_of_values; i++) {
			row = values_array[i].row;
			col = values_array[i].column;
			prev = values_array[i].prev_val;
			updated = values_array[i].updated_val;

			redo_print(row, col, prev, updated);
		}
	}

	return EXIT_SUCCESS;
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
	int updated_val ,add_node_flag;
	int row_index, col_index, board_length, value, **temp_matrice_values;
	board_length = sudoku.block_col_length*sudoku.block_row_length;

	temp_matrice_values = NULL;

	/* Validate the board, and print this if it's not valid: (case c)*/
	/* TODO TODO 
		if( validate() == EXIT_SUCCESS ) {
			printf("Error: board contains erroneous values\n");
			return EXIT_SUCCESS;
		}
	*/

	/* Initialize a temp matrix */
	temp_matrice_values = initialize_integer_board(sudoku.block_col_length, sudoku.block_row_length);
	if (!temp_matrice_values) {
		return EXIT_FAILURE;
	}


	for (col_index = 0; col_index < board_length; col_index++) {
		for (row_index = 0; row_index < board_length; row_index++) {
			if (!sudoku.board[row_index][col_index].value) { /* check if the cell is unfilled (value=0) */
				value = one_possible_value(row_index, col_index);  /* checks that there is only 1 valid value */
				temp_matrice_values[row_index][col_index] = value;
			}
		}
	}

	add_node_flag = true;
	/* Copy value from the temp matrix to the board */
	for (col_index = 0; col_index < board_length; col_index++) {
		for (row_index = 0; row_index < board_length; row_index++) {
			if (sudoku.board[row_index][col_index].value == 0) {
				updated_val = temp_matrice_values[row_index][col_index];
				if (add_node_flag == true) {
					if (add_new_node(row_index, col_index, ZERO, updated_val) == EXIT_FAILURE) {
						printf("add new node failed. Exiting.\n");
						return EXIT_FAILURE;
					}
					add_node_flag = false;
				}
				else {
					if (add_val_to_current_node(row_index, col_index, ZERO, updated_val) == EXIT_FAILURE) {
						printf("add new value to existing node failed. Exiting.\n");
						return EXIT_FAILURE;
					}
				}
				/* Update the value in the board, and print a message regarding */
				sudoku.board[row_index][col_index].value = updated_val;
				printf("Cell <%d,%d> set to %d\n", col_index, row_index, updated_val);

				update_num_of_filled_cells(ZERO, updated_val);
			}
		}
	}

	free_int_matrix(temp_matrice_values, sudoku.block_col_length, sudoku.block_row_length);

	print_board(); /* case g */

	return EXIT_SUCCESS;
}

int one_possible_value(int row_index, int col_index) {
	int i, count, board_length, value;
	count = 0;
	value = 0;
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
*
*	The function handles the printing of the redo function.
*
*/
void redo_print(int row, int column, int prev_val, int updated_val) {
	if (updated_val == 0) {
		if (prev_val == 0) {
			printf("Redo %d,%d from %c to %c\n",
				row, column, '_', '_');
		}
		else {
			printf("Redo %d,%d from %d to %c\n",
				row, column, prev_val, '_');
		}
	}
	else {
		if (prev_val == 0) { /* updated not 0 , prev = 0 */
			printf("Redo %d,%d from %c to %d\n",
				row, column, '_', updated_val);
		}
		else {
			printf("Redo %d,%d from %d to %d\n",
				row, column, prev_val, updated_val);
		}
	}
}


/*
 *	The function handles the printing of the undo function.
 *
 */
void undo_print(int row, int column, int prev_val, int updated_val) {
	if (updated_val == 0) {
		if (prev_val == 0) {
			printf("Undo %d,%d from %c to %c\n",
				row, column, '_', '_');
		}
		else {
			printf("Undo %d,%d from %c to %d\n",
				row, column, '_', prev_val);
		}
	}
	else {
		if (prev_val == 0) {
			printf("Undo %d,%d from %d to %c\n",
				row, column, updated_val, '_');
		}
		else {
			printf("Undo %d,%d from %d to %d\n",
				row, column, updated_val, prev_val);
		}
	}
}
