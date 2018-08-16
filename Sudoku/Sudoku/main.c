
#include <stdio.h>
#include "aux_main.h"
#include "move_list.h"
#include "user_interface.h"
#include "game_logic.h"
#include "file_manipulation.h"

#include "minunit.h" /* a simple testing header. */


int tests_run = 0; /* Declared in minunit.h */
static char * all_tests() {
	mu_run_test(test_list_module);
	return 0;
}


int main() {


	get_command_and_parse();
	initialize_list_parameters();
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

	Edit("example.txt");
	
	



	/* free stuff before exiting cleanly */
	delete_list_on_exit();

	return 0;
	
}