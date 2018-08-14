
#include <stdio.h>
#include "aux_main.h"
#include "move_list.h"
#include "user_interface.h"
#include "game_logic.h"
#include "file_manipulation.h"

#include "minunit.h" /* a simple testing header. */

/* Defining Public varibles that are useable in all code areas. */
GAME_MODE game_mode = init;
sudoku_t sudoku = NULL; /* All fields initialized to 0 */


int tests_run = 0; /* Declared in minunit.h */
static char * all_tests() {
	mu_run_test(test_list_module);
	return 0;
}


int main() {


	initialize_list_parameters();


	char *result = all_tests();
	if (result != 0) {
		printf("%s\n", result);
	}
	else {
		printf("ALL TESTS PASSED\n");
	}
	printf("Tests run: %d\n", tests_run);


	Edit("example.txt");

	



	return 0;
	
}