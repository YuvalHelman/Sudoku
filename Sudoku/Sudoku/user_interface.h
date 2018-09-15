
#ifndef USER_INTERFACE
#define USER_INTERFACE

#include "aux_main.h"
#include "move_list.h"

/*
		This module defines the sudoku's user interface methods.
		Anything related to parsing of standart input commands, and calling the relavent operation by it.
*/

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


struct abc_t{
	sudokuCommands val;
	const char *str;
};

/*
*	The Function handles the user's input from stdin or files.
*	This command is looped until the program terminates (using the exit command or ctrl+C).
*
*   @returns: EXIT_SUCCESS(0) for no errors.
*	         on any error returns EXIT_FAILURE(1) and prints the error.
*/
int get_command_and_parse();

#endif