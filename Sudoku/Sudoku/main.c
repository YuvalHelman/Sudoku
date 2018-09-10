
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "aux_main.h"
#include "user_interface.h"
#include "minunit.h" /* a simple testing header. */
#include <errno.h>
#include <time.h>


/* TODO: */
/*
 *	sudoku.num_of_filled_cells -> check that we update that like we should.
 *	parser: check that input's with tabs\spaces\(\n) are accepted like they should be.
 *  in Solve mode the user may only enter (or edit) values into cells that are not
	fixed, i.e., they were not part of the original puzzle. In Edit mode, the user may enter values
	into any cell. IDO, please check that it works that way.
*
*
 ***/

int tests_run = 0; /* Declared in minunit.h */
/**
static char * all_tests() {
	mu_run_test(test_list_module);
	return 0;
}
*/


/*
char *result;

initialize_list_parameters();


result = all_tests();
if (result != 0) {
printf("%s\n", result);
}
else {
printf("ALL TESTS PASSED\n");
}
printf("Tests run: %d\n", tests_run);
*/



int main() {


	/* Initializing basic utilities for starting the game */
	initialize_list_parameters();
	sudoku.mark_errors = true; /* the default value is 1 */
	srand((unsigned int)time(NULL));

	printf("Sudoku\n------\n");

	get_command_and_parse();



	/* free stuff before exiting when get_command_and_parse might fail. */
	delete_list_on_exit();
	free_board();

	return 0;

}