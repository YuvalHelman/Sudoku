
#include <stdio.h>
#include "aux_main.h"
#include "user_interface.h"
#include "minunit.h" /* a simple testing header. */


/* TODO: */
/*
 *	sudoku.num_of_filled_cells -> check that we update that like we should.
 ***/

void initialize_test_board();

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

	initialize_list_parameters();

	initialize_test_board(); /* test board for testing stuff ... add the function for different boards */

	get_command_and_parse();
	
	

	/* free stuff before exiting cleanly */
	delete_list_on_exit();

	return 0;
	
}


void initialize_test_board() {

	/* Set basic sudoku utilities */
	sudoku.block_row_length = 2;
	sudoku.block_col_length = 3;
	sudoku.num_of_filled_cells = 0;

	sudoku.game_mode = solve;
	delete_list_full();
	free_board();
	sudoku.board = NULL;

	
	/* restart Sudoku's essential variables */
	sudoku.mark_errors = 0;

	initialize_new_board(3, 2);

	/* first row */
	set(1, 1, 1);
	/*set(1, 2, 2);*/
	/*set(1, 3, 3);*/
	set(1, 4, 4);
	set(1, 5, 5);
	set(1, 6, 6);
	/* second row.... etc */
	set(2, 2, 1);
	set(2, 3, 2);
	set(2, 4, 3);
	/*set(2, 5, 4);*/
	set(2, 6, 5);
	set(2, 1, 6);

	/*set(3, 3, 1);*/
	set(3, 4, 2);
	set(3, 5, 3);
	set(3, 6, 4);
	set(3, 1, 5);
	/*set(3, 2, 6);*/

	set(4, 4, 1);
	/*set(4, 5, 2);*/
	set(4, 6, 3);
	set(4, 1, 4);
	set(4, 2, 5);
	/*set(4, 3, 6);*/

	set(5, 5, 1);
	set(5, 6, 2);
	set(5, 1, 3);
	set(5, 2, 4);
	/*set(5, 3, 5);*/
	set(5, 4, 6);

	/*set(6, 6, 1);*/
	/*set(6, 1, 2);*/
	set(6, 2, 3);
	set(6, 3, 4);
	/*set(6, 4, 5);*/
	set(6, 5, 6);



}