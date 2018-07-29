#include <stdio.h>
#include <stdlib.h>
#include "game_logic.h"


/*
* The Function checks if the value can be stored in the cell
* @param sudoku - a struct inckuding all the properties of the game.
* @param row_index - the row of the cell we are checking
* @param col_index - the column of the cell we are checking
* @param value - The value we want to enter
* @return -1(true) if the value can be placed in the cell, 0(false) otherwise
*/
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
			if (valid_value(j, i, sudoku.board[j][i].value))
				sudoku.board[j][i].error = false;
			else sudoku.board[j][i].error = true;
		}
	}
}
