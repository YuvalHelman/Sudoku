#include "aux_main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#define DEFAULT_BASE 10

/* Global Variables: */
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

int** initialize_integer_board() {
	int board_size, i;
	int **board;

	board_size = sudoku.block_col_length * sudoku.block_row_length;

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

int free_board() {
	int board_size, i;

	if (sudoku.board == NULL || sudoku.block_col_length == 0 || sudoku.block_row_length == 0) {
		return EXIT_SUCCESS;
	}

	board_size = sudoku.block_col_length * sudoku.block_row_length;

	for (i = 0; i < board_size; i++) {
		free(sudoku.board[i]);
	}
	free(sudoku.board);

	return EXIT_SUCCESS;
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

int str_to_num(const char* value) {
	int str_as_int;
	errno = 0;
	if (value == NULL) {
		return FAILURE;
	}
	str_as_int = (int)(strtol(value, NULL, 10));
	if (errno == ERANGE) {
		printf("strtol range error");
		errno = 0;
		return FAILURE;
	}
	if (strcmp(value, "0") != 0 && str_as_int == 0) {
		return FAILURE;
	}
	
	return str_as_int;
}

void reset_sudoku_board_values() {
	int i, j, board_length;

	board_length = sudoku.block_col_length * sudoku.block_row_length;

	for (i = 0; i < board_length; i++) {
		for (j = 0; j < board_length; j++) {
			sudoku.board[i][j].value = 0;
		}
	}

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
