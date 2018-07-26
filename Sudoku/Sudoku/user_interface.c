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
* @return true(0) when no errors. 1 otherwise
false(1) if some error occured.
RESTART(2) - Signal the main function to Restart the program
EXIT_GAME(3) - Signal the main function to exit the program
* buffer is destroyed after this function.
*/
int user_command(cell** board, char* buffer, int game_over_flag) {
	/* */
	int x, y, z;
	char *xchar, *ychar, *zchar, *command;
	command = strtok(buffer, " \t\r\n");
	if (command == NULL) {
		return false;
	}