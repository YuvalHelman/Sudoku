
#ifndef GAME_LOGIC
#define GAME_LOGIC

#include "aux_main.h"



/*
* The Function checks if the value is a legal value for cell[x][y].
* @param row_index - index x.
* @param col_index - index y.
* @param value - The value we want to enter.
* @return -true(1) if the value is a legal value for the cell, false(0) otherwise.
*/
int valid_value(int row_index, int col_index, int value);

/*
* The Function update the mark_error for all the cells that have a connection(row,cell,block) with cell[x][y].
* @param row_index - index x.
* @param col_index - index y.
*/
void update_errors(int row_index, int col_index);


/*
* The Function update the mark_error for all the cells.
*/
void update_board_errors();
#endif /* GAME_LOGIC */