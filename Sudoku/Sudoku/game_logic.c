#include <stdio.h>
#include <stdlib.h>
#include "game_logic.h"


/*
* The Function checks if the value can be stored in the cell
* @param sudoku_game - a struct inckuding all the properties of the game.
* @param row_index - the row of the cell we are checking
* @param col_index - the column of the cell we are checking
* @param value - The value we want to enter
* @return -1(true) if the value can be placed in the cell, 0(false) otherwise
*/
bool valid_value(sudoku *sudoku_game, int row_index, int col_index, int value) {
	return check_row(sudoku_game, row_index, col_index, value) &&
		check_col(sudoku_game, row_index, col_index, value) &&
		check_block(sudoku_game, row_index, col_index, value);
}

/* checks if the value can be place in the row */
bool check_row(sudoku *sudoku_game, int row_index, int col_index, int value) {
	int i, board_length;
	board_length = sudoku_game->block_col_length*sudoku_game->block_row_length;
	for (i = 0;i < board_length;i++) {
		if (sudoku_game->board[i][col_index].value == value && i != row_index) {
			return false;
		}
	}
	return true;
}

/* checks if the value can be place in the col */
int check_col(sudoku *sudoku_game, int row_index, int col_index, int value) {
	int i, board_length;
	board_length = sudoku_game->block_col_length*sudoku_game->block_row_length;
	for (i = 0;i < board_length;i++) {
		if (sudoku_game->board[row_index][i].value == value && i!=col_index) {
			return 0;
		}
	}
	return 1;
}

/* checks if the value can be place in the block */
int check_block(sudoku *sudoku_game, int row_index, int col_index, int value) {
	int first_block_index_row, first_block_index_col, i, j;
	first_block_index_row = (row_index / sudoku_game->block_row_length) * sudoku_game->block_row_length; /* this is the number of the block reletive to the rows */
	first_block_index_col = (col_index / sudoku_game->block_col_length) * sudoku_game->block_col_length; /* this is the number of the block reletive to the rows */
	/* Check the block with the boundaries we have set */
	for (i = 0; i < sudoku_game->block_row_length; i++) {
		for (j = 0; j < sudoku_game->block_col_length; j++) {
			if (sudoku_game->board[first_block_index_row + i][first_block_index_col + j].value == value &&
				!(i == col_index && i == row_index))
				return 0;
		}
	}
	return 1;
}
