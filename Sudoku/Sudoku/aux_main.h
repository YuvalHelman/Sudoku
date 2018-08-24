
#ifndef AUX_MAIN
#define AUX_MAIN

#define false 0
#define true 1

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


/* Global Variables: */
extern sudoku_t sudoku;


/*
*   This function initialize the sudoku.board
*	The initialized board has all 0's in its cells.
*	should be free'd with free_board() function when the board isn't needed anymore.
*
*   returns: EXIT_SUCCESS(0) on success.
*			 on any error returns EXIT_FAILURE(1) and prints the error.
*/
int initialize_new_board(int block_col_len, int block_row_len);


/*
 *	This function free's the game board.
 *	Any unsaved data is lost. any other refference to the sudoku.board after that should be only after
 *	initialize_new_board function is called.
 *
 *   returns: EXIT_SUCCESS(0) on success.
 *			 on any error returns EXIT_FAILURE(1) and prints the error.
 */
int free_board();


/*
*	This function is called to update the sudoku.num_of_filled_cells value in the program.
*	
*	prev_val: the value that was previously in the cell that has been updated
*	updated_val: the value that is being updated to.
*/
void update_num_of_filled_cells(int prev_val, int updated_val);
/* TODO: add this function to :
		Edit, Solve, generate, undo, redo, set, autofill.
		** added:** Edit, Solve, set, autofill */


#endif /* AUX_MAIN */