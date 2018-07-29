#include "aux_main.h"


typedef enum {solve, edit, mark_errors,
	print_board, set, validate, generate,
	undo, redo, save, hint,
	num_solutions, autofill, reset, exit} sudokuCommands;


const static struct {
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
	{ 14, "exit" }
};

