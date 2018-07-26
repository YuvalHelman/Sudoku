#ifndef EXIT_GAME
#define RESTART 2
#define EXIT_GAME 3
#define GAME_OVER 4
#endif

#ifndef __TYPES_H
#define __TYPES_H
/* a Cell struct
Fixed explanations:
0 - Should be blank
1 - a fixed cell
2 - a cell that the user inputed and should be showed on the board (not fixed) */
typedef struct cell_t {
	int value;
	int fixed;
	int solution;
}cell;
#endif


typedef enum {solve, edit, mark_errors,
	print_board, set, validate, generate,
	undo, redo, save, hint,
	num_solutions, autofill, reset, exit} sudokuCommends;


const static struct {
	sudokuCommends val;
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