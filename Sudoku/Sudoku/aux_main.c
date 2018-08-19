#include "aux_main.h"
#include <stdio.h>
#include <stdlib.h>


sudoku_t sudoku = { 0 }; /* All fields initialized to 0.
						 game_mode is also 0 (which is init) */


int initialize_new_board(int block_col_len, int block_row_len) {
	int board_size, i;


	board_size = block_col_len * block_row_len;

	sudoku.board = (cell **)malloc(board_size * sizeof(cell *));
	if (!sudoku.board) {
		printf("Error: Malloc has failed allocating the board\n");
		return EXIT_FAILURE;
	}
	for (i = 0; i < board_size; i++) {
		(sudoku.board[i] = (cell *)calloc(board_size, sizeof(cell)));
		if (!(sudoku.board[i])) {
			printf("Error: Calloc has failed allocating the board\n");
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

/*
cell** copy_current_board() {
	cell **current_board, **new_board;
	int i, j, board_size;

	current_board = sudoku.board;
	new_board = NULL;
	board_size = sudoku.block_col_length * sudoku.block_row_length;


	if (initialize_new_board(new_board, sudoku.block_col_length, sudoku.block_row_length) == EXIT_FAILURE) {
		perror("initializing a new board has failed.");
		return NULL;
	}


	for (i = 0; i < board_size; i++) {
		for (j = 0; j < board_size; j++) {
			new_board[i][j].error = sudoku.board[i][j].error;
			new_board[i][j].is_fixed = sudoku.board[i][j].is_fixed;
			new_board[i][j].solution = sudoku.board[i][j].solution;
			new_board[i][j].value = sudoku.board[i][j].value;
		}
	}

	return new_board;
}
*/

int free_board(cell** board, int block_col_len, int block_row_len) {
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




int free_on_exit() {
	
	/* delete list stuff */
	
	return 0;

}

void update_num_of_filled_cells(int prev_val, int updated_val) {

	if (prev_val != updated_val) {
		if (prev_val == 0) { /* + updated_val isn't 0 */
			sudoku.num_of_filled_cells++;
		}
		if (updated_val == 0) { /* + prev_val isn't 0 */
			sudoku.num_of_filled_cells--;
		}
	}
	/* else, if changed to same value.. nothing changes regarding the num_of_filled_cells */
}

