#include "aux_main.h"


typedef enum {solve, edit, mark_errors,
	print_board, set, validate, generate,
	undo, redo, save, hint,
	num_solutions, autofill, reset, exit} sudokuCommands;


const static struct {
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
	{ 14, "exit" }
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
* @return - ture(1) if now errors, otherwise false(0).
*/
int set(int row_index, int col_index, int value);
/*
* The Function prints the sudoku board.
* if mark_erorrs is on put '*' near errors.
* put '.' near fixed value.
*/
int print_board();
/*
* The Function prints 4N+n+1 '-'. (Sub fanc of print board). 
*/
void separator_row();
/*
* The Function ills all the board cells that have only 1 valid value.
*/
int autoFill();

void autofill_board(int row_index, int col_index);
/*
* The Function checks if there is only onr valid value for cell [x][y].
* @param row_index - index x;
* @param col_index - index y;
* @return - the one possible value if there is one, otherwise return 0.
*/
int one_possible_value(int row_index, int col_index);

