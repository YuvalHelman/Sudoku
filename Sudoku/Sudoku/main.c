#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "aux_main.h"
#include "user_interface.h"
#include <errno.h>
#include <time.h>

/*
		This source file implements the program's MAIN function
*/


int main() {

	unsigned int seed;

	/* Initializing basic utilities for starting the game */
	initialize_list_parameters();
	sudoku.mark_errors = true; /* the default value is 1 */

	seed = (unsigned int)time(NULL);
	srand(seed);

	printf("Sudoku\n------\n");

	/* Main Game function where the program is at when user puts commands and plays */
	get_command_and_parse(); 

	/* free stuff before exiting when get_command_and_parse might fail. */
	delete_list_on_exit();
	free_board();

	return EXIT_SUCCESS;

}