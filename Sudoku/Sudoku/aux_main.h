#pragma once

typedef enum { False = 0, True = 1 } bool;
#define false 0
#define true 1

#ifndef __TYPES_H
#define __TYPES_H


/* a Cell struct
Fixed explanations:
0 - Should be blank \\could be replaced for defult value 0=blank
1 - a fixed cell */
typedef struct cell_t {
	int value;
	bool is_fixed;
	int solution;
	int error;
}cell;

typedef struct sudoku_t {
	cell **board;
	int mark_errors;
	int block_col_length;
	int block_row_length;
}sudoku_t;
#endif

typedef enum game_mode_t {
	init, solve, edit
} GAME_MODE;


extern GAME_MODE game_mode;
extern sudoku_t sudoku;

/*
 *  This function creates a new board which is a copy of the current board which is
 *	being played on when the function is called.
 *	The function assumes that there is a sudoku.board which is initialized and that the other
 *	sudoku's values are valid.
 *
 *   returns: a pointer to the newly allocated memmory for the new board.
 *			 on any error returns NULL and prints the error.
 */
cell** copy_current_board();
/*
 *	This function recieves a pointer to a board that needs to be free'd.
 *	Any unsaved data is lost.
 *
 *   returns: EXIT_SUCCESS(0) on success.
 *			 on any error returns EXIT_FAILURE(1) and prints the error.
 */
int free_board(cell** board, int block_col_len, int block_row_len);
/*
 *  This function recieves a pointer to a board that needs to be initialized.
 *	The initialized board has all 0's in its cells.
 *
 *   returns: EXIT_SUCCESS(0) on success.
 *			 on any error returns EXIT_FAILURE(1) and prints the error.
 */
int initialize_new_board(cell** board, int block_col_len, int block_row_len);
