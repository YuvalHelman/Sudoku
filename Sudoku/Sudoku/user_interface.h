

#define DEFAULT_BLOCK_LEN 3
#define MAX_COMMAND_SIZE 260
#define BASE10 10
#define ZERO 0
#define NON_ZERO 1

typedef enum {solve_command, edit_command, mark_errors_command,
	print_board_command, set_command, validate_command, generate_command,
	undo_command, redo_command, save_command, hint_command,
	num_solutions_command, autofill_command, reset_command, exit_command, error_command
} sudokuCommands;

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


#ifndef NODE_STRUCT
#define NODE_STRUCT
/* A Node struct that will hold relavent info for a user move in the linked list
- row,column specify which cell was modified in this move.
- prev_val specifies the value that was at the cell before the turn.
- updated_val specifies the value that was at the cell after the turn.
*/
typedef struct node_t {
	struct node_t *next;
	struct node_t *prev;
	/* Variables that defines a "move" within the board: */
	unsigned int row;
	unsigned int column;
	int prev_val;
	int updated_val;
	/* A whole board saving for the "autofill" command */
	cell **prev_board;
	cell **updated_board;
} Node;
#endif


static struct {
	sudokuCommands val;
	const char *str;
} conversion[] = {
	{ 0, "solve" },
	{ 1, "edit" },
	{ 2, "mark_errors" },
	{ 3, "print_board" },
	{ 4, "set" },
	{ 5, "validate" },
	{ 6, "generate" },
	{ 7, "undo" },
	{ 8, "redo" },
	{ 9, "save" },
	{ 10, "hint" },
	{ 11, "num_solutions" },
	{ 12, "autofill" },
	{ 13, "reset" },
	{ 14, "exit" },
	{ 15, "error_command" }
};


/*
* The Function  
* @param - 
* @param -
* @param -
* @param -
* @return - 
*/

/*
* The Function convert a string to one on the possbilties in the emun sudokuCommands.
* @param str - the given string.
* @return - the maching sudokuCommands(enum);
*/
sudokuCommands str2enum(const char *str);
/*
* The Function recives the command from the user and interprets it to a function that handles the command.
************************* TODO: The function should check if the buffer is even valid, or to accept only valid ones*****
* @param buffer - the user's command. (its contents may be erased after calling this function)
* @return true(1) when no errors. 0 otherwise
false(0) if some error occured.
* buffer is destroyed after this function.
*/
int user_command(char* buffer);
/*
* The Function set a value into cell [x][y].
* @param row_index - index x;
* @param col_index - index y;
* @return - true(1) if now errors, otherwise false(0).
*/
int set(int row_index, int col_index, int value);
/*
* The Function prints the sudoku board.
* if mark_erorrs is on put '*' near errors.
* put '.' near fixed value.
*/
void print_board();
/*
* The Function prints 4N+n+1 '-'. (Sub fanc of print board). 
*/
void separator_row();
/*
* The Function ills all the board cells that have only 1 valid value.
*/
int autofill();
/* 
 *
 */
void autofill_board(int row_index, int col_index);
/*
* The Function checks if there is only onr valid value for cell [x][y].
* @param row_index - index x;
* @param col_index - index y;
* @return - the one possible value if there is one, otherwise return 0.
*/
int one_possible_value(int row_index, int col_index);
/*
 *
 *
 */
int Solve(char* filepath);
/*
*
*
*/
int Edit(char* filepath);
/*
*
*
*/
int Save(char* filepath);
/*
 *
 *
 */

void print_board_solution(); /* Helper function for testing the solution */

/*
 *
 *
 *
 *
 *
 */
int get_command_and_parse();
