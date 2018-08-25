#define _CRT_SECURE_NO_WARNINGS
#define SEEN 1
#define NO_SOLUTION 2
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "game_logic.h"
#include "user_interface.h"
#include "aux_main.h"
#include "move_list.h"
#include "file_manipulation.h"
#include "solver.h"

/*
 *					Private (Static) functions - not available outside this source file.
 */

 /*
 *			Private Helper functions:
 */

/*
*	The Function prints a message accordingly to its arguments regarding a recent redo change in the board.
*	The row\column arguments are the board locations (not the ones we want to print)
*
*	row: the cell's row where the change was made (in the board. 0 <--> board_len-1 )
*	column: the cell's column where the change was made (in the board. 0 <--> board_len-1 )
*	prev_val: the value that was previously in the cell that has been updated
*	updated_val: the value that is being updated to.
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
*	The Function prints a message accordingly to its arguments regarding a recent undo change in the board.
*	The row\column arguments are the board locations (not the ones we want to print)
*
*	row: the cell's row where the change was made (in the board. 0 <--> board_len-1 )
*	column: the cell's column where the change was made (in the board. 0 <--> board_len-1 )
*	prev_val: the value that was previously in the cell that has been updated
*	updated_val: the value that is being updated to.
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

/*
*	This function is used by print_board.
*	The Function prints a separator row accordingly to the current 
*	 sudoku.block_col_length, sudoku.block_row_length global arguments.
*
*/
void separator_row() {
	int i;
	for (i = 0; i <= 4 * sudoku.block_col_length*sudoku.block_row_length + sudoku.block_row_length; i++)
		printf("-");
	printf("\n");
}

/*
*	The Function initializes a new "values_only" board for working on temporary boards.
*
*	block_col_len: the board's block column length
*	block_row_len: the board's block row length
*
*	returns: a pointer to the new board on success.
*	    	 on any error returns NULL and prints the error.
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

/*
*	The function free's an integer matrice according to the board_size given.
*
*	board: the matrice that should be free'd by the function.
*	block_col_len: the board's block column length
*	block_row_len: the board's block row length
*
*	returns: EXIT_SUCCESS(0) on success.
*/
int free_int_matrix(int** board, int block_col_len, int block_row_len) {
	int board_size, i;

	if (board == NULL) {
		return EXIT_SUCCESS;
	}

	board_size = block_col_len * block_row_len;

	for (i = 0; i < board_size; i++) {
		free(board[i]);
	}
	free(board);

	return EXIT_SUCCESS;
}

/*
*	The Function checks if there is only one valid value for cell [row_index][col_index].
*
*	row_index: the board's row of the cell
*	col_index: the board's column of the cell
*
*	returns: the only one valid value (if exists) or 0 , if no value exists, or more then one value exists for this cell.
*/
int one_possible_value(int row_index, int col_index) {
	int i, count, board_length, value;
	count = 0;
	value = 0;
	board_length = sudoku.block_col_length*sudoku.block_row_length;
	for (i = 1; i <= board_length; i++) {
		if (valid_value(row_index, col_index, i)) {
			count++;
			value = i;
			if (count > 1) {
				return 0;
			}
		}
	}
	return value;
}

/*
*	The function checks if the board is empty (all 0's) or not
*
*   returns: true(1) when the board is empty
*			 false(0) otherwise.
*/
int is_board_empty() {
	int i, j, board_size;

	board_size = sudoku.block_col_length * sudoku.block_row_length;

	for (i = 0; i < board_size; i++) {
		for (j = 0; j < board_size; j++) {
			if (sudoku.board[i][j].value != 0) {
				printf("cell %d,%d isn't empty\n", i, j);
				return false;
			}

		}
	}
	return true;
}

/*
*	The Function uses the "move_list" module's functions 'add_new_node' and 'add_val_to_current_node' to add a new node to the move_list.
*	Should be used in "generate", "autofill"
*	The functions receives a temporary matrice with the relavent changes. 
*	It updates the sudoku.board and prints relevant info for the "autofill" command.
*
*	temp_matrice_values: a pointer to the temporary matrice with the values that has been changed. (every cell which has a value other then 0 has been changed)
*	autofill_flag: a boolean indicator. should be sent with "true(1)" when called from the "autofill" function. and "false(0)" otherwise.
*
*   returns: EXIT_SUCCESS(0) on successful load of the file and board.
*	         on any error returns EXIT_FAILURE(1) and prints the error.
*/
int update_board_and_list(int **temp_matrice_values, int autofill_flag) {
	int add_node_flag, col_index, board_length, row_index, updated_val;

	add_node_flag = true;
	/* Copy value from the temp matrix to the board */
	for (col_index = 0; col_index < board_length; col_index++) {
		for (row_index = 0; row_index < board_length; row_index++) {
			if (temp_matrice_values[row_index][col_index] != 0) {
				updated_val = temp_matrice_values[row_index][col_index];
				if (add_node_flag == true) { /* In case this is the first value which is being changed in the board */
					if (add_new_node(row_index, col_index, ZERO, updated_val) == EXIT_FAILURE) {
						printf("adding new node to list failed. Exiting.\n");
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
				if (autofill_flag) {
					printf("Cell <%d,%d> set to %d\n", col_index + 1, row_index + 1, updated_val);
				}

				update_num_of_filled_cells(ZERO, updated_val);
			}
		}
	}

	return EXIT_SUCCESS;
}

/*
*	An helper function for "generate". does the filling of num_of_cells_to_fill random cells with random values
*	then uses ILP to solve the board, and then removes all but 'num_of_cells_to_keep' cells in the board.
*	adds a new Node to the move_list when successful.
*
*	num_of_cells_to_fill: number of cells to fill before solving with ILP
*	num_of_cells_to_clear: number of cells to clear from the solvable board after ILP.
*
*	returns: a pointer to the new board on success.
*	    	 on any error returns EXIT_FAILURE(1) and prints the error.
*			 when ILP fails or a cell has no valid values, return NO_SOLUTION(2).
*/
int generate_a_puzzle(int num_of_cells_to_fill, int num_of_cells_to_keep) {

	int **temp_matrice_values, i, j;
	int rand_row, rand_col, rand_val, board_len, num_of_filled;
	int *optional_values, num_of_options, valid_value_flag, num_of_cells_in_board;

	board_len = sudoku.block_col_length * sudoku.block_row_length;
	num_of_filled = 0;
	optional_values = calloc(board_len, sizeof(int));

	/* Initialize a temp matrix */
	temp_matrice_values = initialize_integer_board(sudoku.block_col_length, sudoku.block_row_length);
	if (!temp_matrice_values) {
		free(optional_values);
		return EXIT_FAILURE;
	}

	/* generate random cells untill 'num_of_cells_to_fill' cells has been filled */
	while(num_of_filled < num_of_cells_to_fill) {
		rand_row = ( rand() % (board_len) ); /* random numbers between 0 ~ board_len-1 */
		rand_col = ( rand() % (board_len) ); /* random numbers between 0 ~ board_len-1 */

		num_of_options = 0;
		valid_value_flag = false;
		if (temp_matrice_values[rand_row][rand_col] == 0) {  /* if random cell hasn't been initialized yet */
			rand_val = (rand() % (board_len) + 1 ); /* random numbers between 1 ~ board_len*/
			
			while (num_of_options != board_len && valid_value_flag == false) {
				if (optional_values[rand_val - 1] != SEEN) {
					if (valid_value(rand_row, rand_col, rand_val)) { /* Not seen. check if valid */
						temp_matrice_values[rand_row][rand_col] = rand_val;
						valid_value_flag = true;
					}
					else { /* Not seen yet, and not valid */
						optional_values[rand_val - 1] == SEEN;
						num_of_options++;
					}
				}
			}
			if (num_of_options != board_len) {
				num_of_filled++;
			}
			else {
				free_int_matrix(temp_matrice_values, sudoku.block_col_length, sudoku.block_row_length);
				return NO_SOLUTION;
			}
		}
	}

	/* Solve the matrice using ILP */
	if (gurobi_initializer(temp_matrice_values) == EXIT_FAILURE) {
		free_int_matrix(temp_matrice_values, sudoku.block_col_length, sudoku.block_row_length);
		free(optional_values);
		return NO_SOLUTION;
	}

	/* remove all but "num_of_cells_to_keep" cells in the board */
	num_of_cells_in_board = board_len * board_len;
	while (num_of_cells_in_board > num_of_cells_to_keep) {
		rand_row = (rand() % (board_len)); /* random numbers between 0 ~ board_len-1 */
		rand_col = (rand() % (board_len)); /* random numbers between 0 ~ board_len-1 */
	
		/* if random cell hasn't been deleted yet */
		if (temp_matrice_values[rand_row][rand_col] != 0) {
			temp_matrice_values[rand_row][rand_col] == 0;
			num_of_cells_in_board--;
		}
	}

	/* Copy the temp_matrice to the sudoku.board and add a new node to the list */
	if (update_board_and_list(temp_matrice_values, false) == EXIT_FAILURE) {
		free_int_matrix(temp_matrice_values, sudoku.block_col_length, sudoku.block_row_length);
		free(optional_values);
		return EXIT_FAILURE;
	}

	free_int_matrix(temp_matrice_values, sudoku.block_col_length, sudoku.block_row_length);
	free(optional_values);
	return EXIT_SUCCESS;

}

/*
*			DEBUGGING functions. public for debugging usage.
*/


void print_board_values() {
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

			if (sudoku.board[i][j].is_fixed) {				/* If fixed number */

				printf(" %2d.", sudoku.board[i][j].solution); /* DOT for fixed number*/
			}
			else if (!sudoku.board[i][j].is_fixed) { /* Non-fixed number that the user inputed */
				printf(" %2d", sudoku.board[i][j].solution);
				if (sudoku.board[i][j].error) /* check if we need to mark an error */
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
*	The Function prints the solution board to the console.
*
*	Copied from print_board, but draws the solution board, and not the game board.
*
*/
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

			if (sudoku.board[i][j].is_fixed) {				/* If fixed number */

				printf(" %2d.", sudoku.board[i][j].solution); /* DOT for fixed number*/
			}
			else if (!sudoku.board[i][j].is_fixed) { /* Non-fixed number that the user inputed */
				printf(" %2d", sudoku.board[i][j].solution);
				if ( sudoku.board[i][j].error) /* check if we need to mark an error */
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
*			Private main functions: used for user commands
*/

/*
*   Starts a puzzle in Solve mode, loaded from a file with the name "filepath".
*   "filepath" can be a full or relative path to the file.
*	We assume the file contains valid data and is correctly formatted.
*	Available from Solve,Edit,Init. 
*
*   filepath: a full or relative path to the file being opened.
*
*   returns: EXIT_SUCCESS(0) on successful load of the file and board.
*	         on any error returns EXIT_FAILURE(1) and prints the error.
*/
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
	free_board();
	sudoku.board = NULL;

	/* Read from the file and initialize the board and sudoku's block_col/row lengths */
	if (read_from_file(fd, &block_rows, &block_cols, &num_of_filled_cells) == EXIT_FAILURE) {
		free_board();
		return EXIT_FAILURE;
	}

	/* Set basic sudoku utilities */
	sudoku.block_row_length = block_rows;
	sudoku.block_col_length = block_cols;
	sudoku.num_of_filled_cells = num_of_filled_cells;

	/* restart Sudoku's essential variables */
	sudoku.mark_errors = 0;

	print_board();

	fclose(fd);

	return EXIT_SUCCESS;
}

/*
*   Starts a puzzle in Edit mode, loaded from a file with the name "filepath".
*   "filepath" can be a full or relative path to the file.
*	If no paramater is passed. the program initiates with an empty 9x9 board.
*	We assume the file contains valid data and is correctly formatted.
*	Available from Solve,Edit,Init.
*
*   filepath: a full or relative path to the file being opened.
*
*   returns: EXIT_SUCCESS(0) on adding a new node.
*	         on any error returns EXIT_FAILURE(1) and prints the error.
*/  
int Edit(char* filepath) {
	FILE* fd;
	int block_rows, block_cols, num_of_filled_cells;

	/* Change the game mode */
	sudoku.game_mode = edit;
	num_of_filled_cells = 0;

	/* Reset basic game utilities (case f) */
	delete_list_full();
	free_board();
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
			return EXIT_FAILURE;
		}

		/* Set basic sudoku utilities */
		sudoku.block_row_length = block_rows;
		sudoku.block_col_length = block_cols;
		sudoku.num_of_filled_cells = num_of_filled_cells;

		fclose(fd);
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

/*
*	The Function receives an integer and if it's 0\1 changes the game's mark_errors field accordingly.
*
*	 value: the integer which decides if errors should be marked or not
*
*/
void mark_errors(int value) {
	if (value != 0 && value != 1) {
		printf("Error: the value should be 0 or 1\n");
	}
	else sudoku.mark_errors = value;
}

/*
*	The Function prints the board to the console accordingly to the standard.
*
*	where board has values of 0, it prints a blank space, as the cell is empty.
*	when mark_errors is ON, print asterisk before the number.
*	fixed cells are printed with a dot before the number.
*
*/
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

				printf(" %2d.", sudoku.board[i][j].value); /* DOT for fixed number*/
			}
			else if (!sudoku.board[i][j].is_fixed) { /* Non-fixed number that the user inputed */
				printf(" %2d", sudoku.board[i][j].value);
				if ( (sudoku.game_mode == edit || sudoku.mark_errors) && sudoku.board[i][j].error) /* check if we need to mark an error */
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

/*	Sets the value of cell <col_index,row_index> to value
*	only available in Edit and Solve modes.
*	This command prints the game board
*	In solve Mode, checks if all board's cells are filled, validates and prints an according message
*
*	The function updates the linked list with the cells that were changed.
*
*	col_index: the cell's column where the change is made (as the user inputted them. 1 <--> board_len )
*	row_index: the cell's row where the change is made (as the user inputted them. 1 <--> board_len )
*	value: the value that that will be put in the cell.
*
*   returns: EXIT_SUCCESS(0) on adding a new node. ////// TODO: check if this is the right returning method 
*	         on any error returns EXIT_FAILURE(1) and prints the error.
*/
int set(int col_index, int row_index, int value) { /* TODO: check if return values could be changed to EXIT_SUCCESS \ EXIT_FAILURE. */

	int prev_val, updated_val, board_len, num_of_cells;
	int row_index_board, col_index_board;

	board_len = sudoku.block_row_length * sudoku.block_col_length;
	num_of_cells = board_len * board_len;

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

	prev_val = sudoku.board[row_index_board][col_index_board].value;


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

/*	
*	Validates the current board using ILP, ensuring it is solvable.
*	available in Edit and Solve modes.
*	
*	the function uses the Gurobi ILP solver in the solver.c function in order to validate and get a solution.
*
*   returns: EXIT_SUCCESS(0) on adding a new node. ////// TODO: check if this is the right returning method
*	         on any error returns EXIT_FAILURE(1) and prints the error.
*/
int validate() {

	/*
	if (is_board_erronous() ) { 
		printf("Error: board contains erroneous values\n");
		return EXIT_SUCCESS;
	}

	if (is_solvable() == true) {
		printf("Validation passed: board is solvable\n");
	}
	else {
		printf("Validation failed: board is unsolvable\n");
	}
	*/

	is_solvable(NULL);
	/* gurobi_initializer();*/

	printf("------\n------Board values:\n");
	print_board_values();
	printf("------\n------Board Solution:\n");
	print_board_solution(); /* TODO: erase this before done */

	return EXIT_SUCCESS;
}

/*
*	Generates a puzzle by randomly filling X cells with random legal values,
*	running ILP to solve the resulting board, and then clearing all but Y random
*	cells.
*
*	 only available in Edit mode
*
*	num_of_cells_to_fill: number of cells to fill before solving with ILP
*	num_of_cells_to_clear: number of cells to clear from the solvable board after ILP.
*
*	returns: a pointer to the new board on success.
*	    	 on any error returns NULL and prints the error.
*/
int generate(int num_of_cells_to_fill, int num_of_cells_to_keep) {
	int DIM, num_of_tries, ret_val, generate_success_flag;

	DIM = sudoku.block_col_length * sudoku.block_row_length;
	num_of_tries = 1000;

	if (num_of_cells_to_fill > DIM*DIM ||
		num_of_cells_to_fill < 0 ||
		num_of_cells_to_keep > DIM*DIM || 
		num_of_cells_to_keep < 0) {
		printf("Error: value not in range 0-%d\n", DIM*DIM); /* case d */
		return EXIT_FAILURE;
	}

	if ( ! is_board_empty()) {
		printf("Error: board is not empty\n");
		return EXIT_SUCCESS;
	}

	generate_success_flag = false;
	while (num_of_tries > 0 && (!generate_success_flag) ) {

		ret_val = generate_a_puzzle(num_of_cells_to_fill, num_of_cells_to_keep); /* helper function to generate a board */

		if (ret_val = EXIT_SUCCESS) {
			generate_success_flag = true;
		}
		else if (ret_val == NO_SOLUTION) {
			num_of_tries--;
		}
		else if (ret_val = EXIT_FAILURE) {
			printf("Error: Generate has failed. Exiting\n");
		}

		if (num_of_tries == 0) {
			printf("Error: puzzle generator failed\n");
			return EXIT_FAILURE;
		}
	}

	print_board();
	return EXIT_SUCCESS;
	
}

/*
*	Undo's a move done by the user (when possible) and updates the board accordingly.
*	only available in Edit and Solve modes. 
*	Uses the undo_list function in order to traverse the moves_list.
*
*/
int undo() {

	int num_of_values, i, row, col, prev, updated;
	node_vals* values_array;
	int reset_flag;

	reset_flag = false;

	values_array = undo_list(&num_of_values, reset_flag);

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

/*
*	Redo's a move done by the user (when possible) and updates the board accordingly.
*	only available in Edit and Solve modes.
*	Uses the redo_list function in order to traverse the moves_list.
*/
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

/*
*   Saves a current board to a file with the name "filepath".
*   "filepath" can be a full or relative path to the file.
*	Available from Solve,Edit modes.
*
*	In edit mode, the board is validated and saved only if valid.
*	All cells are saved as 'fixed'.
*
*	In solve mode, the board can be saved with mistakes.
*	Only cells that were fixed at loading the board are saved as fixed.
*	
*   filepath: a full or relative path to the file being opened.
*
*   returns: EXIT_SUCCESS(0) on adding a new node.
*	         on any error returns EXIT_FAILURE(1) and prints the error.
*/
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
	fclose(fd);
	return EXIT_SUCCESS;
}

/*
*	The Function prints a hint to the user regarding the cell <column,row>.
*	Available in Solve Mode only.
*
*	The function uses the ILP (validate() ) to solve the board, and prints the value of 
*	the cell in the solution given.
*
*	col_index: the cell's column for the hint (as the user inputted them. 1 <--> board_len )
*	row_index: the cell's row for the hint (as the user inputted them. 1 <--> board_len )
*
*   returns: EXIT_SUCCESS(0) on adding a new node.
*	         on any error returns EXIT_FAILURE(1) and prints the error.
*/
int hint(int column, int row) {


	return 0;
}

/*
*	The Function prints the number of solutions for the current board.
*	Available in Solve\Edit modes.
*
*	The function uses an exhaustive backtracking implemented in stack.h
*	the cell in the solution given.
*
*   returns: EXIT_SUCCESS(0) on adding a new node.
*	         on any error returns EXIT_FAILURE(1) and prints the error.
*/
int num_solutions() {

	return 0;
}

/*
*	The Function fills the board's cells which has only a single legal value.
*	Available in Solve mode only.
*
*	The function updates the linked list with the cells that were changed.
*
*   returns: EXIT_SUCCESS(0) on adding a new node.
*	         on any error returns EXIT_FAILURE(1) and prints the error.
*/
int autofill() {
	int updated_val, add_node_flag;
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

	/* Copy the temp_matrice to the sudoku.board and add a new node to the list 
	   Also prints the changes to the std-output */
	if (update_board_and_list(temp_matrice_values, true) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	free_int_matrix(temp_matrice_values, sudoku.block_col_length, sudoku.block_row_length);

	print_board(); /* case g */

	return EXIT_SUCCESS;
}

/*
*	The Function reverts the board to its original loaded state.
*	Available in Edit/Solve modes.
*
*	The function goes over the entire move_list - undo's all moves and deletes all nodes.
*
*   returns: EXIT_SUCCESS(0) on adding a new node.
*	         on any error returns EXIT_FAILURE(1) and prints the error.
*/
int reset() {

	int num_of_values, i, row, col, prev, updated;
	node_vals* values_array;
	int reset_flag;

	reset_flag = true;
	do { /* Initiate undo on the list until the head */
		values_array = undo_list(&num_of_values, reset_flag);

		if (values_array) {
			/* Update the board to the values in the previous turn */
			for (i = 0; i < num_of_values; i++) {
				row = values_array[i].row;
				col = values_array[i].column;
				prev = values_array[i].prev_val;
				updated = values_array[i].updated_val;

				sudoku.board[row][col].value = prev; /* Update the board accordingly */
			}
		}
	} while (values_array);

	/* Delete the list except the head */
	delete_list_from_the_current_node();

	printf("Board reset\n");

	return EXIT_SUCCESS;
}

/*
*	The Function free's all memory resources that are open and terminates the program.
*
*   returns: EXIT_SUCCESS(0) on exiting gracefully.
*/
int Exit() {
	free_board();
	delete_list_on_exit();

	printf("Exiting...\n");

	/* Terminate the program */
	exit(EXIT_SUCCESS);

}


/*
*			private Helper functions: Function used for parsing.
*/

/*
*	The Function converts a string to one of the possbilties in the enum sudokuCommands.
*
*	 str: the given string which will be converted.
*
*	returns: the matching sudokuCommands(enum).
*/
sudokuCommands str2enum(const char *str)
{
	int j;
	for (j = 0; j < sizeof(conversion) / sizeof(conversion[0]); ++j)
		if (!strcmp(str, conversion[j].str))
			return conversion[j].val;
	return error_command;
}

/* TODO: document
* The Function recives the command from the user and interprets it to a function that handles the command.
************************* TODO: The function should check if the buffer is even valid, or to accept only valid ones*****
* @param buffer - the user's command. (its contents may be erased after calling this function)
* @return true(1) when no errors. 0 otherwise
false(0) if some error occured.
* buffer is destroyed after this function.
*/
int user_command(char* buffer) {
	/* */
	sudokuCommands sudoku_command;
	char *xchar, *ychar, *zchar, *command;
	int xchar_asInt, ychar_asInt, zchar_asInt, DIM;
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
		if (sudoku.game_mode != solve || (!xchar) ) {
			printf("ERROR: invalid command\n"); /* case b */
		}
		else {
			errno = 0;
			xchar_asInt = (int)(strtol(xchar, NULL, BASE10));
			if (errno != 0) {
				perror("strtol function failed.");
				return EXIT_FAILURE;
			}
			mark_errors(xchar_asInt);
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
			DIM = sudoku.block_col_length * sudoku.block_row_length;

			errno = 0;
			xchar_asInt = (int)(strtol(xchar, NULL, BASE10));
			if (errno == ERANGE || errno == EINVAL) {
				printf("Error: value not in range 0-%d", DIM*DIM);
				return EXIT_FAILURE;
			}
			errno = 0;
			ychar_asInt = (int)(strtol(ychar, NULL, BASE10));
			if (errno == ERANGE || errno == EINVAL) {
				printf("Error: value not in range 0-%d", DIM*DIM);
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


/*
*			Public functions: used outside this source file
*/

int get_command_and_parse() {
	
	int ret_command;
	char command[MAX_COMMAND_SIZE];
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

		/*printf("board Solution:\n");*/
		/* DEBUG: print_board_solution();*/

		
	} while (fgets_ret != NULL);

	return EXIT_SUCCESS;
	
}










