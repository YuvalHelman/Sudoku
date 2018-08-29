#include "aux_main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

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
	int i, str_as_int;
	if (value == NULL) {
		return -1;
	}
	str_as_int = (int)(strtol(value, NULL, 10));
	if (strcmp(value, "0") != 0 && str_as_int == 0) {
		return -1;
	}
	if (errno != 0) {
		perror("strtol function failed.");
		return -1;
	}
	return str_as_int;
}
