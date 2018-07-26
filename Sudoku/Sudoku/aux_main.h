#pragma once

typedef enum game_mode_t {
	init, solve, edit
} GAME_MODE;

extern GAME_MODE game_mode;
extern int **game_board;
