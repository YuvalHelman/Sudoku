
#include <stdio.h>
#include "aux_main.h"
#include "move_list.h"

#include "minunit.h" /* a simple testing header. */

/* Defining Public varibles that are useable in all code areas. */
GAME_MODE game_mode = init;


int tests_run = 0; /* Declared in minunit.h */
static char * all_tests() {
	mu_run_test(test_list);
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

	return result != 0;



	return 0;
}