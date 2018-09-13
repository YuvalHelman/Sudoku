
#ifndef AUX_MAIN
#define AUX_MAIN

#define FAILURE -1
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
*	@block_col_len: the board's block column length
*	@block_row_len: the board's block row length
*
*   @returns: EXIT_SUCCESS(0) on success.
*			 on any error returns EXIT_FAILURE(1) and prints the error.
*/
int initialize_new_board(int block_col_len, int block_row_len);

/*
*   This function changes the sudoku.board values to 0.	
*/
void reset_sudoku_board_values();

/*
*	The Function initializes a new "values_only" board for working on temporary boards.
*
*
*
*	@returns: a pointer to the new board on success.
*	    	 on any error returns NULL and prints the error.
*/
int** initialize_integer_board();

/*
 *	This function free's the game board.
 *	Any unsaved data is lost. any other refference to the sudoku.board after that should be only after
 *	initialize_new_board function is called.
 *
 *  @returns: EXIT_SUCCESS(0) on success.
 *			 on any error returns EXIT_FAILURE(1) and prints the error.
 */
int free_board();

/*
*	This function is called to update the sudoku.num_of_filled_cells value in the program.
*	
*	@prev_val: the value that was previously in the cell that has been updated
*	@updated_val: the value that is being updated to.
*/
void update_num_of_filled_cells(int prev_val, int updated_val);


/*
*	This function convert a string to an integer using the strtol function.
*
*	@value - a represanting string of the number.
*
*	@return: the value if everything worked fine, otherwise -1.
*/
int str_to_num(const char* value);

/*
*	The function free's an integer matrice according to the board_size given.
*
*	@board: the matrice that should be free'd by the function.
*	@block_col_len: the board's block column length
*	@block_row_len: the board's block row length
*
*	returns: EXIT_SUCCESS(0) on success.
*/
int free_int_matrix(int** board, int block_col_len, int block_row_len);

/*
*	The function checks if there are any errors on the current board.
*
*   @returns: true(1) when there are errors on the current board.
*			 false(0) when there aren't errors.
*/
int is_board_erronous();


#endif /* AUX_MAIN */
