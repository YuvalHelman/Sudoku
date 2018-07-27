#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "user_interface.h"


sudokuCommends 
str2enum(const char *str)
{
	int j;
	for (j = 0; j < sizeof(conversion) / sizeof(conversion[0]); ++j)
		if (!strcmp(str, conversion[j].str))
			return conversion[j].val;
	error_message("no such string");
}


/*
* The Function recives the command from the user and interprets it to a function that handles the command.
* @param buffer - the user's command. (its contents may be erased after calling this function)
* @param game_over_flag - a flag to check if the game is over, and only restart/exit are viable commands.
* @return true(1) when no errors. 0 otherwise
false(0) if some error occured.
RESTART(2) - Signal the main function to Restart the program
EXIT_GAME(3) - Signal the main function to exit the program
* buffer is destroyed after this function.
*/
int user_command(cell** board, char* buffer, int game_over_flag) {
	/* */
	int x, y, z;
	sudokuCommends sudoku_commend;
	char *xchar, *ychar, *zchar, *command;
	command = strtok(buffer, " \t\r\n");
	xchar = strtok(NULL, " \t\r\n");
	ychar = strtok(NULL, " \t\r\n");
	if (command == NULL) {
		return 0;
	}
	sudoku_commend = str2enum(command);
	switch (sudoku_commend)
	{
	solve:break;
	edit:break;
	mark_errors: break;
	print_board:break;
	set:break;
	validate:break;
	generate:break;
	undo:break;
	redo:break;
	save:break;
	hint:break;
	num_solutions:break;
	autofill:break;
	reset:break;
	exit:break;
	default:
		return 0;
		break;
	}
	