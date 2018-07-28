#pragma once

typedef enum game_mode_t {
	init, solve, edit
} GAME_MODE;

typedef enum bool_t {
	false, true
} bool;




extern GAME_MODE game_mode;
extern int **game_board;


#ifndef __TYPES_H
#define __TYPES_H
/* a Cell struct
Fixed explanations:
0 - Should be blank \\could be replaced for defult value 0=blank
1 - a fixed cell */
typedef struct cell_t {
	int value;
	int fixed;
	int solution;
	int valid_value;
}cell;

typedef struct sudoku_t {
	cell **board;
	int mark_errors;
	int block_col_length;
	int block_row_length;
}sudoku;
#endif