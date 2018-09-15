#include <stdio.h>
#include <stdlib.h>
#include "game_logic.h"

/* 
		This module implements the sudoku's game logic so the rules may be kept during every turn of the user. 
*/

/* Private functions decleration */

/*
* The Function checks if the value is legal for this row.
* @param row_index - index x.
* @param col_index - index y.
* @param value - The value we want to enter.
* @return -true(1) if the value can be placed in the cell row wise, false(0) otherwise.
*/
int check_row(int row_index, int col_index, int value);

/*
* The Function checks if the value is legal for this col.
* @param row_index - index x.
* @param col_index - index y.
* @param value - The value we want to enter.
* @return -true(1) if the value can be placed in the cell col wise, false(0) otherwise.
*/
int check_col(int row_index, int col_index, int value);

/*
* The Function checks if the value is legal for this block.
* @param row_index - index x.
* @param col_index - index y.
* @param value - The value we want to enter.
* @return -true(1) if the value can be placed in the cell block wise, false(0) otherwise.
*/
int check_block(int row_index, int col_index, int value);

/*
* The Function update the mark_error for all the cells that have a *ROW* connection with cell[x][y].
* @param row_index - index x.
* @param col_index - index y.
*/
void update_errors_row(int row_index);

/*
* The Function update the mark_error for all the cells that have a *COL* connection with cell[x][y].
* @param row_index - index x.
* @param col_index - index y.
*/
void update_errors_col(int col_index);

/*
* The Function update the mark_error for all the cells that have a *BLOCK* connection with cell[x][y].
* @param row_index - index x.
* @param col_index - index y.
*/
void update_errors_block(int row_index, int col_index);

/* Private functions */

int check_col(int row_index, int col_index, int value) {
	int i, board_length;
	board_length = sudoku.block_col_length*sudoku.block_row_length;

	for (i = 0; i < board_length; i++) {
		if (sudoku.board[i][col_index].value == value && i != row_index) {
			return false;
		}
	}
	return true;
}

int check_row(int row_index, int col_index, int value) {
	int i, board_length;
	board_length = sudoku.block_col_length*sudoku.block_row_length;

	for (i = 0; i < board_length; i++) {
		if (sudoku.board[row_index][i].value == value && i != col_index) {
			return false;
		}
	}
	return true;
}

int check_block(int row_index, int col_index, int value) {
	int first_block_index_row, first_block_index_col, i, j;
	first_block_index_row = (row_index / sudoku.block_row_length) * sudoku.block_row_length; /* this is the number of the block reletive to the rows */
	first_block_index_col = (col_index / sudoku.block_col_length) * sudoku.block_col_length; /* this is the number of the block reletive to the rows */

																							 /* Check the block with the boundaries we have set */
	for (i = 0; i < sudoku.block_row_length; i++) {
		for (j = 0; j < sudoku.block_col_length; j++) {
			if (sudoku.board[first_block_index_row + i][first_block_index_col + j].value == value &&
				!(first_block_index_col + j == col_index && first_block_index_row + i == row_index))
				return false;
		}
	}
	return true;
}

void update_errors_col(int col_index) {
	int i, board_length;
	board_length = sudoku.block_col_length*sudoku.block_row_length;

	for (i = 0; i < board_length; i++) {
		if (valid_value(i, col_index, sudoku.board[i][col_index].value))
			sudoku.board[i][col_index].error = false;
		else sudoku.board[i][col_index].error = true;
	}
}

void update_errors_row(int row_index) {
	int i, board_length;
	board_length = sudoku.block_col_length*sudoku.block_row_length;

	for (i = 0; i < board_length; i++) {
		if (valid_value(row_index, i, sudoku.board[row_index][i].value))
			sudoku.board[row_index][i].error = false;
		else sudoku.board[row_index][i].error = true;
	}
}

void update_errors_block(int row_index, int col_index) {
	int first_block_index_row, first_block_index_col, i, j;
	first_block_index_row = (row_index / sudoku.block_row_length) * sudoku.block_row_length; /* this is the number of the block reletive to the rows */
	first_block_index_col = (col_index / sudoku.block_col_length) * sudoku.block_col_length; /* this is the number of the block reletive to the rows */
																							 /* Check the block with the boundaries we have set */
	for (i = 0; i < sudoku.block_row_length; i++) {
		for (j = 0; j < sudoku.block_col_length; j++) {
			if (!(row_index == i + first_block_index_row || col_index == j + first_block_index_col)) { /* we already updated the row and the col */
				if (valid_value(i + first_block_index_row, j + first_block_index_col, sudoku.board[i + first_block_index_row][j + first_block_index_col].value))
					sudoku.board[i + first_block_index_row][j + first_block_index_col].error = false;
				else sudoku.board[i + first_block_index_row][j + first_block_index_col].error = true;
			}
		}
	}
}


/* Public functions */

int valid_value(int row_index, int col_index, int value) {
	if (value == 0) {
		return true;
	}
	return check_row(row_index, col_index, value) &&
		check_col(row_index, col_index, value) &&
		check_block(row_index, col_index, value);
}

void update_errors(int row_index, int col_index) {
	update_errors_row(row_index);
	update_errors_col(col_index);
	update_errors_block(row_index, col_index);
}

void update_board_errors() {
	int i, j, board_len;

	board_len = sudoku.block_row_length * sudoku.block_col_length;

	for (i = 0; i < board_len; i++) {
		for (j = 0; j < board_len; j++) {
			update_errors(i, j);
		}
	}
}



