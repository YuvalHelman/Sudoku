#include "aux_main.h"
#include <stdio.h>
#include <stdlib.h>

cell** copy_current_board() {
	cell **current_board, **new_board;
	int i, j, board_size;

	current_board = sudoku.board;
	new_board = NULL;
	board_size = sudoku.block_col_length * sudoku.block_row_length;

	/* Allocate new memmory for the new board */
	if (initialize_new_board(new_board) == EXIT_FAILURE) {
		perror("initializing a new board has failed.");
		return NULL;
	}

	/* Copy the values inside the board */
	for (i = 0; i < board_size; i++) {
		for (j = 0; j < board_size; j++) {
			new_board[i][j].error = sudoku.board[i][j].error;
			new_board[i][j].fixed = sudoku.board[i][j].fixed;
			new_board[i][j].solution = sudoku.board[i][j].solution;
			new_board[i][j].value = sudoku.board[i][j].value;
		}
	}

	return new_board;
}


int free_board(cell** board) {
	int board_size, i;

	if (board == NULL) {
		return EXIT_SUCCESS;
	}

	board_size = sudoku.block_col_length * sudoku.block_row_length;

	for (i = 0; i < board_size; i++) {
		free(board[i]);
	}
	free(board);

	return EXIT_SUCCESS;
}


int initialize_new_board(cell** board) {
	int board_size, i;

	if (board) {
		perror("board should be NULL in order to be initialized. might contain needed data");
		return EXIT_FAILURE;
	}

	board_size = sudoku.block_col_length * sudoku.block_row_length;

	if ((board = (cell **)malloc(sizeof(cell *) * board_size)) == NULL) {
		printf("Error: Malloc has failed\n");
		return EXIT_FAILURE;
	}
	for (i = 0; i < board_size; i++) {
		if ( (board[i] = (cell *)calloc(board_size, sizeof(cell)) == NULL) ) {
			printf("Error: Malloc has failed\n");
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}