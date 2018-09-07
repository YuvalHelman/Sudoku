
#ifndef GAME_LOGIC
#define GAME_LOGIC

#include "aux_main.h"


/*
*	The function checks if there are any errors on the current board.
*
*   @returns: true(1) when there are errors on the current board.
*			 false(0) when there aren't errors.
*/
int is_board_erronous();


/*
* The Function checks if the value is a legal value for cell[x][y].
* @param row_index - index x.
* @param col_index - index y.
* @param value - The value we want to enter.
* @return -true(1) if the value is a legal value for the cell, false(0) otherwise.
*/
int valid_value(int row_index, int col_index, int value);

/*
* The Function checks if the value is legal for this row.
* @param row_index - index x.
* @param col_index - index y.
* @param value - The value we want to enter.
* @return -true(1) if the value can be placed in the cell row wise, false(0) otherwise.
*/
int check_row(int row_index, int col_index, int value);

/*
* The Function checks if the value is legal for this col.
* @param row_index - index x.
* @param col_index - index y.
* @param value - The value we want to enter.
* @return -true(1) if the value can be placed in the cell col wise, false(0) otherwise.
*/
int check_col(int row_index, int col_index, int value);

/*
* The Function checks if the value is legal for this block.
* @param row_index - index x.
* @param col_index - index y.
* @param value - The value we want to enter.
* @return -true(1) if the value can be placed in the cell block wise, false(0) otherwise.
*/
int check_block(int row_index, int col_index, int value);



/*
* The Function update the mark_error for all the cells that have a *ROW* connection with cell[x][y].
* @param row_index - index x.
* @param col_index - index y.
*/
void update_errors_row(int row_index, int col_index);

/*
* The Function update the mark_error for all the cells that have a *COL* connection with cell[x][y].
* @param row_index - index x.
* @param col_index - index y.
*/
void update_errors_col(int row_index, int col_index);

/*
* The Function update the mark_error for all the cells that have a *BLOCK* connection with cell[x][y].
* @param row_index - index x.
* @param col_index - index y.
*/
void update_errors_block(int row_index, int col_index);

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