
#ifndef AUX_MAIN
#define AUX_MAIN

#define false 0
#define true 1

#include "move_list.h"

/* Defining main structs needed for our game */
#ifndef __TYPES_H 
#define __TYPES_H

typedef enum game_mode_t {
	init, solve, edit
} GAME_MODE;

/* a Cell struct
Fixed explanations:
0 - Should be blank \\could be replaced for defult value 0=blank
1 - a fixed cell */
typedef struct cell_t {
	int value;
	int is_fixed;
	int solution;
	int error;
}cell;

typedef struct sudoku_t {
	cell **board;
	int mark_errors;
	int block_col_length; /* Meaning the number of ROWS per block in the game */
	int block_row_length; /* Meaning the number of COLUMNS per block in the game */
	GAME_MODE game_mode; /* The Current game-Mode */
	int num_of_filled_cells;
}sudoku_t;
#endif


extern sudoku_t sudoku;

/*
 *  This function creates a new board which is a copy of the current board which is
 *	being played on when the function is called.
 *	The function assumes that there is a sudoku.board which is initialized and that the other
 *	sudoku's values are valid.
 *	mainly used for the "autofill" or "generate" commands where we need to save a
 *	'before' and 'after' boards. 
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

int free_int_matrix(int** board, int block_col_len, int block_row_len);
/*
 *  This function recieves a pointer to a board that needs to be initialized.
 *	The initialized board has all 0's in its cells.
 *
 *   returns: EXIT_SUCCESS(0) on success.
 *			 on any error returns EXIT_FAILURE(1) and prints the error.
 */
int initialize_new_board(cell** board, int block_col_len, int block_row_len);

/*
 *	This function is called to free leftover stuff when exiting cleanly.
 *	Things that are being free'd:
 *	- 
 */
int free_on_exit();
/*
*	This function is called to update the sudoku.num_of_filled_cells value in the program.
*	
*	prev_val: the value that was previously in the cell that has been updated
*	updated_val: the value that is being updated to.
*/
void update_num_of_filled_cells(int prev_val, int updated_val);

// TODO: add this function to :
//		Edit, Solve, generate, undo, redo, set, autofill.
//		** added:** Edit, Solve, set, autofill

#endif /* AUX_MAIN */