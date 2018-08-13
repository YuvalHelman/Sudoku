#include <stdio.h>
#include <stdlib.h>
#include "aux_main.h"
#include "game_logic.h"







int valid_value(int row_index, int col_index, int value) {
	return check_row(row_index, col_index, value) &&
		check_col(row_index, col_index, value) &&
		check_block(row_index, col_index, value);
}

/* checks if the value can be place in the row */
int check_row(int row_index, int col_index, int value) {
	int i, board_length;
	board_length = sudoku.block_col_length*sudoku.block_row_length;
	for (i = 0;i < board_length;i++) {
		if (sudoku.board[i][col_index].value == value && i != row_index) {
			return false;
		}
	}
	return true;
}

/* checks if the value can be place in the col */
int check_col(int row_index, int col_index, int value) {
	int i, board_length;
	board_length = sudoku.block_col_length*sudoku.block_row_length;
	for (i = 0;i < board_length;i++) {
		if (sudoku.board[row_index][i].value == value && i!=col_index) {
			return false;
		}
	}
	return true;
}

/* checks if the value can be place in the block */
int check_block(int row_index, int col_index, int value) {
	int first_block_index_row, first_block_index_col, i, j;
	first_block_index_row = (row_index / sudoku.block_row_length) * sudoku.block_row_length; /* this is the number of the block reletive to the rows */
	first_block_index_col = (col_index / sudoku.block_col_length) * sudoku.block_col_length; /* this is the number of the block reletive to the rows */
	/* Check the block with the boundaries we have set */
	for (i = 0; i < sudoku.block_row_length; i++) {
		for (j = 0; j < sudoku.block_col_length; j++) {
			if (sudoku.board[first_block_index_row + i][first_block_index_col + j].value == value &&
				!(i == col_index && i == row_index))
				return false;
		}
	}
	return true;
}

void update_errors(int row_index, int col_index) {
	update_errors_row(row_index, col_index);
	update_errors_col(row_index, col_index);
	update_errors_block(row_index, col_index);
}


/* checks if the value can be place in the row */
void update_errors_row(int row_index, int col_index) {
	int i, board_length;
	board_length = sudoku.block_col_length*sudoku.block_row_length;
	for (i = 0;i < board_length;i++) {
		if (valid_value(i, col_index, sudoku.board[i][col_index].value)) 
			sudoku.board[i][col_index].error = false;
		else sudoku.board[i][col_index].error = true;
	}
}

/* checks if the value can be place in the col */
void update_errors_col(int row_index, int col_index) {
	int i, board_length;
	board_length = sudoku.block_col_length*sudoku.block_row_length;
	for (i = 0;i < board_length;i++) {
		if (valid_value(row_index, i, sudoku.board[row_index][i].value)) 
			 sudoku.board[row_index][i].error = false;
		else sudoku.board[row_index][i].error = true;	
	}
}

/* checks if the value can be place in the block */
void update_errors_block(int row_index, int col_index) {
	int first_block_index_row, first_block_index_col, i, j;
	first_block_index_row = (row_index / sudoku.block_row_length) * sudoku.block_row_length; /* this is the number of the block reletive to the rows */
	first_block_index_col = (col_index / sudoku.block_col_length) * sudoku.block_col_length; /* this is the number of the block reletive to the rows */
																							 /* Check the block with the boundaries we have set */
	for (i = 0; i < sudoku.block_row_length; i++) {
		for (j = 0; j < sudoku.block_col_length; j++) {
			if (!(row_index == i || col_index == j)) { /* we already updated the row and the col */
				if (valid_value(i, j, sudoku.board[j][i].value))
					sudoku.board[i][j].error = false;
				else sudoku.board[i][j].error = true;
			}
		}
	}
}