
#ifndef USER_INTERFACE
#define USER_INTERFACE

#include "aux_main.h"
#include "move_list.h"

#define DEFAULT_BLOCK_LEN 3
#define MAX_COMMAND_SIZE 258
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


/* TODO: erase those declarations before done:
They are here just for debugging */
int set(int row_index, int col_index, int value);
int undo();
int redo();
void print_board_solution(); /* Helper function for testing the solution */
void print_board();
void print_board_values();
void if_board_finished();

/*
*	The Function handles the user's input from stdin or files.
*	This command is looped until the program terminates (using the exit command or ctrl+C).
*
*   returns: EXIT_SUCCESS(0) for no errors.
*	         on any error returns EXIT_FAILURE(1) and prints the error.
*/
int get_command_and_parse();

#endif