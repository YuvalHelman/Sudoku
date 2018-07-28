
typedef enum {solve, edit, mark_errors,
	print_board, set, validate, generate,
	undo, redo, save, hint,
	num_solutions, autofill, reset, exit} sudokuCommands;


const static struct {
	sudokuCommands val;
	const char *str;
} conversion[] = {
	{ 1, "solve" },
	{ 2, "edit" },
	{ 3, "mark_errors" },
	{ 4, "print_board" },
	{ 5, "set" },
	{ 6, "validate" },
	{ 7, "generate" },
	{ 8, "undo" },
	{ 9, "redo" },
	{ 10, "save" },
	{ 11, "hint" },
	{ 12, "num_solutions" },
	{ 13, "autofill" },
	{ 14, "reset" },
	{ 15, "exit" }
};

