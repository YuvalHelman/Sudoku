#pragma once

// typedef enum { false = 0, true = 1 } bool;
#define false 0
#define true 1

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
	int error;
}cell;

typedef struct sudoku_t {
	cell **board;
	int mark_errors;
	int block_col_length;
	int block_row_length;
}sudoku_t;
#endif

typedef enum game_mode_t {
	init, solve, edit
} GAME_MODE;


extern GAME_MODE game_mode;
extern sudoku_t sudoku;

/* TODO: Add bool enum */