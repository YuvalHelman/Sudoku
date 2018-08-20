
#ifndef USER_INTERFACE
#define USER_INTERFACE

#include "aux_main.h"
#include "move_list.h"

#define DEFAULT_BLOCK_LEN 3
#define MAX_COMMAND_SIZE 260
#define BASE10 10
#define ZERO 0
#define NON_ZERO 1

typedef enum sudokucomm {solve_command, edit_command, mark_errors_command,
	print_board_command, set_command, validate_command, generate_command,
	undo_command, redo_command, save_command, hint_command,
	num_solutions_command, autofill_command, reset_command, exit_command, error_command
} sudokuCommands;



static struct abc_t{
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
 * The Function set a value into cell [x][y].
 * @param row_index - index x;
 * @param col_index - index y;
 * @return - true(1) if now errors, otherwise false(0).
 */
int set(int row_index, int col_index, int value);
/*
 * The Function traverse to the previous turn in the game.
 * 
 * 
 * 
 */
int undo();
/*
 * The Function traverse to the future turn in the game.
 * 
 *
 *
 */
int redo();

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

void print_board_solution(); /* Helper function for testing the solution */

/*
 *
 *
 *
 *
 *
 */
int get_command_and_parse();

#endif